#ifndef __SAMPLER_FILTER_FORMANT__
#define __SAMPLER_FILTER_FORMANT__

#include "includes.h"
#include "filter.h"

class FilterFormant : public Filter
{
public:

    FilterFormant();

    void Compute(float& left, float& right, const vector<int>& params);

private:

    double _Left[10];
    double _Right[10];
};

#endif
