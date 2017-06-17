#include "filter_noise.h"
#include "device.h"

FilterNoise::FilterNoise()
{
    _Initialize(_Left);
    _Initialize(_Right);

    _ID = 0;
}

void FilterNoise::Compute(float& left, float& right, const vector<int>& params)
{
    _Noise = params[PARAM_Noise]/200.0;
    _Pink = params[PARAM_NoisePink]/200.0;

    _ID = (_ID+1)%SAMPLER_RATE;

    double white = Device::Get().GetNoise(_ID);

    _Compute(left, white, _Left);
    _Compute(right, white, _Right);
}

void FilterNoise::_Initialize(Data& data)
{
    memset(&data, 0, sizeof(Data));
}

void FilterNoise::_Compute(float& value, double white, Data& data)
{
    data.B0 = 0.99886*data.B0 + white*0.0555179;
    data.B1 = 0.99332*data.B1 + white*0.0750759;
    data.B2 = 0.96900*data.B2 + white*0.1538520;
    data.B3 = 0.86650*data.B3 + white*0.3104856;
    data.B4 = 0.55000*data.B4 + white*0.5329522;
    data.B5 = -0.7616*data.B5 - white*0.0168980;
    double pink = data.B0+data.B1+data.B2+data.B3+data.B4+data.B5+data.B6+white*0.5362;
    data.B6 = white * 0.115926;

    pink = pink*_Pink*value+white*(1.0-_Pink);

    value = (float)(pink*_Noise+value*(1.0-_Noise));
}
