#include "controller.h"

Controller::Controller() :
    _Ready(false),
    _Quit(false)
{
    if (pthread_create(&_Thread, NULL, _RunThreaded, (void*)this)!=0)
        ERROR("failed to create thread");

    while (!_Ready)
        usleep(10);
}

Controller::~Controller()
{
    _Quit = true;
    pthread_join(_Thread, NULL);
}

void* Controller::_RunThreaded(void* data)
{
    Controller* controller = (Controller*)data;
    controller->_Run();
}

void Controller::_Run()
{
#if ENABLE_HARDWARE
    if (!bcm2835_init())
        ERROR("failed to initialize GPIO");
#endif

    Button* test_button = new Button(RPI_PIN_22);
    Knob* test_knob = new Knob(255, 200, 280, RPI_PIN_20, RPI_PIN_21);

    LOG("controller ready");
    _Ready = true;

    while (!_Quit)
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

    LOG("controller destroyed");
}
