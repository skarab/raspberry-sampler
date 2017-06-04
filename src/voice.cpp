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
        for (int i=0 ; i<(int)PARAM_Count ; ++i)
            _Params[i] = _Params[i]*0.999f+_Sample->GetParam((PARAM)i)*0.001f;

        if (_Position<0.0f || _Position>_Sample->GetLength()-1.0f)
        {
            if (_Sample->IsLooping())
            {
                if (_Params[(int)PARAM_PitchFineTune]>0)
                {
                    while (_Position>=_Sample->GetLength())
                        _Position -= _Sample->GetLength();
                }
                else
                {
                    while (_Position<0.0f)
                        _Position += _Sample->GetLength();
                }

                p = (unsigned int)_Position;
                if (p<0) p = 0;
                if (p>_Sample->GetLength()-1) p = _Sample->GetLength()-1;
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
            _Position += _Pitch*pow(2.0f, _Params[(int)PARAM_PitchSemiTone]/12.0f)*(_Params[(int)PARAM_PitchFineTune]/256.0f);

            if (_Sample->GetMode()==MODE_InstruLegato)
            {
                float legato = 0.999f+_Params[(int)PARAM_Legato]*0.0009/64.0f;
                _Pitch = _Pitch*legato+_LegatoPitch*(1.0f-legato);
            }

            float pan = _Params[(int)PARAM_Pan]/32.0f;
            if (pan<0)
            {
                right = (int)(right*(1.0f+pan));
            }
            else
            {
                left = (int)(left*(1.0f-pan));
            }

            float volume = _Params[(int)PARAM_Volume]/64.0f;
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

    if (sample->GetParam(PARAM_PitchFineTune)<0)
        _Position = sample->GetLength()-1;

    _Note = note;
    _Velocity = velocity;
    _LegatoPitch = 1.0f;

    if (sample->IsInstru() && note!=-1)
        _LegatoPitch = pow(2.0f, (note-sample->GetMidiKey().Note)/12.0f);

    if (_Sample==NULL || (sample->GetMode()!=MODE_InstruLegato))
    {
        _Pitch = _LegatoPitch;

        _Params.resize((int)PARAM_Count);
        for (int i=0 ; i<(int)PARAM_Count ; ++i)
            _Params[i] = sample->GetParam((PARAM)i);
    }

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
