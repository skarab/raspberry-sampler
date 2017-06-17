#ifndef __SAMPLER_SAMPLE__
#define __SAMPLER_SAMPLE__

#include "includes.h"
#include "wav.h"
#include "midi_key.h"

enum MODE
{
    MODE_OneShot = 0,
    MODE_Loop,
    MODE_Key,
    MODE_KeyLoop,
    MODE_Instru,
    MODE_InstruNoRelease,
    MODE_InstruLegato,
    MODE_Count
};

enum PARAM
{
    PARAM_Volume = 0,
    PARAM_StartPercent,
    PARAM_StopPercent,
    PARAM_LoopStartPercent,
    PARAM_LoopStopPercent,
    PARAM_PitchSemiTone,

    PARAM_Pan,
    PARAM_StartFineTune,
    PARAM_StopFineTune,
    PARAM_LoopStartFineTune,
    PARAM_LoopStopFineTune,
    PARAM_PitchFineTune,

    PARAM_LoopDelay,
    PARAM_LoopDelayEnv,
    PARAM_EnvAttack,
    PARAM_EnvRelease,
    PARAM_Legato,
    PARAM_Unused,

    PARAM_HPCutOff,
    PARAM_HPResonance,
    PARAM_LPCutOff,
    PARAM_LPResonance,
    PARAM_Formant,
    PARAM_FormantID,

    PARAM_EQLow,
    PARAM_EQMedium,
    PARAM_EQHigh,

    PARAM_MoogCutOff,
    PARAM_MoogResonance,

    PARAM_Unused2,

    PARAM_NotchCutOff,
    PARAM_NotchBandWidth,

    PARAM_Count
};

struct Param
{
    int Min;
    int Max;
    int Default;
    const char* Name;
};

extern Param PARAM_Values[PARAM_Count];

class Sample
{
public:

    Sample(string name, string path);
    ~Sample();

    const string& GetName() const { return _Name; }
    bool IsValid() const { return _Wav->IsValid(); }
    unsigned int GetLength() const { return _Wav->GetLength(); }
    const short* GetData() const { return _Wav->GetData(); }
    const MidiKey& GetMidiKey() const { return _MidiKey; }
    MidiKey& GetMidiKey() { return _MidiKey; }
    void SetMode(MODE mode) { _Mode = mode; }
    MODE GetMode() const { return _Mode; }
    void SetParam(PARAM param, int value) { _Params[(int)param] = value; }
    int GetParam(PARAM param) const { return _Params[(int)param]; }
    const vector<int>& GetParams() const { return _Params; }

    bool IsLooping() const { return (_Mode==MODE_Loop) || (_Mode==MODE_KeyLoop); }
    bool IsInstru() const { return (_Mode==MODE_Instru) || (_Mode==MODE_InstruNoRelease) || (_Mode==MODE_InstruLegato); }
    bool UseRelease() const { return (_Mode==MODE_Key) || (_Mode==MODE_KeyLoop) || (_Mode==MODE_Instru); }

    float GetStartPosition() const;
    float GetStopPosition() const;
    float GetLoopStartPosition(float start, float stop) const;
    float GetLoopStopPosition(float start, float stop) const;

    void Load(const pugi::xml_node& node);
    void Save(pugi::xml_node& node);

private:

    string _Name;
    Wav* _Wav;
    MidiKey _MidiKey;
    MODE _Mode;
    vector<int> _Params;
};

#endif
