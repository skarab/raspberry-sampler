#ifndef __SAMPLER_SAMPLE__
#define __SAMPLER_SAMPLE__

#include "includes.h"
#include "wav.h"

class Sample
{
public:

    Sample(string path);
    ~Sample();

private:

    Wav* _Wav;
};

#endif
