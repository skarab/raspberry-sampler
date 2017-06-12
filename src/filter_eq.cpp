#include "filter_eq.h"

FilterEQ::FilterEQ()
{
    _lf = 2.0*sin(M_PI*880.0/SAMPLER_RATE);
    _hf = 2.0*sin(M_PI*5000.0/SAMPLER_RATE);

    memset(_left_f1p, 0, sizeof(double)*4);
    memset(_left_f2p, 0, sizeof(double)*4);
    memset(_left_sdm, 0, sizeof(double)*4);
    memset(_right_f1p, 0, sizeof(double)*4);
    memset(_right_f2p, 0, sizeof(double)*4);
    memset(_right_sdm, 0, sizeof(double)*4);
}

inline float FilterEQ_Compute(float input, float low, float mid, float high, double lf, double hf, double* f1p, double* f2p, double* sdm)
{
    static double vsa = 1.0/4294967295.0;
    double l,m,h;

    // Filter #1 (lowpass)

    f1p[0] += lf*(input-f1p[0])+vsa;
    f1p[1] += lf*(f1p[0]-f1p[1]);
    f1p[2] += lf*(f1p[1]-f1p[2]);
    f1p[3] += lf*(f1p[2]-f1p[3]);

    l = f1p[3];

    // Filter #2 (highpass)

    f2p[0] += hf*(input-f2p[0])+vsa;
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
    sdm[1] = input;

    // Return result

    return (float)(l+m+h);
}

void FilterEQ::Compute(float& left, float& right, const vector<int>& params)
{
    float low = params[PARAM_EQLow]/100.0f;
    float mid = params[PARAM_EQMedium]/100.0f;
    float high = params[PARAM_EQHigh]/100.0f;

    left = FilterEQ_Compute(left, low, mid, high, _lf, _hf, _left_f1p, _left_f2p, _left_sdm);
    right = FilterEQ_Compute(right, low, mid, high, _lf, _hf, _right_f1p, _right_f2p, _right_sdm);
}
