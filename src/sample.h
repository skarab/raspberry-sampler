#ifndef __SAMPLER_SAMPLE__
#define __SAMPLER_SAMPLE__

#include "includes.h"
#include "wav.h"
#include "midi_key.h"

enum MODE
{
    MODE_OneShot = 0,
    MODE_OneShotAdd,
    MODE_Loop,
    MODE_LoopOnOff,
    MODE_Instru,
    MODE_InstruAdd,
    MODE_InstruLegato,
    MODE_Count
};

enum PARAM
{
    PARAM_Volume = 0,
    PARAM_PitchSemiTone,
    PARAM_PitchFineTune,
    PARAM_Pan,
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

    bool IsLooping() const { return (_Mode==MODE_Loop) || (_Mode==MODE_LoopOnOff); }
    bool IsInstru() const { return (_Mode==MODE_Instru) || (_Mode==MODE_InstruAdd) || (_Mode==MODE_InstruLegato); }

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
