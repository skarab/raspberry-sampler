#ifndef __SAMPLER_FILTER_LOWPASS__
#define __SAMPLER_FILTER_LOWPASS__

#include "includes.h"
#include "filter.h"

class FilterLowPass : public Filter
{
public:

    FilterLowPass();

    void Compute(float& left, float& right, const vector<int>& params);

private:

    double _InputLeft[2];
    double _OutputLeft[2];
    double _InputRight[2];
    double _OutputRight[2];
};

#endif
