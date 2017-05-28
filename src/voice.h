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
    bool IsPlaying(Sample* sample) const;
    bool IsPlaying(Sample* sample, int note) const;

    void Update(int& left, int& right);

    void Play(Sample* sample, int note, int velocity);
    void Stop(Sample* sample, int note);
    void Stop();

private:

    Sample* _Sample;
    float _Position;
    int _Note;
    int _Velocity;
    float _Pitch;
};

#endif
