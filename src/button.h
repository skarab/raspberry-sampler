#ifndef __SAMPLER_BUTTON__
#define __SAMPLER_BUTTON__

#include "includes.h"

class Button
{
public:

    Button(int pin);
    ~Button();

    bool IsPressed() const { return _Pressed; }
    bool IsJustPressed() const { return _OnPressed; }
    bool IsJustReleased() const { return _OnReleased; }

    bool Update();

private:

    int _Pin;
    bool _Pressed;
    bool _OnPressed;
    bool _OnReleased;
};

#endif
