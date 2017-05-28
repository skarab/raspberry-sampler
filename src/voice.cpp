#include "voice.h"

Voice::Voice() :
    _Sample(NULL),
    _Position(0.0f),
    _Left(0.0f),
    _Right(0.0f)
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

    if (_Sample!=NULL)
    {
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
            left = (int)_Sample->GetData()[p*2];
            right = (int)_Sample->GetData()[p*2+1];
            _Position += _Pitch*pow(_Sample->GetParam(PARAM_Pitch)/32.0f, 2.0f);

            if (_Sample->GetMode()==MODE_InstruLegato)
            {
                float legato = 0.999f+_Sample->GetParam(PARAM_Legato)*0.0009/64.0f;
                _Pitch = _Pitch*legato+_LegatoPitch*(1.0f-legato);
            }

            float pan = (_Sample->GetParam(PARAM_Pan)-32.0f)/32.0f;
            if (pan<0)
            {
                right = (int)(right*(1.0f+pan));
            }
            else
            {
                left = (int)(left*(1.0f-pan));
            }

            float volume = _Sample->GetParam(PARAM_Volume)/64.0f;
            left = (int)(left*volume);
            right = (int)(right*volume);
        }
    }

    _Left = _Left*0.9f+left*0.1f;
    _Right = _Right*0.9f+right*0.1f;
    left = (int)_Left;
    right = (int)_Right;
}

void Voice::Play(Sample* sample, int note, int velocity)
{
    _Position = 0.0f;
    _Note = note;
    _Velocity = velocity;
    _LegatoPitch = 1.0f;

    if (sample->IsInstru() && note!=-1)
        _LegatoPitch = pow(2.0f, (note-sample->GetMidiKey().Note)/12.0f);

    if (_Sample==NULL || (sample->GetMode()!=MODE_InstruLegato))
        _Pitch = _LegatoPitch;

    _Sample = sample;
}

void Voice::Stop(Sample* sample, int note)
{
    Stop();
}

void Voice::Stop()
{
    _Sample = NULL;
}
