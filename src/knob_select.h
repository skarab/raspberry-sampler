#ifndef __SAMPLER_KNOB_SELECT__
#define __SAMPLER_KNOB_SELECT__

#include "includes.h"

class KnobSelect
{
public:

    KnobSelect(int count, int pin_left, int pin_right);
    ~KnobSelect();

    void SetID(int id);
    int GetID() const;
    void SetCount(int count);

    bool Update();

private:

    int _PinLeft;
    int _PinRight;
    int _ID;
    int _Count;
    int _Encoded;
};

#endif
