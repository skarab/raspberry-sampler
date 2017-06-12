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

    float _InputLeft[2];
    float _OutputLeft[2];
    float _InputRight[2];
    float _OutputRight[2];
};

#endif
