#ifndef __SAMPLER_PRESET__
#define __SAMPLER_PRESET__

#include "includes.h"
#include "midi.h"
#include "sample.h"

class Preset
{
public:


private:

    map<MidiKey, Sample*> _Samples;
};

#endif
