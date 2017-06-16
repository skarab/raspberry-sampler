#ifndef __SAMPLER_FILTER_RC__
#define __SAMPLER_FILTER_RC__

#include "includes.h"
#include "filter.h"

class FilterRC : public Filter
{
public:

    FilterRC();

    void Compute(float& left, float& right, const vector<int>& params);

private:

};

#endif
