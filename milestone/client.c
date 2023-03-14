#include "client.h"
#include "joystick.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>
#include <stdbool.h>
#include <string.h>
#include <arpa/inet.h>

static pthread_t thread;
static int socketDescriptor;

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