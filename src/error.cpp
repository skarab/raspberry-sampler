#include "error.h"

Error::Error(string message) :
    _Message(message)
{
}

void Error::Log()
{
    printf("%s\n", _Message.c_str());
}
