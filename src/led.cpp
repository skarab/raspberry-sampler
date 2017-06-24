#include "led.h"

Led::Led(int pin) :
    _Pin(pin),
    _On(false)
{
    bcm2835_gpio_fsel(_Pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(_Pin, LOW);
}

Led::~Led()
{
    bcm2835_gpio_write(_Pin, LOW);
}

void Led::SetOn(bool on)
{
    if (on!=_On)
    {
        _On = on;
        bcm2835_gpio_write(_Pin, _On?HIGH:LOW);
    }
}

