#include <stdio.h>
#include <stdbool.h>

#include "include/memoryShare.h"
#include "include/sharedInputStruct.h"

static bool beatmapLoaded = false;

volatile void *pPruBase;
volatile beatmap_t *pBeatmap;

void Beatmap_init(void)
{
    if(beatmapLoaded) {
        printf("The beatmap is already loaded.\n");
        return;
    }

    pPruBase = PRU_getPruMmapAddr();
	pBeatmap = PRUSHARED_MEM_FROM_BASE(pPruBase);
    
    pBeatmap->totalNotes = 1;

    pBeatmap->notes[0].input = 0x30;
    pBeatmap->notes[0].timestamp = 3000;
}

void Beatmap_cleanup(void)
{
    PRU_freePruMmapAddr(pPruBase);
}
