#include "include/midiParser.h"

/*
    Loading the midi file and reading events is taken from the github page
    in smf.h
    https://github.com/stump/libsmf/blob/master/src/smf.h
*/

// Reference from smf_decode.c https://libsmf.sourceforge.net/api/
static void note_from_int(char *buf, int note_number){
    int songNote, octave;
    char *names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

    octave = note_number / 12 - 1;
    songNote = note_number % 12;

    sprintf(buf, "%s %d", names[songNote], octave);
}

Song* midiParser_readMidi(char* filepath)
{
    Song *song = malloc(sizeof(Song));
	song->size = 0;
    smf_t *smf;
 	smf_event_t *event;
 
 	smf = smf_load(filepath);
  	if (smf == NULL) {
  		printf("Couldn't load file: %s", filepath);
  		exit(-1);
  	}

	char* tokens[16]; //16 was arbitrary, but the smf_event_decode() function only returns a short string
	int idxOn = 0;
	int idxOff = 0;
	double active_note_timestamps[MAX_NOTES];
	double inactive_note_timestamps[MAX_NOTES];
	int octv = 0;
	char songNote[5];
  	while ((event = smf_get_next_event(smf)) != NULL) {
 		if (smf_event_is_metadata(event))
 			continue;
  
		char* eventBuffer = smf_event_decode(event);

		int tokenIndex = 0;
		char* token = strtok(eventBuffer, ", ");

        // Splitting the event string into useful bits
		while (token != NULL) {
			tokens[tokenIndex] = token;
			tokenIndex++;
			token = strtok(NULL, ", ");
    	}

        // If the event does not contain information about a note, skip it
		if(strcmp(tokens[0], "Note")){
			continue;
		}

        // A note time being "active" is when the note starts
		if(strcmp(tokens[1], "On") == 0){
			song->notes[song->size].active = true;
		}
        // A note time being "inactive" is when the note ends
		else{
			song->notes[song->size].active = false;
		}

		char *noteStore = malloc(sizeof(char) * 2);

		// Reference from smf_decode.c https://libsmf.sourceforge.net/api/
		note_from_int(songNote, event->midi_buffer[1]);
		sscanf(songNote, "%s %d", noteStore, &octv);

        // Places the note character (i.e., A, B, C, D, etc.)
		song->notes[song->size].note = noteStore;
        // Places the octave (the 4 in C4 for example)
		song->notes[song->size].octave = octv;

        // The current time in the song from 0. Or 'ppos' if you will
		song->notes[song->size].timeStamp = event->time_seconds;

		// Store the note durations 
		if(strcmp(tokens[1], "On") == 0){
			active_note_timestamps[idxOn] = event->time_seconds;
			idxOn++;
		}
        // A note time being "inactive" is when the note ends
		else{
			inactive_note_timestamps[idxOff] = event->time_seconds;
			idxOff++;
		}

		song->size++;
  	}

	// Fill the note_durations array for our song
	for(int i = 0; i < idxOn; i++){
		song->note_durations[i] = inactive_note_timestamps[i]-active_note_timestamps[i];
	}

    // Used for debugging
	// for(int i = 0; i < song->size; i++){
    //     printf("[%d] Active %d, Note %s%d, Timestamp %f\n", i, song->notes[i].active, song->notes[i].note, song->notes[i].octave, song->notes[i].timeStamp);
    // }

 	smf_delete(smf);
    return song;
}

void midiParser_freeNotes(Song *song){
	for(int i = 0; i < song->size; i++){
		free(song->notes[i].note);
	}
}