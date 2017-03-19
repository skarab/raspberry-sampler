#include "sample.h"

Sample::Sample(string name, string path) :
    _Name(name)
{
    _Wav = new Wav(path);
}

Sample::~Sample()
{
    if (_Wav!=NULL)
        delete _Wav;
}

void Sample::Load(const pugi::xml_node& node)
{
    _MidiKey.Load(node);
}

void Sample::Save(pugi::xml_node& node)
{
    XML_Write(node, "Name", _Name);
    _MidiKey.Save(node);
}
