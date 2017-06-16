#ifndef __SAMPLER_FILTER_MOOG__
#define __SAMPLER_FILTER_MOOG__

#include "includes.h"
#include "filter.h"

class FilterMoog : public Filter
{
public:

    FilterMoog();

    void Compute(float& left, float& right, const vector<int>& params);

private:

    typedef struct
    {
        double y1,y2,y3,y4;
        double oldx;
        double oldy1,oldy2,oldy3;
    } Data;

    void _Initialize(Data& data);
    void _Compute(float& value, Data& data);

    int _CutOff;
    int _Resonance;
    double p, k, r;
    Data _Left;
    Data _Right;
};

#endif
