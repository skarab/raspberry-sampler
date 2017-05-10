#ifndef __SAMPLER_LED__
#define __SAMPLER_LED__

#include "includes.h"

class Led
{
public:

    Led(int pin);
    ~Led();

    void SetOn(bool on);
    bool IsOn() const { return _On; }

private:

    int _Pin;
    bool _On;
};

#endif
