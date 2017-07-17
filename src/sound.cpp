#include "sound.h"

Sound* Sound::_Instance = NULL;

Sound::Sound()
{
    _Instance = this;

    FiltersGlobal::Initialize();

    for (int i=0 ; i<SAMPLER_CORES ; ++i)
        _Cores.push_back(new Core());
}

Sound::~Sound()
{
    for (int i=0 ; i<_Cores.size() ; ++i)
        delete _Cores[i];
}

void Sound::Play(Sample* sample, int note, int velocity)
{
    if (!sample->IsValid())
        return;

    if (sample->IsMonoVoice())
    {
        for (int i=0 ; i<_Cores.size() ; ++i)
        {
            if (_Cores[i]->PlayMono(sample, note, velocity))
                return;
        }
    }

    for (int i=0 ; i<_Cores.size() ; ++i)
    {
        if (_Cores[i]->Play(sample, note, velocity))
            return;
    }
}

void Sound::Stop(Sample* sample, int note)
{
    if (!sample->IsValid() || !sample->UseRelease())
        return;

    for (int i=0 ; i<_Cores.size() ; ++i)
    {
        if (_Cores[i]->Stop(sample, note))
            return;
    }
}

void Sound::StopAll()
{
    for (int i=0 ; i<_Cores.size() ; ++i)
        _Cores[i]->StopAll();
}

void Sound::OnUnloadBank(Bank& bank)
{
    for (int i=0 ; i<_Cores.size() ; ++i)
        _Cores[i]->OnUnloadBank(bank);
}
