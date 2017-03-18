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

    _BankSelect = new Knob(0, 0, _Banks.size()-1, PIN_BANK_SELECT_LEFT, PIN_BANK_SELECT_RIGHT, true, true);
    _BankLoadSave = new Button(PIN_BANK_LOAD_SAVE);

    Display::Get().Print(_BankID);

    _SampleID = 0;

    _SampleSelect = new Knob(0, 0, 12, PIN_SAMPLE_SELECT_LEFT, PIN_SAMPLE_SELECT_RIGHT, true, true);
    _SampleMode = new Button(PIN_SAMPLE_MODE);
    _SampleMidi = new Button(PIN_SAMPLE_MIDI);

    _SamplePlay = new Button(PIN_SAMPLE_PLAY);
}

Controller::~Controller()
{
    delete _SamplePlay;

    delete _SampleMidi;
    delete _SampleMode;
    delete _SampleSelect;

    delete _BankLoadSave;
    delete _BankSelect;

    Bank::Destroy(_Banks);

#if ENABLE_HARDWARE
    bcm2835_close();
#endif
}

void Controller::Update()
{
    _BankSelect->Update();
    _BankLoadSave->Update();
    _SampleSelect->Update();
    _SampleMode->Update();
    _SampleMidi->Update();
    _SamplePlay->Update();
}
