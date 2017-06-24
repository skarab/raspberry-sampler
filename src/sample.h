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
    PARAM_Pan,
    PARAM_Stereo,
    PARAM_PitchSemiTone,
    PARAM_PitchFineTune,
    PARAM_Legato,

    PARAM_BitCrusher,
    PARAM_Distortion,
    PARAM_Overdrive,
    PARAM_Formant,
    PARAM_FormantID,
    PARAM_Noise,

    PARAM_NoiseMix,
    PARAM_EQLow,
    PARAM_EQMedium,
    PARAM_EQHigh,
    PARAM_LPCutOff,
    PARAM_HPCutOff,
    PARAM_Resonance,

    PARAM_LoopStartPercent,
    PARAM_LoopStartFineTune,
    PARAM_LoopStopFineTune,
    PARAM_LoopStopPercent,
    PARAM_LoopDelay,
    PARAM_LoopDelayEnv,

    PARAM_StartPercent,
    PARAM_StartFineTune,
    PARAM_EnvAttack,
    PARAM_EnvRelease,
    PARAM_StopFineTune,
    PARAM_StopPercent,

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

    Sample();
    Sample(string name, string path);
    ~Sample();

    const string& GetName() const { return _Name; }
    bool IsValid() const { return _Wav->IsValid(); }
    int GetLength() const { return (int)_Wav->GetLength(); }
    const short int* GetData() const { return _Wav->GetData(); }
    const MidiKey& GetMidiKey() const { return _MidiKey; }
    MidiKey& GetMidiKey() { return _MidiKey; }
    void SetMode(MODE mode) { _Mode = mode; }
    MODE GetMode() const { return _Mode; }
    void SetParam(PARAM param, int value) { _Params[param] = value; }
    int GetParam(PARAM param) const { return _Params[param]; }
    const vector<int>& GetParams() const { return _Params; }

    bool IsPlaying() const { return _PlayCounter>0; }
    bool IsReverse() const { return _Params[PARAM_PitchFineTune]<0; }
    bool IsLooping() const { return (_Mode==MODE_Loop) || (_Mode==MODE_KeyLoop); }
    bool IsInstru() const { return (_Mode==MODE_Instru) || (_Mode==MODE_InstruNoRelease) || (_Mode==MODE_InstruLegato); }
    bool UseRelease() const { return (_Mode==MODE_Key) || (_Mode==MODE_KeyLoop) || (_Mode==MODE_Instru); }

    int GetStartPosition() const;
    int GetStopPosition() const;
    int GetLoopStartPosition(int start, int stop) const;
    int GetLoopStopPosition(int start, int stop) const;

    void Load(const pugi::xml_node& node);
    void Save(pugi::xml_node& node);

    void NotifyStart() { ++_PlayCounter; }
    void NotifyStop() { --_PlayCounter; }

private:

    string _Name;
    Wav* _Wav;
    MidiKey _MidiKey;
    MODE _Mode;
    vector<int> _Params;
    int _PlayCounter;
};

#endif
