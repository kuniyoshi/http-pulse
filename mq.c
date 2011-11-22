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

void
sort_buffers(ALuint *buffers, int count_of_queued, int count_of_processed) {
    ALuint buffer;
    int i, j;

    for (i = 0; i < count_of_processed; i++) {
        for (j = 1; j < COUNT_OF_BUFFERS; j++) {
            buffer         = buffers[j - 1];
            buffers[j - 1] = buffers[j];
            buffers[j]     = buffer;
        }
    }
}

int
main() {
    char input[INPUT_BUFFER];
    char *words[MAX_WORDS];
    int count_of_words, buffer_index, signal_index, sleep_usec, count_of_processed, count_of_queued;
    int i;
    ALint state;
    ALshort signals[BUFFER];
    ALCdevice *device;
    ALCcontext *context;
    ALuint buffer, buffers[COUNT_OF_BUFFERS], source;

    sleep_usec   = (int)(((1.0 / SAMPLING_FREQUENCY) * MAX_WORDS / 2 ) * 1000 * 1000);
    buffer_index = 0;
    device       = alcOpenDevice(NULL);
    context      = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    alGenBuffers(COUNT_OF_BUFFERS, buffers);
    alGenBuffers(1, &buffer);
    alGenSources(1, &source);

    while (fgets(input, INPUT_BUFFER, stdin) != NULL) {
puts("### got input");
        count_of_words = split_string(input, words);
        signal_index   = 0;

        for (i = 0; i < count_of_words; i++) {
            signals[signal_index++] = (ALshort)atoi(words[i]);
        }

        alGetSourcei(source, AL_BUFFERS_QUEUED, &count_of_queued);
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &count_of_processed);

        while (count_of_queued - count_of_processed >= COUNT_OF_BUFFERS) {
printf("--- sleeping with: %d - %d\n", count_of_queued, count_of_processed);
            usleep(sleep_usec);
            alGetSourcei(source, AL_BUFFERS_PROCESSED, &count_of_processed);
        }

        alGetSourcei(source, AL_BUFFERS_QUEUED, &count_of_queued);
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &count_of_processed);

        sort_buffers(buffers, count_of_queued, count_of_processed);

        for (i = 0; i < count_of_processed; i++) {
printf("--- dequeueing: %d\n", i);
            alSourceUnqueueBuffers(source, 1, &buffer);
        }

        alBufferData(buffer, AL_FORMAT_MONO16, signals, sizeof(ALshort) * signal_index, SAMPLING_FREQUENCY);
puts("--- build buffer");
        buffers[count_of_queued - count_of_processed] = buffer;
        alSourceQueueBuffers(source, 1, &buffer);

        if (alGetSourcei(source, AL_SOURCE_STATE, &state), state != AL_PLAYING) {
puts("!!! start playing");
            alSourcePlay(source);
        }
    }

    alSourceStop(source);
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alDeleteBuffers(COUNT_OF_BUFFERS, buffers);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);

    return 0;
}

