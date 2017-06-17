#ifndef __SAMPLER_FILTERS__
#define __SAMPLER_FILTERS__

#include "includes.h"

#include "filter_stereo.h"
#include "filter_overdrive.h"
#include "filter_bitcrusher.h"
#include "filter_distortion.h"
#include "filter_eq.h"
#include "filter_formant.h"
#include "filter_highpass.h"
#include "filter_lowpass.h"
#include "filter_noise.h"

class Filters
{
public:

    Filters();

    static void Initialize();

    void Compute(double& left, double& right, const vector<int>& params);

private:

    FILTER_EQ _Equalizer;
    FILTER_FORMANT _Formant;
    FILTER_HIGHPASS _HighPass;
    FILTER_LOWPASS _LowPass;
    FILTER_NOISE _Noise;
};

#endif
