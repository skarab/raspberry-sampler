#include "includes.h"
#include "sample.h"

typedef struct
{
    double Y1, Y2;
    double X0, X1, X2;
} FILTER_NOTCH_CHANNEL;

typedef struct
{
    int CutOff;
    int BandWidth;
    double B0, B1, A1, A2;
    FILTER_NOTCH_CHANNEL Left;
    FILTER_NOTCH_CHANNEL Right;
} FILTER_NOTCH;

inline void FILTER_NOTCH_Initialize(FILTER_NOTCH& filter)
{
    filter.CutOff = -1;
    filter.BandWidth = -1;
    memset(&filter.Left, 0, sizeof(FILTER_NOTCH_CHANNEL));
    memset(&filter.Right, 0, sizeof(FILTER_NOTCH_CHANNEL));
}

inline void FILTER_NOTCH_ComputeChannel(double& value, FILTER_NOTCH& filter, FILTER_NOTCH_CHANNEL& data)
{
    double y  = filter.B0*data.X0+filter.B1*data.X1+filter.B0*data.X2-filter.A1*data.Y1-filter.A2*data.Y2;
    data.Y2 = data.Y1;
    data.Y1 = y;
    data.X2 = data.X1;
    data.X1 = data.X0;
    data.X0 = value;

    value = y;
}

inline void FILTER_NOTCH_Compute(double& left, double& right, const vector<int>& params, FILTER_NOTCH& filter)
{
    if ((filter.CutOff!=params[PARAM_NotchCutOff]) || (filter.BandWidth!=params[PARAM_NotchBandWidth]))
    {
        filter.CutOff = params[PARAM_NotchCutOff];
        filter.BandWidth = params[PARAM_NotchBandWidth];

        double cutoff = pow(filter.CutOff/200.0, 2.0)*(170.0/200.0);
        double bandwidth = (1.0-pow(1.0-filter.BandWidth/200.0, 2.0))*(199.5/200.0);
        double z1x = cos(2.0*M_PI*cutoff);
        filter.B0 = (1.0-bandwidth)*(1.0-bandwidth)/(2.0*(fabs(z1x)+1.0))+bandwidth;
        filter.B1 = -2.0*z1x*filter.B0;
        filter.A1 = -2.0*z1x*bandwidth;
        filter.A2 = bandwidth*bandwidth;
    }

    FILTER_NOTCH_ComputeChannel(left, filter, filter.Left);
    FILTER_NOTCH_ComputeChannel(right, filter, filter.Right);
}
