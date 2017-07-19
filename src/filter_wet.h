#include "includes.h"
#include "sample.h"

typedef struct
{
    double Value;
} FILTER_WET;

inline void FILTER_WET_Initialize(FILTER_WET& filter)
{
    filter.Value = 0.0;
}

inline void FILTER_WET_Compute(int& value, int strength, FILTER_WET& filter)
{
    double v = value/32767.0;
    double s = 1.0-pow(1.0-strength/120.0, 2.0);
    filter.Value = filter.Value*s+v*(1.0-s);
    value = filter.Value*32767.0;
}
