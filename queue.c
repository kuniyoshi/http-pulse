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
#define COUNT_OF_BUFFERS    (1 << 4)

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
    int count_of_words, buffer_index, signal_index;
    int i;
//    int frequency, bit_rate, channel_number;
    int source_state;
    ALshort signals[COUNT_OF_BUFFERS][BUFFER];
    ALCdevice *device;
    ALCcontext *context;
    ALuint buffers[COUNT_OF_BUFFERS], source;

    buffer_index = 0;
    device       = alcOpenDevice(NULL);
    context      = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    alGenBuffers(COUNT_OF_BUFFERS, buffers);
    alGenSources(1, &source);

    // Read signals from stdin.
    while (fgets(input, INPUT_BUFFER, stdin) != NULL) {
        count_of_words = split_string(input, words);
        signal_index = 0;

        for (i = 0; i < count_of_words; i++) {
//printf("buffer index: %d\n", buffer_index);
//printf("signal index: %d\n", signal_index);
//printf("print data: %s\n", words[i]);
//printf("data: %d\n", atoi(words[i]));
            signals[buffer_index][signal_index++] = (ALshort)atoi(words[i]);
//printf("signal: %d\n", signals[buffer_index][signal_index]);
        }
//puts("foo");

        alBufferData(buffers[buffer_index], AL_FORMAT_MONO16, signals[buffer_index], sizeof(int) * signal_index, SAMPLING_FREQUENCY);
/*
alGetBufferi(buffers[buffer_index], AL_FREQUENCY, &frequency);
printf("buffer freq: %d\n", frequency);
alGetBufferi(buffers[buffer_index], AL_BITS, &bit_rate);
printf("buffer bit_rate: %d\n", bit_rate);
alGetBufferi(buffers[buffer_index], AL_CHANNELS, &channel_number);
printf("buffer channels: %d\n", channel_number);
*/

        buffer_index++;

        if (buffer_index == COUNT_OF_BUFFERS) {
puts("reatch");
            alSourceQueueBuffers(source, (ALsizei)COUNT_OF_BUFFERS, buffers);
            alSourcePlay(source);

            for (alGetSourcei(source, AL_SOURCE_STATE, &source_state); source_state == AL_PLAYING; alGetSourcei(source, AL_SOURCE_STATE, &source_state)) {
                sleep(1);
            }

            alSourceStop(source);
            alSourceUnqueueBuffers(source, COUNT_OF_BUFFERS, buffers);

            buffer_index = 0;
        }
    }

    alDeleteSources(1, &source);
    alDeleteBuffers(COUNT_OF_BUFFERS, buffers);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);

    return 0;
}

/*
    alSourcei(source, AL_BUFFER, buffer);
*/

