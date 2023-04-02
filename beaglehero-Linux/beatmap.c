#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

    pBeatmap->totalNotes =+ 0;
    while (fgets(buffer, READ_BUFFER_SIZE, fileToRead) != NULL) {
        printf("%s\n", buffer);
        char *input = strtok(buffer, ",");
        pBeatmap->notes[0].input = atoi(input);

        input = strtok(buffer, ",");
        pBeatmap->notes[0].timestamp = atol(input);

        pBeatmap->totalNotes =+ 1;
    }

    fclose(fileToRead);
}

void Beatmap_cleanup(void)
{
    PRU_freePruMmapAddr(pPruBase);
}
