#include "client.h"
#include "joystick.h"
#include <stdio.h>

int main(){
    Client_start();
    Joystick_start();

    Client_waitForShutdown();
    Joystick_waitForShutdown();

    return 0;
}