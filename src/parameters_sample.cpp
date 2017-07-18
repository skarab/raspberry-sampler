#include "parameters_sample.h"

Param PARAM_SAMPLE_Settings[PARAM_SAMPLE_Count] = {
    { 0, 200, 100, "Volume" },
    { -32, 32, 0, "Pan" },
    { 0, 200, 100, "Stereo" },
    { 0, 5, 0, "Formant" },
    { 0, 150, 0, "Distortion" },
    { 0, 100, 0, "BitCrusher" },

    { 0, 2048, 0, "Start" },
    { 0, 2048, 0, "EnvAttack" },
    { 0, 2048, 0, "EnvRelease" },
    { 0, 2048, 2048, "Stop" },
    { -32, 32, 0, "PitchSemiTone" },
    { -2048, 2048, 1024, "PitchFineTune" },

    { 0, 2048, 0, "LoopStart" },
    { 0, 2048, 0, "LoopEnvAttack" },
    { 0, 2048, 0, "LoopEnvRelease" },
    { 0, 2048, 2048, "LoopStop" },
    { 0, 2048, 0, "Silence" },
    { 0, 64, 0, "Legato" }
};
