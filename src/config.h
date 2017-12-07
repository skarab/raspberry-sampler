#ifndef __SAMPLER_CONFIG__
#define __SAMPLER_CONFIG__

#define SAMPLER_DEVICE          "default"
#define SAMPLER_CORES           4
#define SAMPLER_VOICES_PER_CORE 2
#define SAMPLER_BUFFER_SIZE     2048
#define SAMPLER_PERIOD_SIZE     128
#define SAMPLER_RATE            44100
#define SAMPLER_CHANNELS        2
#define SAMPLER_DISPLAY_PIPE    "python /sampler/sources/src/display.py"
#define SAMPLER_USB_KEY_DEVICE  "/dev/disk/by-label/"
#define SAMPLER_USB_KEY_MOUNT   "/mnt/usb"
#define SAMPLER_FOLDER          "/mnt/usb/samples/"

#endif
