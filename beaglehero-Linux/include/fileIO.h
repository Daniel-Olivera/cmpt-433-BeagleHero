#ifndef FILE_H_
#define FILE_H_

//FileIO module:
//a collection of file methods in one place. 
//Typically used for GPIO pins, and has some
//shortcut functions (exportPin() and getReading())
//to simplify this use.

void File_writeFile(char* filename, char* value);

void File_readFile(char* filename, char* buffer, int bufferSize);

void File_exportPin(char* pinName);

int File_getReading(char* filename);

#endif