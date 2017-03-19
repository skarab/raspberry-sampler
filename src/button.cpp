#include "button.h"

Button::Button(int pin) :
    _Pin(pin),
    _Pressed(false),
    _OnPressed(false),
    _OnReleased(false)
{
#if ENABLE_HARDWARE
    bcm2835_gpio_fsel(_Pin, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(_Pin, BCM2835_GPIO_PUD_UP);
#endif
}

Button::~Button()
{
}

bool Button::Update()
{
    bool used = false;
    bool pressed = false;

#if ENABLE_HARDWARE
    pressed = bcm2835_gpio_lev(_Pin)==0;
#endif

    _OnPressed = false;
    _OnReleased = false;

    if (pressed!=_Pressed)
    {
        _Pressed = pressed;
        if (pressed)
        {
            _OnPressed = true;
        }
        else
        {
            _OnReleased = true;
        }
        used = true;
    }

    used |= pressed;
    return used;
}
