#include <stdbool.h>

#include "include/guitar.h"

int main(int argc, char *args[])
{
    Guitar_init();

    while(true){}

    Guitar_cleanup();
}