#include "buzzer.h"
#include "midiParser.h"
#include <stdlib.h>
#include <time.h>

static char note_period_frequencies[NOTES][OCTAVES][FREQ_LEN] = {
    {"61162080", "30581040", "15288182", "7644675", "3822192", "1911132", "955566", "477783", "238891"}, // C
    {"57736721", "28860029", "14430014", "7215528", "3607764", "1803849", "901933", "450966", "225483"}, // C#
    {"54495913", "27240534", "13620267", "6810597", "3405299", "1702620", "851310", "425655", "212828"}, // D
    {"51413882", "25713551", "12856776", "6428388", "3214091", "1607071", "803529", "401765", "200883"}, // D#
    {"48543689", "24271845", "12134450", "6067593", "3033704", "1516875", "758432", "379216", "189608"}, // E
    {"45808520", "22909507", "11453442", "5727049", "2863442", "1431721", "715866", "357932", "178966"}, // F
    {"43252595", "21621622", "10810811", "5405405", "2702776", "1351370", "675685", "337842", "168921"}, // F#
    {"40816327", "20408163", "10204082", "5102041", "2551020", "1275526", "637763", "318882", "159441"}, // G
    {"38520801", "19264111", "9631128", "4815796", "2407898", "1203934", "601967", "300984", "150492"}, // G#
    {"36363636", "18181818", "9090909", "4545455", "2272727", "1136364", "568182", "284091", "142045"}, // A
    {"34317090", "17161490", "8580745", "4290372", "2145186", "1072582", "536291", "268146", "134073"}, // A#
    {"32393910", "16196955", "8099133", "4049567", "2024783", "1012381", "506193", "253096", "126548"}, // B
};

static char note_duty_frequencies[NOTES][OCTAVES][FREQ_LEN] = {
    {"30581040", "15290520", "7644091", "3822338", "1911096", "955566", "477783", "238892", "119446"}, // C
    {"28868360", "14430014", "7215007", "3607764", "1803882", "901924", "450966", "225483", "112742"}, // C#
    {"27247956", "13620267", "6810134", "3405298", "1702650", "851310", "425655", "212828", "106414"}, // D 
    {"25706941", "12856776", "6428388", "3214194", "1607046", "803536", "401764", "200882", "100442"}, // D#
    {"24271844", "12135922", "6067225", "3033796", "1516852", "758438", "379216", "189608", "94804"}, // E
    {"22904260", "11454754", "5726721", "2863524", "1431721", "715860", "357933", "178966", "89483"}, // F
    {"21626298", "10810811", "5405406", "2702702", "1351388", "675685", "337842", "168921", "84460"}, // F#
    {"20408164", "10204082", "5102041", "2551020", "1275510", "637763", "318882", "159441", "79720"}, // G
    {"19260400", "9632056", "4815564", "2407898", "1203949", "601967", "300984", "150492", "75246"}, // G#
    {"18181818", "9090909", "4545454", "2272728", "1136364", "568182", "284091", "142046", "71022"}, // A
    {"17158545", "8580745", "4290372", "2145186", "1072593", "536291", "268146", "134073", "67036"}, // A#
    {"16196955", "8098478", "4049566", "2024784", "1012392", "506190", "253096", "126548", "63274"}, // B
};

static void sleepForMs(long long delayInMs){
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;

    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;

    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

static int int_from_note(char *note){
    if(strcmp(note, "C") == 0){
        return 0;
    }
    else if(strcmp(note, "C#") == 0){
        return 1;
    }
    else if(strcmp(note, "D") == 0){
        return 2;
    }
    else if(strcmp(note, "D#") == 0){
        return 3;
    }
    else if(strcmp(note, "E") == 0){
        return 4;
    }
    else if(strcmp(note, "F") == 0){
        return 5;
    }
    else if(strcmp(note, "F#") == 0){
        return 6;
    }
    else if(strcmp(note, "G") == 0){
        return 7;
    }
    else if(strcmp(note, "G#") == 0){
        return 8;
    }
    else if(strcmp(note, "A") == 0){
        return 9;
    }
    else if(strcmp(note, "A#") == 0){
        return 10;
    }
    else if(strcmp(note, "B") == 0){
        return 11;
    }

    return -1;
}

void Buzzer_configurePin(char* command){
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");

    // Ignore output of the command; but consume it
    // so we don"t get an error when closing the pipe
    char buffer[1024];
    while(!feof(pipe) && !ferror(pipe)){
        if(fgets(buffer, sizeof(buffer), pipe) == NULL){
            break;
        }
    }

    int exitCode = WEXITSTATUS(pclose(pipe));
    if(exitCode != 0){
        perror("Unable to execute command:");
        printf("    command:    %s\n", command);
        printf("    exit code:  %d\n", exitCode);
    }    
}

// Echos 'input' to the Buzzer's appropriate 'fileName'
void Buzzer_configure(char *fileName, char *input){
    FILE *pBuzzerFile = fopen(fileName, "w");

    if(pBuzzerFile == NULL){
        printf("ERROR OPENING %s.\n", fileName);
        exit(-1);
    }

    int charWritten = fprintf(pBuzzerFile, input);
    if(charWritten <= 0){
        printf("ERROR WRITING DATA\n");
        exit(-1);
    }

    fclose(pBuzzerFile);
}

void Buzzer_playNote(char *note, int octave, double duration){
    int convert_note = int_from_note(note);
    char *period = note_period_frequencies[convert_note][octave];
    char *duty_cycle = note_duty_frequencies[convert_note][octave];
    duration = duration * 1000;
    
    Buzzer_configure(BUZZER_DUTY_CYCLE, "0");
    Buzzer_configure(BUZZER_PERIOD, period);
    Buzzer_configure(BUZZER_DUTY_CYCLE, duty_cycle);
    Buzzer_configure(BUZZER_ENABLE, "1");

    sleepForMs(duration);
    Buzzer_configure(BUZZER_ENABLE, "0");
}

/*

Example program of using the buzzer with MIDI:


int main(){

    Song* song = midiParser_readMidi(MY_MIDI);

    for(int i = 0; i < song->size; i++){
        printf("[%d] Active %d, Note %s%d, Timestamp %f\n", i, song->notes[i].active, song->notes[i].note, song->notes[i].octave, song->notes[i].timeStamp);
    }

    int duration_idx = 0;
    for(int i = 0; i < song->size; i++){
        if(song->notes[i].active){
            Buzzer_playNote(song->notes[i].note, song->notes[i].octave, song->note_durations[duration_idx]);
            duration_idx++;
        }
        else{
            continue;
        }
    }

    midiParser_freeNotes(song);
    free(song);
    return 0;
}

*/