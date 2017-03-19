#include "midi.h"
#include "controller.h"

struct MidiDevice
{
    bool NoteOn;
    bool NoteOff;
    int Channel;
    int Note;
};

Midi::Midi() :
    _Ready(false),
    _Quit(false)
{
    if (pthread_create(&_Thread, NULL, _RunThreaded, (void*)this)!=0)
        ERROR("failed to create thread");

    while (!_Ready)
        usleep(10);
}

Midi::~Midi()
{
    _Quit = true;
    pthread_join(_Thread, NULL);
}

void* Midi::_RunThreaded(void* data)
{
    Midi* midi = (Midi*)data;
    midi->_Run();
}

void Midi::_Run()
{
    _Sequencer = open("/dev/sequencer", O_RDONLY);
    if (_Sequencer==-1)
        ERROR("failed to open /dev/sequencer");

    _List();
    LOG("midi ready");
    _Ready = true;

    vector<MidiDevice> devices;

    while (!_Quit)
    {
        unsigned char buffer[8];
        read(_Sequencer, buffer, 4);

        if (buffer[0]>=128)
            read(_Sequencer, buffer+4, 4);

        if (buffer[0]==SEQ_MIDIPUTC && buffer[1]!=SEQ_PRIVATE)
        {
            int value = buffer[1];
            int device_id = buffer[2];

            while (device_id>=devices.size())
            {
                MidiDevice dev;
                dev.NoteOn = false;
                dev.NoteOff = false;
                devices.push_back(dev);
            }

            MidiDevice& device = devices[device_id];
            if (!device.NoteOn && !device.NoteOff)
            {
                if (value>=0x90 && value<=0x9F)
                {
                    device.NoteOn = true;
                    device.Channel = value-0x90+1;
                    device.Note = -1;
                }
                else if (value>=0x80 && value<=0x8F)
                {
                    device.NoteOff = true;
                    device.Channel = value-0x80+1;
                    device.Note = -1;
                }
            }
            else if (device.Note==-1)
            {
                if (value<21 || value>108)
                {
                    LOG("midi skipped message");
                    device.NoteOn = false;
                    device.NoteOff = false;
                }
                else
                {
                    device.Note = value;
                }
            }
            else
            {
                if (value>127)
                {
                    LOG("midi skipped message");
                }
                else
                {
                    int velocity = value;

                    if (device.NoteOn)
                    {
                        Controller::Get().OnNoteOn(device_id, device.Channel, device.Note, velocity);
                    }
                    else if (device.NoteOff)
                    {
                        Controller::Get().OnNoteOff(device_id, device.Channel, device.Note, velocity);
                    }
                }

                device.NoteOn = false;
                device.NoteOff = false;
            }
        }

        usleep(10);
    }

    close(_Sequencer);
    LOG("midi destroyed");
}

void Midi::_List()
{
    printf("midi list:\n");

    int midi_count = 0;
    ioctl(_Sequencer, SNDCTL_SEQ_NRMIDIS, &midi_count);

    midi_info info;
    info.name[0] = 0;
    for (int i=0 ; i<midi_count ; ++i)
    {
        info.device = i;
        ioctl(_Sequencer, SNDCTL_MIDI_INFO, &info);
        printf("MIDI Port %d: %s\n", i, info.name);
    }
}
