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
        double Y1, Y2, Y3, Y4;
        double OldX;
        double OldY1, OldY2, OldY3;
    } Data;

    void _Initialize(Data& data);
    void _Compute(float& value, Data& data);

    int _CutOff;
    int _Resonance;
    double _P, _K, _R;
    Data _Left;
    Data _Right;
};

#endif