#include "includes.h"
#include "device.h"
#include "midi.h"
#include <ncurses.h>

int main(int argc, char *argv[])
{
    Device* device = new Device(SAMPLER_DEVICE, SAMPLER_RATE, SAMPLER_CHANNELS, SAMPLER_BUFFER_SIZE, SAMPLER_VOICES);
    Midi* midi = new Midi();

    Sample* sample = new Sample("sample.wav");

    initscr();
    clear();
    noecho();
    cbreak();
    WINDOW* menu_win = newwin(1, 1, 0, 0);
    keypad(menu_win, TRUE);
    refresh();

    while(1)
    {
        char c = wgetch(menu_win);
        if (c==27) break;

        device->Play(sample);
        usleep(10);
    }

    clrtoeol();
    refresh();
    endwin();

    delete sample;

    delete midi;
    delete device;

    return 0;
}

