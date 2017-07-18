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
    void SetParam(int param, int value) { _Params[param] = value; }
    int GetParam(int param) const { return _Params[param]; }
    const vector<int>& GetParams() const { return _Params; }

    bool IsPlaying() const { return _PlayCounter>0; }
    bool IsReverse() const { return _Params[PARAM_SAMPLE_PitchFineTune]<0; }
    bool IsLooping() const { return _Mode==MODE_Loop; }
    bool IsInstru() const { return _Mode==MODE_Instru; }
    bool UseLegato() const { return _Params[PARAM_SAMPLE_Legato]>0; }
    bool UseRelease() const { return IsInstru() && !UseLegato(); }
    bool IsMonoVoice() const { return IsLooping() || (IsInstru() && UseLegato()); }
    bool IsGlobal() const { return _Name.size()==0; }

    const Param& GetParamSettings(int param) { if (IsGlobal()) return PARAM_GLOBAL_Settings[param]; return PARAM_SAMPLE_Settings[param];}

    int GetStartPosition() const { return (int)(_Params[PARAM_SAMPLE_Start]*(GetLength()-1.0f)/2048.0f); }
    int GetStopPosition() const { return (int)(_Params[PARAM_SAMPLE_Stop]*(GetLength()-1.0f)/2048.0f); }
    int GetLoopStartPosition(int start, int stop) const { return (int)(start+(stop-start)*(_Params[PARAM_SAMPLE_LoopStart]/2048.0f)); }
    int GetLoopStopPosition(int start, int stop) const { return (int)(start+(stop-start)*(_Params[PARAM_SAMPLE_LoopStop]/2048.0f)); }

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
