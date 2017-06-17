#include "includes.h"
#include "sample.h"

inline void FILTER_BITCRUSHER_ComputeChannel(double& value, double bitcrush)
{
    int v = (int)(value*65536.0f);

    int t1 = (int)floor(bitcrush*30.0);
    int t2 = (int)ceil(bitcrush*30.0);
    int b1 = (v & (-1 << t1));
    int b2 = (v & (-1 << t2));

    double v1 = b1/65536.0;
    double v2 = b2/65536.0;

    double lerp = bitcrush*30.0-t1;

    value = (v1*(1.0-lerp)+v2*lerp)/(1.0+pow(bitcrush*2.5, 14.0)*0.5);
}

inline void FILTER_BITCRUSHER_Compute(double& left, double& right, const vector<int>& params)
{
    double bitcrush = params[PARAM_BitCrusher]/200.0;

    FILTER_BITCRUSHER_ComputeChannel(left, bitcrush);
    FILTER_BITCRUSHER_ComputeChannel(right, bitcrush);
}

