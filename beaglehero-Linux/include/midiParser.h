#ifndef _MIDI_PARSE_H_
#define _MIDI_PARSE_H_

#include <smf.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

typedef struct Note {
	bool active;
	char note;
	int octave;
	float timeStamp;
} Note;

// Reads a midi file and returns an array of Notes as defined above
Note* midiParser_readMidi(char* filepath);

#endif