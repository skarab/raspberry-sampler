#ifndef __SAMPLER_CONTROLLER__
#define __SAMPLER_CONTROLLER__

#include "includes.h"
#include "button.h"
#include "led.h"
#include "knob.h"
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

    void _OnLoadBank();
    void _OnSaveBank();
    void _OnMidiAttach();
    void _OnMidiDetach();
    void _OnStartSample();
    void _OnStopSample();

    static Controller* _Instance;

    vector<Bank*> _Banks;

    Knob* _BankSelect;
    Led* _BankStatus;
    Button* _BankLoad;
    Button* _BankSave;

    Sample* _Sample;

    Knob* _SampleSelect;
    Button* _SampleMode;
    Button* _SampleMidiSet;
    Button* _SampleMidiUnset;

    Button* _SamplePlay;

    bool _AttachMidi;

    pthread_mutex_t _Lock;
};

#endif
