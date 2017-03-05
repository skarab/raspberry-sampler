#include "voice.h"

Voice::Voice() :
    _Sample(NULL),
    _Position(0.0f)
{
}

Voice::~Voice()
{
}

bool Voice::IsPlaying()
{
    return _Sample!=NULL;
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

void Voice::Play(Sample* sample)
{
    if (sample->IsValid())
    {
        _Position = 0.0f;
        _Sample = sample;
    }
}
