#include "includes.h"
#include "device.h"
#include "midi.h"

int main(int argc, char *argv[])
{
    Log* log = new Log();
    Device* device = new Device();
    Midi* midi = new Midi();

    Sample* sample = new Sample("data/sample.wav");

    while(1)
    {
        char c = getchar();
        if (c==27) break;

        device->Play(sample);
        usleep(10);
    }

    delete sample;

    delete midi;
    delete device;
    delete log;

    return 0;
}

