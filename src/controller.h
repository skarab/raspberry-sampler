#ifndef __SAMPLER_CONTROLLER__
#define __SAMPLER_CONTROLLER__

#include "includes.h"
#include "button.h"
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

    void _OnLoadBank(int id);
    void _OnSaveBank();
    void _OnMidiAttach();
    void _OnMidiDetach();
    void _OnStartSample();
    void _OnStopSample();


    static Controller* _Instance;

    vector<Bank*> _Banks;
    int _BankID;
    Bank* _Bank;

    Knob* _BankSelect;
    Button* _BankLoad;
    Button* _BankSave;

    int _SampleID;
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
