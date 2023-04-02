#include <stdbool.h>

#include "include/comboCounter.h"
#include "include/guitar.h"
#include "include/beatmap.h"

int main(int argc, char *args[])
{
    Combo_init();
    Beatmap_init("beatmaps/test.csv");
    Guitar_init();

    while(true){}

    Guitar_cleanup();
    Beatmap_cleanup();
    Combo_cleanup();
}