#include "includes.h"
#include "device.h"
#include "midi.h"
#include "controller.h"

int main(int argc, char *argv[])
{
    Log* log = new Log();
    Device* device = new Device();
    Midi* midi = new Midi();
    Controller* controller = new Controller();

    while(1)
    {
        char c = getchar();
        if (c==27) break;

        device->OnNoteOn(0, 0, 0, 0);

        usleep(10);
    }

    delete controller;
    delete midi;
    delete device;
    delete log;

    return 0;
}

