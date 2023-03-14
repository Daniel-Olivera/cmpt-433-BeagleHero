#include "server.h"
#include "led.h"
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
static int client_socket;

static bool led0 = false;
static bool led1 = false;
static bool led2 = false;
static bool led3 = false;

static void sleepForMs(long long delayInMs){
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;

    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;

    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

void* server_function(void *args){
    struct sockaddr_in sin;
    struct sockaddr_in client;
    memset(&sin, 0, sizeof(sin));
    memset(&client, 0, sizeof(client));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    sin.sin_addr.s_addr = inet_addr(CLIENT_IP);

    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socketDescriptor == -1){
        printf("ERROR: Failed to create socket\n");
        exit(-1);
    }

    if(bind(socketDescriptor, (struct sockaddr*)&sin, sizeof(sin)) < 0){
        printf("ERROR: Could not bind to port\n");
        exit(-1);
    }

    if(listen(socketDescriptor, 1) < 0){
        printf("Error while listening\n");
        exit(-1);
    }
    printf("Listening for incoming connections..\n");

    socklen_t client_size = sizeof(client);
    client_socket = accept(socketDescriptor, (struct sockaddr*)&client, &client_size);

    if(client_socket < 0){
        printf("Can't accept connection\n");
    }
    printf("Connected to client with IP: %s, and port: %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    while(true){
        char messageRx[MAX_LEN];
        char messageTx[MAX_LEN];
        // printf("Begin loop\n");

        int bytesRx = recv(client_socket, messageRx, MAX_LEN - 1, 0);
        if(bytesRx < 0){
            printf("Error receiving message from the client\n");
        }
        messageRx[bytesRx] = 0;
        printf("Message from client: %s\n", messageRx);

        // if(strcmp(messageRx, "hello") == 0){
        //     if(send(client_socket, messageTx, strlen(messageTx), 0) < 0){
        //         printf("ERROR: Failed to send message\n");
        //     }
        // }

        if(strcmp(messageRx, "0") == 0){
            if(!led0){
                LED_powerOn(LED0_BRIGHTNESS);
                led0 = true;
            }
            else{
                LED_powerOff(LED0_BRIGHTNESS);
                led0 = false;
            }

            snprintf(messageTx, MAX_LEN, "Changed LED0");
            if(send(client_socket, messageTx, strlen(messageTx), 0) < 0){
                printf("ERROR: Failed to send message\n");
            }
        }
        else if(strcmp(messageRx, "1") == 0){
            if(!led1){
                LED_powerOn(LED1_BRIGHTNESS);
                led1 = true;
            }
            else{
                LED_powerOff(LED1_BRIGHTNESS);
                led1 = false;
            }

            snprintf(messageTx, MAX_LEN, "Changed LED1");
            if(send(client_socket, messageTx, strlen(messageTx), 0) < 0){
                printf("ERROR: Failed to send message\n");
            }
        }
        else if(strcmp(messageRx, "2") == 0){
            if(!led2){
                LED_powerOn(LED2_BRIGHTNESS);
                led2 = true;
            }
            else{
                LED_powerOff(LED2_BRIGHTNESS);
                led2 = false;
            }

            snprintf(messageTx, MAX_LEN, "Changed LED2");
            if(send(client_socket, messageTx, strlen(messageTx), 0) < 0){
                printf("ERROR: Failed to send message\n");
            }
        }
        else if(strcmp(messageRx, "3") == 0){
            if(!led3){
                LED_powerOn(LED3_BRIGHTNESS);
                led3 = true;
            }
            else{
                LED_powerOff(LED3_BRIGHTNESS);
                led3 = false;
            }

            snprintf(messageTx, MAX_LEN, "Changed LED3");
            if(send(client_socket, messageTx, strlen(messageTx), 0) < 0){
                printf("ERROR: Failed to send message\n");
            }
        }
        else{
            snprintf(messageTx, MAX_LEN, "No joystick input");
            if(send(client_socket, messageTx, strlen(messageTx), 0) < 0){
                printf("ERROR: Failed to send message\n");
            }
        }

        sleepForMs(1000);
    }

    return NULL;
}

void Server_start(void){
    pthread_create(&thread, NULL, server_function, NULL);
}

void Server_waitForShutdown(void){
    pthread_join(thread, NULL);
}

void Server_stop(void){
    pthread_cancel(thread);
}