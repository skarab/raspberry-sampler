#include "display.h"

Display::Display() :
    _Ready(false),
    _Quit(false)
{
    if (pthread_create(&_Thread, NULL, _RunThreaded, (void*)this)!=0)
        ERROR("failed to create thread");

    if (pthread_mutex_init(&_Lock, NULL)!=0)
        ERROR("failed to create mutex");

    while (!_Ready)
        usleep(10);
}

Display::~Display()
{
    _Quit = true;
    pthread_join(_Thread, NULL);
    pthread_mutex_destroy(&_Lock);

}

void Display::Clear()
{
    Print(-1);
}

void Display::Print(int value)
{
    pthread_mutex_lock(&_Lock);

    if (value<0)
    {
        _Value = "";
    }
    else
    {
        if (value>9999)
            value = 9999;

        char str[5];
        sprintf(str, "%d", value);
        _Value = str;
    }

    pthread_mutex_unlock(&_Lock);
}

void* Display::_RunThreaded(void* data)
{
    Display* display = (Display*)data;
    display->_Run();
}

void Display::_Run()
{
    LOG("display ready");
    _Ready = true;
    Print(255);

    while (!_Quit)
    {
        string new_value;
        pthread_mutex_lock(&_Lock);
        new_value = _Value;
        pthread_mutex_unlock(&_Lock);

        if (new_value!=_OldValue)
            _Print(new_value);

        usleep(100);
    }

    _Print("");
    LOG("display destroyed");
}

void Display::_Print(string str)
{
#if ENABLE_GPIO
    string cmd = "python sources/src/display.py";
    if (str.size()>0)
        cmd += " "+str;
    system(cmd.c_str());

#endif

    _OldValue = str;
}
