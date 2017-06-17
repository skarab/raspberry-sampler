#include "includes.h"
#include "sample.h"

inline void FILTER_OVERDRIVE_Compute(double& left, double& right, const vector<int>& params)
{
    double amount = params[PARAM_Overdrive]/201.0;
    double k = 2.0*amount/(1.0-amount);

    left = (1.0+k)*left/(1.0+k*fabs(left));
    right = (1.0+k)*right/(1.0+k*fabs(right));
}
