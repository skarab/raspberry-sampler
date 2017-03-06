#ifndef __SAMPLER_DEVICE__
#define __SAMPLER_DEVICE__

#include "includes.h"
#include "voice.h"

class Device
{
public:

    Device();
    ~Device();

    static Device& Get() { return *_Instance; }

    void OnNoteOn(int device_id, int channel, int note, int velocity);
    void OnNoteOff(int device_id, int channel, int note, int velocity);

    void Play(Sample* sample);

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
};

#endif
