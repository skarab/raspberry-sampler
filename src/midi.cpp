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
    printf("midi ready.\n");

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
                    printf("MIDIPUTC: %d %d %d %d\n", p[0], p[1], p[2], p[3]);
                break;

            case SEQ_NOTEOFF: printf("NOTEOFF: %d %d %d %d\n", p[0], p[1], p[2], p[3]); break;
            case SEQ_NOTEON: printf("NOTEON: %d %d %d %d\n", p[0], p[1], p[2], p[3]); break;
            case SEQ_WAIT: break;
            case SEQ_PGMCHANGE: printf("PGMCHANGE\n");
            case SEQ_SYNCTIMER: printf("SYNCTIMER\n");
            case SEQ_AFTERTOUCH: printf("AFTERTOUCH: %d %d %d %d\n", p[0], p[1], p[2], p[3]); break;
            case SEQ_CONTROLLER: printf("CONTROLLER\n");
            default:
                printf("%d %d %d %d\n", p[0], p[1], p[2], p[3]);
        }

        usleep(10);
    }

    close(_Sequencer);
    printf("midi destroyed.\n");
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
