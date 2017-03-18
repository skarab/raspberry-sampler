#ifndef __SAMPLER_KNOB__
#define __SAMPLER_KNOB__

#include "includes.h"

class Knob
{
public:

    Knob(int value, int minimum, int maximum, int pin_left, int pin_right, bool notched);
    ~Knob();

    void Update();

private:

    int _PinLeft;
    int _PinRight;
    float _Value;
    int _Minimum;
    int _Maximum;
    bool _Notched;
    int _Encoded;
};

#endif
