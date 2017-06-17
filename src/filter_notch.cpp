#include "filter_notch.h"

FilterNotch::FilterNotch()
{
    _CutOff = -1;
    _BandWidth = -1;

    _Initialize(_Left);
    _Initialize(_Right);
}

void FilterNotch::Compute(float& left, float& right, const vector<int>& params)
{
    if ((_CutOff!=params[PARAM_NotchCutOff]) || (_BandWidth!=params[PARAM_NotchBandWidth]))
    {
        _CutOff = params[PARAM_NotchCutOff];
        _BandWidth = params[PARAM_NotchBandWidth];

        double cutoff = pow(_CutOff/200.0, 2.0)*(170.0/200.0);
        double bandwidth = (1.0-pow(1.0-_BandWidth/200.0, 2.0))*(199.5/200.0);
        double z1x = cos(2.0*M_PI*cutoff);
        _B0 = (1.0-bandwidth)*(1.0-bandwidth)/(2.0*(fabs(z1x)+1.0))+bandwidth;
        _B1 = -2.0*z1x*_B0;
        _A1 = -2.0*z1x*bandwidth;
        _A2 = bandwidth*bandwidth;
    }

    _Compute(left, _Left);
    _Compute(right, _Right);
}

void FilterNotch::_Initialize(Data& data)
{
    memset(&data, 0, sizeof(Data));
}

void FilterNotch::_Compute(float& value, Data& data)
{
    double y  = _B0*data.X0+_B1*data.X1+_B0*data.X2-_A1*data.Y1-_A2*data.Y2;
    data.Y2 = data.Y1;
    data.Y1 = y;
    data.X2 = data.X1;
    data.X1 = data.X0;
    data.X0 = value;

    value = (float)y;
}
