#include "includes.h"
#include "sample.h"

typedef struct
{
    int CutOff;
    int Resonance;
    double P, K, R;
    double Y1, Y2, Y3, Y4;
    double OldX;
    double OldY1, OldY2, OldY3;
} FILTER_LOWPASS;

inline void FILTER_LOWPASS_Initialize(FILTER_LOWPASS& filter)
{
    memset(&filter, 0, sizeof(FILTER_LOWPASS));

    filter.CutOff = -1;
    filter.Resonance = -1;
}

inline void FILTER_LOWPASS_Compute(double& value, FILTER_LOWPASS& filter)
{
    // process input
    double x = value - filter.R*filter.Y4;

    //Four cascaded onepole filters (bilinear transform)
    filter.Y1 = x*filter.P + filter.OldX*filter.P - filter.K*filter.Y1;
    filter.Y2 = filter.Y1*filter.P + filter.OldY1*filter.P - filter.K*filter.Y2;
    filter.Y3 = filter.Y2*filter.P + filter.OldY2*filter.P - filter.K*filter.Y3;
    filter.Y4 = filter.Y3*filter.P + filter.OldY3*filter.P - filter.K*filter.Y4;

    //Clipper band limited sigmoid
    filter.Y4 -= (filter.Y4*filter.Y4*filter.Y4)/6.0;

    filter.OldX = x;
    filter.OldY1 = filter.Y1;
    filter.OldY2 = filter.Y2;
    filter.OldY3 = filter.Y3;

    if (filter.Y4<-10.0 || filter.Y4>10.0) filter.Y4 = 0.0;

    value = filter.Y4;
}

inline void FILTER_LOWPASS_Compute(int& value, const vector<int>& params, FILTER_LOWPASS& filter)
{
    if ((filter.CutOff!=params[PARAM_GLOBAL_LPCutOff]) || (filter.Resonance!=params[PARAM_GLOBAL_LPResonance]))
    {
        filter.CutOff = params[PARAM_GLOBAL_LPCutOff];
        filter.Resonance = params[PARAM_GLOBAL_LPResonance];

        double cutoff = 40.0+filter.CutOff*(200.0-40.0)/200.0;

        double f = pow(cutoff/200.0, 4.0);
        filter.P = f*(1.8-0.8*f);
        filter.K = filter.P+filter.P-1.0;

        double t = (1.0-filter.P)*1.386249;
        double t2 = 12.0+t*t;
        filter.R = (1.0-pow(1.0-filter.Resonance/200.0, 4.0))*(t2+6.0*t)/(t2-6.0*t);
    }

    double in = value/32767.0;
    FILTER_LOWPASS_Compute(in, filter);
    value = (int)(in*32767.0);
}

