#include "includes.h"
#include "device.h"

int main(int argc, char *argv[])
{
    try
    {
        Device* device = new Device(SAMPLER_DEVICE, SAMPLER_RATE, SAMPLER_CHANNELS, SAMPLER_BUFFER_SIZE, SAMPLER_VOICES);

        while(1)
        {
            usleep(10);
        }

        delete device;
    }
    catch(Error& error)
    {
        error.Log();
    }
    return 0;
}

