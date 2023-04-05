#include <stdbool.h>
#include <stdio.h>
#include "include/freeplay.h"
#include "include/buzzer.h"
#include "include/memoryShare.h"
#include "include/sharedStructs.h"

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
    unsigned char input;
    while(!stopping){

        if(pSharedInput->songPlaying)
            continue;
        
        input = 0;
 
        if(pSharedInput->freeplayInput)
            input = pSharedInput->input;


        if((input & STRUM_MASK) != 0){
            switch (input)
            {
            case GREEN:
                Buzzer_playNote("C", 4, 0.2);
                break;
            case RED:
                Buzzer_playNote("D", 4, 0.2);
                break;
            case YELLOW:
                Buzzer_playNote("E", 4, 0.2);
                break;
            case BLUE:
                Buzzer_playNote("F", 4, 0.2);
                break;
            case ORANGE:
                Buzzer_playNote("G", 4, 0.2);
                break;
            
            default:
                break;
            }

            pSharedInput->freeplayInput = false;
        }
    }
    printf("Freeplay exited.\n"); 
}