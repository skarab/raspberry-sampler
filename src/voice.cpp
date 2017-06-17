#include "voice.h"

Voice::Voice() :
    _Sample(NULL),
    _Position(0.0)
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

void Voice::Update(double& left, double& right)
{
    bool over = false;
    const vector<int>& params = _Sample->GetParams();

    // get current sample data

    double start = _Sample->GetStartPosition();
    double stop = _Sample->GetStopPosition();
    if (stop<start+1)
        stop = start+1;

    double clamp_position = _Position;
    if (clamp_position<start) clamp_position = start;
    if (clamp_position>stop) clamp_position = stop;

    int p = (int)clamp_position;
    left = _Sample->GetData()[p*2]/32768.0f;
    right = _Sample->GetData()[p*2+1]/32768.0f;

    // update position & pitch

    _Position += _Pitch*pow(2.0, params[PARAM_PitchSemiTone]/12.0)*(params[PARAM_PitchFineTune]/512.0);

    if (_Sample->GetMode()==MODE_InstruLegato)
    {
        double legato = 0.999+params[PARAM_Legato]*0.0009/64.0;
        _Pitch = _Pitch*legato+_LegatoPitch*(1.0-legato);
    }

    // panning

    double pan = params[PARAM_Pan]/32.0;
    if (pan<0) right = right*(1.0+pan);
    else left = left*(1.0-pan);

    // envelop

    double volume = params[PARAM_Volume]/100.0;

    double env_attack = params[PARAM_EnvAttack]*50.0;
    double env_release = params[PARAM_EnvRelease]*50.0;
    if (_Position<start+env_attack)
        volume *= _Position>=start?(_Position-start)/env_attack:0.0;

    if (_Position>stop-env_release)
        volume *= _Position<stop?1.0-(_Position-(stop-env_release))/env_release:0.0;

    if (_Stop && _Sample->UseRelease())
    {
        _StopTime += 1.0;
        volume *= _StopTime>env_release?0.0:1.0-_StopTime/env_release;
    }

    // loop

    if (_Sample->IsLooping() && !_Stop)
    {
        double loop_delay = params[PARAM_LoopDelay]*50.0;
        double loop_delay_env = loop_delay==0.0?0.0:params[PARAM_LoopDelayEnv]*50.0;
        double loop_start = _Sample->GetLoopStartPosition(start, stop);
        double loop_stop = _Sample->GetLoopStopPosition(start, stop);
        if (loop_stop<loop_start+1)
            loop_stop = loop_start+1;

        // reverse ?
        if (params[PARAM_PitchFineTune]>0)
        {
            while (_Position>=loop_stop+loop_delay)
                _Position -= loop_stop-loop_start+loop_delay;

            if (_Position>loop_stop-loop_delay_env)
                volume *= _Position>loop_stop?0.0:1.0-(_Position-(loop_stop-loop_delay_env))/loop_delay_env;
        }
        else
        {
            while (_Position<=loop_start-loop_delay)
                _Position += loop_stop-loop_start+loop_delay;

            if (_Position<loop_start+loop_delay_env)
                volume *= _Position<loop_start?0.0:(_Position-loop_start)/loop_delay_env;
        }
    }
    else if (_Position<start || _Position>=stop || volume==0.0)
    {
        over = true;
    }

    // filters & volume

    _Filters.Compute(left, right, params);

    left *= volume;
    right *= volume;

    if (over)
        _Sample = NULL;
}

void Voice::Play(Sample* sample, int note, int velocity)
{
    double start = sample->GetStartPosition();
    double stop = sample->GetStopPosition();
    if (stop<start)
        stop = start;

    _Position = start;

    if (sample->GetParam(PARAM_PitchFineTune)<0)
        _Position = stop;

    _Note = note;
    _Velocity = velocity;
    _LegatoPitch = 1.0;

    if (sample->IsInstru() && note!=-1)
        _LegatoPitch = pow(2.0, (note-sample->GetMidiKey().Note)/12.0);

    if (_Sample==NULL || (sample->GetMode()!=MODE_InstruLegato))
        _Pitch = _LegatoPitch;

    _Sample = sample;
    _Stop = false;
}

void Voice::Stop(Sample* sample, int note)
{
    _Stop = true;
    _StopTime = 0.0;
}

void Voice::ForceStop()
{
    _Sample = NULL;
}
