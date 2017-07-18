#include "sample.h"

Sample::Sample() :
    _Name(""),
    _Wav(NULL),
    _PlayCounter(0)
{
    _Params.resize(PARAM_SAMPLE_Count);
    for (int i=0 ; i<PARAM_SAMPLE_Count ; ++i)
        _Params[i] = GetParamSettings(i).Default;
}

Sample::Sample(string name, string path) :
    _Name(name),
    _Mode(MODE_Loop),
    _PlayCounter(0)
{
    _Wav = new Wav(path);

    _Params.resize(PARAM_SAMPLE_Count);
    for (int i=0 ; i<PARAM_SAMPLE_Count ; ++i)
        _Params[i] = GetParamSettings(i).Default;
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

    for (int i=0 ; i<PARAM_SAMPLE_Count ; ++i)
    {
        const Param& settings = GetParamSettings(i);

        XML_Read(node, settings.Name, _Params[i]);

        if (_Params[i]<settings.Min) _Params[i] = settings.Min;
        if (_Params[i]>settings.Max) _Params[i] = settings.Max;
    }
}

void Sample::Save(pugi::xml_node& node)
{
    XML_Write(node, "Name", _Name);
    _MidiKey.Save(node);

    int mode = (int)_Mode;
    XML_Write(node, "Mode", mode);

    for (int i=0 ; i<PARAM_SAMPLE_Count ; ++i)
        XML_Write(node, GetParamSettings(i).Name, _Params[i]);
}
