#include "sample.h"

Param PARAM_Values[PARAM_Count] = {
    { 0, 200, 100, "Volume" },
    { -32, 32, 0, "Pan" },
    { 0, 200, 100, "Stereo" },
    { -32, 32, 0, "PitchSemiTone" },
    { -512, 1024, 512, "PitchFineTune" },
    { 0, 64, 32, "Legato" },

    { 0, 200, 0, "BitCrusher" },
    { 0, 200, 0, "Distortion" },
    { 0, 200, 0, "Overdrive" },
    { -1, 4, -1, "Formant" },
    { 0, 200, 0, "Noise" },
    { 0, 100, 0, "NoiseMix" },

    { 0, 200, 100, "EQLow" },
    { 0, 200, 100, "EQMedium" },
    { 0, 200, 100, "EQHigh" },
    { 0, 200, 200, "LPCutOff" },
    { 0, 200, 0, "HPCutOff" },
    { 0, 200, 0, "Resonance" },

    { 0, 100, 0, "LoopStartPercent" },
    { -512, 512, 0, "LoopStartFineTune" },
    { -512, 512, 0, "LoopStopFineTune" },
    { 0, 100, 100, "LoopStopPercent" },
    { 0, 1024, 0, "LoopDelay" },
    { 0, 512, 1, "LoopDelayEnv" },

    { 0, 100, 0, "StartPercent" },
    { -512, 512, 0, "StartFineTune" },
    { 0, 2048, 0, "EnvAttack" },
    { 0, 2048, 0, "EnvRelease" },
    { -512, 512, 0, "StopFineTune" },
    { 0, 100, 100, "StopPercent" }
};

Sample::Sample() :
    _Wav(NULL),
    _PlayCounter(0)
{
    _Params.resize(PARAM_Count);
    for (int i=0 ; i<PARAM_Count ; ++i)
        _Params[i] = PARAM_Values[i].Default;
}

Sample::Sample(string name, string path) :
    _Name(name),
    _Mode(MODE_OneShot),
    _PlayCounter(0)
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

int Sample::GetStartPosition() const
{
    int position = _Params[PARAM_StartPercent]*(GetLength()-1)/100;
    position += _Params[PARAM_StartFineTune]*50;

    if (position<0) position = 0;
    else if (position>GetLength()-1) position = GetLength()-1;

    return position;
}

int Sample::GetStopPosition() const
{
    int position = _Params[PARAM_StopPercent]*(GetLength()-1)/100;
    position += _Params[PARAM_StopFineTune]*50;

    if (position<0) position = 0;
    else if (position>GetLength()-1) position = GetLength()-1;

    return position;
}

int Sample::GetLoopStartPosition(int start, int stop) const
{
    int position = _Params[PARAM_LoopStartPercent]*(stop-start)/100;
    position += _Params[PARAM_LoopStartFineTune]*50;

    if (position<start) position = start;
    else if (position>stop) position = stop;

    return position;
}

int Sample::GetLoopStopPosition(int start, int stop) const
{
    int position = _Params[PARAM_LoopStopPercent]*(stop-start)/100;
    position += _Params[PARAM_LoopStopFineTune]*50;

    if (position<start) position = start;
    else if (position>stop) position = stop;

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
