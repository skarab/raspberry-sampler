#include "parameters_sample.h"

Param PARAM_SAMPLE_Settings[PARAM_SAMPLE_Count] = {
    { 0, 200, 90, "Volume" },
    { -32, 32, 2, "Pan" },
    { 0, 200, 110, "Stereo" },
    { 0, 5, 1, "Formant" },
    { 0, 150, 10, "Distortion" },
    { 0, 100, 10, "BitCrusher" },

    { 0, 2048, 10, "Start" },
    { 0, 2048, 10, "EnvAttack" },
    { 0, 2048, 10, "EnvRelease" },
    { 0, 2048, 2018, "Stop" },
    { -32, 32, 1, "PitchSemiTone" },
    { -2048, 2048, 1020, "PitchFineTune" },

    { 0, 2048, 10, "LoopStart" },
    { 0, 2048, 10, "LoopEnvAttack" },
    { 0, 2048, 10, "LoopEnvRelease" },
    { 0, 2048, 2038, "LoopStop" },
    { 0, 2048, 10, "Silence" },
    { 0, 64, 10, "Legato" }
};
