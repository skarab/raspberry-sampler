#include "sample.h"

Param PARAM_Values[PARAM_Count] = {
    { 0, 400, 100, "PreAmpli" },
    { -32, 32, 0, "Pan" },
    { 0, 200, 100, "Stereo" },
    { 0, 400, 100, "PostAmpli" },
    { 0, 100, 0, "Overdrive" },
    { 0, 100, 0, "BitCrusher" },

    { 0, 200, 100, "EQLow" },
    { 0, 200, 100, "EQMedium" },
    { 0, 200, 100, "EQHigh" },
    { 0, 150, 0, "Distortion" },
    { 0, 200, 0, "Noise" },
    { 0, 100, 0, "NoiseMix" },

    { 0, 200, 200, "LPCutOff" },
    { 0, 200, 0, "LPResonance" },
    { 0, 200, 0, "HPCutOff" },
    { 0, 200, 0, "HPResonance" },
    { 0, 100, 0, "Formant" },
    { 0, 5, 1, "FormantID" },

    { 0, 512, 0, "Start" },
    { 0, 2048, 0, "EnvAttack" },
    { 0, 2048, 0, "EnvRelease" },
    { 0, 512, 512, "Stop" },
    { -32, 32, 0, "PitchSemiTone" },
    { -512, 1024, 512, "PitchFineTune" },

    { 0, 512, 0, "LoopStart" },
    { 0, 2048, 0, "LoopEnvAttack" },
    { 0, 2048, 0, "LoopEnvRelease" },
    { 0, 512, 512, "LoopStop" },
    { 0, 1024, 0, "Delay" },
    { 0, 64, 32, "Legato" }
};

/*
DEBUG: all filters running

Param PARAM_Values[PARAM_Count] = {
    { 0, 400, 60, "PreAmpli" },
    { -32, 32, 2, "Pan" },
    { 0, 200, 150, "Stereo" },
    { 0, 400, 100, "PostAmpli" },
    { 0, 100, 20, "Overdrive" },
    { 0, 100, 20, "BitCrusher" },

    { 0, 200, 80, "EQLow" },
    { 0, 200, 100, "EQMedium" },
    { 0, 200, 100, "EQHigh" },
    { 0, 150, 20, "Distortion" },
    { 0, 200, 20, "Noise" },
    { 0, 100, 1, "NoiseMix" },

    { 0, 200, 160, "LPCutOff" },
    { 0, 200, 20, "LPResonance" },
    { 0, 200, 20, "HPCutOff" },
    { 0, 200, 20, "HPResonance" },
    { 0, 100, 20, "Formant" },
    { 0, 5, 1, "FormantID" },

    { 0, 512, 0, "Start" },
    { 0, 2048, 0, "EnvAttack" },
    { 0, 2048, 0, "EnvRelease" },
    { 0, 512, 512, "Stop" },
    { -32, 32, 0, "PitchSemiTone" },
    { -512, 1024, 512, "PitchFineTune" },

    { 0, 512, 0, "LoopStart" },
    { 0, 2048, 0, "LoopEnvAttack" },
    { 0, 2048, 0, "LoopEnvRelease" },
    { 0, 512, 512, "LoopStop" },
    { 0, 1024, 0, "Delay" },
    { 0, 64, 32, "Legato" }
};

*/

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
