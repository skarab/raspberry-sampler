#ifndef __SAMPLER_DISPLAY__
#define __SAMPLER_DISPLAY__

#include "includes.h"

class Display
{
public:

    Display();
    ~Display();

    void Clear();
    void Print(int value);

private:

    static void* _RunThreaded(void* data);

    void _Run();
    void _Print(string str);

    bool _Ready;
    bool _Quit;
    pthread_t _Thread;
    pthread_mutex_t _Lock;
    string _Value;
    string _OldValue;
};


#endif
