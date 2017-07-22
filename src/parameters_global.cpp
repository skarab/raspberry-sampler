#include "parameters_global.h"

Param PARAM_GLOBAL_Settings[PARAM_GLOBAL_Count] = {
    { 0, 200, 100, "Volume" },
    { -32, 32, 0, "Pan" },
    { 0, 200, 100, "Stereo" },
    { -100, 100, 0, "EQLow" },
    { -100, 100, 0, "EQMedium" },
    { -100, 100, 0, "EQHigh" },

    { 0, 200, 200, "LPCutOff" },
    { 0, 200, 0, "LPResonance" },
    { 0, 200, 0, "HPCutOff" },
    { 0, 200, 200, "HPResonance" },
    { 0, 100, 0, "Wet" },
    { 0, 100, 0, "Resonance" },

    { 0, 200, 0, "Noise" },
    { 0, 200, 0, "NoiseTexture" },
    { 0, 200, 0, "Clicks" },
    { 0, 200, 0, "ClicksTexture" },
    { 0, 500, 100, "ClicksPitch" },
    { 0, 100, 0, "NoiseMix" }
};
