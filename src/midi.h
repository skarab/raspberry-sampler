#ifndef __SAMPLER_MIDI__
#define __SAMPLER_MIDI__

#include "includes.h"

class MidiKey
{
public:


private:

};

#endif





/*

typedef struct
{
    char name[30];
    int device;
    char dummies[255];
} midi_info;

int main()
{
    unsigned char p[8];

    int seqfd = open("/dev/sequencer", O_RDONLY);
    if (seqfd==-1)
    {
        printf("failed to open\n");
        exit(1);
    }

    int status;
    int midi_count;
    status = ioctl(seqfd, SNDCTL_SEQ_NRMIDIS, &midi_count);

    midi_info info;
    for (int i=0 ; i<midi_count ; ++i)
    {
        info.device = i;
        status = ioctl(seqfd, SNDCTL_MIDI_INFO, &info);
        printf("MIDI Port %d: %s\n", i, info.name);
    }

    while(1)
    {
        unsigned char old[8];
        memcpy(old, p, 8);
        read(seqfd, p, 4);

        if (p[0]>=128)
            read(seqfd, p+4, 4);

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
    }


    close(ioctl);
}

*/

