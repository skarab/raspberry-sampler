#ifndef __SAMPLER_SAMPLE__
#define __SAMPLER_SAMPLE__

#include "includes.h"
#include "wav.h"
#include "midi_key.h"

enum MODE
{
    MODE_OneShot = 0,
    MODE_Loop,
    MODE_Instru,
    MODE_Count
};

enum PARAM
{
    PARAM_PreAmpli,
    PARAM_Pan,
    PARAM_Stereo,
    PARAM_PostAmpli,
    PARAM_Distortion,
    PARAM_BitCrusher,

    PARAM_EQLow,
    PARAM_EQMedium,
    PARAM_EQHigh,
    PARAM_Overdrive,
    PARAM_Noise,
    PARAM_NoiseMix,

    PARAM_LPCutOff,
    PARAM_LPResonance,
    PARAM_HPCutOff,
    PARAM_HPResonance,
    PARAM_Formant,
    PARAM_FormantID,

    PARAM_Start,
    PARAM_EnvAttack,
    PARAM_EnvRelease,
    PARAM_Stop,
    PARAM_PitchSemiTone,
    PARAM_PitchFineTune,

    PARAM_LoopStart,
    PARAM_LoopEnvAttack,
    PARAM_LoopEnvRelease,
    PARAM_LoopStop,
    PARAM_Delay,
    PARAM_Legato,

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
    bool IsLooping() const { return _Mode==MODE_Loop; }
    bool IsInstru() const { return _Mode==MODE_Instru; }
    bool UseLegato() const { return _Params[PARAM_Legato]>0; }
    bool UseRelease() const { return IsInstru() && !UseLegato(); }
    bool IsMonoVoice() const { return IsLooping() || (IsInstru() && UseLegato()); }

    int GetStartPosition() const { return (int)(_Params[PARAM_Start]*(GetLength()-1.0f)/512.0f); }
    int GetStopPosition() const { return (int)(_Params[PARAM_Stop]*(GetLength()-1.0f)/512.0f); }
    int GetLoopStartPosition(int start, int stop) const { return (int)(start+(stop-start)*(_Params[PARAM_LoopStart]/512.0f)); }
    int GetLoopStopPosition(int start, int stop) const { return (int)(start+(stop-start)*(_Params[PARAM_LoopStop]/512.0f)); }

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
