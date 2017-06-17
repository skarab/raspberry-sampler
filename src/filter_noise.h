#ifndef __SAMPLER_FILTER_NOISE__
#define __SAMPLER_FILTER_NOISE__

#include "includes.h"
#include "filter.h"

class FilterNoise : public Filter
{
public:

    FilterNoise();

    void Compute(float& left, float& right, const vector<int>& params);

private:

    typedef struct
    {
        double B0, B1, B2, B3, B4, B5, B6;
    } Data;

    void _Initialize(Data& data);
    void _Compute(float& value, double white, Data& data);

    Data _Left;
    Data _Right;
    double _Noise;
    double _Pink;
    int _ID;
};

#endif
