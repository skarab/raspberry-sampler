#ifndef __SAMPLER_CONFIG__
#define __SAMPLER_CONFIG__

#define SAMPLER_DEVICE          "default"
#define SAMPLER_CORES           4
#define SAMPLER_VOICES_PER_CORE 3
#define SAMPLER_BUFFER_SIZE     2048
#define SAMPLER_PERIOD_SIZE     128
#define SAMPLER_RATE            44100
#define SAMPLER_CHANNELS        2
#define SAMPLER_BANKS           "../samples/"
#define SAMPLER_DISPLAY_PIPE    "python sources/src/display.py"

#endif
