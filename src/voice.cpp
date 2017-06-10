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
    if (_Sample!=NULL)
    {
        for (int i=0 ; i<(int)PARAM_Count ; ++i)
            _Params[i] = _Params[i]*0.999f+_Sample->GetParam((PARAM)i)*0.001f;

        float start = _Params[(int)PARAM_Start]*(_Sample->GetLength()-1.0f)/1024.0f;
        float stop = _Params[(int)PARAM_Stop]*(_Sample->GetLength()-1.0f)/1024.0f;
        if (stop<start)
            stop = start;

        float clamp_position = _Position;
        if (clamp_position<start) clamp_position = start;
        if (clamp_position>stop) clamp_position = stop;

        int p = (int)clamp_position;
        left = (int)_Sample->GetData()[p*2];
        right = (int)_Sample->GetData()[p*2+1];

        _Position += _Pitch*pow(2.0f, _Params[(int)PARAM_PitchSemiTone]/12.0f)*(_Params[(int)PARAM_PitchFineTune]/512.0f);

        if (_Sample->GetMode()==MODE_InstruLegato)
        {
            float legato = 0.999f+_Params[(int)PARAM_Legato]*0.0009/64.0f;
            _Pitch = _Pitch*legato+_LegatoPitch*(1.0f-legato);
        }

        float pan = _Params[(int)PARAM_Pan]/32.0f;
        if (pan<0) right = (int)(right*(1.0f+pan));
        else left = (int)(left*(1.0f-pan));

        float volume = _Params[(int)PARAM_Volume]/64.0f;

        float env_attack = _Params[(int)PARAM_EnvAttack]*100.0f;
        float env_release = _Params[(int)PARAM_EnvRelease]*100.0f;
        if (_Position<start+env_attack)
        {
            volume *= _Position>=start?(_Position-start)/env_attack:0.0f;
        }

        if (_Position>stop-env_release)
        {
            volume *= _Position<stop?1.0f-(_Position-(stop-env_release))/env_release:0.0f;
        }

        if (_Stop && _Sample->UseRelease())
        {
            _StopTime += 1.0f;
            volume *= _StopTime>env_release?0.0f:1.0f-_StopTime/env_release;
        }

        left = (int)(left*volume);
        right = (int)(right*volume);

        if (_Sample->IsLooping() && !_Stop)
        {
            float loop_delay = _Params[(int)PARAM_LoopDelay]*100.0f;
            float loop_start = start+_Params[(int)PARAM_LoopStart]*(stop-start)/1024.0f;
            float loop_stop = start+_Params[(int)PARAM_LoopStop]*(stop-start)/1024.0f;
            if (loop_stop<loop_start)
                loop_stop = loop_start;

            if (_Params[(int)PARAM_PitchFineTune]>0)
            {
                while (_Position>=loop_stop+loop_delay)
                    _Position -= loop_stop-loop_start+loop_delay;

                if (_Position>loop_stop)
                {
                    left = 0;
                    right = 0;
                }
            }
            else
            {
                while (_Position<=loop_start-loop_delay)
                    _Position += loop_stop-loop_start+loop_delay;

                if (_Position<loop_start)
                {
                    left = 0;
                    right = 0;
                }
            }
        }
        else if (_Position<start || _Position>=stop || volume==0.0f)
        {
            _Sample = NULL;
        }
    }

    _Left = _Left*0.9f+left*0.1f;
    _Right = _Right*0.9f+right*0.1f;
    left = (int)_Left;
    right = (int)_Right;
}

void Voice::Play(Sample* sample, int note, int velocity)
{
    float start = sample->GetParam(PARAM_Start)*(sample->GetLength()-1.0f)/1024.0f;
    float stop = sample->GetParam(PARAM_Stop)*(sample->GetLength()-1.0f)/1024.0f;
    if (stop<start)
        stop = start;

    _Position = start;

    if (sample->GetParam(PARAM_PitchFineTune)<0)
        _Position = stop;

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
    _Stop = false;
}

void Voice::Stop(Sample* sample, int note)
{
    _Stop = true;
    _StopTime = 0.0f;
}

void Voice::ForceStop()
{
    _Sample = NULL;
}
