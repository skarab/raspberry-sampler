#ifndef __SAMPLER_WAV__
#define __SAMPLER_WAV__

#include "includes.h"

class Wav
{
public:

    Wav(string path);
    ~Wav();

    bool IsValid() const { return _Valid; }
    unsigned int GetLength() const { return _Length; }
    const short int* GetData() const { return _Data; }

private:

    bool _Load(string path);

    string _Path;
    bool _Valid;
    char* _FileContent;
    unsigned int _Length;
    short int* _Data;
};

#endif
