#include "includes.h"
#include "display.h"
#include "sound.h"
#include "midi.h"
#include "bank.h"
#include "controller.h"
#include "usb_key.h"

int main(int argc, char *argv[])
{
    if ((int)PARAM_GLOBAL_Count!=(int)PARAM_SAMPLE_Count)
        return -1;

    Log* log = new Log();
    UsbKey* usb_key = new UsbKey();
    Display* display = new Display();
    Controller* controller = new Controller();
    Sound* sound = new Sound();
    Midi* midi = new Midi();

    try
    {
        while (1)
        {
            usb_key->Update();
            controller->Update();
            usleep(3000);
        }
    }
    catch(...)
    {
        printf("Aborted!\n");
    }

    delete controller;
    delete midi;
    delete sound;
    delete display;
    delete usb_key;
    delete log;

    return 0;
}

