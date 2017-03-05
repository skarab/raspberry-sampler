#ifndef __SAMPLER_BANK__
#define __SAMPLER_BANK__

#include "includes.h"
#include "midi.h"
#include "sample.h"

class Bank
{
public:


private:

    map<MidiKey, Sample*> _Samples;
};

#endif
