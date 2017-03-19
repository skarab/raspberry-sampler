#ifndef __SAMPLER_SAMPLE__
#define __SAMPLER_SAMPLE__

#include "includes.h"
#include "wav.h"
#include "midi_key.h"

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

    void Load(const pugi::xml_node& node);
    void Save(pugi::xml_node& node);

private:

    string _Name;
    Wav* _Wav;
    MidiKey _MidiKey;
};

#endif
