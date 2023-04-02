#include <stdbool.h>
#include <stdio.h>
#include "include/guitar.h"
#include "include/beatmap.h"
#include "include/freeplay.h"

int main(int argc, char *args[])
{
    Beatmap_init();
    Guitar_init();

    // while(true){}
    freeplay_start();

    Guitar_cleanup();
    Beatmap_cleanup();
}