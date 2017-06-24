#include "includes.h"
#include "sample.h"

typedef struct
{
    long acc, A, prev_x, prev_y;
} FILTER_DC;

inline void FILTER_DC_Initialize(FILTER_DC& filter)
{
    double pole = 0.9999;
    filter.A = (long)(32768.0*(1.0-pole));
    filter.acc = 0;
    filter.prev_x = 0;
    filter.prev_y = 0;
}

inline void FILTER_DC_Compute(int& value, FILTER_DC& filter)
{
    filter.acc -= filter.prev_x;
    filter.prev_x = (long)value<<15;
    filter.acc += filter.prev_x;
    filter.acc -= filter.A*filter.prev_y;
    filter.prev_y = filter.acc>>15;
    value = filter.prev_y;
}




