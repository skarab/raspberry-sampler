#include "sample.h"

Param PARAM_Values[PARAM_Count] = {
    { 0, 200, 100, "Volume" },
    { 0, 100, 0, "StartPercent" },
    { 0, 100, 100, "StopPercent" },
    { 0, 100, 0, "LoopStartPercent" },
    { 0, 100, 100, "LoopStopPercent" },
    { -32, 32, 0, "PitchSemiTone" },

    { -32, 32, 0, "Pan" },
    { -512, 512, 0, "StartFineTune" },
    { -512, 512, 0, "StopFineTune" },
    { -512, 512, 0, "LoopStartFineTune" },
    { -512, 512, 0, "LoopStopFineTune" },
    { -512, 1024, 512, "PitchFineTune" },

    { 0, 1024, 0, "LoopDelay" },
    { 0, 512, 1, "LoopDelayEnv" },
    { 0, 1024, 1, "EnvAttack" },
    { 0, 1024, 1, "EnvRelease" },
    { 0, 64, 32, "Legato" },
    { 0, 64, 32, "Unused" },

    { 0, 200, 0, "HPCutOff" },
    { 0, 200, 0, "HPResonance" },
    { 0, 200, 200, "LPCutOff" },
    { 0, 200, 0, "LPResonance" },
    { 0, 127, 0, "RCCutOff" },
    { 0, 127, 0, "RCResonance" }
};

Sample::Sample(string name, string path) :
    _Name(name),
    _Mode(MODE_OneShot)
{
    _Wav = new Wav(path);

    _Params.resize(PARAM_Count);
    for (int i=0 ; i<PARAM_Count ; ++i)
        _Params[i] = PARAM_Values[i].Default;
}

Sample::~Sample()
{
    if (_Wav!=NULL)
        delete _Wav;
}

float Sample::GetStartPosition() const
{
    float position = _Params[PARAM_StartPercent]*(GetLength()-1.0f)/100.0f;
    position += _Params[PARAM_StartFineTune]*50.0f;

    if (position<0.0f) position = 0.0f;
    if (position>GetLength()-1.0f) position = GetLength()-1.0f;

    return position;
}

float Sample::GetStopPosition() const
{
    float position = _Params[PARAM_StopPercent]*(GetLength()-1.0f)/100.0f;
    position += _Params[PARAM_StopFineTune]*50.0f;

    if (position<0.0f) position = 0.0f;
    if (position>GetLength()-1.0f) position = GetLength()-1.0f;

    return position;
}

float Sample::GetLoopStartPosition(float start, float stop) const
{
    float position = _Params[PARAM_LoopStartPercent]*(stop-start)/100.0f;
    position += _Params[PARAM_LoopStartFineTune]*50.0f;

    if (position<start) position = start;
    if (position>stop) position = stop;

    return position;
}

float Sample::GetLoopStopPosition(float start, float stop) const
{
    float position = _Params[PARAM_LoopStopPercent]*(stop-start)/100.0f;
    position += _Params[PARAM_LoopStopFineTune]*50.0f;

    if (position<start) position = start;
    if (position>stop) position = stop;

    return position;
}

void Sample::Load(const pugi::xml_node& node)
{
    _MidiKey.Load(node);

    int mode = 0;
    XML_Read(node, "Mode", mode);
    _Mode = (MODE)mode;

    /*
    for (int i=0 ; i<PARAM_Count ; ++i)
    {
        XML_Read(node, PARAM_Values[i].Name, _Params[i]);

        if (_Params[i]<PARAM_Values[i].Min) _Params[i] = PARAM_Values[i].Min;
        if (_Params[i]>PARAM_Values[i].Max) _Params[i] = PARAM_Values[i].Max;
    }*/
}

void Sample::Save(pugi::xml_node& node)
{
    XML_Write(node, "Name", _Name);
    _MidiKey.Save(node);

    int mode = (int)_Mode;
    XML_Write(node, "Mode", mode);

    for (int i=0 ; i<PARAM_Count ; ++i)
        XML_Write(node, PARAM_Values[i].Name, _Params[i]);
}
