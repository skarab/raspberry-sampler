#ifndef __SAMPLER_DEVICE__
#define __SAMPLER_DEVICE__

#include "includes.h"
#include "voice.h"
#include "bank.h"

class Device
{
public:

    Device();
    ~Device();

    static Device& Get() { return *_Instance; }

    void Play(Sample* sample, int note, int velocity);
    void Stop(Sample* sample, int note);
    void StopAll();
    void OnUnloadBank(Bank& bank);

private:

    static void* _RunThreaded(void* data);

    void _Run();
    void _Create();
    void _Destroy();
    void _Update(snd_pcm_uframes_t frames);

    static Device* _Instance;

    bool _Ready;
    bool _Quit;
    pthread_t _Thread;
    snd_pcm_t* _PlaybackHandle;
    short* _Buffer;
    vector<Voice*> _Voices;
    pthread_mutex_t _Lock;
    float _Left;
    float _Right;
};

#endif
