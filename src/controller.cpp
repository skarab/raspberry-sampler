#include "controller.h"
#include "display.h"
#include "sound.h"

Controller* Controller::_Instance = NULL;

Controller::Controller() :
    _Sample(NULL),
    _AttachMidi(false)
{
    _Instance = this;

    if (pthread_mutex_init(&_Lock, NULL)!=0)
        ERROR("failed to create mutex");

    if (!bcm2835_init())
        ERROR("failed to initialize GPIO");

    _Banks = Bank::List();

    _BankSelect = new KnobSelect(_Banks.size(), PIN_BANK_SELECT_LEFT, PIN_BANK_SELECT_RIGHT);
    _BankStatus = new Led(PIN_BANK_STATUS);
    _BankLoad = new Button(PIN_BANK_LOAD);
    _BankSave = new Button(PIN_BANK_SAVE);

    _SampleSelect = new KnobSelect(0, PIN_SAMPLE_SELECT_LEFT, PIN_SAMPLE_SELECT_RIGHT);
    _SampleMode = new Button(PIN_SAMPLE_MODE);
    _SampleMidiSet = new Button(PIN_SAMPLE_MIDI_SET);
    _SampleMidiStatus = new Led(PIN_SAMPLE_MIDI_STATUS);

    _SamplePlay = new Button(PIN_SAMPLE_PLAY);
    _SamplePlayStatus = new Led(PIN_SAMPLE_PLAY_STATUS);

    _ControlSelect = new KnobSelect(3, PIN_CONTROL_SELECT_LEFT, PIN_CONTROL_SELECT_RIGHT);
    _Controls.resize(6);
    _Controls[0] = new KnobControl(PIN_CONTROL_01_LEFT, PIN_CONTROL_01_RIGHT);
    _Controls[1] = new KnobControl(PIN_CONTROL_02_LEFT, PIN_CONTROL_02_RIGHT);
    _Controls[2] = new KnobControl(PIN_CONTROL_03_LEFT, PIN_CONTROL_03_RIGHT);
    _Controls[3] = new KnobControl(PIN_CONTROL_04_LEFT, PIN_CONTROL_04_RIGHT);
    _Controls[4] = new KnobControl(PIN_CONTROL_05_LEFT, PIN_CONTROL_05_RIGHT);
    _Controls[5] = new KnobControl(PIN_CONTROL_06_LEFT, PIN_CONTROL_06_RIGHT);

    _SampleSelect->SetCount(1);
    _SampleSelect->SetID(0);
    _Sample = _GetBank()->GetSample(0);

    _UpdateControls();
    _BankStatus->SetOn(true);
    Update();
    Display::Get().Print(_BankSelect->GetID());
}

Controller::~Controller()
{
    for (int i=0 ; i<_Controls.size() ; ++i)
        delete _Controls[i];
    delete _ControlSelect;

    delete _SamplePlayStatus;
    delete _SamplePlay;
    delete _SampleMidiStatus;
    delete _SampleMidiSet;
    delete _SampleMode;
    delete _SampleSelect;

    delete _BankSave;
    delete _BankLoad;
    delete _BankStatus;
    delete _BankSelect;

    Bank::Destroy(_Banks);

    bcm2835_close();

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
        {
            _Sample->GetMidiKey() = key;
            _UpdateMidiStatus();
        }

        sample = _Sample;
        _AttachMidi = false;
    }

    if (sample!=NULL)
        Sound::Get().Play(sample, key.Note, velocity);
    pthread_mutex_unlock(&_Lock);
}

void Controller::OnNoteOff(const MidiKey& key)
{
    pthread_mutex_lock(&_Lock);
    Sample* sample = _FindSample(key);
    if (sample!=NULL)
        Sound::Get().Stop(sample, key.Note);
    pthread_mutex_unlock(&_Lock);
}

