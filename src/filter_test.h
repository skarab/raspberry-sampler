#ifndef __SAMPLER_FILTER_TEST__
#define __SAMPLER_FILTER_TEST__

#include "includes.h"
#include "filter.h"

class FilterTest : public Filter
{
public:

    FilterTest();

    void Compute(float& left, float& right, const vector<int>& params);

private:

    typedef struct
    {
    } Data;

    void _Initialize(Data& data);
    void _Compute(float& value, Data& data);

    Data _Left;
    Data _Right;
};

#endif
