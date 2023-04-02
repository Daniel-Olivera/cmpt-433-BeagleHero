#include <stdbool.h>
#include <stdio.h>
#include "include/freeplay.h"
#include "include/buzzer.h"
#include "include/memoryShare.h"
#include "include/sharedInputStruct.h"

#define GREEN   48
#define RED     40
#define YELLOW  36
#define BLUE    34
#define ORANGE  33

bool stopping = false;

void freeplay_start(void)
{
    volatile void *pPruBase = PRU_getPruMmapAddr();
	volatile sharedInputStruct_t *pSharedInput = PRU0_MEM_FROM_BASE(pPruBase);
    Buzzer_configurePin(BUZZER_COMMAND);
    Buzzer_start();
    pSharedInput->input = 0;
    unsigned char input;
    while(!stopping){
        input = pSharedInput->input;

        if((input == MINUS_MASK)){
            freeplay_end();
        }

        if((input & STRUM_MASK) != 0){
            switch (input)
            {
            case GREEN:
                pSharedInput->input = 0;
                Buzzer_playNote("C", 4, 0.2);
                break;
            case RED:
                pSharedInput->input = 0;
                Buzzer_playNote("D", 4, 0.2);
                break;
            case YELLOW:
                pSharedInput->input = 0;
                Buzzer_playNote("E", 4, 0.2);
                break;
            case BLUE:
                pSharedInput->input = 0;
                Buzzer_playNote("F", 4, 0.2);
                break;
            case ORANGE:
                pSharedInput->input = 0;
                Buzzer_playNote("G", 4, 0.2);
                break;
            
            default:
                break;
            }
        }
        
    }
    printf("Freeplay exited.\n"); 
}

void freeplay_end(void)
{
    stopping = true;
}