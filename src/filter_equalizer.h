#include "includes.h"
#include "sample.h"

typedef struct
{
    double lf;
    double hf;
    double f1p[4];
    double f2p[4];
    double sdm[4];
} FILTER_EQUALIZER;

inline void FILTER_EQUALIZER_Clear(FILTER_EQUALIZER& filter)
{
    memset(filter.f1p, 0, sizeof(double)*4);
    memset(filter.f2p, 0, sizeof(double)*4);
    memset(filter.sdm, 0, sizeof(double)*4);
}

inline void FILTER_EQUALIZER_Initialize(FILTER_EQUALIZER& filter)
{
    filter.lf = 2.0*sin(M_PI*880.0/SAMPLER_RATE);
    filter.hf = 2.0*sin(M_PI*5000.0/SAMPLER_RATE);

    FILTER_EQUALIZER_Clear(filter);
}

inline void FILTER_EQUALIZER_Compute(double& value, double low, double mid, double high, double lf, double hf, double* f1p, double* f2p, double* sdm)
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

inline void FILTER_EQUALIZER_Compute(int& value, const vector<int>& params, FILTER_EQUALIZER& filter)
{
    double low = pow(2.0, (params[PARAM_GLOBAL_EQLow]-100.0)/10.0);
    double mid = pow(2.0, (params[PARAM_GLOBAL_EQMedium]-100.0)/10.0);
    double high = pow(2.0, (params[PARAM_GLOBAL_EQHigh]-100.0)/10.0);

    double in = value/32767.0;
    FILTER_EQUALIZER_Compute(in, low, mid, high, filter.lf, filter.hf, filter.f1p, filter.f2p, filter.sdm);
    value = (int)(in*32767.0);
}

