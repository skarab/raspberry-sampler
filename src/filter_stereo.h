#include "includes.h"
#include "sample.h"

inline void FILTER_STEREO_Compute(double& left, double& right, const vector<int>& params)
{
    double stereo = pow(params[PARAM_Stereo]/100.0, 2.0);

    double scale = stereo*0.5;
    double m = (left+right)*0.5;
    double s = (right-left )*scale;

    left = m-s;
    right = m+s;
}

