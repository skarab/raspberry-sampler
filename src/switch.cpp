#include "switch.h"
#include "display.h"

Switch::Switch(int pin_up, int pin_down) :
    _Value(0),
    _PinUp(pin_up),
    _PinDown(pin_down)
{
    bcm2835_gpio_fsel(_PinUp, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(_PinUp, BCM2835_GPIO_PUD_UP);

    bcm2835_gpio_fsel(_PinDown, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(_PinDown, BCM2835_GPIO_PUD_UP);
}

Switch::~Switch()
{
}

bool Switch::Update()
{
    bool changed = false;
    int value = 1;

    if (bcm2835_gpio_lev(_PinDown)==0) value=0;
    else if (bcm2835_gpio_lev(_PinUp)==0) value=2;

    if (value!=_Value)
    {
        changed = true;
        _Value = value;
        Display::Get().Print(_Value);
    }

    return changed;
}
