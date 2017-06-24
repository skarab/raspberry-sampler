#include "knob_control.h"
#include "display.h"

KnobControl::KnobControl(int pin_left, int pin_right) :
    _Value(0),
    _Minimum(0),
    _Maximum(0),
    _PinLeft(pin_left),
    _PinRight(pin_right),
    _Encoded(0)
{
    bcm2835_gpio_fsel(_PinLeft, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(_PinLeft, BCM2835_GPIO_PUD_UP);

    bcm2835_gpio_fsel(_PinRight, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(_PinRight, BCM2835_GPIO_PUD_UP);
}

KnobControl::~KnobControl()
{
}

void KnobControl::SetValue(int value)
{
    _Value = value;

    if (_Value<_Minimum) _Value = _Minimum;
    else if (_Value>_Maximum) _Value = _Maximum;
}

int KnobControl::GetValue() const
{
    return _Value;
}

void KnobControl::SetRange(int minimum, int maximum)
{
    _Minimum = minimum;
    _Maximum = maximum;
    SetValue(GetValue());
}

bool KnobControl::Update()
{
    bool changed = false;

    int msb = bcm2835_gpio_lev(_PinLeft);
    int lsb = bcm2835_gpio_lev(_PinRight);

    int encoded = (msb<<1)|lsb;
    int sum = (_Encoded<<2)|encoded;

    if (sum==0b1101 || sum==0b0100 || sum==0b0010 || sum==0b1011)
    {
        ++_Value;
        changed = true;

        if (_Value>_Maximum)
            _Value = _Maximum;

        Display::Get().Print(GetValue());
    }
    else if (sum==0b1110 || sum==0b0111 || sum==0b0001 || sum==0b1000)
    {
        --_Value;
        changed = true;

        if (_Value<_Minimum)
            _Value = _Minimum;

        Display::Get().Print(GetValue());
    }

    _Encoded = encoded;
    return changed;
}
