#include "midi.h"

Midi::Midi() :
    _Ready(false),
    _Quit(false)
{
    if (pthread_create(&_Thread, NULL, _RunThreaded, (void*)this)!=0)
        ERROR("failed to create thread");

    while (!_Ready)
        usleep(10);
}

Midi::~Midi()
{
    _Quit = true;
    pthread_join(_Thread, NULL);
}

void* Midi::_RunThreaded(void* data)
{
    Midi* midi = (Midi*)data;
    midi->_Run();
}

void Midi::_Run()
{
    _Sequencer = open("/dev/sequencer", O_RDONLY);
    if (_Sequencer==-1)
        ERROR("failed to open /dev/sequencer");

    _Ready = true;
    _List();
    LOG("midi ready");

    unsigned char p[8];
    while (!_Quit)
    {
        unsigned char old[8];
        memcpy(old, p, 8);
        read(_Sequencer, p, 4);

        if (p[0]>=128)
            read(_Sequencer, p+4, 4);

        if (memcmp(old, p, 8)==0)
            continue;

        switch (p[0])
        {
            case SEQ_MIDIPUTC:
                if (p[1]!=SEQ_PRIVATE)// && p[1]!=0 && p[1]!=248 && p[1]!=42)
                    LOG("MIDIPUTC: %d %d %d %d", p[0], p[1], p[2], p[3]);
                break;

            case SEQ_NOTEOFF: LOG("NOTEOFF: %d %d %d %d", p[0], p[1], p[2], p[3]); break;
            case SEQ_NOTEON: LOG("NOTEON: %d %d %d %d", p[0], p[1], p[2], p[3]); break;
            case SEQ_WAIT: break;
            case SEQ_PGMCHANGE: LOG("PGMCHANGE");
            case SEQ_SYNCTIMER: LOG("SYNCTIMER");
            case SEQ_AFTERTOUCH: LOG("AFTERTOUCH: %d %d %d %d", p[0], p[1], p[2], p[3]); break;
            case SEQ_CONTROLLER: LOG("CONTROLLER");
            default:
                LOG("%d %d %d %d", p[0], p[1], p[2], p[3]);
        }

        usleep(10);
    }

    close(_Sequencer);
    LOG("midi destroyed");
}

/*
typedef struct
{
    char name[30];
    int device;
    char dummies[255];
} midi_info;
*/

void Midi::_List()
{
    printf("midi list:\n");

    int midi_count = 0;
    ioctl(_Sequencer, SNDCTL_SEQ_NRMIDIS, &midi_count);

    midi_info info;
    info.name[0] = 0;
    for (int i=0 ; i<midi_count ; ++i)
    {
        info.device = i;
        ioctl(_Sequencer, SNDCTL_MIDI_INFO, &info);
        printf("MIDI Port %d: %s\n", i, info.name);
    }
}
