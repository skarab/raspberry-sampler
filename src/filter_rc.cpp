#include "filter_rc.h"

FilterRC::FilterRC()
{
    memset(_Left, 0, sizeof(float)*2);
    memset(_Right, 0, sizeof(float)*2);
}

void FilterRC::Compute(float& left, float& right, const vector<int>& params)
{
    return;

    float c = powf(0.5f, (128-params[PARAM_RCCutOff])/16.0f);
    float r = powf(0.5f, (params[PARAM_RCResonance]+24)/16.0f);

    _Left[0] = (1.0f-r*c)*_Left[0]-c*_Left[1]+c*left;
    _Left[1] = (1.0f-r*c)*_Left[1]+c*_Left[0];
    left = _Left[1];

    _Right[0] = (1.0f-r*c)*_Right[0]-c*_Right[1]+c*right;
    _Right[1] = (1.0f-r*c)*_Right[1]+c*_Right[0];
    right = _Right[1];
}
