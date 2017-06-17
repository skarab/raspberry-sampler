#include "includes.h"
#include "sample.h"

typedef struct
{
    int CutOff;
    int Resonance;
    double A1, A2, B1, B2;
    double InputLeft[2];
    double OutputLeft[2];
    double InputRight[2];
    double OutputRight[2];
} FILTER_HIGHPASS;

inline void FILTER_HIGHPASS_Initialize(FILTER_HIGHPASS& filter)
{
    filter.CutOff = -1;
    filter.Resonance = -1;

    memset(filter.InputLeft, 0, sizeof(double)*2);
    memset(filter.OutputLeft, 0, sizeof(double)*2);
    memset(filter.InputRight, 0, sizeof(double)*2);
    memset(filter.OutputRight, 0, sizeof(double)*2);
}

inline void FILTER_HIGHPASS_ComputeChannel(double& value, double* inputs, double* outputs, double a1, double a2, double b1, double b2)
{
    double result = a1*value+a2*inputs[0]+a1*inputs[1]-b1*outputs[0]-b2*outputs[1];

    inputs[1] = inputs[0];
    inputs[0] = value;
    outputs[1] = outputs[0];
    outputs[0] = result;

    value = result;
}

inline void FILTER_HIGHPASS_Compute(double& left, double& right, const vector<int>& params, FILTER_HIGHPASS& filter)
{
    if ((filter.CutOff!=params[PARAM_HPCutOff]) || (filter.Resonance!=params[PARAM_HPResonance]))
    {
        filter.CutOff = params[PARAM_HPCutOff];
        filter.Resonance = params[PARAM_HPResonance];
        int cutoff = filter.CutOff;
        double resonance = filter.Resonance;

        // fix low cutoff scratches
        if (resonance>0.0 && cutoff<26)
            cutoff = 20;

        resonance = pow(1.0-resonance/200.0, 2.0);

        double r = 0.04+resonance*(sqrt(2.0)-0.04);
        double f = 0.00001+pow(cutoff/400.0, 3.0);

        double c = tan(M_PI*f);

        filter.A1 = 1.0/(1.0+r*c+c*c);
        filter.A2 = -2.0*filter.A1;
        filter.B1 = 2.0*(c*c-1.0)*filter.A1;
        filter.B2 = (1.0-r*c+c*c)*filter.A1;
    }

    FILTER_HIGHPASS_ComputeChannel(left, filter.InputLeft, filter.OutputLeft, filter.A1, filter.A2, filter.B1, filter.B2);
    FILTER_HIGHPASS_ComputeChannel(right, filter.InputRight, filter.OutputRight, filter.A1, filter.A2, filter.B1, filter.B2);
}
