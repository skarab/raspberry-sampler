#ifndef __SAMPLER_MIDI__
#define __SAMPLER_MIDI__

#include "includes.h"
#include "midi_key.h"

class Midi
{
public:

    Midi();
    ~Midi();

private:

    static void* _RunThreaded(void* data);

    void _Run();
    void _List();

    bool _Ready;
    bool _Quit;
    pthread_t _Thread;
    int _Sequencer;
};

#endif
