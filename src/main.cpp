#include "includes.h"
#include "display.h"
#include "device.h"
#include "midi.h"
#include "button.h"
#include "knob.h"

int main(int argc, char *argv[])
{
    Log* log = new Log();
    Display* display = new Display();
    Device* device = new Device();
    Midi* midi = new Midi();

    #if ENABLE_HARDWARE
        if (!bcm2835_init())
            ERROR("failed to initialize GPIO");
    #endif

    Button* test_button = new Button(RPI_PIN_22);
    Knob* test_knob = new Knob(255, 200, 280, RPI_PIN_20, RPI_PIN_21);

    while(1)
    {
        test_button->Update();
        test_knob->Update();

        usleep(100);
    }

    delete test_knob;
    delete test_button;

    #if ENABLE_HARDWARE
        bcm2835_close();
    #endif

    delete midi;
    delete device;
    delete display;
    delete log;

    return 0;
}

