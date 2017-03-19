#ifndef __SAMPLER_MIDI_KEY__
#define __SAMPLER_MIDI_KEY__

#include "includes.h"

struct MidiKey
{
    MidiKey();

    void SetNull();
    bool IsNull() const;

    void Load(const pugi::xml_node& node);
    void Save(pugi::xml_node& node);

    int DeviceID;
    int Channel;
    int Note;
    int Velocity;
};


#endif
