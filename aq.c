#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#define INPUT_BUFFER        (1 << 16)
#define BUFFER              (1 << 16)
#define MAX_WORDS           9000
#define SEP_CHAR            "\t"
#define SAMPLING_FREQUENCY  44100
#define COUNT_OF_BUFFERS    (1 << 3)

int
split_string(char *chars, char **words) {
    char *cp = chars;
    int length;

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
    int count_of_words, signal_index, sleep_usec, count_of_processed, count_of_queued;
    int i;
    ALint state;
    ALshort signals[BUFFER];
    ALCdevice *device;
    ALCcontext *context;
    ALuint buffer, source;

    sleep_usec = (int)(((1.0 / SAMPLING_FREQUENCY) * MAX_WORDS / 2 ) * 1000 * 1000);
    device     = alcOpenDevice(NULL);
    context    = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    alGenSources(1, &source);
    alSourcei(source, AL_SOURCE_TYPE, AL_STREAMING);

    while (fgets(input, INPUT_BUFFER, stdin) != NULL) {
puts("### got input");
        count_of_words = split_string(input, words);
        signal_index   = 0;

        for (i = 0; i < count_of_words; i++) {
            signals[signal_index++] = (ALshort)atoi(words[i]);
        }

        if (alGetSourcei(source, AL_BUFFERS_QUEUED, &count_of_queued), count_of_queued < COUNT_OF_BUFFERS) {
            alGenBuffers(1, &buffer);
        }
        else {
            while (alGetSourcei(source, AL_BUFFERS_PROCESSED, &count_of_processed), count_of_processed == 0) {
                usleep(sleep_usec);
            }

            for (i = 0; i < count_of_processed; i++) {
                alSourceUnqueueBuffers(source, 1, &buffer);
            }
        }

        alBufferData(buffer, AL_FORMAT_MONO16, signals, sizeof(ALshort) * signal_index, SAMPLING_FREQUENCY);
        alSourceQueueBuffers(source, 1, &buffer);

        if (alGetSourcei(source, AL_SOURCE_STATE, &state), state != AL_PLAYING) {
puts("--- start playing");
            alGetSourcei(source, AL_BUFFERS_PROCESSED, &count_of_processed);
            
            for (i = 0; i < count_of_processed - 1; i++) {
printf("--- unqueue: %d\n", i);
                alSourceUnqueueBuffers(source, 1, &buffer);
            }

            alSourcePlay(source);
        }
    }

    alSourceStop(source);
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);

    return 0;
}

