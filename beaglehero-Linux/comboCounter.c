#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "include/memoryShare.h"
#include "include/sharedStructs.h"

// volatile void *pPruBase;
// volatile sharedResponseStruct_t *pResponse;

static bool thread_shutdown = false;
static pthread_t combo_pthread;

static uint16_t combo = 0;

static void *comboThreadMain(void *args);

void Combo_init(void)
{
    pthread_create(&combo_pthread, NULL, comboThreadMain, NULL);
}

void Combo_cleanup(void)
{
    thread_shutdown = true;

    pthread_join(combo_pthread, NULL);
}

static void *comboThreadMain(void *args)
{
    volatile void *pPruBase = PRU_getPruMmapAddr();

    volatile sharedResponseStruct_t *pResponse = PRUSHARED_MEM_FROM_BASE(pPruBase);

    while(!thread_shutdown) {
        if(pResponse->newResponse) {
        	if(pResponse->noteHit) {
                combo += 1;
        		printf("Correct, %dx combo!\n", combo);
        	} else {
                combo = 0;
        		printf("Incorrect, %dx combo!\n", combo);
        	}
        	pResponse->newResponse = false;
        }
        if(pResponse->songStarting) {
            combo = 0;
            pResponse->songStarting = false;
        }
    }

    PRU_freePruMmapAddr(pPruBase);

    return NULL;
}