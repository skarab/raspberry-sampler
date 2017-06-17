#include "filters.h"

Filters::Filters()
{
    FILTER_EQ_Initialize(_Equalizer);
    FILTER_FORMANT_Initialize(_Formant);
    FILTER_HIGHPASS_Initialize(_HighPass);
    FILTER_LOWPASS_Initialize(_LowPass);
    FILTER_MOOG_Initialize(_Moog);
    FILTER_NOISE_Initialize(_Noise);
    FILTER_NOTCH_Initialize(_Notch);
}

void Filters::Initialize()
{
    FILTER_NOISE_STATIC_INITIALIZE();
}

void Filters::Compute(double& left, double& right, const vector<int>& params)
{
    FILTER_STEREO_Compute(left, right, params);
    FILTER_OVERDRIVE_Compute(left, right, params);
    FILTER_DISTORTION_Compute(left, right, params);
    FILTER_BITCRUSHER_Compute(left, right, params);
    FILTER_EQ_Compute(left, right, params, _Equalizer);
    FILTER_FORMANT_Compute(left, right, params, _Formant);
    FILTER_HIGHPASS_Compute(left, right, params, _HighPass);
    FILTER_LOWPASS_Compute(left, right, params, _LowPass);
    FILTER_MOOG_Compute(left, right, params, _Moog);
    FILTER_NOISE_Compute(left, right, params, _Noise);
    FILTER_NOTCH_Compute(left, right, params, _Notch);
}

