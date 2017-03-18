#include "controller.h"
#include "display.h"

Controller::Controller()
{
#if ENABLE_HARDWARE
    if (!bcm2835_init())
        ERROR("failed to initialize GPIO");
#endif

    _Banks = Bank::List();

    if (_Banks.size()==0)
        ERROR("no bank!");

    _BankID = 0;
    _Banks[_BankID]->Load();
    Display::Get().Print(_BankID);
}

Controller::~Controller()
{
    Bank::Destroy(_Banks);

#if ENABLE_HARDWARE
    bcm2835_close();
#endif
}

void Controller::Update()
{
}
