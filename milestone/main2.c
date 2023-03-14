#include "server.h"
#include "led.h"
#include <stdio.h>

int main(){
    LED_initialize();
    LED_dimAll();
    Server_start();
    Server_waitForShutdown();

    return 0;
}