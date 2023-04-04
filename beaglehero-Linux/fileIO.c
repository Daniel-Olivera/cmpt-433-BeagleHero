#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/timing.h"

#define UNEXPORT_FILE "/sys/class/gpio/unexport"
#define EXPORT_FILE "/sys/class/gpio/export"
#define EXPORT_WAIT 300

void File_writeFile(char* filename, char* value) 
{
    FILE *fileToWrite = fopen(filename, "w");

    if (fileToWrite == NULL) {
        printf("ERROR OPENING %s.", filename);
        exit(1);
    }

    int charWritten = fprintf (fileToWrite, value);
    if (charWritten <= 0) {
        printf("ERROR WRITING");
        exit(1);
    }

    fclose(fileToWrite);
}

void File_readFile(char* filename, char* buffer, int bufferSize)
{
    FILE *fileToRead = fopen(filename, "r");

    if (fileToRead == NULL) {
        printf("ERROR OPENING %s.", filename);
        exit(1);
    }

    fgets(buffer, bufferSize, fileToRead);

    fclose(fileToRead);
}

void File_exportPin(char* pinName)
{

    FILE *unexportFile = fopen(UNEXPORT_FILE, "w");

    if (unexportFile == NULL) {
        printf("ERROR OPENING %s.", UNEXPORT_FILE);
        exit(1);
    }

    fprintf(unexportFile, pinName);
    fclose(unexportFile);
    
    //add a check for if the pin is already exported
    FILE *fileToWrite = fopen(EXPORT_FILE, "w");

    if (fileToWrite == NULL) {
        printf("ERROR OPENING %s.", EXPORT_FILE);
        exit(1);
    }

    //no error check because, as mentioned in the GPIO guide
    //the pin may already be exported 
    fprintf (fileToWrite, pinName);

    fclose(fileToWrite);
    Timer_sleepForMs(EXPORT_WAIT);
}

//based off code found in A2D Guide by Brian Fraser
int File_getReading(char* filename)
{
    FILE *fileToRead = fopen(filename, "r");

    if (fileToRead == NULL) {
        printf("ERROR OPENING %s.", filename);
        exit(1);
    }

    int reading = 0;
    int itemsRead = fscanf(fileToRead, "%d", &reading);
    if(itemsRead <= 0) {
        printf("ERROR: unable to read values from File");
        exit(-1);
    }

    fclose(fileToRead);

    return reading;
}