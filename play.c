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

/*for (i = 0; i < length; i++) {
    printf("%d - %d\n", signals[i], i);
}*/

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
/*
    char read_buffer[BUFFER];
    ALCdevice *device;
    ALCcontext *context;
    ALshort *signals;
    ALuint buffer, source;
    int length = 0;
    int frequency, bit_rate, channel_number;
    int i;
//    length = 0;

printf("buffer: %d\n", BUFFER);

puts("start");

    signals = malloc(sizeof(signals) * LENGHT);

puts("done malloc");

    while (fgets(read_buffer, sizeof(read_buffer), stdin) != NULL) {
        if (length > LENGHT) {
            exit(1);
        }

        signals[length] = atoi(read_buffer);

        length++;
    }

printf("length: %d\n", length);

for (i = 0; i < length; i++) {
//    printf("[%d] - ", signals[i]);
}
puts("");

puts("done signals");

    device  = alcOpenDevice(NULL);
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    alGenBuffers(1, &buffer);

puts("done init audio");

    alBufferData(buffer, AL_FORMAT_MONO16, signals, length, SAMPLING_FREQUENCY);

puts("done prepare buffer");
alGetBufferi(buffer, AL_FREQUENCY, &frequency);
printf("buffer freq: %d\n", frequency);
alGetBufferi(buffer, AL_BITS, &bit_rate);
printf("buffer bit_rate: %d\n", bit_rate);
alGetBufferi(buffer, AL_CHANNELS, &channel_number);
printf("buffer channels: %d\n", channel_number);

    alGenSources(1, &source);

    alSourcei(source, AL_BUFFER, buffer);

puts("start plaing");

    alSourcePlay(source);

puts("now sleeping");

    sleep(302);

puts("done plaing");

    alSourceStop(source);

    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);

    return 0;
}
*/
