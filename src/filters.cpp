#include "filters.h"

FiltersGlobal::FiltersGlobal()
{
    FILTER_DC_Initialize(_DC);
    FILTER_NOISE_Initialize(_Noise);
    FILTER_FORMANT_Initialize(_Formant);
}

void FiltersGlobal::Initialize()
{
    FILTER_NOISE_STATIC_INITIALIZE();
}

void FiltersGlobal::Compute(int& value, const vector<int>& params)
{
    FILTER_NOISE_Compute(value, params, _Noise);
    FILTER_FORMANT_Compute(value, params, _Formant);
    FILTER_DC_Compute(value, _DC);

    float volume = powf(2.0f, (params[PARAM_Volume]-100.0f)/10.0f);
    value = (int)(value*volume);
}

void FiltersGlobal::ComputeStereo(int& left, int& right, const vector<int>& params)
{
    FILTER_STEREO_Compute(left, right, params);

    float volume_left = 1.0f;
    float volume_right = 1.0f;
    if (params[PARAM_Pan]>0) volume_left = 1.0f-pow(params[PARAM_Pan]/32.0f, 2.0f);
    else if (params[PARAM_Pan]<0) volume_right = 1.0f-pow(params[PARAM_Pan]/32.0f, 2.0f);

    left = (int)(left*volume_left);
    right = (int)(right*volume_right);
}

FiltersVoice::FiltersVoice()
{
    FILTER_DC_Initialize(_DC);
    FILTER_NOISE_Initialize(_Noise);
    FILTER_FORMANT_Initialize(_Formant);

    //FILTER_EQ_Initialize(_Equalizer);
    //
    //FILTER_HIGHPASS_Initialize(_HighPass);
    //FILTER_LOWPASS_Initialize(_LowPass);
}

void FiltersVoice::Clear()
{
    FILTER_FORMANT_Initialize(_Formant);
}

void FiltersVoice::Compute(int& value, const vector<int>& params)
{
    //FILTER_NOISE_Compute(left, right, params, _Noise);
    //FILTER_DISTORTION_Compute(left, right, params);
    //FILTER_BITCRUSHER_Compute(left, right, params);
    //FILTER_EQ_Compute(left, right, params, _Equalizer);
    //FILTER_HIGHPASS_Compute(left, right, params, _HighPass);
    //FILTER_LOWPASS_Compute(left, right, params, _LowPass);
    //FILTER_OVERDRIVE_Compute(left, right, params);

    FILTER_NOISE_Compute(value, params, _Noise);
    FILTER_FORMANT_Compute(value, params, _Formant);
    FILTER_DC_Compute(value, _DC);
}

void FiltersVoice::ComputeStereo(int& left, int& right, const vector<int>& params)
{
    FILTER_STEREO_Compute(left, right, params);
}
