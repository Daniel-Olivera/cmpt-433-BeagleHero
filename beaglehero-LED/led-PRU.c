#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "../beaglehero-Linux/include/sharedStructs.h"


#define STR_LEN         64      // # LEDs in our string
#define oneCyclesOn     700/5   // Stay on 700ns
#define oneCyclesOff    800/5
#define zeroCyclesOn    350/5
#define zeroCyclesOff   600/5
#define resetCycles     60000/5 // Must be at least 50u, use 60u

#define delayCycle_ms 30000000

#define Color(r,g,b) ((r&0xFF) << 8 | (g&0xFF) << 16 | (b&0xff))

#define RED (Color(63,0,0))
#define GREEN (Color(0,63,0))
#define BLUE (Color(0,0,63))
#define ORANGE (Color(63,12,0))
#define YELLOW (Color(63,37,0))

#define DATA_PIN 15       // Bit number to output on

#define PRU_SHAREDMEM 0x10000 

#define NOTE_WINDOW_MS 30

volatile register uint32_t __R30;

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
    uint32_t colours[5] = {GREEN,RED,YELLOW,BLUE,ORANGE};
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

void shiftBinary(unsigned int* binary){
    *binary <<= 1;
    return;
}

void turnOnAll(void){
    for(int j = 0; j < 64; j++) {
        for(int i = 23; i >= 0; i--) {
            if(ORANGE & ((uint32_t)0x1 << i)) {
                turnBitOn();
            }else{
                turnBitOff();
            }
        }
    } 
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

unsigned int addToBinary(unsigned int* binary)
{
    *binary = *binary + 1;
}

void cleanUp(unsigned int* grid)
{
    free(grid);

    // Send Reset
    __R30 &= ~(0x1<<DATA_PIN);   // Clear the GPIO pin
    __delay_cycles(resetCycles);
    __halt();

}

void main(void)
{
    // Clear SYSCFG[STANDBY_INIT] to enable OCP master port
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    __delay_cycles(resetCycles);
    unsigned int* grid = initLedGrid();

    setLedByArray(grid);
    int flag = 1;
    while(true){
        setLedByArray(grid);
        for(int i = 0; i < 5; i++){
            shiftBinary(&grid[i]);
        }
        for(int i = 0; i < 5; i++){
            if(flag == 8){
                addToBinary(&grid[i]);
                flag = 1;
            }
            else{
                flag++;
            }
        }
        __delay_cycles(delayCycle_ms);
    }

    // turnAllOff();
    cleanUp(grid);

}