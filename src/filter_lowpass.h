#ifndef __SAMPLER_FILTER_LOWPASS__
#define __SAMPLER_FILTER_LOWPASS__

#include "includes.h"
#include "filter_highpass.h"

class FilterLowPass : public FilterHighPass
{
public:

    virtual void Compute(float& left, float& right, const vector<int>& params);
};

#endif
