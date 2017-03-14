#ifndef __SAMPLER_DISPLAY__
#define __SAMPLER_DISPLAY__

#include "includes.h"

class Display
{
public:

    Display();
    ~Display();

    static Display& Get() { return *_Instance; }

    void Clear();
    void Print(int value);
    void SetLoading(bool loading);

private:

    static void* _RunThreaded(void* data);

    void _Run();
    void _Print(string str);
    void _PrintIP(string device);
    string _GetIP(string device);

    static Display* _Instance;

    bool _Ready;
    bool _Quit;
    pthread_t _Thread;
    pthread_mutex_t _Lock;
    string _Value;
    string _OldValue;
    FILE* _Pipe;
    bool _Loading;
};


#endif
