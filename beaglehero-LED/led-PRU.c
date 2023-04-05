#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "../beaglehero-Linux/include/sharedStructs.h"


volatile register uint32_t __R30;

#define STR_LEN         64      // # LEDs in our Matrix

// Cycle timings taken from Dr. Brian Fraser's LED RGBW demo
// ---------------------------------------------------------
#define oneCyclesOn     700/5   // Stay on 700ns
#define oneCyclesOff    800/5
#define zeroCyclesOn    350/5
#define zeroCyclesOff   600/5
#define cleanupCycles   60000/5 // Must be at least 50u, use 60u

// Delay is calculated by beat intervals vs LED spacing
// (beatATimeStamp - beatBTimeStamp)/number of desired LED spacing
#define cyclesPerLoop 200000

#define updateCycle 1

#define PRU_SHAREDMEM 0x10000 // Offset to shared memory

// Reference for bit shifted RGB Colour Macros: 
// https://os.mbed.com/components/NeoPixel-NeoMatrix-8x8/ 
#define Color(r,g,b) ((r&0xFF) << 8 | (g&0xFF) << 16 | (b&0xff))
#define RED (Color(63,0,0))
#define GREEN (Color(0,63,0))
#define BLUE (Color(0,0,63))
#define ORANGE (Color(63,12,0))
#define YELLOW (Color(63,37,0))

// GPIO Input: P8_41 = pru1_pru_r30_4
// Drives the LED Matrix
#define DATA_PIN 4      

volatile sharedResponseStruct_t *pSharedResponse =
    (volatile void *)PRU_SHAREDMEM;

volatile beatmap_t *pBeatmap = 
    (volatile void *)PRU_SHAREDMEM;

uint32_t newNote_index = 0;

uint32_t targetNote_index = 0;

uint64_t timeSinceStart = 0;

static void turnBitOn(void);

static void turnBitOff(void);

static void setLED(uint32_t colour);

static void setLedByArray(unsigned int* array);

static void turnAllOff(void);

static unsigned int* initLedGrid();

// static void cleanUp(unsigned int* grid);

static void turnAllOn(uint32_t colour);

void main(void)
{
    // Clear SYSCFG[STANDBY_INIT] to enable OCP master port
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    unsigned int* grid = initLedGrid();
    pSharedResponse->noteHit = false;
    pSharedResponse->newResponseCombo = false;
    // pSharedResponse->songStarting = false;
    pSharedResponse->noteAttemptedLED = false;
    pBeatmap->totalNotes = 0;
    
    //nextNote change to 
    uint32_t nextNoteTimeStamp= pBeatmap->notes[newNote_index].timestamp;
    uint32_t timeStampOffset = 1000;

    uint16_t colourMaskArray[5] = {ORANGE_MASK,BLUE_MASK,YELLOW_MASK,RED_MASK,GREEN_MASK};
    bool flag = false;
    bool test = false;
    while(true){

    
        if(!pSharedResponse->songStarting){
            if(!flag){
                grid[0] += 1; 
                flag = true;
            }
            else{
                flag = false;
            }
        }
        else if(pSharedResponse->songStarting){
            if(!test){
                grid[1] += 1; 
                test = true;
            }
            else{
                test = false;
            }
        }
        // else{
        //     if(!test){
        //         grid[2] += 1; 
        //         test = true;
        //     }
        //     else{
        //         test = false;
        //     }
        // }
        // else if(!pSharedResponse->songStarting && wentIn){
        //     grid[1] += 1;
        // }
        // else if(!pSharedResponse->songStarting){
        //     continue;
        // }

        // grid[0] += 1;

        // turnAllOff();

        // grid[0] += 1;

        // If the timeSinceStart + Offset is larger than or equal to the timestamp of the next note,
        // get the input bit of the newNote and set the bit in the corresponding row based on the input bit colour
        // if(timeSinceStart + timeStampOffset >= nextNoteTimeStamp){
        //     unsigned char newNote = pBeatmap->notes[newNote_index].input;
        //     // unsigned char newNote = 48;

        //     // turnAllOn(BLUE);

        //     if((newNote & GREEN_MASK) != 0){
        //         grid[4] += 1;
        //     }

        //     if((newNote & RED_MASK) != 0){
        //         grid[3] += 1;
        //     }

        //     if((newNote & YELLOW_MASK) != 0){
        //         grid[2] += 1;
        //     }

        //     if((newNote & BLUE_MASK) != 0){
        //         grid[1] += 1;
        //     }

        //     if((newNote & ORANGE_MASK) != 0){
        //         grid[0] += 1;
        //     }


        //     // for(int i = 0; i < 5; i++){
        //     //     if ((newNote & colourMaskArray[i] != 0)){
        //     //         grid[i] += 1;
        //     //     }
        //     // }

        //     newNote_index += 1;
        //     nextNoteTimeStamp = pBeatmap->notes[newNote_index].timestamp;
        //     // setLedByArray(grid);
        // }

        // If note was hit, turn off the MSB of the binary literal and update the grid and continue with no delay
        // if(pSharedResponse->noteAttemptedLED){
        //     char targetNote = pBeatmap->notes[targetNote_index].input;

        //     for(int i = 0; i < 5; i++){
        //         if ((targetNote && colourMaskArray[i] != 0)){
        //             grid[i] &= ~(1 << 7);
        //         }
        //     }
        //     setLedByArray(grid);
        //     targetNote_index += 1;
        //     pSharedResponse->noteAttemptedLED = false;
        //     __delay_cycles(1);
        // }

        setLedByArray(grid);
        for(int i = 0; i < 5; i++){
            grid[i] <<= 1;
        }

        timeSinceStart += 125;
        __delay_cycles(cyclesPerLoop*125);
    }

    // cleanUp(grid);
}


