#include "includes.h"
#include "sample.h"

typedef struct
{
    double Y1, Y2, Y3, Y4;
    double OldX;
    double OldY1, OldY2, OldY3;
} FILTER_MOOG_CHANNEL;

typedef struct
{
    int CutOff;
    int Resonance;
    double P, K, R;
    FILTER_MOOG_CHANNEL Left;
    FILTER_MOOG_CHANNEL Right;
} FILTER_MOOG;

inline void FILTER_MOOG_Initialize(FILTER_MOOG& filter)
{
    filter.CutOff = -1;
    filter.Resonance = -1;

    memset(&filter.Left, 0, sizeof(FILTER_MOOG_CHANNEL));
    memset(&filter.Right, 0, sizeof(FILTER_MOOG_CHANNEL));
}

inline void FILTER_MOOG_ComputeChannel(double& value, FILTER_MOOG& filter, FILTER_MOOG_CHANNEL& data)
{
    // process input
    double x = value - filter.R*data.Y4;

    //Four cascaded onepole filters (bilinear transform)
    data.Y1 = x*filter.P +  data.OldX*filter.P - filter.K*data.Y1;
    data.Y2 = data.Y1*filter.P + data.OldY1*filter.P - filter.K*data.Y2;
    data.Y3 = data.Y2*filter.P + data.OldY2*filter.P - filter.K*data.Y3;
    data.Y4 = data.Y3*filter.P + data.OldY3*filter.P - filter.K*data.Y4;

    //Clipper band limited sigmoid
    data.Y4 -= (data.Y4*data.Y4*data.Y4)/6.0;

    data.OldX = x;
    data.OldY1 = data.Y1;
    data.OldY2 = data.Y2;
    data.OldY3 = data.Y3;

    value = data.Y4;
}

inline void FILTER_MOOG_Compute(double& left, double& right, const vector<int>& params, FILTER_MOOG& filter)
{
    if ((filter.CutOff!=params[PARAM_MoogCutOff]) || (filter.Resonance!=params[PARAM_MoogResonance]))
    {
        filter.CutOff = params[PARAM_MoogCutOff];
        filter.Resonance = params[PARAM_MoogResonance];

        double f = pow(filter.CutOff/200.0, 3.0);
        filter.P = f*(1.8-0.8*f);
        filter.K = filter.P+filter.P-1.0;

        double t = (1.0-filter.P)*1.386249;
        double t2 = 12.0+t*t;
        filter.R = (1.0-pow(1.0-filter.Resonance/200.0, 3.0))*(t2+6.0*t)/(t2-6.0*t);
    }

    FILTER_MOOG_ComputeChannel(left, filter, filter.Left);
    FILTER_MOOG_ComputeChannel(right, filter, filter.Right);
}

