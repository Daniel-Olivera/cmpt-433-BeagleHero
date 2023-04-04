#include <stdbool.h>

#include "include/i2cDisplay.h"
#include "include/comboCounter.h"
#include "include/beatmap.h"
#include "include/guitar.h"
#include "include/freeplay.h"

int main(int argc, char *args[])
{
    Display_init();
    Combo_init();
    Beatmap_init("beatmaps/test.csv");
    Guitar_init();

    // while(true){}
    freeplay_start();

    Guitar_cleanup();
    Beatmap_cleanup();
    Combo_cleanup();
    Display_cleanup();
}