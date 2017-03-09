#ifndef __SAMPLER_INCLUDES__
#define __SAMPLER_INCLUDES__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <poll.h>
#include <alsa/asoundlib.h>
#include <sys/soundcard.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <pthread.h>
#include <string>
#include <vector>
#include <map>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <sstream>

#if ENABLE_HARDWARE
    #include <bcm2835.h>
#endif

using namespace std;

#include "config.h"
#include "log.h"
#include "error.h"

#endif

