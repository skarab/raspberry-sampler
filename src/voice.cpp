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

bool Voice::IsPlaying(Sample* sample) const
{
    return _Sample==sample;
}

bool Voice::IsPlaying(Sample* sample, int note) const
{
    return _Sample==sample && _Note==note;
}

void Voice::Update(int& left, int& right)
{
    unsigned int p = (unsigned int)_Position;
    if (p<0.0f || p>=_Sample->GetLength())
    {
        if (_Sample->IsLooping())
        {
            while (p>=_Sample->GetLength())
            {
                _Position -= _Sample->GetLength();
                if (_Position<0.0f)
                    _Position = 0.0f;
                p = (unsigned int)_Position;
            }

            while (p<0.0f)
            {
                _Position += _Sample->GetLength();
                if (_Position>_Sample->GetLength()-1)
                    _Position = _Sample->GetLength()-1;
                p = (unsigned int)_Position;
            }
        }
        else
        {
            _Sample = NULL;
            left = 0;
            right = 0;
        }
    }

    if (_Sample!=NULL)
    {
        left = ((int)_Sample->GetData()[p*2])*_Sample->GetParam(PARAM_Volume)/255;
        right = ((int)_Sample->GetData()[p*2+1])*_Sample->GetParam(PARAM_Volume)/255;
        _Position += _Pitch*_Sample->GetParam(PARAM_Pitch)/64.0f;
    }
}

void Voice::Play(Sample* sample, int note, int velocity)
{
    _Position = 0.0f;
    _Sample = sample;
    _Note = note;
    _Velocity = velocity;
    _Pitch = 1.0f;

    if (sample->IsInstru() && note!=-1)
        _Pitch = pow(2.0f, (note-_Sample->GetMidiKey().Note)/12.0f);
}

void Voice::Stop(Sample* sample, int note)
{
    Stop();
}

void Voice::Stop()
{
    _Sample = NULL;
}