void Controller::Update()
{
    bool changed = false;

    if (_BankSelect->Update())
    {
        changed = true;
        _BankStatus->SetOn(_GetBank()->IsLoaded());

        if (_IsOnPlayBank())
            Bank::UpdatePlayBank();

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
        _ControlSelect->SetID(0);
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
        _ControlSelect->SetID(0);
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

        _SamplePlayStatus->SetOn(_Sample->IsPlaying());
    }
    else
    {
        _SamplePlayStatus->SetOn(false);
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
    for (int i=1 ; i<_Banks.size() ; ++i)
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
    Bank* bank = _GetBank();

    if (_IsOnPlayBank())
    {
        Sound::Get().StopAll();
        return;
    }

    if (bank->IsLoaded())
    {
        pthread_mutex_lock(&_Lock);

        Sound::Get().OnUnloadBank(*bank);
        bank->Unload();
        _Sample = NULL;

        pthread_mutex_unlock(&_Lock);
    }
    else
    {
        bank->Load();

        pthread_mutex_lock(&_Lock);
        bank->SetLoaded();
        _SampleSelect->SetCount(bank->GetSampleCount());
        _SampleSelect->SetID(0);
        _Sample = bank->GetSample(0);
        pthread_mutex_unlock(&_Lock);
    }

    _UpdateControls();
    _BankStatus->SetOn(bank->IsLoaded());
    Display::Get().Print(_BankSelect->GetID());
}

void Controller::_OnSaveBank()
{
    if (_IsOnPlayBank())
    {
        pthread_mutex_lock(&_Lock);
        Bank::DetachAll();
        _Sample = _GetBank()->GetSample(0);
        _SampleSelect->SetCount(_GetBank()->GetSampleCount());
        _SampleSelect->SetID(0);
        _UpdateControls();
        pthread_mutex_unlock(&_Lock);
        return;
    }

    _GetBank()->Save();
    Display::Get().Print(_BankSelect->GetID());
}

void Controller::_OnChangeMode()
{
    pthread_mutex_lock(&_Lock);
    if (_IsOnGlobalParams())
    {
    }
    else if (_Sample!=NULL)
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
    if (_IsOnGlobalParams())
    {
    }
    else
    {
        if (_AttachMidi)
        {
            _AttachMidi = false;
            if (_Sample!=NULL)
            {
                _Sample->GetMidiKey().SetNull();
                _UpdateMidiStatus();
            }
        }
        else
        {
            _AttachMidi = true;
        }
    }
    pthread_mutex_unlock(&_Lock);
}

void Controller::_OnStartSample()
{
    if (_IsOnGlobalParams())
    {
    }
    else if (_Sample!=NULL)
    {
        Sound::Get().Play(_Sample, -1, 64);
    }
}

void Controller::_OnStopSample()
{
    if (!_IsOnGlobalParams() && _Sample!=NULL)
        Sound::Get().Stop(_Sample, -1);
}

int Controller::_GetControlID(int knob_id)
{
    int id = _ControlSelect->GetID();
    return id*6+knob_id;
}

void Controller::_UpdateMidiStatus()
{
    _SampleMidiStatus->SetOn((_Sample!=NULL) && !_Sample->GetMidiKey().IsNull());
}

void Controller::_UpdateControls()
{
    _UpdateMidiStatus();

    for (int i=0 ; i<_Controls.size() ; ++i)
    {
        int id = _GetControlID(i);

        if (id<PARAM_SAMPLE_Count && _Sample!=NULL)
        {
            const Param& settings = _Sample->GetParamSettings(id);

            _Controls[i]->SetRange(settings.Min, settings.Max);
            _Controls[i]->SetValue(_Sample->GetParam(id));
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
    int id = _GetControlID(i);
    if (id<PARAM_SAMPLE_Count && _Sample!=NULL)
    {
        const Param& settings = _Sample->GetParamSettings(id);
        int value = _Controls[i]->GetValue();
        if (value<settings.Min) value = settings.Min;
        if (value>settings.Max) value = settings.Max;
        _Sample->SetParam(id, value);
    }
}
