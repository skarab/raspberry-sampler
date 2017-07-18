#ifndef __SAMPLER_PARAMETERS_SAMPLE__
#define __SAMPLER_PARAMETERS_SAMPLE__

enum PARAM_SAMPLE
{
    PARAM_SAMPLE_Volume,
    PARAM_SAMPLE_Pan,
    PARAM_SAMPLE_Stereo,
    PARAM_SAMPLE_Formant,
    PARAM_SAMPLE_Distortion,
    PARAM_SAMPLE_BitCrusher,

    PARAM_SAMPLE_Start,
    PARAM_SAMPLE_EnvAttack,
    PARAM_SAMPLE_EnvRelease,
    PARAM_SAMPLE_Stop,
    PARAM_SAMPLE_PitchSemiTone,
    PARAM_SAMPLE_PitchFineTune,

    PARAM_SAMPLE_LoopStart,
    PARAM_SAMPLE_LoopEnvAttack,
    PARAM_SAMPLE_LoopEnvRelease,
    PARAM_SAMPLE_LoopStop,
    PARAM_SAMPLE_Silence,
    PARAM_SAMPLE_Legato,

    PARAM_SAMPLE_Count
};

struct Param
{
    int Min;
    int Max;
    int Default;
    const char* Name;
};

extern Param PARAM_SAMPLE_Settings[PARAM_SAMPLE_Count];

#endif
