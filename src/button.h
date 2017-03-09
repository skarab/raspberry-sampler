#ifndef __SAMPLER_BUTTON__
#define __SAMPLER_BUTTON__

#include "includes.h"

class Button
{
public:

    Button(int pin);
    ~Button();

    void Update();

private:

    int _Pin;
    bool _Pressed;
};


#endif
