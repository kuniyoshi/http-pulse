#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#define INTERVAL        1
#define SOUND_BUFFER    262144
#define VOLUME          32767
#define FREQUENCY_OF_LA 440
#define FREQUENCY       3 * FREQUENCY_OF_LA

int main() {
    ALCdevice *device;
    ALCcontext *context;
    ALshort sound[SOUND_BUFFER];
    ALuint buffer, source;
    int i;

    device  = alcOpenDevice(NULL);
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    alGenBuffers(1, &buffer);

    for (i = 0; i < SOUND_BUFFER; i++) {
        sound[i] = VOLUME * sin(2 * M_PI * FREQUENCY * i / SOUND_BUFFER);
    }

    alBufferData(buffer, AL_FORMAT_MONO16, sound, sizeof(sound), SOUND_BUFFER);

    alGenSources(1, &source);

    alSourcei(source, AL_BUFFER, buffer);

    alSourcePlay(source);

    sleep(10);

    alSourceStop(source);

    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);

    return 0;
}


