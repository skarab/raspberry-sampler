#ifndef __SAMPLER_CONFIG__
#define __SAMPLER_CONFIG__

#define SAMPLER_DEVICE          "default"
#define SAMPLER_VOICES          8
#define SAMPLER_BUFFER_SIZE     4096
#define SAMPLER_PERIOD_SIZE     128

#define SAMPLER_NETWORK_DEVICE  "eth0"
#define SAMPLER_BANKS           "../samples/"
#define SAMPLER_DISPLAY_PIPE    "python sources/src/display.py"

// hard-fixed

#define SAMPLER_RATE            44100
#define SAMPLER_CHANNELS        2

#endif
