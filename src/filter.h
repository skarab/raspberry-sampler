#ifndef __SAMPLER_FILTER__
#define __SAMPLER_FILTER__

#include "includes.h"
#include "sample.h"

class Filter
{
public:

    virtual void Compute(float& left, float& right, vector<int>& params) {}
};

#endif
