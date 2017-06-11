#include "filter_lowpass.h"

FilterLowPass::FilterLowPass()
{
    memset(_InputLeft, 0, sizeof(float)*2);
    memset(_OutputLeft, 0, sizeof(float)*2);
    memset(_InputRight, 0, sizeof(float)*2);
    memset(_OutputRight, 0, sizeof(float)*2);
}

void FilterLowPass::Compute(float& left, float& right, vector<int>& params)
{
    Compute(left, _InputLeft, _OutputLeft, params);
    Compute(right, _InputRight, _OutputRight, params);
}

void FilterLowPass::Compute(float& value, float* inputs, float* outputs, vector<int>& params)
{
    if (params[PARAM_LPCutOff]==256)
        return;

    float r = 0.1f+(256-params[PARAM_LPResonance])*(sqrtf(2.0f)-0.1f)/256.0f;
    float f = powf(params[PARAM_LPCutOff]/512.0f, 3.0f);

    float c = 1.0f/tanf(M_PI*f);

    float a1 = 1.0f/(1.0f+r*c+c*c);
    float a2 = 2.0f*a1;
    float a3 = a1;
    float b1 = 2.0f*(1.0f-c*c)*a1;
    float b2 = (1.0f-r*c+c*c)*a1;

    float input = value;
    value = a1*input+a2*inputs[0]+a3*inputs[1]-b1*outputs[0]-b2*outputs[1];

    inputs[1] = inputs[0];
    inputs[0] = input;

    outputs[1] = outputs[0];
    outputs[0] = value;
}
