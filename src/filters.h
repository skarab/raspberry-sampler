#ifndef __SAMPLER_FILTERS__
#define __SAMPLER_FILTERS__

#include "includes.h"

#include "filter_dc.h"
#include "filter_stereo.h"
#include "filter_noise.h"
#include "filter_formant.h"
#include "filter_bitcrusher.h"
#include "filter_distortion.h"
#include "filter_overdrive.h"
#include "filter_equalizer.h"
#include "filter_lowpass.h"
#include "filter_highpass.h"

class FiltersGlobal
{
public:

    FiltersGlobal();

    static void Initialize();

    void Compute(int& value, const vector<int>& params);
    void ComputeStereo(int& left, int& right, const vector<int>& params);

private:

    FILTER_DC _DC;
    FILTER_NOISE _Noise;
    FILTER_FORMANT _Formant;
    FILTER_EQUALIZER _Equalizer;
    FILTER_LOWPASS _LowPass;
};

class FiltersVoice
{
public:

    FiltersVoice();

    void Clear();

    void Compute(int& value, const vector<int>& params);
    void ComputeStereo(int& left, int& right, const vector<int>& params);

private:

    FILTER_DC _DC;
    FILTER_NOISE _Noise;
    FILTER_FORMANT _Formant;
    FILTER_EQUALIZER _Equalizer;
    FILTER_LOWPASS _LowPass;

    /*
    FILTER_HIGHPASS _HighPass;
    */
};

#endif
