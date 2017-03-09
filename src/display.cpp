#include "display.h"

Display::Display() :
    _Ready(false),
    _Quit(false),
    _Pipe(NULL)
{
    if (pthread_create(&_Thread, NULL, _RunThreaded, (void*)this)!=0)
        ERROR("failed to create thread");

    if (pthread_mutex_init(&_Lock, NULL)!=0)
        ERROR("failed to create mutex");

    while (!_Ready)
        usleep(10);

    _PrintIP(SAMPLER_NETWORK_DEVICE);
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
#if ENABLE_GPIO
    _Pipe = popen(SAMPLER_DISPLAY_PIPE, "w");
#endif

    LOG("display ready");
    _Ready = true;

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

    if (_Pipe!=NULL)
        pclose(_Pipe);

    LOG("display destroyed");
}

void Display::_Print(string str)
{
    if (_Pipe!=NULL)
    {
        fprintf(_Pipe, "%s\n", str.c_str());
        fflush(_Pipe);
    }

    pthread_mutex_lock(&_Lock);
    _OldValue = str;
    pthread_mutex_unlock(&_Lock);
}

void Display::_PrintIP(string device)
{
    string ip = _GetIP(device);
    if (ip.size()>0)
    {
        LOG("ip: %s", ip.c_str());

        stringstream ss;
        ss.str(ip);
        string item;
        while (getline(ss, item, '.'))
        {
            pthread_mutex_lock(&_Lock);
            _Value = item;
            pthread_mutex_unlock(&_Lock);

            usleep(1000000);
        }
    }
}

string Display::_GetIP(string device)
{
    string ip = "";

    struct ifaddrs* addrs = NULL;
    struct ifaddrs* ifa = NULL;
    getifaddrs(&addrs);
    ifa = addrs;

    while (ifa!=NULL)
    {
        if ((ifa!=NULL) && (device==ifa->ifa_name) && (ifa->ifa_addr!=NULL) && (ifa->ifa_addr->sa_family==AF_INET))
        {
            void* tmp = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char buf[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmp, buf, INET_ADDRSTRLEN);
            ip = buf;
        }

        ifa = ifa->ifa_next;
    }

    if (addrs!=NULL)
        freeifaddrs(addrs);

    return ip;
}
