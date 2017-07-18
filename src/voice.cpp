#include "voice.h"
#include "filter_stereo.h"

Voice::Voice() :
    _Sample(NULL),
    _Position(0.0),
    _Pitch(1.0),
    _LegatoPitch(1.0)
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
    return (_Sample==sample) && (_Note==note);
}

bool Voice::IsLooping() const
{
    return (_Sample!=NULL) && _Sample->IsLooping();
}

void Voice::Play(Sample* sample, int note, int velocity)
{
    if (_Sample!=NULL)
        ForceStop();

    double silence = sample->GetParam(PARAM_SAMPLE_Silence)*100.0;
    if (sample->IsLooping())
        silence = 0.0;

    if (sample->IsReverse()) _Position = sample->GetStopPosition()+silence;
    else _Position = sample->GetStartPosition()-silence;

    _Note = note;
    _Velocity = velocity;

    _LegatoPitch = _Pitch;

    if (sample->IsInstru() && note!=-1) _Pitch = pow(2.0, (note-sample->GetMidiKey().Note)/12.0);
    else _Pitch = 1.0;

    _Sample = sample;
    _Sample->NotifyStart();
    _Stop = false;
    _InLoop = false;
}

void Voice::Stop(Sample* sample, int note)
{
    _Stop = true;
    _StopTime = 0.0;
    _InLoop = false;
}

void Voice::ForceStop()
{
    if (_Sample!=NULL)
        _Sample->NotifyStop();
    _Sample = NULL;
}

void Voice::Update(int& left, int& right)
{
    if (_Sample==NULL)
        return;

    bool over = false;
    const vector<int>& params = _Sample->GetParams();

    // get current sample data

    int start = _Sample->GetStartPosition();
    int stop = _Sample->GetStopPosition();
    if (stop<start)
        stop = start;

    int p = (int)_Position;
    if (p<start) p = start;
    else if (p>stop) p = stop;

    left = (int)_Sample->GetData()[p*2];
    right = (int)_Sample->GetData()[p*2+1];

    // update position & pitch

    bool legato = _Sample->UseLegato();
    double pitch = legato?_LegatoPitch:_Pitch;
    _Position += pitch*pow(2.0, params[PARAM_SAMPLE_PitchSemiTone]/12.0)*(params[PARAM_SAMPLE_PitchFineTune]/1024.0);

    if (legato)
    {
        double lerp = 0.999+params[PARAM_SAMPLE_Legato]*0.0009/64.0;
        _LegatoPitch = _LegatoPitch*lerp+_Pitch*(1.0-lerp);
    }

    // envelop

    float volume = powf(2.0f, (params[PARAM_SAMPLE_Volume]-100.0f)/10.0f);

    float env_attack = params[PARAM_SAMPLE_EnvAttack]*100.0f;
    if (env_attack>0.0f && _Position<start+env_attack)
        volume *= _Position>=start?powf((_Position-start)/env_attack, 2.0f):0.0f;

    float env_release = params[PARAM_SAMPLE_EnvRelease]*100.0f;
    if (env_release>0.0f && _Position>stop-env_release)
        volume *= _Position<stop?1.0f-(_Position-(stop-env_release))/env_release:0.0f;

    if (_Stop && _Sample->UseRelease())
    {
        _StopTime += 1.0f;
        volume *= _StopTime>env_release?0.0f:1.0f-_StopTime/env_release;
    }

    // loop

    if (_Sample->IsLooping() && !_Stop)
    {
        float silence = params[PARAM_SAMPLE_Silence]*100.0f;
        float loop_env_attack = params[PARAM_SAMPLE_LoopEnvAttack]*100.0f;
        float loop_env_release = params[PARAM_SAMPLE_LoopEnvRelease]*100.0f;
        float loop_start = _Sample->GetLoopStartPosition(start, stop);
        float loop_stop = _Sample->GetLoopStopPosition(start, stop);
        if (loop_stop<loop_start+1)
            loop_stop = loop_start+1;

        if (_Sample->IsReverse())
        {
            if (_InLoop && _Position>loop_stop)
                _Position = loop_stop;

            while (_Position<=loop_start-silence)
            {
                _Position += loop_stop-loop_start+silence;
                _InLoop = true;
            }
        }
        else
        {
            if (_InLoop && _Position<loop_start)
                _Position = loop_start;

            while (_Position>=loop_stop+silence)
            {
                _Position -= loop_stop-loop_start+silence;
                _InLoop = true;
            }
        }

        if (_InLoop)
        {
            if (loop_env_attack>0.0f && _Position<loop_start+loop_env_attack)
                volume *= _Position<loop_start?0.0f:powf((_Position-loop_start)/loop_env_attack, 2.0f);

            if (loop_env_release>0.0f && _Position>loop_stop-loop_env_release)
                volume *= _Position>loop_stop?0.0f:1.0f-(_Position-(loop_stop-loop_env_release))/loop_env_release;
        }
    }
    else if ((_Sample->IsReverse() && _Position<start) || (!_Sample->IsReverse() && _Position>stop))
    {
        over = true;
    }

    // filters & volume

    left *= volume;
    right *= volume;

    _LeftFilters.Compute(left, params);
    _RightFilters.Compute(right, params);
    _LeftFilters.ComputeStereo(left, right, params);

    if (over)
        ForceStop();
}
