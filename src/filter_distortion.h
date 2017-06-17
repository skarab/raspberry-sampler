#include "includes.h"
#include "sample.h"

inline void FILTER_DISTORTION_ComputeChannel(double& in, double threshold)
{
    if (in>threshold || in<-threshold)
        in = fabs(fabs(fmod(in-threshold, threshold*4.0))-threshold*2.0)-threshold;
}

inline void FILTER_DISTORTION_Compute(double& left, double& right, const vector<int>& params)
{
    double threshold = pow(1.0-params[PARAM_Distortion]/200.0, 4.0);

    FILTER_DISTORTION_ComputeChannel(left, threshold);
    FILTER_DISTORTION_ComputeChannel(right, threshold);
}

