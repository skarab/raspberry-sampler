#ifndef __SAMPLER_DEVICE__
#define __SAMPLER_DEVICE__

#include "includes.h"
#include "voice.h"

class Device
{
public:

    Device(string path, unsigned int rate, unsigned int channels, unsigned int buffer_size, unsigned int voices);
    ~Device();

    static Device& Get() { return *_Instance; }

private:

    static void* _RunThreaded(void* data);

    void _Run();
    void _Create();
    void _Destroy();
    void _Update(snd_pcm_uframes_t frames);

    static Device* _Instance;

    string _Path;
    unsigned int _Rate;
    unsigned int _Channels;
    unsigned int _BufferSize;
    bool _Quit;
    pthread_t _Thread;
    snd_pcm_t* _PlaybackHandle;
    short* _Buffer;
    unsigned int _VoicesCount;
    vector<Voice*> _Voices;
};

#endif
