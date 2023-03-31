#include "midiParser.h"

/*
    Loading the midi file and reading events is taken from the github page
    in smf.h
    https://github.com/stump/libsmf/blob/master/src/smf.h
*/
Note* midiParser_readMidi(char* filepath)
{
    Note notes[1024];
    smf_t *smf;
 	smf_event_t *event;
 
 	smf = smf_load(filepath);
  	if (smf == NULL) {
  		printf("Couldn't load file: %s", filepath);
  		return 1;
  	}

	char* tokens[16]; //16 was arbitrary, but the smf_event_decode() function only returns a short string
	int notesIndex = 0;

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
			notes[notesIndex].active = true;
		}
        // A note time being "inactive" is when the note ends
		else{
			notes[notesIndex].active = false;
		}

        // Places the note character (i.e., A, B, C, D, etc.)
		notes[notesIndex].note = tokens[5][0];
        // Places the octave (the 4 in C4 for example)
		notes[notesIndex].octave = tokens[5][1] - '0';
        // The current time in the song from 0. Or 'ppos' if you will
		notes[notesIndex].timeStamp = event->time_seconds;

		notesIndex++;

  	}

    // Used for debugging
	// for(int i = 0; i < notesIndex; i++){
	// 	printf("[%d] Active %d, Note %c%d, Timestamp %f\n", i, notes[i].active, notes[i].note, notes[i].octave, notes[i].timeStamp);
	// }

 	smf_delete(smf);
    return notes;
}