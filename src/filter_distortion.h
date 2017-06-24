#include "includes.h"
#include "sample.h"

inline void FILTER_DISTORTION_Compute(int& value, const vector<int>& params)
{
    float threshold = powf(1.0f-params[PARAM_Distortion]/200.0f, 4.0f);
    float in = value/32767.0f;
    if (in>threshold || in<-threshold)
        value = (int)((fabs(fabs(fmod(in-threshold, threshold*4.0f))-threshold*2.0f)-threshold)*32767.0f);
}

