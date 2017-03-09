#ifndef __SAMPLER_PINS__
#define __SAMPLER_PINS__

#if ENABLE_HARDWARE
    #include <bcm2835.h>

    #define RPI_PIN_01      RPI_V2_GPIO_P1_28   // EEC
    #define RPI_PIN_04      RPI_V2_GPIO_P1_07   // . GPCLK0
    #define RPI_PIN_05      RPI_V2_GPIO_P1_29
    #define RPI_PIN_06      RPI_V2_GPIO_P1_31
    #define RPI_PIN_07      RPI_V2_GPIO_P1_26   // CE1
    #define RPI_PIN_08      RPI_V2_GPIO_P1_24   // CE0
    #define RPI_PIN_09      RPI_V2_GPIO_P1_21   // MISO
    #define RPI_PIN_10      RPI_V2_GPIO_P1_19   // MOSI
    #define RPI_PIN_11      RPI_V2_GPIO_P1_23   // SCLK
    #define RPI_PIN_12      RPI_V2_GPIO_P1_32   // . PWM0
    #define RPI_PIN_13      RPI_V2_GPIO_P1_33   // . PWM1
    #define RPI_PIN_14      RPI_V2_GPIO_P1_08   // TXD
    #define RPI_PIN_15      RPI_V2_GPIO_P1_10   // RXD
    #define RPI_PIN_16      RPI_V2_GPIO_P1_36
    #define RPI_PIN_17      RPI_V2_GPIO_P1_11
    #define RPI_PIN_18      RPI_V2_GPIO_P1_12   // . PWM0
    #define RPI_PIN_19      RPI_V2_GPIO_P1_35   // . MISO
    #define RPI_PIN_20      RPI_V2_GPIO_P1_38   // . MOSI
    #define RPI_PIN_21      RPI_V2_GPIO_P1_40   // . SLCK
    #define RPI_PIN_22      RPI_V2_GPIO_P1_15
    #define RPI_PIN_23      RPI_V2_GPIO_P1_16
    #define RPI_PIN_24      RPI_V2_GPIO_P1_18
    #define RPI_PIN_25      RPI_V2_GPIO_P1_22
    #define RPI_PIN_26      RPI_V2_GPIO_P1_37
    #define RPI_PIN_27      RPI_V2_GPIO_P1_13

#else

    #define RPI_PIN_01      1
    #define RPI_PIN_04      4
    #define RPI_PIN_05      5
    #define RPI_PIN_06      6
    #define RPI_PIN_07      7
    #define RPI_PIN_08      8
    #define RPI_PIN_09      9
    #define RPI_PIN_10      10
    #define RPI_PIN_11      11
    #define RPI_PIN_12      12
    #define RPI_PIN_13      13
    #define RPI_PIN_14      14
    #define RPI_PIN_15      15
    #define RPI_PIN_16      16
    #define RPI_PIN_17      17
    #define RPI_PIN_18      18
    #define RPI_PIN_19      19
    #define RPI_PIN_20      20
    #define RPI_PIN_21      21
    #define RPI_PIN_22      22
    #define RPI_PIN_23      23
    #define RPI_PIN_24      24
    #define RPI_PIN_25      25
    #define RPI_PIN_26      26
    #define RPI_PIN_27      27

#endif

#endif
