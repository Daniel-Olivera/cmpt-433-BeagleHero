#ifndef _JOYSTICK_H
#define _JOYSTICK_H

typedef enum joystick_direction{
    JOYSTICK_UP,
    JOYSTICK_DOWN,
    JOYSTICK_LEFT,
    JOYSTICK_RIGHT,
} joystick_direction;

#define JOYSTICK_UP_COMMAND "config-pin p8.14 gpio"
#define JOYSTICK_DOWN_COMMAND "config-pin p8.16 gpio"
#define JOYSTICK_LEFT_COMMAND "config-pin p8.18 gpio"
#define JOYSTICK_RIGHT_COMMAND "config-pin p8.15 gpio"
// #define JOYSTICK_CENTER_COMMAND "config-pin p8.17 gpio"

#define JOYSTICK_UP_PATH "/sys/class/gpio/gpio26/value"
#define JOYSTICK_DOWN_PATH "/sys/class/gpio/gpio46/value"
#define JOYSTICK_LEFT_PATH "/sys/class/gpio/gpio65/value"
#define JOYSTICK_RIGHT_PATH "/sys/class/gpio/gpio47/value"
// #define JOYSTICK_CENTER_PATH "/sys/class/gpio/gpio27/value"

#define JOYSTICK_UP_IO "/sys/class/gpio/gpio26/direction"
#define JOYSTICK_DOWN_IO "/sys/class/gpio/gpio46/direction"
#define JOYSTICK_LEFT_IO "/sys/class/gpio/gpio65/direction"
#define JOYSTICK_RIGHT_IO "/sys/class/gpio/gpio47/direction"
// #define JOYSTICK_CENTER_IO "/sys/class/gpio/gpio27/direction"

void Joystick_start(void);
void Joystick_stop(void);
void Joystick_waitForShutdown(void);
joystick_direction Joystick_getInput(void);

#endif