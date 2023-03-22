#include "led.h"
#include <stdio.h>
#include <stdlib.h>

void LED_initialize(void){
    LED_setTrigger(LED0_TRIGGER, "none");
    LED_setTrigger(LED1_TRIGGER, "none");
    LED_setTrigger(LED2_TRIGGER, "none");
    LED_setTrigger(LED3_TRIGGER, "none");
}

void LED_setTrigger(char *fileName, char *triggerOption){
    FILE *pLedTriggerFile = fopen(fileName, "w");

    if(pLedTriggerFile == NULL){
        printf("ERROR OPENING %s.\n", fileName);
        exit(-1);
    }

    int charWritten = fprintf(pLedTriggerFile, triggerOption);
    if(charWritten <= 0){
        printf("ERROR WRITING DATA\n");
        exit(-1);
    }

    fclose(pLedTriggerFile);
}

void LED_powerOn(char *fileName){
    FILE *pLedBrightnessFile = fopen(fileName, "w");

    if(pLedBrightnessFile == NULL){
        printf("ERROR OPENING %s.\n", fileName);
        exit(-1);
    }

    int charWritten = fprintf(pLedBrightnessFile, "1");
    if(charWritten <= 0){
        printf("ERROR WRITING DATA\n");
        exit(-1);
    }

    fclose(pLedBrightnessFile);
}

void LED_powerOff(char *fileName){
    FILE *pLedBrightnessFile = fopen(fileName, "w");

    if(pLedBrightnessFile == NULL){
        printf("ERROR OPENING %s.\n", fileName);
        exit(-1);
    }

    int charWritten = fprintf(pLedBrightnessFile, "0");
    if(charWritten <= 0){
        printf("ERROR WRITING DATA\n");
        exit(-1);
    }

    fclose(pLedBrightnessFile);
}

void LED_dimAll(void){
    LED_powerOff(LED0_BRIGHTNESS);
    LED_powerOff(LED1_BRIGHTNESS);
    LED_powerOff(LED2_BRIGHTNESS);
    LED_powerOff(LED3_BRIGHTNESS);
}

void LED_powerAll(void){
    LED_powerOn(LED0_BRIGHTNESS);
    LED_powerOn(LED1_BRIGHTNESS);
    LED_powerOn(LED2_BRIGHTNESS);
    LED_powerOn(LED3_BRIGHTNESS);
}
