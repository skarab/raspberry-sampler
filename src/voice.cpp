#include "voice.h"

Voice::Voice() :
    _Sample(NULL),
    _Position(0.0f)
{
}

Voice::~Voice()
{
}

bool Voice::IsBusy() const
{
    return _Sample!=NULL;
}

bool Voice::IsPlaying(int device_id, int channel, int note) const
{
    return IsBusy() && _DeviceID==device_id && _Channel==channel && _Note==note;
}

void Voice::Update(int& left, int& right)
{
    unsigned int p = (unsigned int)_Position;
    if (p>=_Sample->GetLength())
    {
        _Sample = NULL;
        left = 0;
        right = 0;
    }
    else
    {
        left = _Sample->GetData()[p*2];
        right = _Sample->GetData()[p*2+1];
        _Position += 1.0f;
    }
}

void Voice::OnNoteOn(Sample* sample, int device_id, int channel, int note, int velocity)
{
    if (!IsBusy() && sample->IsValid())
    {
        _Position = 0.0f;
        _Sample = sample;
        _DeviceID = device_id;
        _Channel = channel;
        _Note = note;
        _Velocity = velocity;
    }
}

void Voice::OnNoteOff(int velocity)
{
}
