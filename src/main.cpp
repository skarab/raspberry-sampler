#include "includes.h"
#include "display.h"
#include "device.h"
#include "midi.h"
#include "bank.h"
#include "controller.h"

int main(int argc, char *argv[])
{
    Log* log = new Log();
    Display* display = new Display();
    Device* device = new Device();
    Midi* midi = new Midi();

#if ENABLE_HARDWARE
    Controller* controller = new Controller();

    while (1)
    {
        controller->Update();
        usleep(100);
    }

    delete controller;
#endif

    delete midi;
    delete device;
    delete display;
    delete log;

    return 0;
}

