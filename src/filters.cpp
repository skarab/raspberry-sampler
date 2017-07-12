#include "filters.h"
#include "filter_stereo.h"
#include "filter_bitcrusher.h"
#include "filter_distortion.h"
#include "filter_overdrive.h"

FiltersGlobal::FiltersGlobal()
{
    FILTER_DC_Initialize(_DC);
    FILTER_NOISE_Initialize(_Noise);
    FILTER_FORMANT_Initialize(_Formant);
    FILTER_LOWPASS_Initialize(_LowPass);
    FILTER_HIGHPASS_Initialize(_HighPass);
    FILTER_EQUALIZER_Initialize(_Equalizer);
}

void FiltersGlobal::Initialize()
{
    FILTER_NOISE_STATIC_INITIALIZE();
}

void FiltersGlobal::Compute(int& value, const vector<int>& params)
{
    float preampli = powf(2.0f, (params[PARAM_PreAmpli]-100.0f)/10.0f);
    value = (int)(value*preampli);

    FILTER_NOISE_Compute(value, params, _Noise);
    FILTER_FORMANT_Compute(value, params, _Formant);
    FILTER_DISTORTION_Compute(value, params);
    FILTER_BITCRUSHER_Compute(value, params);
    FILTER_OVERDRIVE_Compute(value, params);
    FILTER_DC_Compute(value, _DC);
    FILTER_LOWPASS_Compute(value, params, _LowPass);
    FILTER_HIGHPASS_Compute(value, params, _HighPass);
    FILTER_EQUALIZER_Compute(value, params, _Equalizer);

    float postampli = powf(2.0f, (params[PARAM_PostAmpli]-100.0f)/10.0f);
    value = (int)(value*postampli);
}

void FiltersGlobal::ComputeStereo(int& left, int& right, const vector<int>& params)
{
    FILTER_STEREO_Compute(left, right, params);

    float volume_left = 1.0f;
    float volume_right = 1.0f;
    if (params[PARAM_Pan]>0) volume_left = 1.0f-params[PARAM_Pan]/32.0f;
    else if (params[PARAM_Pan]<0) volume_right = 1.0f+params[PARAM_Pan]/32.0f;

    left = (int)(left*volume_left);
    right = (int)(right*volume_right);
}

void FiltersVoice::Compute(int& value, const vector<int>& params)
{
    FILTER_DISTORTION_Compute(value, params);
    FILTER_BITCRUSHER_Compute(value, params);
}
