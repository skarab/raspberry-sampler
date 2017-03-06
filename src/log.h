#ifndef __SAMPLER_LOG__
#define __SAMPLER_LOG__

#include "includes.h"

#define LOG Log::Get().Write

class Log
{
public:

    Log();
    ~Log();

    static Log& Get() { return *_Instance; }

    void Write(const char* text, ...);

private:

    static Log* _Instance;

    pthread_mutex_t _Lock;
};

#endif
