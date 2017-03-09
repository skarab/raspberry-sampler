#ifndef __SAMPLER_CONTROLLER__
#define __SAMPLER_CONTROLLER__

#include "includes.h"

class Controller
{
public:

    Controller();
    ~Controller();

private:

    static void* _RunThreaded(void* data);

    void _Run();

    bool _Ready;
    bool _Quit;
    pthread_t _Thread;
};


#endif
