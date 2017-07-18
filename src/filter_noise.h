#include "includes.h"
#include "sample.h"

static int FILTER_NOISE_BUFFER[21][SAMPLER_RATE];
static int FILTER_CLICK_BUFFER[21][SAMPLER_RATE];

inline void FILTER_NOISE_STATIC_INITIALIZE()
{
    for (int i=0 ; i<SAMPLER_RATE ; ++i)
    {
        FILTER_NOISE_BUFFER[0][i] = rand()%(32767*2)-32767;

        int r = rand()%2000;
        FILTER_CLICK_BUFFER[0][i] = r<1?-32767:(r>1998?32767:0);
    }

    for (int j=1 ; j<21 ; ++j)
    {
        int s = j*2;

        for (int i=0 ; i<SAMPLER_RATE ; ++i)
        {
            int v = 0;
            int c = 0;
            for (int k=i-s ; k<i+s ; ++k)
            {
                int id = k;
                if (id<0) id += SAMPLER_RATE;
                else if (id>SAMPLER_RATE-1) id -= SAMPLER_RATE;
                v += FILTER_NOISE_BUFFER[0][id];
                c += FILTER_CLICK_BUFFER[0][id];
            }
            FILTER_NOISE_BUFFER[j][i] = v/(s*0.8f);
            FILTER_CLICK_BUFFER[j][i] = c/(s*0.4f);
        }
    }
}

typedef struct
{
    int ID;
    double HardPosition;
} FILTER_NOISE;

inline void FILTER_NOISE_Initialize(FILTER_NOISE& filter)
{
    filter.ID = 0;
    filter.HardPosition = 0.0;
}

inline void FILTER_NOISE_Compute(int& value, const vector<int>& params, FILTER_NOISE& filter)
{
    float strength = powf(params[PARAM_GLOBAL_Noise]/200.0f, 2.0f);
    filter.ID = (filter.ID+1)%SAMPLER_RATE;
    float noise = strength*FILTER_NOISE_BUFFER[params[PARAM_GLOBAL_NoiseTexture]][filter.ID];

    strength = powf(params[PARAM_GLOBAL_Motor]/200.0f, 2.0f);
    filter.HardPosition += (params[PARAM_GLOBAL_MotorSpeed]/200.0f)*8.0f+0.1f;
    int p = (int)filter.HardPosition;
    if (p>=SAMPLER_RATE)
    {
        p = 0;
        filter.HardPosition = 0.0;
    }
    noise += strength*FILTER_CLICK_BUFFER[params[PARAM_GLOBAL_MotorTexture]][p];

    float mix = params[PARAM_GLOBAL_NoiseMix]/100.0f;
    value = (int)((noise/32767.0f+1.0f-strength)*value*mix+(noise+value)*(1.0f-mix));
}
