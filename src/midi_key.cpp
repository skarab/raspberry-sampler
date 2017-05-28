#include "midi_key.h"

MidiKey::MidiKey()
{
    SetNull();
}

MidiKey::MidiKey(int device, int channel, int note) :
    DeviceID(device),
    Channel(channel),
    Note(note)
{
}

bool MidiKey::operator==(const MidiKey& key) const
{
    return DeviceID==key.DeviceID
        && Channel==key.Channel
        && Note==key.Note;
}

bool MidiKey::IsOnDevice(const MidiKey& key) const
{
    return DeviceID==key.DeviceID
        && Channel==key.Channel;
}

void MidiKey::SetNull()
{
    DeviceID = -1;
    Channel = -1;
    Note = -1;
}

bool MidiKey::IsNull() const
{
    return DeviceID==-1
        && Channel==-1
        && Note==-1;
}

void MidiKey::Load(const pugi::xml_node& node)
{
    SetNull();

    pugi::xml_node xml_midi = node.child("Midi");
    if (xml_midi)
    {
        XML_Read(xml_midi, "Device", DeviceID);
        XML_Read(xml_midi, "Channel", Channel);
        XML_Read(xml_midi, "Note", Note);
    }
}

void MidiKey::Save(pugi::xml_node& node)
{
    if (!IsNull())
    {
        pugi::xml_node xml_midi = node.append_child("Midi");

        if (DeviceID!=-1) XML_Write(xml_midi, "Device", DeviceID);
        if (Channel!=-1) XML_Write(xml_midi, "Channel", Channel);
        if (Note!=-1) XML_Write(xml_midi, "Note", Note);
    }
}
