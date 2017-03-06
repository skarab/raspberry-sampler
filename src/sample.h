#ifndef __SAMPLER_SAMPLE__
#define __SAMPLER_SAMPLE__

#include "includes.h"
#include "wav.h"

class Sample
{
public:

    Sample(string path);
    ~Sample();

    bool IsValid() const { return _Wav->IsValid(); }
    unsigned int GetLength() const { return _Wav->GetLength(); }
    const short* GetData() const { return _Wav->GetData(); }

private:

    Wav* _Wav;
};

#endif
