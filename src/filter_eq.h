#ifndef __SAMPLER_FILTER_EQ__
#define __SAMPLER_FILTER_EQ__

#include "includes.h"
#include "filter.h"

class FilterEQ : public Filter
{
public:

    FilterEQ();

    void Compute(float& left, float& right, const vector<int>& params);

private:

    double _lf;
    double _hf;
    double _left_f1p[4];
    double _left_f2p[4];
    double _left_sdm[4];
    double _right_f1p[4];
    double _right_f2p[4];
    double _right_sdm[4];
};

#endif
