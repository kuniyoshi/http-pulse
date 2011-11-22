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
    int count_of_words, buffer_index, signal_index, sleep_usec, count_of_processed, count_of_queued;
    int i, j;
    ALint state;
    ALshort signals[BUFFER];
    ALCdevice *device;
    ALCcontext *context;
    ALuint buffer, buffers[COUNT_OF_BUFFERS], source;

    sleep_usec   = 1000;
//    sleep_usec   = (int)(((1.0 / SAMPLING_FREQUENCY) * MAX_WORDS / 2 ) * 1000 * 1000);
printf("sampling frequency: %d\n", SAMPLING_FREQUENCY);
printf("max words: %d\n", MAX_WORDS);
printf("usleep: %d\n", sleep_usec);
    buffer_index = 0;
    device       = alcOpenDevice(NULL);
    context      = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    alGenBuffers(COUNT_OF_BUFFERS, buffers);
    alGenBuffers(1, &buffer);
    alGenSources(1, &source);

    for (i = 0; i < COUNT_OF_BUFFERS; i++) {
        for (j = 0; j < MAX_WORDS; j++) {
            signals[j] = 0;
        }

        alBufferData(buffers[i], AL_FORMAT_MONO16, signals, MAX_WORDS, SAMPLING_FREQUENCY);
    }

//    alSourceQueueBuffers(source, COUNT_OF_BUFFERS, buffers);

//    alSourcePlay(source);
    alSourceStop(source);
    alGetSourcei(source, AL_SOURCE_STATE, &state);

    // Read signals from stdin.
    while (fgets(input, INPUT_BUFFER, stdin) != NULL) {
        count_of_words = split_string(input, words);
        signal_index = 0;

        for (i = 0; i < count_of_words; i++) {
            signals[signal_index++] = (ALshort)atoi(words[i]);
        }

        alBufferData(buffer, AL_FORMAT_MONO16, signals, sizeof(ALshort) * signal_index, SAMPLING_FREQUENCY);

        alGetSourcei(source, AL_SOURCE_STATE, &state);

        while (state == AL_PLAYING) {
            usleep(sleep_usec);
            alGetSourcei(source, AL_SOURCE_STATE, &state);
        }

        alSourceRewind(source);
        alSourcei(source, AL_BUFFER, buffer);
        alSourcePlay(source);

//        count_of_processed = 0;

/*
        while (! count_of_processed > 0) {
            alGetSourcei(source, AL_BUFFERS_PROCESSED, &count_of_processed);
//printf("%d: \n", count_of_processed);
            usleep(sleep_usec);
        }
*/
//printf("%d: \n", count_of_processed);
/*
        alSourceUnqueueBuffers(source, 1, &buffer);
        alBufferData(buffer, AL_FORMAT_MONO16, signals, sizeof(ALshort) * signal_index, SAMPLING_FREQUENCY);
        alSourceQueueBuffers(source, 1, &buffer);
*/
//        alGetSourcei(source, AL_SOURCE_STATE, &state);

/*
        if (state != AL_PLAYING) {
puts("start plaing");
            alSourceRewind(source);
            alSourcePlay(source);
        }
*/
    }
/*
alGetBufferi(buffers[buffer_index], AL_FREQUENCY, &frequency);
printf("buffer freq: %d\n", frequency);
alGetBufferi(buffers[buffer_index], AL_BITS, &bit_rate);
printf("buffer bit_rate: %d\n", bit_rate);
alGetBufferi(buffers[buffer_index], AL_CHANNELS, &channel_number);
printf("buffer channels: %d\n", channel_number);
*/

//        alSourcePlay(source);

/*
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &count_of_processed);

        while ((count_of_all_buffers - count_of_processed) > (int)(COUNT_OF_BUFFERS - 3)) {
//puts("unqueue is needed");
//        while (is_unqueue_needed(source)) {
            alGetSourcei(source, AL_BUFFERS_QUEUED, &count_of_all_buffers);
            alGetSourcei(source, AL_BUFFERS_PROCESSED, &count_of_processed);
printf("%3d - %3d - %3d\n", buffer_index, count_of_all_buffers, count_of_processed);
//printf("count of all buffers: %d\n", count_of_all_buffers);
//printf("count of processed buffers: %d\n", count_of_processed);

            if (count_of_processed > 0) {
                alSourceUnqueueBuffers(source, count_of_processed, buffers);
            }
            else {
//                usleep(sleep_usec);
            }
        }
        alSourcePlay(source);
*/
/*
        queue_states[buffer_index] = true;

        while (is_unqueue_needed(queue_states)) {
puts("unqueue is needed");
            if (alGetSourcei(source, AL_BUFFER_PROCESSED) > 0) {
            }
            else {
                usleep(sleep_usec);
            }
            unqueued_count = 0;

            for (i = 0; i < COUNT_OF_BUFFERS; i++) {
                alSourceUnqueueBuffers(source, 1, &buffers[i]);

                if (alGetError() != AL_INVALID_VALUE) {
puts("dequeue");
                    queue_states[i] = false;
                    unqueued_count++;
                }
            }

            if (unqueued_count == 0) {
                usleep(sleep_usec);
            }
        }
*/
/*
        if (++buffer_index == COUNT_OF_BUFFERS) {
            buffer_index = 0;
        }
    }
*/
    alSourceStop(source);
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

