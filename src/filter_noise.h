#include "includes.h"
#include "sample.h"

static int FILTER_NOISE_BUFFER[SAMPLER_RATE];

inline void FILTER_NOISE_STATIC_INITIALIZE()
{
    for (int i=0 ; i<SAMPLER_RATE ; ++i)
        FILTER_NOISE_BUFFER[i] = rand()%(32767*2)-32767;
}

typedef struct
{
    int ID;
} FILTER_NOISE;

inline void FILTER_NOISE_Initialize(FILTER_NOISE& filter)
{
    filter.ID = 0;
}

inline void FILTER_NOISE_Compute(int& value, const vector<int>& params, FILTER_NOISE& filter)
{
    float strength = powf(params[PARAM_Noise]/200.0f, 3.0f);
    float mix = params[PARAM_NoiseMix]/100.0f;
    filter.ID = (filter.ID+1)%SAMPLER_RATE;
    float noise = strength*FILTER_NOISE_BUFFER[filter.ID];
    value = (int)((noise/32767.0f+1.0f-strength)*value*mix+(noise+value)*(1.0f-mix));
}
