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
#define cleanupCycles     60000/5 // Must be at least 50u, use 60u

// Delay is calculated by beat intervals vs LED spacing
// (beatATimeStamp - beatBTimeStamp)/number of desired LED spacing
#define cyclesPerLoop 200000000/5

#define updateCycle 1




#define Color(r,g,b) ((r&0xFF) << 8 | (g&0xFF) << 16 | (b&0xff))
#define RED (Color(63,0,0))
#define GREEN (Color(0,63,0))
#define BLUE (Color(0,0,63))
#define ORANGE (Color(63,12,0))
#define YELLOW (Color(63,37,0))

// GPIO Input: P8_11 = pru0_pru_r30_15
// Drives the LED Matrix
#define DATA_PIN 15       

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

static void cleanUp(unsigned int* grid);

void main(void)
{
    // Clear SYSCFG[STANDBY_INIT] to enable OCP master port
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    unsigned int* grid = initLedGrid();
    pSharedResponse->noteHit = false;
    pSharedResponse->newResponseCombo = false;
    pSharedResponse->songStarting = false;
    pSharedResponse->noteAttemptedLED = false;
    pBeatmap->totalNotes = 0;
    

    int flag = 1;
    uint32_t nextNoteTimeStamp= pBeatmap->notes[currentNote].timestamp;
    uint32_t timeStampOffset = pBeatmap->notes[currentNote].timestamp;
    while(true){

        if(!pSharedResponse->songStarting) continue;
        
        setLedByArray(grid);
        for(int i = 0; i < 5; i++){
            grid[i] <<= 1;
        }

        // If the timeSinceStart + Offset is larger than or equal to the timestamp of the next note,
        // get the input bit of the newNote and set the bit in the corresponding row based on the input bit colour
        if((timeSinceStart/1000) + timeStampOffset >= nextNoteTimeStamp){
            char newNote = pBeatmap->notes.[newNote_index]->input;
            grid[(int)newNote] += 1;
            newNote_index += 1;
            nextNoteTimeStamp = pBeatmap->notes.[newNote_index]->timestamp;
            setLedByArray(grid);
        }

        // If note was hit, turn off the MSB of the binary literal and update the grid and continue with no delay
        if(pSharedResponse->noteAttemptedLED){
            char targetNote = pBeatmap->notes.[targetNote_index]->input;
            grid[(int)newNote] &= ~(1 << 7);
            setLedByArray(grid);
            pSharedResponse->noteAttemptedLED = false;
            __delay_cycles(1)
        }

        timeSinceStart += cyclesPerLoop;
        __delay_cycles(cyclesPerLoop);
    }

    // turnAllOff();
    cleanUp(grid);
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

void setLED(uint32_t colour){
    for(int i = 23; i >= 0; i--) {
        if(colour & ((uint32_t)0x1 << i)) {
            turnBitOn();
        }else{
            turnBitOff();
        }
    }
}

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


void turnAllOff(void){
    for(int j = 0; j < 64; j++) {
        for(int i = 23; i >= 0; i--) {
            turnBitOff();
        }
    } 
}

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


void cleanUp(unsigned int* grid)
{
    free(grid);

    // Send Reset
    __R30 &= ~(0x1<<DATA_PIN);   // Clear the GPIO pin
    __delay_cycles(cleanupCycles);
    __halt();

}