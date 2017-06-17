#ifndef __SAMPLER_FILTER_BITCRUSHER__
#define __SAMPLER_FILTER_BITCRUSHER__

#include "includes.h"
#include "filter.h"

class FilterBitCrusher : public Filter
{
public:

    void Compute(float& left, float& right, const vector<int>& params);
};

#endif
