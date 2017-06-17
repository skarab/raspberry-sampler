#include "filter_stereo.h"

void FilterStereo::Compute(float& left, float& right, const vector<int>& params)
{
    double stereo = pow(params[PARAM_Stereo]/100.0f, 2.0);

    double scale = stereo*0.5;
    double m = (left+right)*0.5;
    double s = (right-left )*scale;

    left = (float)(m-s);
    right = (float)(m+s);
}
