#ifndef __SAMPLER_FILTER_LOWPASS__
#define __SAMPLER_FILTER_LOWPASS__

#include "includes.h"
#include "filter.h"

class FilterLowPass : public Filter
{
public:

    FilterLowPass();

    virtual void Compute(float& left, float& right, vector<int>& params);

private:

    void Compute(float& value, float* inputs, float* outputs, vector<int>& params);

    float _InputLeft[2];
    float _OutputLeft[2];
    float _InputRight[2];
    float _OutputRight[2];
};

#endif
