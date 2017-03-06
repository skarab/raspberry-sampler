#include "includes.h"
#include "device.h"
#include "midi.h"

int main(int argc, char *argv[])
{
    Log* log = new Log();
    Device* device = new Device(SAMPLER_DEVICE, SAMPLER_RATE, SAMPLER_CHANNELS, SAMPLER_BUFFER_SIZE, SAMPLER_VOICES);
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

