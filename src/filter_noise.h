#include "includes.h"
#include "sample.h"

static double FILTER_NOISE_BUFFER[SAMPLER_RATE];

inline void FILTER_NOISE_STATIC_INITIALIZE()
{
    for (int i=0 ; i<SAMPLER_RATE ; ++i)
        FILTER_NOISE_BUFFER[i] = ((rand()%20001)/10000.0-1.0)*0.06;
}

typedef struct
{
    double B0, B1, B2, B3, B4, B5, B6;
} FILTER_NOISE_CHANNEL;

typedef struct
{
    double Noise;
    double Pink;
    int ID;
    FILTER_NOISE_CHANNEL Left;
    FILTER_NOISE_CHANNEL Right;
} FILTER_NOISE;

inline void FILTER_NOISE_Initialize(FILTER_NOISE& filter)
{
    filter.ID = 0;
    memset(&filter.Left, 0, sizeof(FILTER_NOISE_CHANNEL));
    memset(&filter.Right, 0, sizeof(FILTER_NOISE_CHANNEL));
}

inline void FILTER_NOISE_ComputeChannel(double& value, double white, FILTER_NOISE& filter, FILTER_NOISE_CHANNEL& data)
{
    data.B0 = 0.99886*data.B0 + white*0.0555179;
    data.B1 = 0.99332*data.B1 + white*0.0750759;
    data.B2 = 0.96900*data.B2 + white*0.1538520;
    data.B3 = 0.86650*data.B3 + white*0.3104856;
    data.B4 = 0.55000*data.B4 + white*0.5329522;
    data.B5 = -0.7616*data.B5 - white*0.0168980;
    double pink = data.B0+data.B1+data.B2+data.B3+data.B4+data.B5+data.B6+white*0.5362;
    data.B6 = white * 0.115926;

    pink = pink*filter.Pink*value+white*(1.0-filter.Pink);

    value = pink*filter.Noise+value*(1.0-filter.Noise);
}

inline void FILTER_NOISE_Compute(double& left, double& right, const vector<int>& params, FILTER_NOISE& filter)
{
    filter.Noise = params[PARAM_Noise]/200.0;
    filter.Pink = params[PARAM_NoisePink]/200.0;

    filter.ID = (filter.ID+1)%SAMPLER_RATE;

    double white = FILTER_NOISE_BUFFER[filter.ID];

    FILTER_NOISE_ComputeChannel(left, white, filter, filter.Left);
    FILTER_NOISE_ComputeChannel(right, white, filter, filter.Right);
}
