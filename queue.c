#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#define INPUT_BUFFER        (1 << 16)
#define BUFFER              (1 << 16)
#define SEP_CHAR            "\t"
#define MAX_WORDS           INPUT_BUFFER
#define SAMPLING_FREQUENCY  44100
#define COUNT               62182844

int
split_string(char *chars, char **words) {
    char *cp;
    int length;

    cp = chars;

    for (length = 0; length < MAX_WORDS; length++) {
        if ((words[length] = strtok(cp, SEP_CHAR)) == NULL) {
            break;
        }

        cp = NULL;
    }

    return length;
}

int
main() {
    char input[INPUT_BUFFER];
    char *words[MAX_WORDS];
    int length, count_of_words;
    int i;
    ALshort *signals;
    ALCdevice *device;
    ALCcontext *context;
    ALuint buffer, source;

    signals = malloc(sizeof(signals) * COUNT);

    length = 0;

    while (fgets(input, INPUT_BUFFER, stdin) != NULL) {
        count_of_words = split_string(input, words);

        for (i = 0; i < count_of_words; i++) {
            signals[length++] = atoi(words[i]);
        }
    }

    device  = alcOpenDevice(NULL);
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    alGenBuffers(1, &buffer);

    alBufferData(buffer, AL_FORMAT_MONO16, signals, length, SAMPLING_FREQUENCY);

    alGenSources(1, &source);

    alSourcei(source, AL_BUFFER, buffer);

    alSourcePlay(source);

    sleep(302);

    alSourceStop(source);

    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);

    return 0;
}

