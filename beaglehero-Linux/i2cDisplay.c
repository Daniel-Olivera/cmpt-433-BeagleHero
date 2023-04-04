#include <pthread.h>
#include <stdbool.h>
#include "include/i2cController.h"
#include "include/timing.h"

#define NUM_INTEGERS 10
#define MAX_DISPLAYED_INTEGER 99
#define DISPLAY_TIME_MS 5



DisplayChar integers[NUM_INTEGERS] = {{0x86, 0xA1}, {0x12, 0x80}, {0x0E, 0x31},
                                      {0x06, 0xB0}, {0x8A, 0x90}, {0x8C, 0xB0},
                                      {0x8C, 0xB1}, {0x14, 0x04}, {0x8E, 0xB1},
                                      {0x8E, 0x90}};

bool displayActive = false;
pthread_t displayThread;
pthread_mutex_t displayInUseLock;

DisplayValue currentValue;

static void *displayThreadMain(void *args);
static void updateDisplay(void);

void Display_init(void) 
{
    I2c_init();

    currentValue.leftChar = integers[0];
    currentValue.rightChar = integers[0];
    
    displayActive = true;
    pthread_create(&displayThread, NULL, displayThreadMain, NULL);
}

void Display_cleanup(void) 
{
    displayActive = false;
    
    pthread_join(displayThread, NULL);

    I2c_cleanup();
}


void Display_updateInteger(unsigned int output) 
{
    //convert integer into 2 digits, and update currentValue
    if(output > MAX_DISPLAYED_INTEGER) {
        currentValue.leftChar = integers[9];
        currentValue.rightChar = integers[9];
        return;
    }

    unsigned int tens = output / 10;
    unsigned int ones = output % 10;

    currentValue.leftChar = integers[tens];
    currentValue.rightChar = integers[ones];
}

static void *displayThreadMain(void *args)
{
    updateDisplay();

    return NULL;
}

static void updateDisplay(void)
{
    while(displayActive) {
        //side switching to keep display active
        pthread_mutex_lock(&displayInUseLock);
        {
            I2c_writeToDisplay(currentValue.leftChar);
            I2c_activateLeftDigit();
            Timer_sleepForMs(DISPLAY_TIME_MS);
            I2c_deactivateBothDigits();
            I2c_writeToDisplay(currentValue.rightChar);
            I2c_activateRightDigit();
            Timer_sleepForMs(DISPLAY_TIME_MS);
            I2c_deactivateBothDigits();
        }
        pthread_mutex_unlock(&displayInUseLock);
    }
}
