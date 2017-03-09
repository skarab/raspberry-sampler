#include "knob.h"
#include "display.h"

Knob::Knob(int value, int minimum, int maximum, int pin_left, int pin_right) :
    _Value(value),
    _Minimum(minimum),
    _Maximum(maximum),
    _PinLeft(pin_left),
    _PinRight(pin_right),
    _Encoded(0)
{
#if ENABLE_HARDWARE
    bcm2835_gpio_fsel(_PinLeft, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(_PinLeft, BCM2835_GPIO_PUD_UP);

    bcm2835_gpio_fsel(_PinRight, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(_PinRight, BCM2835_GPIO_PUD_UP);
#endif
}

Knob::~Knob()
{
}

void Knob::Update()
{
    int msb = 0;
    int lsb = 0;

#if ENABLE_HARDWARE
    msb = bcm2835_gpio_lev(_PinLeft);
    lsb = bcm2835_gpio_lev(_PinRight);
#endif

    int encoded = (msb<<1)|lsb;
    int sum = (_Encoded<<2)|encoded;

    if (sum==0b1101 || sum==0b0100 || sum==0b0010 || sum==0b1011)
    {
        ++_Value;
        if (_Value>_Maximum)
            _Value = _Maximum;

        Display::Get().Print(_Value);
    }
    else if (sum==0b1110 || sum==0b0111 || sum==0b0001 || sum==0b1000)
    {
        --_Value;
        if (_Value<_Minimum)
            _Value = _Minimum;

        Display::Get().Print(_Value);
    }

    _Encoded = encoded;
}
