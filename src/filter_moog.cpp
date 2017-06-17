#include "filter_moog.h"

FilterMoog::FilterMoog()
{
    _CutOff = -1;
    _Resonance = -1;

    _Initialize(_Left);
    _Initialize(_Right);
}

void FilterMoog::Compute(float& left, float& right, const vector<int>& params)
{
    if ((_CutOff!=params[PARAM_MoogCutOff]) || (_Resonance!=params[PARAM_MoogResonance]))
    {
        _CutOff = params[PARAM_MoogCutOff];
        _Resonance = params[PARAM_MoogResonance];

        double f = pow(_CutOff/200.0, 3.0);
        _P = f*(1.8-0.8*f);
        _K = _P+_P-1.0;

        double t = (1.0-_P)*1.386249;
        double t2 = 12.0+t*t;
        _R = (1.0-pow(1.0-_Resonance/200.0, 3.0))*(t2+6.0*t)/(t2-6.0*t);
    }

    _Compute(left, _Left);
    _Compute(right, _Right);
}

void FilterMoog::_Initialize(Data& data)
{
    memset(&data, 0, sizeof(Data));
}

void FilterMoog::_Compute(float& value, Data& data)
{
    // process input
    double x = value - _R*data.Y4;

    //Four cascaded onepole filters (bilinear transform)
    data.Y1 = x*_P +  data.OldX*_P - _K*data.Y1;
    data.Y2 = data.Y1*_P + data.OldY1*_P - _K*data.Y2;
    data.Y3 = data.Y2*_P + data.OldY2*_P - _K*data.Y3;
    data.Y4 = data.Y3*_P + data.OldY3*_P - _K*data.Y4;

    //Clipper band limited sigmoid
    data.Y4 -= (data.Y4*data.Y4*data.Y4)/6.0;

    data.OldX = x;
    data.OldY1 = data.Y1;
    data.OldY2 = data.Y2;
    data.OldY3 = data.Y3;

    value = (float)data.Y4;
}
