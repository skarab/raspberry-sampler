#include "includes.h"
#include "sample.h"

typedef struct
{
    double lf;
    double hf;
    double left_f1p[4];
    double left_f2p[4];
    double left_sdm[4];
    double right_f1p[4];
    double right_f2p[4];
    double right_sdm[4];
} FILTER_EQ;

inline void FILTER_EQ_Initialize(FILTER_EQ& filter)
{
    filter.lf = 2.0*sin(M_PI*880.0/SAMPLER_RATE);
    filter.hf = 2.0*sin(M_PI*5000.0/SAMPLER_RATE);

    memset(filter.left_f1p, 0, sizeof(double)*4);
    memset(filter.left_f2p, 0, sizeof(double)*4);
    memset(filter.left_sdm, 0, sizeof(double)*4);
    memset(filter.right_f1p, 0, sizeof(double)*4);
    memset(filter.right_f2p, 0, sizeof(double)*4);
    memset(filter.right_sdm, 0, sizeof(double)*4);
}

inline void FILTER_EQ_ComputeChannel(double& value, double low, double mid, double high, double lf, double hf, double* f1p, double* f2p, double* sdm)
{
    static const double vsa = 1.0/4294967295.0;
    double l,m,h;

    // Filter #1 (lowpass)

    f1p[0] += lf*(value-f1p[0])+vsa;
    f1p[1] += lf*(f1p[0]-f1p[1]);
    f1p[2] += lf*(f1p[1]-f1p[2]);
    f1p[3] += lf*(f1p[2]-f1p[3]);

    l = f1p[3];

    // Filter #2 (highpass)

    f2p[0] += hf*(value-f2p[0])+vsa;
    f2p[1] += hf*(f2p[0]-f2p[1]);
    f2p[2] += hf*(f2p[1]-f2p[2]);
    f2p[3] += hf*(f2p[2]-f2p[3]);

    h = sdm[3]-f2p[3];

    // Calculate midrange (signal - (low + high))

    m = sdm[3]-(h+l);

    // Scale, Combine and store

    l *= low;
    m *= mid;
    h *= high;

    // Shuffle history buffer

    sdm[3] = sdm[2];
    sdm[2] = sdm[1];
    sdm[1] = value;

    // Return result

    value = l+m+h;
}

inline void FILTER_EQ_Compute(double& left, double& right, const vector<int>& params, FILTER_EQ& filter)
{
    double low = params[PARAM_EQLow]/100.0;
    double mid = params[PARAM_EQMedium]/100.0;
    double high = params[PARAM_EQHigh]/100.0;

    FILTER_EQ_ComputeChannel(left, low, mid, high, filter.lf, filter.hf, filter.left_f1p, filter.left_f2p, filter.left_sdm);
    FILTER_EQ_ComputeChannel(right, low, mid, high, filter.lf, filter.hf, filter.right_f1p, filter.right_f2p, filter.right_sdm);
}




