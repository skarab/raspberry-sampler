#include "button.h"

Button::Button(int pin) :
    _Pin(pin),
    _Pressed(false)
{
#if ENABLE_HARDWARE
    bcm2835_gpio_fsel(_Pin, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(_Pin, BCM2835_GPIO_PUD_UP);
#endif
}

Button::~Button()
{
}

void Button::Update()
{
    bool pressed = false;

#if ENABLE_HARDWARE
    pressed = bcm2835_gpio_lev(_Pin)==0;
#endif

    if (pressed!=_Pressed)
    {
        _Pressed = pressed;
        if (pressed)
        {
            LOG("press %d", _Pin);
        }
        else
        {
            LOG("release %d", _Pin);
        }
    }
}
