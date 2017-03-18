#ifndef __SAMPLER_CONTROLLER__
#define __SAMPLER_CONTROLLER__

#include "includes.h"
#include "button.h"
#include "knob.h"
#include "bank.h"

class Controller
{
public:

    Controller();
    ~Controller();

    void Update();

private:

    vector<Bank*> _Banks;
    int _BankID;
};

#endif
