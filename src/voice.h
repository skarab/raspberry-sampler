#ifndef __SAMPLER_VOICE__
#define __SAMPLER_VOICE__

#include "includes.h"


class Voice
{
public:

    bool IsPlaying();
    void Compute(int& left, int &right);

private:


};

#endif
