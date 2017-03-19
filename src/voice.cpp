#include "voice.h"

Voice::Voice() :
    _Sample(NULL),
    _Position(0.0f)
{
}

Voice::~Voice()
{
}

bool Voice::IsBusy() const
{
    return _Sample!=NULL;
}

bool Voice::IsPlaying(Sample* sample, int note) const
{
    return _Sample==sample && _Note==note;
}

void Voice::Update(int& left, int& right)
{
    unsigned int p = (unsigned int)_Position;
    if (p>=_Sample->GetLength())
    {
        _Sample = NULL;
        left = 0;
        right = 0;
    }
    else
    {
        left = _Sample->GetData()[p*2];
        right = _Sample->GetData()[p*2+1];
        _Position += 1.0f;
    }
}

void Voice::Play(Sample* sample, int note, int velocity)
{
    _Position = 0.0f;
    _Sample = sample;
    _Note = note;
    _Velocity = velocity;
}

void Voice::Stop(Sample* sample, int note)
{
    Stop();
}

void Voice::Stop()
{
    _Sample = NULL;
}
