#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#define SAMPLING_FREQUENCY  44100
#define FREQUENCY_OF_LA     440
#define VOLUME              ((1 << 14) - 1)

int main() {
    ALCdevice *device;
    ALCcontext *context;
    ALshort signals[SAMPLING_FREQUENCY];
    ALuint buffer, source, other_source;
    int i;

    device  = alcOpenDevice(NULL);
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    alGenBuffers(1, &buffer);

    for (i = 0; i < SAMPLING_FREQUENCY; i++) {
        signals[i] = VOLUME * sin(2 * M_PI * FREQUENCY_OF_LA * i / SAMPLING_FREQUENCY);
    }

    alBufferData(buffer, AL_FORMAT_MONO16, signals, sizeof(signals), SAMPLING_FREQUENCY);
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);

    alSourcePlay(source);
puts("source");

    sleep(1);

    alSourceStop(source);
    alDeleteSources(1, &source);

    for (i = 0; i < SAMPLING_FREQUENCY; i++) {
        signals[i] = VOLUME * sin(2 * M_PI * FREQUENCY_OF_LA * 2 * i / SAMPLING_FREQUENCY);
    }

    alBufferData(buffer, AL_FORMAT_MONO16, signals, sizeof(signals), SAMPLING_FREQUENCY);
    alGenSources(1, &other_source);
    alSourcei(other_source, AL_BUFFER, buffer);

    alSourcePlay(other_source);
puts("other source");

    sleep(1);

    alSourceStop(other_source);
    alDeleteSources(1, &other_source);

    alDeleteBuffers(1, &buffer);

    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);

    return 0;
}

