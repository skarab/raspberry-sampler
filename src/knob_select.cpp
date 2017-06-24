#include "knob_select.h"
#include "display.h"

KnobSelect::KnobSelect(int count, int pin_left, int pin_right) :
    _ID(0),
    _Count(count),
    _PinLeft(pin_left),
    _PinRight(pin_right),
    _Encoded(0)
{
    bcm2835_gpio_fsel(_PinLeft, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(_PinLeft, BCM2835_GPIO_PUD_UP);

    bcm2835_gpio_fsel(_PinRight, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(_PinRight, BCM2835_GPIO_PUD_UP);
}

KnobSelect::~KnobSelect()
{
}

void KnobSelect::SetID(int id)
{
    _ID = id*2;

    int maximum = (_Count-1)*2+1;
    if (_ID<0) _ID = 0;
    else if (_ID>maximum) _ID = maximum;
}

int KnobSelect::GetID() const
{
    return _ID/2;
}

void KnobSelect::SetCount(int count)
{
    _Count = count;
    SetID(GetID());
}

bool KnobSelect::Update()
{
    bool changed = false;

    int msb = bcm2835_gpio_lev(_PinLeft);
    int lsb = bcm2835_gpio_lev(_PinRight);

    int encoded = (msb<<1)|lsb;
    int sum = (_Encoded<<2)|encoded;

    int maximum = (_Count-1)*2+1;

    if (sum==0b1101 || sum==0b0100 || sum==0b0010 || sum==0b1011)
    {
        ++_ID;
        changed = true;

        while (_ID>maximum)
            _ID -= maximum+1;

        Display::Get().Print(GetID());
    }
    else if (sum==0b1110 || sum==0b0111 || sum==0b0001 || sum==0b1000)
    {
        --_ID;
        changed = true;

        while (_ID<0)
            _ID += maximum+1;

        Display::Get().Print(GetID());
    }

    _Encoded = encoded;
    return changed;
}
