#include "controller.h"
#include "display.h"
#include "device.h"

Controller* Controller::_Instance = NULL;

Controller::Controller()
{
    _Instance = this;

#if ENABLE_HARDWARE
    if (!bcm2835_init())
        ERROR("failed to initialize GPIO");
#endif

    _Banks = Bank::List();

    if (_Banks.size()==0)
        ERROR("no bank!");

    _BankID = -1;
    _Bank = NULL;

    _BankSelect = new Knob(0, 0, _Banks.size()-1, PIN_BANK_SELECT_LEFT, PIN_BANK_SELECT_RIGHT, 2, true);
    _BankLoad = new Button(PIN_BANK_LOAD);
    _BankSave = new Button(PIN_BANK_SAVE);

    _SampleID = -1;
    _Sample = NULL;

    _SampleSelect = new Knob(0, 0, 0, PIN_SAMPLE_SELECT_LEFT, PIN_SAMPLE_SELECT_RIGHT, 2, true);
    _SampleMode = new Button(PIN_SAMPLE_MODE);
    _SampleMidiSet = new Button(PIN_SAMPLE_MIDI_SET);
    _SampleMidiUnset = new Button(PIN_SAMPLE_MIDI_UNSET);

    _SamplePlay = new Button(PIN_SAMPLE_PLAY);

    _OnLoadBank(0);
}

Controller::~Controller()
{
    delete _SamplePlay;

    delete _SampleMidiUnset;
    delete _SampleMidiSet;
    delete _SampleMode;
    delete _SampleSelect;

    delete _BankSave;
    delete _BankLoad;
    delete _BankSelect;

    Bank::Destroy(_Banks);

#if ENABLE_HARDWARE
    bcm2835_close();
#endif
}

void Controller::OnNoteOn(int device_id, int channel, int note, int velocity)
{
}

void Controller::OnNoteOff(int device_id, int channel, int note, int velocity)
{
}

void Controller::Update()
{
    _BankSelect->Update();

    _BankLoad->Update();
    if (_BankLoad->IsJustPressed())
        _OnLoadBank(_BankSelect->GetValue());

    _BankSave->Update();
    if (_BankSave->IsJustPressed())
        _OnSaveBank();

    _SampleSelect->Update();
    _SampleID = _SampleSelect->GetValue();
    _Sample = _Bank->GetSample(_SampleID);

    _SampleMode->Update();
    _SampleMidiSet->Update();
    _SampleMidiUnset->Update();

    _SamplePlay->Update();
    if (_SamplePlay->IsJustPressed())
        _OnPlaySample();
}

void Controller::_OnLoadBank(int id)
{
    if (id<0 || id>=_Banks.size())
        ERROR("_OnLoadBank id");

    Device::Get().Stop();

    if (_BankID>=0)
        _Banks[_BankID]->Unload();

    _Banks[id]->Load();
    _BankID = id;
    _Bank = _Banks[_BankID];

    _SampleSelect->SetRange(0, _Bank->GetSampleCount()-1);
    _SampleSelect->SetValue(0);
    _SampleID = 0;
    _Sample = _Bank->GetSample(_SampleID);

    Display::Get().Print(_BankID);
}

void Controller::_OnSaveBank()
{
    _Banks[_BankID]->Save();
    Display::Get().Print(_BankID);

}

void Controller::_OnPlaySample()
{
    if (_Sample!=NULL)
    {
        Device::Get().OnNoteOn(_Sample, 0, 0, 0, 0);
        Display::Get().Print(_SampleID);
    }
}
