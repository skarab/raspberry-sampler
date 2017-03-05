#include "includes.h"
#include "device.h"

int main(int argc, char *argv[])
{
    try
    {
        Device* device = new Device(SAMPLER_DEVICE, SAMPLER_RATE, SAMPLER_CHANNELS, SAMPLER_BUFFER_SIZE, SAMPLER_VOICES);

        Sample* sample = new Sample("sample.wav");

        while(1)
        {
            char c = getchar();
            if (c==27) break;

            printf("play\n");

            device->Play(sample);
            usleep(10);
        }

        delete sample;

        delete device;
    }
    catch(Error& error)
    {
        error.Log();
    }
    return 0;
}