void turnBitOn(void){
    __R30 |= 0x1<<DATA_PIN;      // Set the GPIO pin to 1
    __delay_cycles(oneCyclesOn-1);
    __R30 &= ~(0x1<<DATA_PIN);   // Clear the GPIO pin
    __delay_cycles(oneCyclesOff-2);
}

void turnBitOff(void){
    __R30 |= 0x1<<DATA_PIN;      // Set the GPIO pin to 1
    __delay_cycles(zeroCyclesOn-1);
    __R30 &= ~(0x1<<DATA_PIN);   // Clear the GPIO pin
    __delay_cycles(zeroCyclesOff-2);
}


// Set LED based on provided colour
void setLED(uint32_t colour){
    for(int i = 23; i >= 0; i--) {
        if(colour & ((uint32_t)0x1 << i)) {
            turnBitOn();
        }else{
            turnBitOff();
        }
    }
}

// Set the entire grid/array of LEDs based on the corresponding colours and provided matrix
void setLedByArray(unsigned int* array){
    uint32_t colours[5] = {ORANGE,BLUE,YELLOW,RED,GREEN};
    for(int i= 0; i < 8; i++){
        for(int j = 7; j >= 0; j--){
            if(array[i] & (0b1 << j)){
                setLED(colours[i]);
            }
            else{
                for(int k = 23; k >= 0; k--){
                    turnBitOff();
                }
            }
        }
    }
    return;
} 

// Turns off all LEDs
void turnAllOff(void){
    for(int j = 0; j < 64; j++) {
        for(int i = 23; i >= 0; i--) {
            turnBitOff();
        }
    } 
}

void turnAllOn(uint32_t colour){
    for(int j = 0; j < 64; j++) {
        for(int i = 23; i >= 0; i--) {
            if(colour & (0x1 << i)){
                turnBitOn();
            }
            else{
                turnBitOff();
            }
        }
    } 
}


// Reference for idea of using binary literal: 
// https://stackoverflow.com/questions/2611764/can-i-use-a-binary-literal-in-c-or-c 
// Initializes the LED grid with an array of binary Literals
unsigned int* initLedGrid(){
    unsigned int* grid = malloc(8*sizeof(int));
    grid[0] = 0b00000000;
    grid[1] = 0b00000000;
    grid[2] = 0b00000000; 
    grid[3] = 0b00000000;
    grid[4] = 0b00000000;
    grid[5] = 0b00000000;
    grid[6] = 0b00000000;
    grid[7] = 0b00000000;
    return grid;
}


// Cleans up the PRU pins as well as clearing any dynamically allocated memory
void cleanUp(unsigned int* grid)
{
    // turnAllOff();
    free(grid);
    // Send Reset
    __R30 &= ~(0x1<<DATA_PIN);   // Clear the GPIO pin
    __delay_cycles(cleanupCycles);
    __halt();

}
