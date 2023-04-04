#ifndef _DISPLAY_H_
#define _DISPLAY_H_

//Display Module:
//creates a module which controls the Zen Cape's 14 segment display.
//internally handles questions of timing, and provides a simple
//setter function to update the value displayed.

//Provides the DisplayChar struct for direct driving to the I2C bus.
//A displayChar holds the pair of bitwise values required to drive the
//display.



void Display_init(void);
void Display_cleanup(void);

void Display_updateInteger(unsigned int output);

#endif