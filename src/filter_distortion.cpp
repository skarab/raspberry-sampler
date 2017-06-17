#include "filter_distortion.h"

inline void FilterDistortion_Compute(float& in, float threshold)
{
    if (in>threshold || in<-threshold)
        in = fabs(fabs(fmod(in-threshold, threshold*4.0f))-threshold*2.0f)-threshold;
}

void FilterDistortion::Compute(float& left, float& right, const vector<int>& params)
{
    float threshold = pow(1.0f-params[PARAM_Distortion]/200.0f, 4.0f);

    FilterDistortion_Compute(left, threshold);
    FilterDistortion_Compute(right, threshold);
}
