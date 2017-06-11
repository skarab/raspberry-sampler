#include "voice.h"
#include "filter_highpass.h"
#include "filter_lowpass.h"

Voice::Voice() :
    _Sample(NULL),
    _Position(0.0f)
{
    _Filters.push_back(new FilterHighPass());
    _Filters.push_back(new FilterLowPass());
}

Voice::~Voice()
{
    for (int i=0 ; i<_Filters.size() ; ++i)
        delete _Filters[i];
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

void Voice::Update(float& left, float& right)
{
    if (_Sample!=NULL)
    {
        memcpy(&_Params[0], &_Sample->GetParams()[0], sizeof(int)*_Sample->GetParams().size());

        float start = _Sample->GetStartPosition();
        float stop = _Sample->GetStopPosition();
        if (stop<start+1)
            stop = start+1;

        float clamp_position = _Position;
        if (clamp_position<start) clamp_position = start;
        if (clamp_position>stop) clamp_position = stop;

        int p = (int)clamp_position;
        left = _Sample->GetData()[p*2]/32768.0f;
        right = _Sample->GetData()[p*2+1]/32768.0f;

        _Position += _Pitch*pow(2.0f, _Params[PARAM_PitchSemiTone]/12.0f)*(_Params[PARAM_PitchFineTune]/512.0f);

        if (_Sample->GetMode()==MODE_InstruLegato)
        {
            float legato = 0.999f+_Params[PARAM_Legato]*0.0009/64.0f;
            _Pitch = _Pitch*legato+_LegatoPitch*(1.0f-legato);
        }

        float pan = _Params[PARAM_Pan]/32.0f;
        if (pan<0) right = right*(1.0f+pan);
        else left = left*(1.0f-pan);

        float volume = _Params[PARAM_Volume]/100.0f;

        float env_attack = _Params[PARAM_EnvAttack]*50.0f;
        float env_release = _Params[PARAM_EnvRelease]*50.0f;
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

        if (_Sample->IsLooping() && !_Stop)
        {
            float loop_delay = _Params[PARAM_LoopDelay]*50.0f;
            float loop_delay_env = loop_delay==0.0f?0.0f:_Params[PARAM_LoopDelayEnv]*50.0f;
            float loop_start = _Sample->GetLoopStartPosition(start, stop);
            float loop_stop = _Sample->GetLoopStopPosition(start, stop);
            if (loop_stop<loop_start+1)
                loop_stop = loop_start+1;

            if (_Params[PARAM_PitchFineTune]>0)
            {
                while (_Position>=loop_stop+loop_delay)
                    _Position -= loop_stop-loop_start+loop_delay;

                if (_Position>loop_stop-loop_delay_env)
                {
                    volume *= _Position>loop_stop?0.0f:1.0f-(_Position-(loop_stop-loop_delay_env))/loop_delay_env;
                }
            }
            else
            {
                while (_Position<=loop_start-loop_delay)
                    _Position += loop_stop-loop_start+loop_delay;

                if (_Position<loop_start+loop_delay_env)
                {
                    volume *= _Position<loop_start?0.0f:(_Position-loop_start)/loop_delay_env;
                }
            }
        }
        else if (_Position<start || _Position>=stop || volume==0.0f)
        {
            _Sample = NULL;
        }

        left *= volume;
        right *= volume;
    }

    if (_Params.size()>0)
    {
        for (int i=0 ; i<_Filters.size() ; ++i)
            _Filters[i]->Compute(left, right, _Params);
    }
}

void Voice::Play(Sample* sample, int note, int velocity)
{
    float start = sample->GetStartPosition();
    float stop = sample->GetStopPosition();
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

        _Params.resize(PARAM_Count);
        for (int i=0 ; i<PARAM_Count ; ++i)
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
