#ifndef __SAMPLER_VOICE__
#define __SAMPLER_VOICE__

#include "includes.h"
#include "sample.h"
#include "filter.h"

class Voice
{
public:

    Voice();
    ~Voice();

    bool IsBusy() const;
    bool IsPlaying(Sample* sample) const;
    bool IsPlaying(Sample* sample, int note) const;
    Sample* GetSample() const { return _Sample; };

    void Update(float& left, float& right);

    void Play(Sample* sample, int note, int velocity);
    void Stop(Sample* sample, int note);
    void ForceStop();

private:

    Sample* _Sample;
    vector<float> _Params;
    float _Position;
    bool _Stop;
    float _StopTime;
    int _Note;
    int _Velocity;
    float _Pitch;
    float _LegatoPitch;
    vector<Filter*> _Filters;
};

#endif
