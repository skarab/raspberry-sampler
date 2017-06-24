#include "filters.h"

Filters::Filters()
{
    FILTER_DC_Initialize(_DC);

    //FILTER_EQ_Initialize(_Equalizer);
    //FILTER_FORMANT_Initialize(_Formant);
    //FILTER_HIGHPASS_Initialize(_HighPass);
    //FILTER_LOWPASS_Initialize(_LowPass);
    //FILTER_NOISE_Initialize(_Noise);
}

void Filters::Initialize()
{
    //FILTER_NOISE_STATIC_INITIALIZE();
}

void Filters::Compute(int& value, const vector<int>& params)
{
    //FILTER_STEREO_Compute(left, right, params);
    //FILTER_NOISE_Compute(left, right, params, _Noise);
    //FILTER_DISTORTION_Compute(left, right, params);
    //FILTER_BITCRUSHER_Compute(left, right, params);
    //FILTER_EQ_Compute(left, right, params, _Equalizer);
    //FILTER_HIGHPASS_Compute(left, right, params, _HighPass);
    //FILTER_LOWPASS_Compute(left, right, params, _LowPass);
    //FILTER_FORMANT_Compute(left, right, params, _Formant);
    //FILTER_OVERDRIVE_Compute(left, right, params);


    FILTER_CLICK_REMOVER_Compute(value, _ClickRemover);
    FILTER_DC_Compute(value, _DC);
}

