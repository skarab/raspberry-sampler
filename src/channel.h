#ifndef __SYNTH_CHANNEL__
#define __SYNTH_CHANNEL__

#include "includes.h"

class Channel
{
public:

    Channel(string device, unsigned int rate=44100, unsigned int channels=2, unsigned int buffer_size=4096);
    ~Channel();

    bool HasError() const { return _Error; }

private:

    bool _Update(snd_pcm_uframes_t frames);
    void _Run();
    bool _Create();
    void _Destroy();

    static void* _RunThreaded(void* data);

    string _Device;
    unsigned int _Rate;
    unsigned int _Channels;
    unsigned int _BufferSize;
    bool _Error;
    bool _Quit;
    pthread_t _Thread;
    snd_pcm_t* _PlaybackHandle;
    short* _Buffer;
};


#endif
