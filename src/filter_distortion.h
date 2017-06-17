#ifndef __SAMPLER_FILTER_DISTORTION__
#define __SAMPLER_FILTER_DISTORTION__

#include "includes.h"
#include "filter.h"

class FilterDistortion : public Filter
{
public:

    void Compute(float& left, float& right, const vector<int>& params);
};

#endif
