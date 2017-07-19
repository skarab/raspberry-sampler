#include "parameters_global.h"

Param PARAM_GLOBAL_Settings[PARAM_GLOBAL_Count] = {
    { 0, 200, 70, "Volume" },
    { -32, 32, 2, "Pan" },
    { 0, 200, 110, "Stereo" },
    { -100, 100, 2, "EQLow" },
    { -100, 100, -4, "EQMedium" },
    { -100, 100, -7, "EQHigh" },

    { 0, 200, 190, "LPCutOff" },
    { 0, 200, 40, "LPResonance" },
    { 0, 200, 40, "HPCutOff" },
    { 0, 200, 40, "HPResonance" },
    { 0, 100, 10, "Wet" },
    { 0, 100, 20, "Resonance" },

    { 0, 200, 10, "Noise" },
    { 0, 200, 10, "NoiseTexture" },
    { 0, 200, 10, "Clicks" },
    { 0, 200, 10, "ClicksTexture" },
    { 0, 500, 100, "ClicksPitch" },
    { 0, 100, 10, "NoiseMix" }
};
