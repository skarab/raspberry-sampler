#ifndef __SAMPLER_CONTROLLER__
#define __SAMPLER_CONTROLLER__

#include "includes.h"
#include "button.h"
#include "led.h"
#include "knob_select.h"
#include "knob_control.h"
#include "bank.h"
#include "midi_key.h"

class Controller
{
public:

    Controller();
    ~Controller();

    static Controller& Get() { return *_Instance; }

    void OnNoteOn(const MidiKey& key, int velocity);
    void OnNoteOff(const MidiKey& key);

    void Update();

private:

    Bank* _GetBank();
    Sample* _FindSample(const MidiKey& key);
    bool _IsOnPlayBank() { return _GetBank()==Bank::PlayBank; }
    bool _IsOnGlobalParams() { return _IsOnPlayBank() && _Sample==Bank::PlayBank->GetSample(0); }

    void _OnLoadBank();
    void _OnSaveBank();
    void _OnChangeMode();
    void _OnMidiSet();
    void _OnStartSample();
    void _OnStopSample();

    void _UpdateMidiStatus();
    int _GetControlID(int knob_id);
    void _UpdateControls();
    void _OnControlChanged(int id);

    static Controller* _Instance;

    vector<Bank*> _Banks;

    KnobSelect* _BankSelect;
    Led* _BankStatus;
    Button* _BankLoad;
    Button* _BankSave;

    Sample* _Sample;

    KnobSelect* _SampleSelect;
    Button* _SampleMode;

    Button* _SampleMidiSet;
    Led* _SampleMidiStatus;
    bool _AttachMidi;

    Button* _SamplePlay;
    Led* _SamplePlayStatus;

    KnobSelect* _ControlSelect;
    vector<KnobControl*> _Controls;

    pthread_mutex_t _Lock;
};

#endif
