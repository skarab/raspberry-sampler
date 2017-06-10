#ifndef __SAMPLER_FILTER_HIGHPASS__
#define __SAMPLER_FILTER_HIGHPASS__

#include "includes.h"
#include "filter.h"

class FilterHighPass : public Filter
{
public:

    FilterHighPass();

    virtual void Compute(float& left, float& right, vector<float>& params);

private:

    void Compute(float& value, float* inputs, float* outputs, vector<float>& params);

    float _InputLeft[2];
    float _OutputLeft[2];
    float _InputRight[2];
    float _OutputRight[2];
};

#endif
