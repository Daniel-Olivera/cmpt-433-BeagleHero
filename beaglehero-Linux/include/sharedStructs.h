#ifndef _SHARED_INPUT_STRUCT_H_
#define _SHARED_INPUT_STRUCT_H_

#include <stdint.h>

#define MINUS_BIT   7
#define START_BIT   6

#define STRUM_BIT   5

#define GREEN_BIT   4
#define RED_BIT     3
#define YELLOW_BIT  2
#define BLUE_BIT    1
#define ORANGE_BIT  0

#define MINUS_MASK  (1 << MINUS_BIT)
#define START_MASK  (1 << START_BIT)

#define STRUM_MASK  (1 << STRUM_BIT)

#define GREEN_MASK  (1 << GREEN_BIT)
#define RED_MASK    (1 << RED_BIT)
#define YELLOW_MASK (1 << YELLOW_BIT)
#define BLUE_MASK   (1 << BLUE_BIT)
#define ORANGE_MASK (1 << ORANGE_BIT)

/*
 *   uint64_t inputTimestamp
 *   - the time of the input, relative to the start of the song.
 *     This value is reported back by the PRU for testing
 *   unsigned char input
 *   - the actual input, expressed bitwise
 *   bool newInput
 *   - set to true when a new input is written, 
 *     and must be set to false when it is read and reacted to
*/

typedef struct {
    // uint64_t songBeginning; 
    // int32_t inputTimestamp;
    bool songPlaying;
    unsigned char input;
    bool newInput;
    bool freeplayInput;
} sharedInputStruct_t;

typedef struct {
    int32_t currentNoteIndex;
    bool noteHit;
    bool newResponseCombo;
    bool songStarting;
    bool noteAttemptedLED;
} sharedResponseStruct_t;

// WARNING:
// Fields in the struct must be aligned to match ARM's alignment
// bool/char, uint8_t: byte aligned
// int/long, uint32_t: word (4 byte) aligned
// double, uint64_t: dword (8 byte) aligned
// Add padding fields (char _p1) to pad out to alignment.

typedef struct {
    unsigned char input;
    unsigned char _p1, _p2, _p3;
    uint32_t timestamp;
} note_t;

typedef struct {
    uint64_t p1; //padding
    uint32_t totalNotes;
    note_t notes[100];
} beatmap_t;

#endif
