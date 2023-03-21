#include "client.h"
#include "joystick.h"
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

static pthread_t thread;
static int socketDescriptor;

static void setupWiimotes(wiimote **wiimotes);
static void getGuitarInput(struct wiimote_t *wm);

static void sleepForMs(long long delayInMs){
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;

    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;

    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

void* send_function(void *args){
    wiimote** wiimotes;
    setupWiimotes(wiimotes);

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

    while(true){ // send messages to server
        char messageTx[MAX_LEN];
        char messageRx[MAX_LEN];

        //get guitar input here
        joystick_direction dir = Joystick_getInput();
        snprintf(messageTx, MAX_LEN, "%d", dir);

        if(send(socketDescriptor, messageTx, strlen(messageTx), 0) < 0){
            printf("ERROR: Failed to send message\n");
        }

        int bytesRx = recv(socketDescriptor, messageRx, MAX_LEN - 1, 0);
        if(bytesRx == -1){
            printf("Error receiving response from server\n");
        }
        messageRx[bytesRx] = 0;

        printf("Server response: %s\n", messageRx);

        sleepForMs(1000);
    }

    return NULL;
}

static void setupWiimotes(wiimote **wiimotes)
{
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
}

static void getGuitarInput(struct wiimote_t *wm)
{
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
	}
}


void Client_start(void){
    pthread_create(&thread, NULL, send_function, NULL);
}

void Client_waitForShutdown(void){
    pthread_join(thread, NULL);
}

void Client_stop(void){
    close(socketDescriptor);
    pthread_cancel(thread);
}