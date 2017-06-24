#include "includes.h"
#include "sample.h"

inline void FILTER_BITCRUSHER_Compute(int& value, const vector<int>& params)
{
    const float vol[10] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.9f, 0.6f, 0.5f, 0.4f, 0.2f };

    float bitcrush = params[PARAM_BitCrusher]/100.0f;
    if (bitcrush>0.0f)
    {
        int id0 = floorf(bitcrush*9.0f);
        int id1 = ceilf(bitcrush*9.0f);
        float lerp = bitcrush*9.0f-id0;
        float b0 = (value & (-1<<(id0+5)))*vol[id0];
        float b1 = (value & (-1<<(id1+5)))*vol[id1];
        value = (int)(b1*lerp+b0*(1.0f-lerp));
    }
}

