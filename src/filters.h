#ifndef __SAMPLER_FILTERS__
#define __SAMPLER_FILTERS__

#include "includes.h"

#include "filter_dc.h"
#include "filter_noise.h"
#include "filter_resonance.h"
#include "filter_formant.h"
#include "filter_lowpass.h"
#include "filter_highpass.h"
#include "filter_wet.h"
#include "filter_equalizer.h"

class FiltersGlobal
{
public:

    FiltersGlobal();

    static void Initialize();

    void Compute(int& value, const vector<int>& params);
    void ComputeStereo(int& left, int& right, const vector<int>& params);

private:

    float _Volume;
    FILTER_DC _DC;
    FILTER_NOISE _Noise;
    FILTER_LOWPASS _LowPass;
    FILTER_HIGHPASS _HighPass;
    FILTER_WET _Wet;
    FILTER_RESONANCE _Resonance;
    FILTER_EQUALIZER _Equalizer;
};

class FiltersVoice
{
public:

    FiltersVoice();

    void Compute(int& value, const vector<int>& params);
    void ComputeStereo(int& left, int& right, const vector<int>& params);

private:

    FILTER_DC _DC;
    FILTER_FORMANT _Formant;
};

#endif
