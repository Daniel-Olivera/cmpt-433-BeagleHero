#include <wiiuse.h>
#include <stdio.h>
#include <stdbool.h>

#define WIIMOTE_PATH "~/sys/bus/hid/devices/0005:057E:0306.0001"
#define MAX_WIIMOTES 1
// struct xwii_iface* dev;

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
void handle_event(struct wiimote_t* wm) {
	printf("\n\n--- EVENT [id %i] ---\n", wm->unid);

	/* if a button is pressed, report it */
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_A)) {
		printf("A pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_B)) {
		printf("B pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_UP)) {
		printf("UP pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_DOWN))	{
		printf("DOWN pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_LEFT))	{
		printf("LEFT pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_RIGHT))	{
		printf("RIGHT pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_MINUS))	{
		printf("MINUS pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_PLUS))	{
		printf("PLUS pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_ONE)) {
		printf("ONE pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_TWO)) {
		printf("TWO pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_HOME))	{
		printf("HOME pressed\n");
	}

	/*
	 *	Pressing minus will tell the wiimote we are no longer interested in movement.
	 *	This is useful because it saves battery power.
	 */
	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_MINUS)) {
		wiiuse_motion_sensing(wm, 0);
	}

	/*
	 *	Pressing plus will tell the wiimote we are interested in movement.
	 */
	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_PLUS)) {
		wiiuse_motion_sensing(wm, 1);
	}

	/*
	 *	Pressing B will toggle the rumble
	 *
	 *	if B is pressed but is not held, toggle the rumble
	 */
	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_B)) {
		wiiuse_toggle_rumble(wm);
	}

	if (wm->exp.type == EXP_GUITAR_HERO_3) {
		/* guitar hero 3 guitar */
		struct guitar_hero_3_t* gh3 = (guitar_hero_3_t*)&wm->exp.gh3;

		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_STRUM_UP)) {
			printf("Guitar: Strum Up pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_STRUM_DOWN))	{
			printf("Guitar: Strum Down pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_YELLOW)) {
			printf("Guitar: Yellow pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_GREEN)) {
			printf("Guitar: Green pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_BLUE)) {
			printf("Guitar: Blue pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_RED)) {
			printf("Guitar: Red pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_ORANGE)) {
			printf("Guitar: Orange pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_PLUS)) {
			printf("Guitar: Plus pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_MINUS)) {
			printf("Guitar: Minus pressed\n");
		}

		printf("Guitar whammy bar:          %f\n", gh3->whammy_bar);
		printf("Guitar joystick angle:      %f\n", gh3->js.ang);
		printf("Guitar joystick magnitude:  %f\n", gh3->js.mag);
	} 
}

// source: wiiuse example code by Michael Laforest
//  https://github.com/wiiuse/wiiuse
short any_wiimote_connected(wiimote** wm, int wiimotes) {
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

int main()
{
    printf("Hello World!\n");

    wiimote** wiimotes;
    int found, connected;

    wiimotes = wiiuse_init(MAX_WIIMOTES);


    found = wiiuse_find(wiimotes, MAX_WIIMOTES, 5);
	if (!found) {
		printf("No wiimotes found.\n");
		return 0;
	}


    connected = wiiuse_connect(wiimotes, MAX_WIIMOTES);
	if (connected) {
		printf("Connected to %i wiimotes (of %i found).\n", connected, found);
	} else {
		printf("Failed to connect to any wiimote.\n");
		return 0;
	}
    
    wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1);
	wiiuse_motion_sensing(wiimotes[0], 0);
	wiiuse_set_ir(wiimotes[0], 0);

    while(any_wiimote_connected(wiimotes, MAX_WIIMOTES)) {
        if(wiiuse_poll(wiimotes, MAX_WIIMOTES)) {
            

            for(int i = 0; i < MAX_WIIMOTES; i++) {
                switch (wiimotes[i]->event) {
					case WIIUSE_EVENT:
						/* a generic event occurred */
						handle_event(wiimotes[i]);
						break;

					default:
						break;
                }
            }
        }
    }

    wiiuse_cleanup(wiimotes, MAX_WIIMOTES);

    return 0;
}