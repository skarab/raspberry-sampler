#ifndef __SAMPLER_FILTER_LOWPASS__
#define __SAMPLER_FILTER_LOWPASS__

#include "includes.h"
#include "filter.h"

class FilterLowPass : public Filter
{
public:

    void Compute(float& left, float& right, const vector<int>& params)
    {
    /*
        float r = 0.10001f+(256-params[PARAM_LPResonance])*(sqrtf(2.0f)-0.10001f)/256.0f;
        float f = 0.00001f+powf(params[PARAM_LPCutOff]/512.0f, 3.0f);

        float c = 1.0f/tanf(M_PI*f);

        float a1 = 1.0f/(1.0f+r*c+c*c);
        float a2 = 2.0f*a1;
        float b1 = 2.0f*(1.0f-c*c)*a1;
        float b2 = (1.0f-r*c+c*c)*a1;

        _Compute(left, _InputLeft, _OutputLeft, a1, a2, b1, b2);
        _Compute(right, _InputRight, _OutputRight, a1, a2, b1, b2);*/
    }

};

#endif
