#include "sample.h"

Param PARAM_Values[PARAM_Count] = {
    { 0, 64, 50, "Volume" },
    { -64, 64, 32, "Pitch" },
    { -32, 32, 0, "Pan" },
    { 0, 64, 32, "Legato" }
};

Sample::Sample(string name, string path) :
    _Name(name),
    _Mode(MODE_OneShot)
{
    _Wav = new Wav(path);

    _Params.resize(PARAM_Count);
    for (int i=0 ; i<PARAM_Count ; ++i)
        _Params[i] = PARAM_Values[i].Default;
}

Sample::~Sample()
{
    if (_Wav!=NULL)
        delete _Wav;
}

void Sample::Load(const pugi::xml_node& node)
{
    _MidiKey.Load(node);

    int mode = 0;
    XML_Read(node, "Mode", mode);
    _Mode = (MODE)mode;

    for (int i=0 ; i<PARAM_Count ; ++i)
    {
        XML_Read(node, PARAM_Values[i].Name, _Params[i]);

        if (_Params[i]<PARAM_Values[i].Min) _Params[i] = PARAM_Values[i].Min;
        if (_Params[i]>PARAM_Values[i].Max) _Params[i] = PARAM_Values[i].Max;
    }
}

void Sample::Save(pugi::xml_node& node)
{
    XML_Write(node, "Name", _Name);
    _MidiKey.Save(node);

    int mode = (int)_Mode;
    XML_Write(node, "Mode", mode);

    for (int i=0 ; i<PARAM_Count ; ++i)
        XML_Write(node, PARAM_Values[i].Name, _Params[i]);
}
