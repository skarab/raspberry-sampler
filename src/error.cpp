#include "error.h"

Error::Error(string message) :
    _Message(message)
{
    Log();
}

void Error::Log() const
{
    printf("%s\n", _Message.c_str());
}
