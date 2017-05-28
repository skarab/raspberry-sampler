#ifndef __SAMPLER_KNOB_CONTROL__
#define __SAMPLER_KNOB_CONTROL__

#include "includes.h"

class KnobControl
{
public:

    KnobControl(int pin_left, int pin_right);
    ~KnobControl();

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
    int _Encoded;
};

#endif
