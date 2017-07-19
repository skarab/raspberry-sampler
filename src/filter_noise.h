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
            FILTER_NOISE_BUFFER[j][i] = v/(s*0.6f);
            FILTER_CLICK_BUFFER[j][i] = c/(s*0.2f);
        }
    }
}

typedef struct
{
    int ID;
    double ClicksPosition;
} FILTER_NOISE;

inline void FILTER_NOISE_Initialize(FILTER_NOISE& filter)
{
    filter.ID = rand()%SAMPLER_RATE;
    filter.ClicksPosition = rand()%SAMPLER_RATE;
}

inline void FILTER_NOISE_Compute(int& value, const vector<int>& params, FILTER_NOISE& filter)
{
    // Noise

    filter.ID = (filter.ID+1)%SAMPLER_RATE;
    float t = params[PARAM_GLOBAL_NoiseTexture]*20.0f/200.0f;
    int t1 = floor(t);
    int t2 = ceil(t);
    float l = t-t1;
    float s1 = powf(params[PARAM_GLOBAL_Noise]/200.0f, 2.0f);
    float noise = (FILTER_NOISE_BUFFER[t1][filter.ID]*(1.0f-l)+FILTER_NOISE_BUFFER[t2][filter.ID]*l)*s1;

    // Clicks

    filter.ClicksPosition += (params[PARAM_GLOBAL_ClicksPitch]/200.0)*4.0+0.01;
    int p = (int)filter.ClicksPosition;
    if (p>=SAMPLER_RATE)
    {
        p = 0;
        filter.ClicksPosition = 0.0;
    }

    t = params[PARAM_GLOBAL_ClicksTexture]*20.0f/200.0f;
    t1 = floor(t);
    t2 = ceil(t);
    l = t-t1;
    float s2 = powf(params[PARAM_GLOBAL_Clicks]/200.0f, 2.0f);
    noise += (FILTER_CLICK_BUFFER[t1][p]*(1.0f-l)+FILTER_CLICK_BUFFER[t2][p]*l)*s2;

    // Mix

    float mix = params[PARAM_GLOBAL_NoiseMix]/100.0f;
    value = (int)((noise/32767.0f+1.0f-(s1+s2)/2.0f)*value*mix+(noise+value)*(1.0f-mix));
}
