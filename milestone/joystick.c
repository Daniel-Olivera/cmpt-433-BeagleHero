#include "joystick.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <pthread.h>
#include <stdbool.h>

static pthread_t thread;
static joystick_direction joystickRes = -1;

static void sleepForMs(long long delayInMs){
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;

    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;

    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

static void Joystick_configureIO(char *fileName){
    // Use fopen() to open the file for write access
    FILE *pFile = fopen(fileName, "w");

    if(pFile == NULL){
        printf("ERROR: Unable to open %s.\n", fileName);
        exit(-1);
    }

    // Write to data to the file using fprintf()
    fprintf(pFile, "%s", "in");
    
    // Close the file using fclose()
    fclose(pFile);
}

static void Joystick_configurePin(char* command){
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");

    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe
    char buffer[1024];
    while(!feof(pipe) && !ferror(pipe)){
        if(fgets(buffer, sizeof(buffer), pipe) == NULL){
            break;
        }
    }

    int exitCode = WEXITSTATUS(pclose(pipe));
    if(exitCode != 0){
        perror("Unable to execute command:");
        printf("    command:    %s\n", command);
        printf("    exit code:  %d\n", exitCode);
    }    
}

static char *Joystick_readInput(char *fileName){
    FILE *pFile = fopen(fileName, "r");
    if (pFile == NULL){
        printf("ERROR: Unable to open file (%s) for read\n", fileName);
        exit(-1);
    }

    // Read string (line)
    const int MAX_LENGTH = 1024;
    char *buff = malloc(MAX_LENGTH);
    fgets(buff, MAX_LENGTH, pFile);

    // Close
    fclose(pFile);

    // printf("Read: '%s' \n", buff);
    return buff;
}

static void Joystick_initialize(void){
    Joystick_configurePin(JOYSTICK_UP_COMMAND);
    sleepForMs(300);

    Joystick_configurePin(JOYSTICK_DOWN_COMMAND);
    sleepForMs(300);

    Joystick_configurePin(JOYSTICK_LEFT_COMMAND);
    sleepForMs(300);

    Joystick_configurePin(JOYSTICK_RIGHT_COMMAND);
    sleepForMs(300);

    Joystick_configureIO(JOYSTICK_UP_IO);
    Joystick_configureIO(JOYSTICK_DOWN_IO);
    Joystick_configureIO(JOYSTICK_LEFT_IO);
    Joystick_configureIO(JOYSTICK_RIGHT_IO);
}

void *joystick_function(void* args){

    char *upInput;
    char *downInput;
    char *leftInput;
    char *rightInput;

    while(true){
        joystickRes = -1;
        upInput = Joystick_readInput(JOYSTICK_UP_PATH);
        downInput = Joystick_readInput(JOYSTICK_DOWN_PATH);
        leftInput = Joystick_readInput(JOYSTICK_LEFT_PATH);
        rightInput = Joystick_readInput(JOYSTICK_RIGHT_PATH);

        if(*upInput == '0'){
            joystickRes = JOYSTICK_UP;
            sleepForMs(200);
        }
        else if(*downInput == '0'){
            joystickRes = JOYSTICK_DOWN;
            sleepForMs(200);
        }
        else if(*leftInput == '0'){
            joystickRes = JOYSTICK_LEFT;
            sleepForMs(200);
        }
        else if(*rightInput == '0'){
            joystickRes = JOYSTICK_RIGHT;
            sleepForMs(200);
        }
        free(upInput);
        free(downInput);
        free(leftInput);
        free(rightInput);

        sleepForMs(10);
    }

    return NULL;
}

joystick_direction Joystick_getInput(void){
    return joystickRes;
}

void Joystick_start(void){
    Joystick_initialize();
    pthread_create(&thread, NULL, joystick_function, NULL);
}

void Joystick_stop(void){
    pthread_cancel(thread);
}

void Joystick_waitForShutdown(void){
    pthread_join(thread, NULL);
}