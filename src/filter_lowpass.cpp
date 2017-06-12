#include "filter_lowpass.h"

FilterLowPass::FilterLowPass()
{
    memset(_InputLeft, 0, sizeof(double)*2);
    memset(_OutputLeft, 0, sizeof(double)*2);
    memset(_InputRight, 0, sizeof(double)*2);
    memset(_OutputRight, 0, sizeof(double)*2);
}

inline void FilterLowPass_Compute(float& value, double* inputs, double* outputs, double a1, double a2, double b1, double b2)
{
    double result = a1*value+a2*inputs[0]+a1*inputs[1]-b1*outputs[0]-b2*outputs[1];

    inputs[1] = inputs[0];
    inputs[0] = value;
    outputs[1] = outputs[0];
    outputs[0] = result;

    value = (float)result;
}

void FilterLowPass::Compute(float& left, float& right, const vector<int>& params)
{
    int cutoff = params[PARAM_LPCutOff];
    double resonance = params[PARAM_LPResonance];

    // fix low cutoff scratches
    if (resonance>0.0 && cutoff<26)
        cutoff = 20;

    resonance = pow(1.0-resonance/200.0, 2.0);

    double r = 0.04+resonance*(sqrt(2.0)-0.04);
    double f = 0.00001+pow(cutoff/400.0, 3.0);

    double c = 1.0/tan(M_PI*f);

    double a1 = 1.0/(1.0+r*c+c*c);
    double a2 = 2.0*a1;
    double b1 = 2.0*(1.0-c*c)*a1;
    double b2 = (1.0-r*c+c*c)*a1;

    FilterLowPass_Compute(left, _InputLeft, _OutputLeft, a1, a2, b1, b2);
    FilterLowPass_Compute(right, _InputRight, _OutputRight, a1, a2, b1, b2);
}
