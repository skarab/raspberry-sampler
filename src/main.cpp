#include "includes.h"
#include "display.h"
#include "sound.h"
#include "midi.h"
#include "bank.h"
#include "controller.h"

int main(int argc, char *argv[])
{
    Log* log = new Log();
    Display* display = new Display();
    Controller* controller = new Controller();
    Sound* sound = new Sound();
    Midi* midi = new Midi();

    while (1)
    {
        controller->Update();
        usleep(3000);
    }

    delete controller;

    delete midi;
    delete sound;
    delete display;
    delete log;

    return 0;
}

