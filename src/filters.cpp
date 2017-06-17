#include "filters.h"

Filters::Filters()
{
    FILTER_EQ_Initialize(_Equalizer);
}

void Filters::Compute(double& left, double& right, const vector<int>& params)
{
    FILTER_STEREO_Compute(left, right, params);
    FILTER_OVERDRIVE_Compute(left, right, params);
    FILTER_DISTORTION_Compute(left, right, params);
    FILTER_BITCRUSHER_Compute(left, right, params);
    FILTER_EQ_Compute(left, right, params, _Equalizer);
}
