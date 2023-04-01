#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "include/memoryShare.h"
#include "include/sharedInputStruct.h"

#define READ_BUFFER_SIZE 100

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
    
    FILE *fileToRead = fopen("beatmaps/test.csv", "r");
    char buffer[READ_BUFFER_SIZE];

    if (fileToRead == NULL) {
        printf("ERROR OPENING %s.", "test.csv");
        exit(1);
    }

    while (fgets(buffer, READ_BUFFER_SIZE, fileToRead) != NULL) {
        printf("%s\n", buffer);
        
        pBeatmap->totalNotes =+ 1;
    }

    fclose(fileToRead);

    // pBeatmap->totalNotes = 1;

    // pBeatmap->notes[0].input = 0x30;
    // pBeatmap->notes[0].timestamp = 3000;
}

void Beatmap_cleanup(void)
{
    PRU_freePruMmapAddr(pPruBase);
}
