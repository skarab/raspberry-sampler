#include "error.h"

Error::Error(string message) :
    _Message(message)
{
    Log();
}

void Error::Log() const
{
    LOG("ERROR: %s", _Message.c_str());
}
