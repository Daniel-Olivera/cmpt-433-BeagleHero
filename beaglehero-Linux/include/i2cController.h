#ifndef _I2C_H_
#define _I2C_H_

//I2C module:
//direct controller for the I2C interface to the Zen Cape's 14-segment display
//provides easy access functions for turning on and off digits, as well as
//accepting DisplayChar's to write to the display.

typedef struct {
    unsigned char upper; //number to write bitwise to the upper part of the display
    unsigned char lower; //number to write bitwise to the lower part of the display
} DisplayChar;

typedef struct {
    DisplayChar leftChar;
    DisplayChar rightChar;
} DisplayValue;

void I2c_init(void);
void I2c_cleanup(void);

void I2c_activateLeftDigit(void);
void I2c_activateRightDigit(void);
void I2c_deactivateLeftDigit(void);
void I2c_deactivateRightDigit(void);
void I2c_deactivateBothDigits(void);

void I2c_writeToDisplay(DisplayChar character);

#endif