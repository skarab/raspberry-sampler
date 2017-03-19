#include "controller.h"
#include "display.h"
#include "device.h"

Controller* Controller::_Instance = NULL;

Controller::Controller() :
    _BankID(-1),
    _Bank(NULL),
    _SampleID(-1),
    _Sample(NULL),
    _AttachMidi(false)
{
    _Instance = this;

    if (pthread_mutex_init(&_Lock, NULL)!=0)
        ERROR("failed to create mutex");

#if ENABLE_HARDWARE
    if (!bcm2835_init())
        ERROR("failed to initialize GPIO");
#endif

    _Banks = Bank::List();

    if (_Banks.size()==0)
        ERROR("no bank!");

    _BankSelect = new Knob(0, 0, _Banks.size()-1, PIN_BANK_SELECT_LEFT, PIN_BANK_SELECT_RIGHT, 2, true);
    _BankLoad = new Button(PIN_BANK_LOAD);
    _BankSave = new Button(PIN_BANK_SAVE);

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

    pthread_mutex_destroy(&_Lock);
}

void Controller::OnNoteOn(const MidiKey& key, int velocity)
{
    pthread_mutex_lock(&_Lock);
    Sample* sample = _Bank->GetSample(key);

    if (_AttachMidi)
    {
        if (sample!=NULL)
            sample->GetMidiKey().SetNull();

        _Sample->GetMidiKey() = key;
        sample = _Sample;
        _AttachMidi = false;
    }

    if (sample!=NULL)
        Device::Get().Play(sample, key.Note, velocity);
    pthread_mutex_unlock(&_Lock);
}

void Controller::OnNoteOff(const MidiKey& key)
{
    pthread_mutex_lock(&_Lock);
    Sample* sample = _Bank->GetSample(key);
    if (sample!=NULL)
        Device::Get().Stop(sample, key.Note);
    pthread_mutex_unlock(&_Lock);
}

void Controller::Update()
{
    bool changed = false;

    changed |= _BankSelect->Update();

    changed |= _BankLoad->Update();
    if (_BankLoad->IsJustPressed())
        _OnLoadBank(_BankSelect->GetValue());

    changed |= _BankSave->Update();
    if (_BankSave->IsJustPressed())
        _OnSaveBank();

    changed |= _SampleSelect->Update();
    _SampleID = _SampleSelect->GetValue();
    _Sample = _Bank->GetSample(_SampleID);

    changed |= _SampleMode->Update();
    _SampleMidiSet->Update();
    _SampleMidiUnset->Update();
    if (_SampleMidiSet->IsJustPressed())
        _OnMidiAttach();
    if (_SampleMidiUnset->IsJustPressed())
        _OnMidiDetach();

    changed |= _SamplePlay->Update();
    if (_SamplePlay->IsJustPressed())
        _OnStartSample();
    if (_SamplePlay->IsJustReleased())
        _OnStopSample();

    if (changed)
        _AttachMidi = false;
}

void Controller::_OnLoadBank(int id)
{
    pthread_mutex_lock(&_Lock);

    if (id<0 || id>=_Banks.size())
        ERROR("_OnLoadBank id");

    Device::Get().StopAll();

    if (_BankID>=0)
        _Banks[_BankID]->Unload();

    _Banks[id]->Load();
    _BankID = id;
    _Bank = _Banks[_BankID];

    _SampleSelect->SetRange(0, _Bank->GetSampleCount()-1);
    _SampleSelect->SetValue(0);
    _SampleID = 0;
    _Sample = _Bank->GetSample(_SampleID);

    pthread_mutex_unlock(&_Lock);

    Display::Get().Print(_BankID);
}

void Controller::_OnSaveBank()
{
    _Banks[_BankID]->Save();
    Display::Get().Print(_BankID);

}

void Controller::_OnMidiAttach()
{
    pthread_mutex_lock(&_Lock);
    _AttachMidi = true;
    pthread_mutex_unlock(&_Lock);
}

void Controller::_OnMidiDetach()
{
    pthread_mutex_lock(&_Lock);
    _AttachMidi = false;
    _Sample->GetMidiKey().SetNull();
    pthread_mutex_unlock(&_Lock);
}

void Controller::_OnStartSample()
{
    Device::Get().Play(_Sample, -1, 64);
    Display::Get().Print(_SampleID);
}

void Controller::_OnStopSample()
{
    Device::Get().Stop(_Sample, -1);
}
