#include "filter_highpass.h"

FilterHighPass::FilterHighPass()
{
    memset(_InputLeft, 0, sizeof(float)*2);
    memset(_OutputLeft, 0, sizeof(float)*2);
    memset(_InputRight, 0, sizeof(float)*2);
    memset(_OutputRight, 0, sizeof(float)*2);
}

void FilterHighPass::Compute(float& left, float& right, const vector<int>& params)
{
    float r = 0.1f+(256-params[PARAM_HPResonance])*(sqrtf(2.0f)-0.1f)/256.0f;
    float f = powf(params[PARAM_HPCutOff]/512.0f, 3.0f);

    float c = tanf(M_PI*f);

    float a1 = 1.0f/(1.0f+r*c+c*c);
    float a2 = -2.0f*a1;
    float b1 = 2.0f*(c*c-1.0f)*a1;
    float b2 = (1.0f-r*c+c*c)*a1;

    _Compute(left, _InputLeft, _OutputLeft, a1, a2, b1, b2);
    _Compute(right, _InputRight, _OutputRight, a1, a2, b1, b2);
}
