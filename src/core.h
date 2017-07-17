#ifndef __SAMPLER_CORE__
#define __SAMPLER_CORE__

#include "includes.h"
#include "voice.h"
#include "bank.h"

class Core
{
public:

    Core();
    ~Core();

    bool PlayMono(Sample* sample, int note, int velocity);
    bool Play(Sample* sample, int note, int velocity);
    bool Stop(Sample* sample, int note);
    void StopAll();
    void OnUnloadBank(Bank& bank);

private:

    static void* _RunThreaded(void* data);

    void _Run();
    void _Create();
    void _Destroy();
    void _Update(snd_pcm_uframes_t frames);

    static Core* _Instance;

    bool _Ready;
    bool _Quit;
    pthread_t _Thread;
    snd_pcm_t* _PlaybackHandle;
    short int* _Buffer;
    vector<Voice*> _Voices;
    pthread_mutex_t _Lock;
    FiltersGlobal _LeftFilters;
    FiltersGlobal _RightFilters;
};

#endif
