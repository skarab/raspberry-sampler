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
#if ENABLE_GPIO
    if (!bcm2835_init())
        ERROR("failed to initialize GPIO");

    // PIN RPI_GPIO_P1_15
    //bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_INPT);
    //bcm2835_gpio_set_pud(PIN, BCM2835_GPIO_PUD_UP);
#endif

    LOG("controller ready");
    _Ready = true;

    while (!_Quit)
    {
        //uint8_t value = bcm2835_gpio_lev(PIN);
        //LOG("%d", value);

        usleep(10);
    }

#if ENABLE_GPIO
    bcm2835_close();
#endif

    LOG("controller destroyed");
}
