#include "includes.h"
#include "sample.h"

typedef struct
{
    int prev;
} FILTER_CLICK_REMOVER;

inline void FILTER_CLICK_REMOVER_Initialize(FILTER_CLICK_REMOVER& filter)
{
    filter.prev = 0;
}

inline void FILTER_CLICK_REMOVER_Compute(int& value, FILTER_CLICK_REMOVER& filter)
{
    //value = (filter.prev*10+value)/11;
    //filter.prev = value;
}




