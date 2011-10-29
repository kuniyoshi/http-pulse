#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>//sleep関数用

int main(){
   ALCdevice *device;
   ALCcontext *context;
   ALshort data[22050];
   ALuint buffer, source;
   int i;

   //デバイスを開く
   device = alcOpenDevice(NULL);
   //コンテキストを生成
   context = alcCreateContext(device, NULL);
   //使用するコンテキストの指定
   alcMakeContextCurrent(context);
   //バッファの生成
   alGenBuffers(1, &buffer);

   //信号を生成する
   for (i = 0; i < 22050; ++i) {
       data[i] = sin(i * 3.14159 * 2 * 440 / 22050) * 32767;
   }
   //信号をバッファに入れる
   alBufferData(buffer, AL_FORMAT_MONO16, data, sizeof(data), 22050);
   //ソースを生成
   alGenSources(1, &source);
   //バッファからソースを作る
   alSourcei(source, AL_BUFFER, buffer);
   //ソースを再生する
   alSourcePlay(source);
   sleep(1);

   //お片づけ
   alSourceStop(source);
   alDeleteSources(1, &source);
   alDeleteBuffers(1, &buffer);
   alcMakeContextCurrent(NULL);
   alcDestroyContext(context);
   alcCloseDevice(device);
   return 0;
}

