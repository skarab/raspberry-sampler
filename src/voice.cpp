#include "voice.h"
#include "filter_stereo.h"

Voice::Voice() :
    _Sample(NULL),
    _Position(0.0f),
    _Pitch(1.0f),
    _LegatoPitch(1.0f)
{
}

Voice::~Voice()
{
}

bool Voice::IsBusy() const
{
    return (_Sample!=NULL) && !_Stop;
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

    float delay = sample->GetParam(PARAM_Delay)*100.0f;
    if (sample->IsLooping())
        delay = 0.0f;

    if (sample->IsReverse()) _Position = (float)sample->GetStopPosition()+delay;
    else _Position = (float)sample->GetStartPosition()-delay;

    _Note = note;
    _Velocity = velocity;

    if (sample->GetMode()==MODE_InstruLegato)
        _LegatoPitch = _Pitch;

    if (sample->IsInstru() && note!=-1) _Pitch = powf(2.0f, (note-sample->GetMidiKey().Note)/12.0f);
    else _Pitch = 1.0f;

    _Sample = sample;
    _Sample->NotifyStart();
    _Stop = false;
    _InLoop = false;
}

void Voice::Stop(Sample* sample, int note)
{
    _Stop = true;
    _StopTime = 0.0f;
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

    bool legato = _Sample->GetMode()==MODE_InstruLegato;
    float pitch = legato?_LegatoPitch:_Pitch;
    _Position += pitch*powf(2.0f, params[PARAM_PitchSemiTone]/12.0f)*(params[PARAM_PitchFineTune]/512.0f);

    if (legato)
    {
        float lerp = 0.999f+params[PARAM_Legato]*0.0009f/64.0f;
        _LegatoPitch = _LegatoPitch*lerp+_Pitch*(1.0f-lerp);
    }

    // envelop

    float preampli = powf(2.0f, (params[PARAM_PreAmpli]-100.0f)/10.0f);
    float volume = powf(2.0f, (params[PARAM_PostAmpli]-100.0f)/10.0f);

    float env_attack = params[PARAM_EnvAttack]*100.0f;
    if (env_attack>0.0f && _Position<start+env_attack)
        volume *= _Position>=start?pow((_Position-start)/env_attack, 2.0f):0.0f;

    float env_release = params[PARAM_EnvRelease]*100.0f;
    if (env_release>0.0f && _Position>stop-env_release)
        volume *= _Position<stop?1.0f-(_Position-(stop-env_release))/env_release:0.0f;

    if (_Stop && _Sample->UseRelease())
    {
        _StopTime += 1.0f;
        volume *= _StopTime>env_release?0.0f:1.0f-_StopTime/env_release;
    }

    // panning

    float volume_left = 1.0f;
    float volume_right = 1.0f;
    if (params[PARAM_Pan]>0) volume_left = 1.0f-params[PARAM_Pan]/32.0f;
    else if (params[PARAM_Pan]<0) volume_right = 1.0f-params[PARAM_Pan]/32.0f;

    // loop

    if (_Sample->IsLooping() && !_Stop)
    {
        float delay = params[PARAM_Delay]*100.0f;
        float loop_env_attack = params[PARAM_LoopEnvAttack]*100.0f;
        float loop_env_release = params[PARAM_LoopEnvRelease]*100.0f;
        float loop_start = _Sample->GetLoopStartPosition(start, stop);
        float loop_stop = _Sample->GetLoopStopPosition(start, stop);
        if (loop_stop<loop_start+1)
            loop_stop = loop_start+1;

        if (_Sample->IsReverse())
        {
            if (_InLoop && _Position>loop_stop)
                _Position = loop_stop;

            while (_Position<=loop_start-delay)
            {
                _Position += loop_stop-loop_start+delay;
                _InLoop = true;
            }
        }
        else
        {
            if (_InLoop && _Position<loop_start)
                _Position = loop_start;

            while (_Position>=loop_stop+delay)
            {
                _Position -= loop_stop-loop_start+delay;
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

    left *= preampli;
    right *= preampli;

    _LeftFilters.Compute(left, params);
    _RightFilters.Compute(right, params);

    FILTER_STEREO_Compute(left, right, params);

    left *= volume*volume_left;
    right *= volume*volume_right;

    if (left<-32767) left = -32767;
    else if (left>32767) left = 32767;
    if (right<-32767) right = -32767;
    else if (right>32767) right = 32767;

    if (over)
        ForceStop();
}
