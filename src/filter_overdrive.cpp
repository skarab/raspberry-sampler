#include "filter_overdrive.h"

void FilterOverdrive::Compute(float& left, float& right, const vector<int>& params)
{
    double amount = params[PARAM_Overdrive]/201.0;
    double k = 2.0*amount/(1.0-amount);

    left = (float)((1.0+k)*left/(1.0+k*fabs(left)));
    right = (float)((1.0+k)*right/(1.0+k*fabs(right)));
}
