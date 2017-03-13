#ifndef __SAMPLER_BANK__
#define __SAMPLER_BANK__

#include "includes.h"
#include "midi.h"
#include "sample.h"

class Bank
{
public:

    static vector<Bank*> List();
    static void Destroy(vector<Bank*>& banks);

    Bank(string path);
    ~Bank();

private:

    string _Path;
    map<MidiKey, Sample*> _Samples;
};

#endif
