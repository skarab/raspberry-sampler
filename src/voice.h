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
    bool IsLooping() const;
    Sample* GetSample() const { return _Sample; };

    void Play(Sample* sample, int note, int velocity);
    void Stop(Sample* sample, int note);
    void ForceStop();

    void Update(int& left, int& right);

private:

    float _Volume;
    Sample* _Sample;
    double _Position;
    bool _Stop;
    double _StopTime;
    int _Note;
    int _Velocity;
    double _Pitch;
    double _LegatoPitch;
    bool _InLoop;
    FiltersVoice _LeftFilters;
    FiltersVoice _RightFilters;
};

#endif
