#include <wiiuse.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>

#include "include/timing.h"
#include "include/memoryShare.h"
#include "include/sharedInputStruct.h"

#define MAX_WIIMOTES 1

static bool thread_shutdown = false;
static pthread_t guitar_pthread;


static void setupWiimotes(wiimote **wiimotes);
static void *guitarThreadMain(void *args);

// source: wiiuse example code by Michael Laforest
//  https://github.com/wiiuse/wiiuse
/**
 *	@brief Callback that handles an event.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *
 *	This function is called automatically by the wiiuse library when an
 *	event occurs on the specified wiimote.
 */
static unsigned char handle_event(struct wiimote_t* wm);

// source: wiiuse example code by Michael Laforest
//  https://github.com/wiiuse/wiiuse
static short any_wiimote_connected(wiimote** wm, int wiimotes);

void Guitar_init(void)
{
	pthread_create(&guitar_pthread, NULL, guitarThreadMain, NULL);
}

void Guitar_cleanup(void)
{
	thread_shutdown = true;

	pthread_join(guitar_pthread, NULL);
}

static void *guitarThreadMain(void *args)
{
	volatile void *pPruBase = PRU_getPruMmapAddr();
	volatile sharedInputStruct_t *pSharedInput = PRU0_MEM_FROM_BASE(pPruBase);
	while(!thread_shutdown) {

    	wiimote** wiimotes;
		wiimotes = wiiuse_init(MAX_WIIMOTES);

		setupWiimotes(wiimotes);

		while(any_wiimote_connected(wiimotes, MAX_WIIMOTES)) {

			if(pSharedInput->newResponse) {
				if(pSharedInput->noteHit) {
					printf("Correct\n");
				} else {
					printf("Incorrect\n");
				}
				printf("Input off by %d ms\n", pSharedInput->inputTimestamp);
				pSharedInput->newResponse = false;
			}

			if(wiiuse_poll(wiimotes, MAX_WIIMOTES) == 0) {
				continue;
			}

			unsigned char input;

			switch (wiimotes[0]->event) {
				case WIIUSE_EVENT:
					/* a generic event occurred */
					input = 0x00;
					input = handle_event(wiimotes[0]);
					//if strum or start were hit
					if(    (input & STRUM_MASK) != 0
						|| (input & START_MASK) != 0
						|| (input & MINUS_MASK) != 0) {
						pSharedInput->input = input;
						pSharedInput->newInput = true;
					}
					
					break;

				default:
					break;
			}

			
		}

		wiiuse_cleanup(wiimotes, MAX_WIIMOTES);
	}
    PRU_freePruMmapAddr(pPruBase);

    return NULL;
}

static void setupWiimotes(wiimote **wiimotes)
{
    int found, connected;

    found = wiiuse_find(wiimotes, MAX_WIIMOTES, 5);
	if (!found) {
		printf("No wiimotes found.\n");
		return;
	}

    connected = wiiuse_connect(wiimotes, MAX_WIIMOTES);
	if (connected) {
		printf("Connected to %i wiimotes (of %i found).\n", connected, found);
	} else {
		printf("Failed to connect to any wiimote.\n");
		return;
	}

    wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1);
	wiiuse_motion_sensing(wiimotes[0], 0);
	wiiuse_set_ir(wiimotes[0], 0);
}

static short any_wiimote_connected(wiimote** wm, int wiimotes)
{
	int i;
	if (!wm) {
		return 0;
	}

	for (i = 0; i < wiimotes; i++) {
		if (wm[i] && WIIMOTE_IS_CONNECTED(wm[i])) {
			return 1;
		}
	}

	return 0;
}

static unsigned char handle_event(struct wiimote_t* wm) 
{
	printf("\n\n--- EVENT [id %i] ---\n", wm->unid);
	unsigned char input = 0x00;

	if (wm->exp.type == EXP_GUITAR_HERO_3) {
		/* guitar hero 3 guitar */
		struct guitar_hero_3_t* gh3 = (guitar_hero_3_t*)&wm->exp.gh3;

		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_STRUM_UP)) {
			printf("Guitar: Strum Up pressed\n");
			input |= STRUM_MASK;
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_STRUM_DOWN))	{
			printf("Guitar: Strum Down pressed\n");
			input |= STRUM_MASK;
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_GREEN)) {
			printf("Guitar: Green pressed\n");
			input |= GREEN_MASK;
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_RED)) {
			printf("Guitar: Red pressed\n");
			input |= RED_MASK;
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_YELLOW)) {
			printf("Guitar: Yellow pressed\n");
			input |= YELLOW_MASK;
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_BLUE)) {
			printf("Guitar: Blue pressed\n");
			input |= BLUE_MASK;
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_ORANGE)) {
			printf("Guitar: Orange pressed\n");
			input |= ORANGE_MASK;
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_PLUS)) {
			printf("Guitar: Plus pressed\n");
			input |= START_MASK;
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_MINUS)) {
			printf("Guitar: Minus pressed\n");
			input |= MINUS_MASK;
		}


		// printf("Guitar whammy bar:          %f\n", gh3->whammy_bar);
		// printf("Guitar joystick angle:      %f\n", gh3->js.ang);
		// printf("Guitar joystick magnitude:  %f\n", gh3->js.mag);

	}
	return input;

		/* if a button is pressed, report it */
		// if (IS_PRESSED(wm, WIIMOTE_BUTTON_A)) {
		// 	printf("A pressed\n");
		// }
		// if (IS_PRESSED(wm, WIIMOTE_BUTTON_B)) {
		// 	printf("B pressed\n");
		// }
		// if (IS_PRESSED(wm, WIIMOTE_BUTTON_UP)) {
		// 	printf("UP pressed\n");
		// }
		// if (IS_PRESSED(wm, WIIMOTE_BUTTON_DOWN))	{
		// 	printf("DOWN pressed\n");
		// }
		// if (IS_PRESSED(wm, WIIMOTE_BUTTON_LEFT))	{
		// 	printf("LEFT pressed\n");
		// }
		// if (IS_PRESSED(wm, WIIMOTE_BUTTON_RIGHT))	{
		// 	printf("RIGHT pressed\n");
		// }
		// if (IS_PRESSED(wm, WIIMOTE_BUTTON_MINUS))	{
		// 	printf("MINUS pressed\n");
		// }
		// if (IS_PRESSED(wm, WIIMOTE_BUTTON_PLUS))	{
		// 	printf("PLUS pressed\n");
		// }
		// if (IS_PRESSED(wm, WIIMOTE_BUTTON_ONE)) {
		// 	printf("ONE pressed\n");
		// }
		// if (IS_PRESSED(wm, WIIMOTE_BUTTON_TWO)) {
		// 	printf("TWO pressed\n");
		// }
		// if (IS_PRESSED(wm, WIIMOTE_BUTTON_HOME))	{
		// 	printf("HOME pressed\n");
		// }
	
}