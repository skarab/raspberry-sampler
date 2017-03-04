#ifndef __SAMPLER_VOICE__
#define __SAMPLER_VOICE__

#include "includes.h"

class Voice
{
public:

    Voice(string device, unsigned int rate=44100, unsigned int channels=2, unsigned int buffer_size=4096);
    ~Voice();

private:

    static void* _RunThreaded(void* data);

    void _Run();
    void _Create();
    void _Destroy();
    void _Update(snd_pcm_uframes_t frames);

    string _Device;
    unsigned int _Rate;
    unsigned int _Channels;
    unsigned int _BufferSize;
    bool _Quit;
    pthread_t _Thread;
    snd_pcm_t* _PlaybackHandle;
    short* _Buffer;
};


#endif
