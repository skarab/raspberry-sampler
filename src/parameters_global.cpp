#include "parameters_global.h"

Param PARAM_GLOBAL_Settings[PARAM_GLOBAL_Count] = {
    { 0, 400, 100, "PreAmpli" },
    { -32, 32, 0, "Pan" },
    { 0, 200, 100, "Stereo" },
    { 0, 400, 100, "PostAmpli" },
    { 0, 150, 0, "Distortion" },
    { 0, 100, 0, "BitCrusher" },

    { 0, 200, 100, "EQLow" },
    { 0, 200, 100, "EQMedium" },
    { 0, 200, 100, "EQHigh" },
    { 0, 100, 0, "Overdrive" },
    { 0, 200, 0, "Noise" },
    { 0, 100, 0, "NoiseMix" },

    { 0, 200, 200, "LPCutOff" },
    { 0, 200, 0, "LPResonance" },
    { 0, 200, 0, "HPCutOff" },
    { 0, 200, 0, "HPResonance" },
    { 0, 100, 0, "Formant" },
    { 0, 4, 0, "FormantID" }
};
