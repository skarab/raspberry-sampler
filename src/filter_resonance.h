#include "includes.h"
#include "sample.h"

typedef struct
{
    double Value;
    double Dt;
} FILTER_RESONANCE;

inline void FILTER_RESONANCE_Initialize(FILTER_RESONANCE& filter)
{
    filter.Value = 0.0;
    filter.Dt = 0.0;
}

inline void FILTER_RESONANCE_Compute(int& value, int strength, FILTER_RESONANCE& filter)
{
    double v = value/32767.0;
    double s = 1.0-pow(1.0-strength/120.0, 4.0);
    double dt = v-filter.Value;
    filter.Dt = filter.Dt*s+dt*(1.0f-s);
    v = filter.Value+filter.Dt;
    filter.Value = v;
    value = v*32767.0;
}
