#include <stdint.h>
#include <stdbool.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "../beaglehero-Linux/include/sharedInputStruct.h"

// Reference for shared RAM:
// https://markayoder.github.io/PRUCookbook/05blocks/blocks.html#_controlling_the_pwm_frequency

// GPIO Configuration
// ----------------------------------------------------------
volatile register uint32_t __R30; // output GPIO register
volatile register uint32_t __R31; // input GPIO register

// GPIO Output: P8_12 = pru0_pru_r30_14
// = LEDDP2 (Turn on/off right 14-seg digit) on Zen cape
#define DIGIT_ON_OFF_MASK (1 << 14)

// Shared Memory Configuration
// -----------------------------------------------------------
#define THIS_PRU_DRAM 0x00000 // Address of DRAM
#define OFFSET 0x200 // Skip 0x100 for Stack,
// 0x100 for Heap (from makefile)

#define THIS_PRU_DRAM_USABLE (THIS_PRU_DRAM + OFFSET)

// This works for both PRU0 and PRU1 as both map their own memory to 0x0000000
volatile sharedInputStruct_t *pSharedInputStruct =
    (volatile void *)THIS_PRU_DRAM_USABLE;

void main(void)
{
    // Initialize:
    pSharedInputStruct->songBeginning = 0;
    pSharedInputStruct->inputTimestamp = 0;
    pSharedInputStruct->input = 0x00;
    pSharedInputStruct->newInput = false;

    while (true) {
        if(pSharedInputStruct->newInput) {
            __R30 ^= DIGIT_ON_OFF_MASK;
            pSharedInputStruct->newInput = false;
        }
    // // Sample button state to shared memory
    // pSharedMemStruct->isButtonPressed = (__R31 & JOYSTICK_RIGHT_MASK) != 0;
    }
}
