#ifndef __SAMPLER_VOICE__
#define __SAMPLER_VOICE__

#include "includes.h"
#include "sample.h"

class Voice
{
public:

    Voice();
    ~Voice();

    bool IsBusy() const;
    bool IsPlaying(int device_id, int channel, int note) const;

    void Update(int& left, int& right);

    void OnNoteOn(Sample* sample, int device_id, int channel, int note, int velocity);
    void OnNoteOff(int velocity);
    void Stop();

private:

    Sample* _Sample;
    float _Position;
    int _DeviceID;
    int _Channel;
    int _Note;
    int _Velocity;
};

#endif
