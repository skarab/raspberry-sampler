#ifndef __SAMPLER_SWITCH__
#define __SAMPLER_SWITCH__

#include "includes.h"

class Switch
{
public:

    Switch(int pin_up, int pin_down);
    ~Switch();

    int GetValue() const { return _Value; }

    bool Update();

private:

    int _PinUp;
    int _PinDown;
    int _Value;
};

#endif
