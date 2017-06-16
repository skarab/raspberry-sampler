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
        p=f*(1.8-0.8*f);
        k=p+p-1.0;

        double t=(1.0-p)*1.386249;
        double t2=12.0+t*t;
        r = (1.0-pow(1.0-_Resonance/200.0, 3.0))*(t2+6.0*t)/(t2-6.0*t);
    }

    _Compute(left, _Left);
    _Compute(right, _Right);
}

void FilterMoog::_Initialize(Data& data)
{
    data.y1=data.y2=data.y3=data.y4=data.oldx=data.oldy1=data.oldy2=data.oldy3=0.0;
}

void FilterMoog::_Compute(float& value, Data& data)
{
    // process input
    double x = value - r*data.y4;

    //Four cascaded onepole filters (bilinear transform)
    data.y1= x*p +  data.oldx*p - k*data.y1;
    data.y2=data.y1*p + data.oldy1*p - k*data.y2;
    data.y3=data.y2*p + data.oldy2*p - k*data.y3;
    data.y4=data.y3*p + data.oldy3*p - k*data.y4;

    //Clipper band limited sigmoid
    data.y4-=(data.y4*data.y4*data.y4)/6.0;

    data.oldx = x; data.oldy1 = data.y1; data.oldy2 = data.y2; data.oldy3 = data.y3;
    value = (float)data.y4;
}
