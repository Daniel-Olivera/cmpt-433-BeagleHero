#ifndef _MIDI_PARSE_H_
#define _MIDI_PARSE_H_

#include <smf.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NOTES 1024

typedef struct Note {
	bool active;
	char *note;
	int octave;
	float timeStamp;
} Note;

typedef struct Song{
	Note notes[MAX_NOTES];
	double note_durations[MAX_NOTES];
	int size;
} Song;

// Reads a midi file and returns a Song struct containing:
// an array of Notes, 
// an array of note durations, 
// and the total # of events
Song* midiParser_readMidi(char* filepath);

// Frees the allocated memory for the notes of the song
void midiParser_freeNotes(Song *song);

#endif