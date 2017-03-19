#include "midi_key.h"

MidiKey::MidiKey()
{
    SetNull();
}

void MidiKey::SetNull()
{
    DeviceID = -1;
    Channel = -1;
    Note = -1;
    Velocity = -1;
}

bool MidiKey::IsNull() const
{
    return DeviceID==-1
        && Channel==-1
        && Note==-1
        && Velocity==-1;
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
        XML_Read(xml_midi, "Velocity", Velocity);
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
        if (Velocity!=-1) XML_Write(xml_midi, "Velocity", Velocity);
    }
}
