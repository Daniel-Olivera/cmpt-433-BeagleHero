#include <stdbool.h>

#include "include/guitar.h"
#include "include/beatmap.h"

int main(int argc, char *args[])
{
    Beatmap_init("beatmaps/test.csv");
    Guitar_init();

    while(true){}

    Guitar_cleanup();
    Beatmap_cleanup();
}