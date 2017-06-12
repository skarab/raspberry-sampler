#ifndef __SAMPLER_FILTER_HIGHPASS__
#define __SAMPLER_FILTER_HIGHPASS__

#include "includes.h"
#include "filter.h"

class FilterHighPass : public Filter
{
public:

    FilterHighPass();

    void Compute(float& left, float& right, const vector<int>& params);

private:

    int _CutOff;
    int _Resonance;
    double _A1, _A2, _B1, _B2;
    double _InputLeft[2];
    double _OutputLeft[2];
    double _InputRight[2];
    double _OutputRight[2];
};

#endif
