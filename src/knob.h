#ifndef __SAMPLER_KNOB__
#define __SAMPLER_KNOB__

#include "includes.h"

class Knob
{
public:

    Knob(int value, int minimum, int maximum, int pin_left, int pin_right, bool loop);
    ~Knob();

    void SetValue(int value);
    int GetValue() const;
    void SetRange(int minimum, int maximum);

    bool Update();

private:

    int _PinLeft;
    int _PinRight;
    int _Value;
    int _Minimum;
    int _Maximum;
    bool _Loop;
    int _Encoded;
};

#endif
