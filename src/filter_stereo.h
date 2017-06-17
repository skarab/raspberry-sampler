#ifndef __SAMPLER_FILTER_STEREO__
#define __SAMPLER_FILTER_STEREO__

#include "includes.h"
#include "filter.h"

class FilterStereo : public Filter
{
public:

    void Compute(float& left, float& right, const vector<int>& params);

};

#endif
