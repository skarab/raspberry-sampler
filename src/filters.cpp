#include "filters.h"
#include "filter_stereo.h"
#include "filter_bitcrusher.h"
#include "filter_distortion.h"
#include "filter_overdrive.h"

FiltersGlobal::FiltersGlobal()
{
    FILTER_DC_Initialize(_DC);
    FILTER_NOISE_Initialize(_Noise);
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
    FILTER_NOISE_Compute(value, params, _Noise);
    FILTER_DC_Compute(value, _DC);
    FILTER_LOWPASS_Compute(value, params, _LowPass);
    FILTER_HIGHPASS_Compute(value, params, _HighPass);
    FILTER_EQUALIZER_Compute(value, params, _Equalizer);

    float volume = powf(2.0f, (params[PARAM_GLOBAL_Volume]-100.0f)/10.0f);
    value = (int)(value*volume);
}

void FiltersGlobal::ComputeStereo(int& left, int& right, const vector<int>& params)
{
    FILTER_STEREO_Compute(left, right, params[PARAM_GLOBAL_Stereo]);

    float pan = params[PARAM_GLOBAL_Pan]/32.0f;
    if (pan>0.0f) left = (int)(left*(1.0f-pan));
    else if (pan<0.0f) right = (int)(right*(1.0f+pan));
}

FiltersVoice::FiltersVoice()
{
    FILTER_DC_Initialize(_DC);
    FILTER_FORMANT_Initialize(_Formant);
}

void FiltersVoice::Compute(int& value, const vector<int>& params)
{
    FILTER_DC_Compute(value, _DC);
    FILTER_FORMANT_Compute(value, params[PARAM_SAMPLE_Formant], _Formant);
    FILTER_DISTORTION_Compute(value, params[PARAM_SAMPLE_Distortion]);
    FILTER_BITCRUSHER_Compute(value, params[PARAM_SAMPLE_BitCrusher]);
}

void FiltersVoice::ComputeStereo(int& left, int& right, const vector<int>& params)
{
    FILTER_STEREO_Compute(left, right, params[PARAM_SAMPLE_Stereo]);

    float pan = params[PARAM_SAMPLE_Pan]/32.0f;
    if (pan>0.0f) left = (int)(left*(1.0f-pan));
    else if (pan<0.0f) right = (int)(right*(1.0f+pan));
}
