#include "parameters_global.h"

Param PARAM_GLOBAL_Settings[PARAM_GLOBAL_Count] = {
    { 0, 200, 100, "Volume" },
    { -32, 32, 0, "Pan" },
    { 0, 200, 100, "Stereo" },
    { 0, 200, 100, "EQLow" },
    { 0, 200, 100, "EQMedium" },
    { 0, 200, 100, "EQHigh" },

    { 0, 200, 200, "LPCutOff" },
    { 0, 200, 0, "LPResonance" },
    { 0, 200, 0, "HPCutOff" },
    { 0, 200, 0, "HPResonance" },
    { 0, 0, 0, "Unused1" },
    { 0, 0, 0, "Unused2" },

    { 0, 200, 0, "Noise" },
    { 0, 100, 0, "NoiseMix" },
    { 0, 0, 0, "Unused3" },
    { 0, 0, 0, "Unused4" },
    { 0, 0, 0, "Unused5" },
    { 0, 0, 0, "Unused6" }
};
