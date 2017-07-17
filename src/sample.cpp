#include "sample.h"

Param PARAM_Values[PARAM_Count] = {
    { 0, 400, 110, "PreAmpli" },
    { -32, 32, 1, "Pan" },
    { 0, 200, 120, "Stereo" },
    { 0, 400, 110, "PostAmpli" },
    { 0, 150, 20, "Distortion" },
    { 0, 100, 20, "BitCrusher" },

    { 0, 200, 90, "EQLow" },
    { 0, 200, 110, "EQMedium" },
    { 0, 200, 90, "EQHigh" },
    { 0, 100, 2, "Overdrive" },
    { 0, 200, 10, "Noise" },
    { 0, 100, 10, "NoiseMix" },

    { 0, 200, 180, "LPCutOff" },
    { 0, 200, 20, "LPResonance" },
    { 0, 200, 40, "HPCutOff" },
    { 0, 200, 40, "HPResonance" },
    { 0, 100, 40, "Formant" },
    { 0, 4, 1, "FormantID" },

    { 0, 512, 12, "Start" },
    { 0, 2048, 12, "EnvAttack" },
    { 0, 2048, 12, "EnvRelease" },
    { 0, 512, 500, "Stop" },
    { -32, 32, 1, "PitchSemiTone" },
    { -512, 1024, 513, "PitchFineTune" },

    { 0, 512, 10, "LoopStart" },
    { 0, 2048, 10, "LoopEnvAttack" },
    { 0, 2048, 10, "LoopEnvRelease" },
    { 0, 512, 510, "LoopStop" },
    { 0, 1024, 10, "Delay" },
    { 0, 64, 2, "Legato" }
};

/*
Param PARAM_Values[PARAM_Count] = {
    { 0, 400, 100, "PreAmpli" },
    { -32, 32, 0, "Pan" },
    { 0, 200, 100, "Stereo" },
    { 0, 400, 100, "PostAmpli" },
    { 0, 150, 0, "Distortion" },
    { 0, 100, 0, "BitCrusher" },

    { 0, 200, 100, "EQLow" },
    { 0, 200, 100, "EQMedium" },
    { 0, 200, 100, "EQHigh" },
    { 0, 100, 0, "Overdrive" },
    { 0, 200, 0, "Noise" },
    { 0, 100, 0, "NoiseMix" },

    { 0, 200, 200, "LPCutOff" },
    { 0, 200, 0, "LPResonance" },
    { 0, 200, 0, "HPCutOff" },
    { 0, 200, 0, "HPResonance" },
    { 0, 100, 0, "Formant" },
    { 0, 4, 0, "FormantID" },

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
    { 0, 64, 0, "Legato" }
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
    _Mode(MODE_Loop),
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
