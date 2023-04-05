#include <stdbool.h>

#include "include/i2cDisplay.h"
#include "include/comboCounter.h"
#include "include/beatmap.h"
#include "include/guitar.h"
#include "include/freeplay.h"
#include "include/midiParser.h"
#include <stdio.h>

int main(int argc, char *args[])
{
    Display_init();
    Combo_init();
    Beatmap_init("beatmaps/twinkle.csv");
    Guitar_init();

    freeplay_start();
 
    Guitar_cleanup();
    Beatmap_cleanup();
    Combo_cleanup();
    Display_cleanup();
}