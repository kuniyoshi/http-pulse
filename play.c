#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#define BUFFER              65536
#define SAMPLING_FREQUENCY  44100
#define LENGHT              53252540

int
main() {
    char read_buffer[BUFFER];
    ALCdevice *device;
    ALCcontext *context;
    ALshort *signals;
    ALuint buffer, source;
    int i, length;
    length = 0;

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

    for (i = 0; i < LENGHT; i++) {
        printf("%d\n", signals[i]);
    }


/*
puts("done signals");

    device  = alcOpenDevice(NULL);
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    alGenBuffers(1, &buffer);

puts("done init audio");

    alBufferData(buffer, AL_FORMAT_MONO16, signals, sizeof(signals), BUFFER);

puts("done prepare buffer");

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
*/
    return 0;
}

