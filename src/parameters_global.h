#ifndef __SAMPLER_PARAMETERS_GLOBAL__
#define __SAMPLER_PARAMETERS_GLOBAL__

#include "parameters_sample.h"

enum PARAM_GLOBAL
{
    PARAM_GLOBAL_Volume,
    PARAM_GLOBAL_Pan,
    PARAM_GLOBAL_Stereo,
    PARAM_GLOBAL_EQLow,
    PARAM_GLOBAL_EQMedium,
    PARAM_GLOBAL_EQHigh,

    PARAM_GLOBAL_LPCutOff,
    PARAM_GLOBAL_LPResonance,
    PARAM_GLOBAL_HPCutOff,
    PARAM_GLOBAL_HPResonance,
    PARAM_GLOBAL_Unused1,
    PARAM_GLOBAL_Unused2,

    PARAM_GLOBAL_Noise,
    PARAM_GLOBAL_NoiseTexture,
    PARAM_GLOBAL_Motor,
    PARAM_GLOBAL_MotorTexture,
    PARAM_GLOBAL_MotorSpeed,
    PARAM_GLOBAL_NoiseMix,

    PARAM_GLOBAL_Count
};

extern Param PARAM_GLOBAL_Settings[PARAM_GLOBAL_Count];

#endif
