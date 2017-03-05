#ifndef __SAMPLER_VOICE__
#define __SAMPLER_VOICE__

#include "includes.h"
#include "sample.h"

class Voice
{
public:

    Voice();
    ~Voice();

    bool IsPlaying();
    void Update(int& left, int& right);

    void Play(Sample* sample);

private:

    Sample* _Sample;
    float _Position;
};

#endif
