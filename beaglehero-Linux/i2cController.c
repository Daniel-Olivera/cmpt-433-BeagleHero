#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include "include/fileIO.h"
#include "include/i2cController.h"
// #include "include/display.h"

#define CONFIG_PIN_18 "config-pin P9_18 i2c"
#define CONFIG_PIN_17 "config-pin P9_17 i2c"
#define LEFT_DIGIT_PIN "61"
#define RIGHT_DIGIT_PIN "44"
#define LEFT_DIGIT_DIRECTION "/sys/class/gpio/gpio61/direction"
#define RIGHT_DIGIT_DIRECTION "/sys/class/gpio/gpio44/direction"
#define LEFT_DIGIT_ACTIVE "/sys/class/gpio/gpio61/value"
#define RIGHT_DIGIT_ACTIVE "/sys/class/gpio/gpio44/value"

//#defines for i2c code taken from i2c guide
#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"
#define I2C_DEVICE_ADDRESS 0x20
#define REG_DIRA 0x00
#define REG_DIRB 0x01
#define REG_OUTA 0x14
#define REG_OUTB 0x15

int i2cFile;

static void runCommand(char* command);
static int initI2cBus(char* bus, int address);
static void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);
// static unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr);

void I2c_init(void)
{
    runCommand(CONFIG_PIN_18);
    runCommand(CONFIG_PIN_17);
    File_exportPin(LEFT_DIGIT_PIN);
    File_exportPin(RIGHT_DIGIT_PIN);

    File_writeFile(LEFT_DIGIT_DIRECTION, "out");
    File_writeFile(RIGHT_DIGIT_DIRECTION, "out");

    File_writeFile(LEFT_DIGIT_ACTIVE, "0");
    File_writeFile(RIGHT_DIGIT_ACTIVE, "0");

    i2cFile = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);

    writeI2cReg(i2cFile, REG_DIRA, 0x00);
    writeI2cReg(i2cFile, REG_DIRB, 0x00);
}

void I2c_cleanup(void)
{
    close(i2cFile);
}

void I2c_activateLeftDigit(void)
{
    File_writeFile(LEFT_DIGIT_ACTIVE, "1");
}

void I2c_activateRightDigit(void)
{
    File_writeFile(RIGHT_DIGIT_ACTIVE, "1");
}

void I2c_deactivateLeftDigit(void)
{
    File_writeFile(LEFT_DIGIT_ACTIVE, "0");
}

void I2c_deactivateRightDigit(void)
{
    File_writeFile(RIGHT_DIGIT_ACTIVE, "0");
}

void I2c_deactivateBothDigits(void) 
{
    File_writeFile(LEFT_DIGIT_ACTIVE, "0");
    File_writeFile(RIGHT_DIGIT_ACTIVE, "0");
}


void I2c_writeToDisplay(DisplayChar character) 
{
    writeI2cReg(i2cFile, REG_OUTB, character.upper);
    writeI2cReg(i2cFile, REG_OUTA, character.lower);
}

//Taken from Assignment 1 by Brian Fraser
static void runCommand(char* command)
{
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");

    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
        // printf("--> %s", buffer); // Uncomment for debugging
    }

    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}

//the following functions taken from I2C Guide by Brian Fraser
static int initI2cBus(char* bus, int address)
{
    int i2cFileDesc = open(bus, O_RDWR);
    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if (result < 0) {
        perror("I2C: Unable to set I2C device to slave address.");
        exit(1);
    }
    return i2cFileDesc;
}

static void writeI2cReg(int i2cFileDesc, unsigned char regAddr,
unsigned char value)
{
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int res = write(i2cFileDesc, buff, 2);
    if (res != 2) {
        perror("I2C: Unable to write i2c register.");
        exit(1);
    }
}

// static unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr)
// {
//     // To read a register, must first write the address
//     int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
//     if (res != sizeof(regAddr)) {
//         perror("I2C: Unable to write to i2c register.");
//         exit(1);
//     }
//     // Now read the value and return it
//     char value = 0;
//     res = read(i2cFileDesc, &value, sizeof(value));
//     if (res != sizeof(value)) {
//         perror("I2C: Unable to read from i2c register");
//         exit(1);
//     }
//     return value;
// }