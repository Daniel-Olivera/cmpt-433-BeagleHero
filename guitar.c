#include <wiiuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>
#include <stdbool.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_WIIMOTES 1

#define PORT 12345
#define SERVER_IP "192.168.1.81"
#define MAX_LEN 1024

static int socketDescriptor;

static void setupWiimotes(wiimote **wiimotes);
static void createTcpConnection();

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
void handle_event(struct wiimote_t* wm, char *message, int messageLength) {
	printf("\n\n--- EVENT [id %i] ---\n", wm->unid);

	/* if a button is pressed, report it */
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_A)) {
		snprintf(message, messageLength, "A pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_B)) {
		snprintf(message, messageLength, "B pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_UP)) {
		snprintf(message, messageLength, "UP pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_DOWN))	{
		snprintf(message, messageLength, "DOWN pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_LEFT))	{
		snprintf(message, messageLength, "LEFT pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_RIGHT))	{
		snprintf(message, messageLength, "RIGHT pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_MINUS))	{
		snprintf(message, messageLength, "MINUS pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_PLUS))	{
		snprintf(message, messageLength, "PLUS pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_ONE)) {
		snprintf(message, messageLength, "ONE pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_TWO)) {
		snprintf(message, messageLength, "TWO pressed\n");
	}
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_HOME))	{
		snprintf(message, messageLength, "HOME pressed\n");
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
			snprintf(message, messageLength, "Guitar: Strum Up pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_STRUM_DOWN))	{
			snprintf(message, messageLength, "Guitar: Strum Down pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_YELLOW)) {
			snprintf(message, messageLength, "Guitar: Yellow pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_GREEN)) {
			snprintf(message, messageLength, "Guitar: Green pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_BLUE)) {
			snprintf(message, messageLength, "Guitar: Blue pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_RED)) {
			snprintf(message, messageLength, "Guitar: Red pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_ORANGE)) {
			snprintf(message, messageLength, "Guitar: Orange pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_PLUS)) {
			snprintf(message, messageLength, "Guitar: Plus pressed\n");
		}
		if (IS_PRESSED(gh3, GUITAR_HERO_3_BUTTON_MINUS)) {
			snprintf(message, messageLength, "Guitar: Minus pressed\n");
		}

		// printf("Guitar whammy bar:          %f\n", gh3->whammy_bar);
		// printf("Guitar joystick angle:      %f\n", gh3->js.ang);
		// printf("Guitar joystick magnitude:  %f\n", gh3->js.mag);
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
	wiimotes = wiiuse_init(MAX_WIIMOTES);
    
	setupWiimotes(wiimotes);

	createTcpConnection();

    while(any_wiimote_connected(wiimotes, MAX_WIIMOTES)) {
        if(wiiuse_poll(wiimotes, MAX_WIIMOTES) == 0) {
			continue;
		}

		char messageTx[MAX_LEN];
		// char messageRx[MAX_LEN];

		switch (wiimotes[0]->event) {
			case WIIUSE_EVENT:
				/* a generic event occurred */
				handle_event(wiimotes[0], messageTx, MAX_LEN);
				break;

			default:
				break;
		}

		if(send(socketDescriptor, messageTx, strlen(messageTx), 0) < 0){
			printf("ERROR: Failed to send message\n");
		}
        
    }

    wiiuse_cleanup(wiimotes, MAX_WIIMOTES);
	close(socketDescriptor);

    return 0;
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

static void createTcpConnection()
{
	struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    sin.sin_addr.s_addr = inet_addr(SERVER_IP);
    
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socketDescriptor == -1){
        printf("ERROR: Failed to create socket\n");
        exit(-1);
    }

    if(connect(socketDescriptor, (struct sockaddr*)&sin, sizeof(sin)) < 0){
        printf("ERROR: Failed to connect\n");
        exit(-1);
    }

    printf("Connected to BBG server\n");
}