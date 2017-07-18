#include "includes.h"
#include "sample.h"

inline void FILTER_STEREO_Compute(int& left, int& right, int strength)
{
    float stereo = powf(strength/100.0f, 2.0f);
    float scale = stereo*0.5f;
    float m = (left+right)*0.5f;
    float s = (right-left )*scale;

    left = (int)(m-s);
    right = (int)(m+s);
}

