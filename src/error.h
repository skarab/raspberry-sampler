#ifndef __SAMPLER_ERROR__
#define __SAMPLER_ERROR__

#include "includes.h"

#define ERROR(_message_) throw Error(_message_)

class Error
{
public:

    Error(string message);

    void Log();

private:

    string _Message;
};

#endif
