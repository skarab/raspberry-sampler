#ifndef __SAMPLER_SOUND__
#define __SAMPLER_SOUND__

#include "includes.h"
#include "core.h"

class Sound
{
public:

    Sound();
    ~Sound();

    static Sound& Get() { return *_Instance; }

    void Play(Sample* sample, int note, int velocity);
    void Stop(Sample* sample, int note);
    void StopAll();
    void OnUnloadBank(Bank& bank);

private:

    static Sound* _Instance;

    vector<Core*> _Cores;
};

#endif
