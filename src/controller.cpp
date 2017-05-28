#include "controller.h"
#include "display.h"
#include "device.h"

Controller* Controller::_Instance = NULL;

Controller::Controller() :
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

    _BankSelect = new KnobSelect(_Banks.size(), PIN_BANK_SELECT_LEFT, PIN_BANK_SELECT_RIGHT);
    _BankStatus = new Led(PIN_BANK_STATUS);
    _BankLoad = new Button(PIN_BANK_LOAD);
    _BankSave = new Button(PIN_BANK_SAVE);

    _SampleSelect = new KnobSelect(0, PIN_SAMPLE_SELECT_LEFT, PIN_SAMPLE_SELECT_RIGHT);
    _SampleMode = new Button(PIN_SAMPLE_MODE);
    _SampleMidiSet = new Button(PIN_SAMPLE_MIDI_SET);

    _SamplePlay = new Button(PIN_SAMPLE_PLAY);

    _ControlSelect = new KnobSelect((PARAM_Count-1)/6+1, PIN_CONTROL_SELECT_LEFT, PIN_CONTROL_SELECT_RIGHT);
    _Controls.resize(6);
    _Controls[0] = new KnobControl(0, 0, 255, PIN_CONTROL_01_LEFT, PIN_CONTROL_01_RIGHT, false);
    _Controls[1] = new KnobControl(0, 0, 255, PIN_CONTROL_02_LEFT, PIN_CONTROL_02_RIGHT, false);
    _Controls[2] = new KnobControl(0, 0, 255, PIN_CONTROL_03_LEFT, PIN_CONTROL_03_RIGHT, false);
    _Controls[3] = new KnobControl(0, 0, 255, PIN_CONTROL_04_LEFT, PIN_CONTROL_04_RIGHT, false);
    _Controls[4] = new KnobControl(0, 0, 255, PIN_CONTROL_05_LEFT, PIN_CONTROL_05_RIGHT, false);
    _Controls[5] = new KnobControl(0, 0, 255, PIN_CONTROL_06_LEFT, PIN_CONTROL_06_RIGHT, false);

    _OnLoadBank();
}

