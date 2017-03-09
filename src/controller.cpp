#include "controller.h"
#include "display.h"

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

    int PIN = RPI_V2_GPIO_P1_15; // #22
    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(PIN, BCM2835_GPIO_PUD_UP);

    int ENCODER_PIN1 = RPI_V2_GPIO_P1_38; // #20
    int ENCODER_PIN2 = RPI_V2_GPIO_P1_40; // #21
    bcm2835_gpio_fsel(ENCODER_PIN1, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(ENCODER_PIN1, BCM2835_GPIO_PUD_UP);
    bcm2835_gpio_fsel(ENCODER_PIN2, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(ENCODER_PIN2, BCM2835_GPIO_PUD_UP);

#endif

    LOG("controller ready");
    _Ready = true;

    bool pressed = false;
    bool was_pressed = false;

    int encoder_value = 255;
    int last_encoded = 0;

    while (!_Quit)
    {
#if ENABLE_HARDWARE

        // button test

        uint8_t value = bcm2835_gpio_lev(PIN);

        pressed = value==0;

        if (pressed!=was_pressed)
        {
            was_pressed = pressed;
            if (pressed) { LOG("press"); encoder_value = 255; Display::Get().Print(encoder_value); }
            else LOG("release");
        }

        // rotary encoder test

        int MSB = bcm2835_gpio_lev(ENCODER_PIN1);
        int LSB = bcm2835_gpio_lev(ENCODER_PIN2);

        int encoded = (MSB << 1) | LSB;
        int sum = (last_encoded << 2) | encoded;

        if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
        {
            ++encoder_value;
            Display::Get().Print(encoder_value);
        }

        if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
        {
            --encoder_value;
            Display::Get().Print(encoder_value);
        }

        last_encoded = encoded;
#endif

        usleep(100);
    }

#if ENABLE_HARDWARE
    bcm2835_close();
#endif

    LOG("controller destroyed");
}
