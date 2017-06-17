#ifndef __SAMPLER_FILTER_OVERDRIVE__
#define __SAMPLER_FILTER_OVERDRIVE__

#include "includes.h"
#include "filter.h"

class FilterOverdrive : public Filter
{
public:

    void Compute(float& left, float& right, const vector<int>& params);

};

#endif
