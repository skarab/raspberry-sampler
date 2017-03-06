 #include "log.h"

Log* Log::_Instance = NULL;

Log::Log()
{
    _Instance = this;
    pthread_mutex_init(&_Lock, NULL);
}

Log::~Log()
{
    _Instance = NULL;
    pthread_mutex_destroy(&_Lock);
}

void Log::Write(const char* text, ...)
{
    pthread_mutex_lock(&_Lock);

    va_list va_array;
    static char line[4096];
    va_start(va_array, text);
        vsnprintf(line, 4096, text, va_array);
    va_end(va_array);

    printf("%s\n", line);

    pthread_mutex_unlock(&_Lock);
}
