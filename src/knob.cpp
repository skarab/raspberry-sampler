#include "knob.h"
#include "display.h"

Knob::Knob(int value, int minimum, int maximum, int pin_left, int pin_right, int multiplier, bool loop) :
    _Minimum(minimum),
    _Maximum(maximum),
    _PinLeft(pin_left),
    _PinRight(pin_right),
    _Multiplier(multiplier),
    _Loop(loop),
    _Encoded(0)
{
#if ENABLE_HARDWARE
    bcm2835_gpio_fsel(_PinLeft, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(_PinLeft, BCM2835_GPIO_PUD_UP);

    bcm2835_gpio_fsel(_PinRight, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(_PinRight, BCM2835_GPIO_PUD_UP);
#endif

    SetValue(value);
}

Knob::~Knob()
{
}

void Knob::SetValue(int value)
{
    _Value = (value-_Minimum)*_Multiplier;

    int maximum = (_Maximum-_Minimum)*_Multiplier+_Multiplier-1;
    if (_Value<0) _Value = 0;
    else if (_Value>maximum) _Value = maximum;
}

int Knob::GetValue() const
{
    return _Value/_Multiplier+_Minimum;
}

void Knob::SetRange(int minimum, int maximum)
{
    _Minimum = minimum;
    _Maximum = maximum;
    SetValue(GetValue());
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

    int maximum = (_Maximum-_Minimum)*_Multiplier+_Multiplier-1;

    if (sum==0b1101 || sum==0b0100 || sum==0b0010 || sum==0b1011)
    {
        ++_Value;

        if (_Value>maximum)
        {
            if (_Loop)
            {
                while (_Value>maximum)
                    _Value -= maximum+1;
            }
            else
            {
                _Value = maximum;
            }
        }

        Display::Get().Print(GetValue());
    }
    else if (sum==0b1110 || sum==0b0111 || sum==0b0001 || sum==0b1000)
    {
        --_Value;

        if (_Value<0)
        {
            if (_Loop)
            {
                while (_Value<0)
                    _Value += maximum+1;
            }
            else
            {
                _Value = 0;
            }
        }

        Display::Get().Print(GetValue());
    }

    _Encoded = encoded;
}
