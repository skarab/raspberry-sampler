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

#if ENABLE_GPIO
    #include <bcm2835.h>
#endif

using namespace std;

#include "config.h"
#include "log.h"
#include "error.h"

#endif

