#ifndef __SAMPLER_FILTER_NOTCH__
#define __SAMPLER_FILTER_NOTCH__

#include "includes.h"
#include "filter.h"

class FilterNotch : public Filter
{
public:

    FilterNotch();

    void Compute(float& left, float& right, const vector<int>& params);

private:

    typedef struct
    {
        double Y1, Y2;
        double X0, X1, X2;
    } Data;

    void _Initialize(Data& data);
    void _Compute(float& value, Data& data);

    int _CutOff;
    int _BandWidth;
    double _B0, _B1, _A1, _A2;
    Data _Left;
    Data _Right;
};

#endif
