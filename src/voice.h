#ifndef __SAMPLER_VOICE__
#define __SAMPLER_VOICE__

#include "includes.h"
#include "sample.h"
#include "filters.h"

class Voice
{
public:

    Voice();
    ~Voice();

    bool IsBusy() const;
    bool IsPlaying(Sample* sample) const;
    bool IsPlaying(Sample* sample, int note) const;
    Sample* GetSample() const { return _Sample; };

    void Update(double& left, double& right);

    void Play(Sample* sample, int note, int velocity);
    void Stop(Sample* sample, int note);
    void ForceStop();

private:

    Sample* _Sample;
    double _Position;
    bool _Stop;
    double _StopTime;
    int _Note;
    int _Velocity;
    double _Pitch;
    double _LegatoPitch;
    Filters _Filters;
};

#endif
