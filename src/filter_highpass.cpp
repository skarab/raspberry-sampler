#include "filter_highpass.h"

FilterHighPass::FilterHighPass()
{
    memset(_InputLeft, 0, sizeof(float)*2);
    memset(_OutputLeft, 0, sizeof(float)*2);
    memset(_InputRight, 0, sizeof(float)*2);
    memset(_OutputRight, 0, sizeof(float)*2);
}

void FilterHighPass::Compute(float& left, float& right, vector<float>& params)
{
    Compute(left, _InputLeft, _OutputLeft, params);
    Compute(right, _InputRight, _OutputRight, params);
}

void FilterHighPass::Compute(float& value, float* inputs, float* outputs, vector<float>& params)
{
    if (params[(int)PARAM_HPCutOff]==0)
        return;

    float r = 0.01f+(256-params[(int)PARAM_HPResonance])*(sqrtf(2.0f)-0.01f)/256.0f;
    float f = powf(params[(int)PARAM_HPCutOff]/512.0f, 3.0f);

    float c = tanf(M_PI*f);

    float a1 = 1.0f/(1.0f+r*c+c*c);
    float a2 = -2.0f*a1;
    float a3 = a1;
    float b1 = 2.0f*(c*c-1.0f)*a1;
    float b2 = (1.0f-r*c+c*c)*a1;

    float input = value;
    value = a1*input+a2*inputs[0]+a3*inputs[1]-b1*outputs[0]-b2*outputs[1];

    inputs[1] = inputs[0];
    inputs[0] = input;

    outputs[1] = outputs[0];
    outputs[0] = value;
}
