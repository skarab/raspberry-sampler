#include "includes.h"
#include "sample.h"

inline void FILTER_OVERDRIVE_Compute(int& value, const vector<int>& params)
{
    float amount = params[PARAM_Overdrive]/101.0f;
    if (amount>0.0f)
    {
        float k = 2.0f*amount/(1.0f-amount);
        float in = value/32767.0f;
        value = (int)((1.0f+k)*in*32767.0f/(1.0f+k*fabs(in)));
    }
}
