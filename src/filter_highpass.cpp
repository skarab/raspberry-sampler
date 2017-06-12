#include "filter_highpass.h"

FilterHighPass::FilterHighPass()
{
    memset(_InputLeft, 0, sizeof(float)*2);
    memset(_OutputLeft, 0, sizeof(float)*2);
    memset(_InputRight, 0, sizeof(float)*2);
    memset(_OutputRight, 0, sizeof(float)*2);
}

inline void FilterHighPass_Compute(float& value, float* inputs, float* outputs, float a1, float a2, float b1, float b2)
{
    float input = value;
    value = a1*input+a2*inputs[0]+a1*inputs[1]-b1*outputs[0]-b2*outputs[1];

    if (fabs(value)>1.0f)
        value /= fabs(value);

    inputs[1] = inputs[0];
    inputs[0] = input;
    outputs[1] = outputs[0];
    outputs[0] = value;
}

void FilterHighPass::Compute(float& left, float& right, const vector<int>& params)
{
    int cutoff = params[PARAM_HPCutOff];
    float resonance = params[PARAM_HPResonance];

    // fix low cutoff scratches
    if (resonance>0.0f && cutoff<26)
        cutoff = 20;

    resonance = powf(1.0f-resonance/200.0f, 2.0f);

    float r = 0.04f+resonance*(sqrtf(2.0f)-0.04f);
    float f = 0.00001f+powf(cutoff/400.0f, 3.0f);

    float c = tanf(M_PI*f);

    float a1 = 1.0f/(1.0f+r*c+c*c);
    float a2 = -2.0f*a1;
    float b1 = 2.0f*(c*c-1.0f)*a1;
    float b2 = (1.0f-r*c+c*c)*a1;

    FilterHighPass_Compute(left, _InputLeft, _OutputLeft, a1, a2, b1, b2);
    FilterHighPass_Compute(right, _InputRight, _OutputRight, a1, a2, b1, b2);
}
