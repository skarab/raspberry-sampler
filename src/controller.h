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

    Knob* _BankSelect;
    Button* _BankLoadSave;

    int _SampleID;

    Knob* _SampleSelect;
    Button* _SampleMode;
    Button* _SampleMidi;

    Button* _SamplePlay;
};

#endif
