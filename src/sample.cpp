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
    { 0, 100, 0, "Formant" },
    { 0, 4, 0, "FormantID" },

    { 0, 200, 100, "EQLow" },
    { 0, 200, 100, "EQMedium" },
    { 0, 200, 100, "EQHigh" },

    { 0, 200, 200, "MoogCutOff" },
    { 0, 200, 0, "MoogResonance" },

    { 0, 64, 32, "Unused2" },

    { 0, 200, 0, "NotchCutOff" },
    { 0, 200, 200, "NotchBandWidth" },
    { 0, 200, 0, "Noise" },
    { 0, 200, 0, "NoisePink" },

    { 0, 200, 0, "BitCrusher" },
    { 0, 64, 32, "Unused3" },

    { 0, 200, 0, "Distortion" },
    { 0, 200, 100, "Stereo" },
    { 0, 200, 0, "Overdrive" }
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

double Sample::GetStartPosition() const
{
    double position = _Params[PARAM_StartPercent]*(GetLength()-1.0)/100.0;
    position += _Params[PARAM_StartFineTune]*50.0;

    if (position<0.0) position = 0.0;
    if (position>GetLength()-1.0) position = GetLength()-1.0;

    return position;
}

double Sample::GetStopPosition() const
{
    double position = _Params[PARAM_StopPercent]*(GetLength()-1.0)/100.0;
    position += _Params[PARAM_StopFineTune]*50.0;

    if (position<0.0) position = 0.0;
    if (position>GetLength()-1.0) position = GetLength()-1.0;

    return position;
}

double Sample::GetLoopStartPosition(double start, double stop) const
{
    double position = _Params[PARAM_LoopStartPercent]*(stop-start)/100.0;
    position += _Params[PARAM_LoopStartFineTune]*50.0;

    if (position<start) position = start;
    if (position>stop) position = stop;

    return position;
}

double Sample::GetLoopStopPosition(double start, double stop) const
{
    double position = _Params[PARAM_LoopStopPercent]*(stop-start)/100.0;
    position += _Params[PARAM_LoopStopFineTune]*50.0;

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