Controller::~Controller()
{
    for (int i=0 ; i<_Controls.size() ; ++i)
        delete _Controls[i];
    delete _ControlSelect;

    delete _SamplePlay;

    delete _SampleMidiSet;
    delete _SampleMode;
    delete _SampleSelect;

    delete _BankSave;
    delete _BankLoad;
    delete _BankStatus;
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
    Sample* sample = _FindSample(key);

    if (_AttachMidi)
    {
        if (sample!=NULL)
            sample->GetMidiKey().SetNull();

        if (_Sample!=NULL)
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
    Sample* sample = _FindSample(key);
    if (sample!=NULL)
        Device::Get().Stop(sample, key.Note);
    pthread_mutex_unlock(&_Lock);
}

void Controller::Update()
{
    bool changed = false;

    if (_BankSelect->Update())
    {
        changed = true;
        _BankStatus->SetOn(_GetBank()->IsLoaded());

        if (_GetBank()->IsLoaded())
        {
            _Sample = _GetBank()->GetSample(0);
            _SampleSelect->SetCount(_GetBank()->GetSampleCount());
            _SampleSelect->SetID(0);
        }
        else
        {
            _Sample = NULL;
        }
        _UpdateControls();
    }

    changed |= _BankLoad->Update();
    if (_BankLoad->IsJustPressed())
        _OnLoadBank();

    changed |= _BankSave->Update();
    if (_BankSave->IsJustPressed())
        _OnSaveBank();

    if (_Sample!=NULL && _SampleSelect->Update())
    {
        changed = true;
        _Sample = _GetBank()->GetSample(_SampleSelect->GetID());
        _UpdateControls();
    }

    changed |= _SampleMode->Update();
    if (_SampleMode->IsJustPressed())
        _OnChangeMode();

    _SampleMidiSet->Update();
    if (_SampleMidiSet->IsJustPressed())
        _OnMidiSet();

    changed |= _SamplePlay->Update();
    if (_SamplePlay->IsJustPressed())
        _OnStartSample();
    if (_SamplePlay->IsJustReleased())
        _OnStopSample();

    if (_Sample!=NULL)
    {
        if (_ControlSelect->Update())
        {
            changed = true;
            _UpdateControls();
        }

        for (int i=0 ; i<_Controls.size() ; ++i)
        {
            if (_Controls[i]->Update())
            {
                changed = true;
                _OnControlChanged(i);
            }
        }
    }

    if (changed)
        _AttachMidi = false;
}

Bank* Controller::_GetBank()
{
    return _Banks[_BankSelect->GetID()];
}

Sample* Controller::_FindSample(const MidiKey& key)
{
    for (int i=0 ; i<_Banks.size() ; ++i)
    {
        if (_Banks[i]->IsLoaded())
        {
            Sample* sample = _Banks[i]->GetSample(key);
            if (sample!=NULL)
                return sample;
        }
    }
    return NULL;
}

void Controller::_OnLoadBank()
{
    pthread_mutex_lock(&_Lock);

    Device::Get().StopAll();

    Bank* bank = _GetBank();

    if (bank->IsLoaded())
    {
        bank->Unload();
        _Sample = NULL;
    }
    else
    {
        bank->Load();

        _SampleSelect->SetCount(bank->GetSampleCount());
        _SampleSelect->SetID(0);
        _Sample = bank->GetSample(0);
    }

    _UpdateControls();

    pthread_mutex_unlock(&_Lock);

    _BankStatus->SetOn(bank->IsLoaded());

    Display::Get().Print(_BankSelect->GetID());
}

void Controller::_OnSaveBank()
{
    _GetBank()->Save();
    Display::Get().Print(_BankSelect->GetID());
}

void Controller::_OnChangeMode()
{
    pthread_mutex_lock(&_Lock);
    if (_Sample!=NULL)
    {
        int mode = (int)_Sample->GetMode();
        ++mode;
        if (mode>=(int)MODE_Count)
            mode = 0;

        _Sample->SetMode((MODE)mode);
        Display::Get().Print(mode);
    }
    pthread_mutex_unlock(&_Lock);
}

void Controller::_OnMidiSet()
{
    pthread_mutex_lock(&_Lock);
    if (_AttachMidi)
    {
        _AttachMidi = false;
        if (_Sample!=NULL)
            _Sample->GetMidiKey().SetNull();
    }
    else
    {
        _AttachMidi = true;
    }
    pthread_mutex_unlock(&_Lock);
}

void Controller::_OnStartSample()
{
    if (_Sample!=NULL)
        Device::Get().Play(_Sample, -1, 64);
}

void Controller::_OnStopSample()
{
    if (_Sample!=NULL)
        Device::Get().Stop(_Sample, -1);
}

void Controller::_UpdateControls()
{
    for (int i=0 ; i<_Controls.size() ; ++i)
    {
        int id = _ControlSelect->GetID()*6+i;
        if (id<(int)PARAM_Count && _Sample!=NULL)
        {
            _Controls[i]->SetRange(PARAM_Values[id].Min, PARAM_Values[id].Max);
            _Controls[i]->SetValue(_Sample->GetParam((PARAM)id));
        }
        else
        {
            _Controls[i]->SetRange(0, 0);
            _Controls[i]->SetValue(0);
        }
    }
}

void Controller::_OnControlChanged(int i)
{
    int id = _ControlSelect->GetID()*6+i;
    if (id<(int)PARAM_Count && _Sample!=NULL)
    {
        int value = _Controls[i]->GetValue();
        if (value<PARAM_Values[id].Min) value = PARAM_Values[id].Min;
        if (value>PARAM_Values[id].Max) value = PARAM_Values[id].Max;
        _Sample->SetParam((PARAM)id, value);
    }
}
