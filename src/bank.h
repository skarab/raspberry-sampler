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

    Bank(string name, string path);
    ~Bank();

    const string& GetName() const { return _Name; }
    int GetSampleCount();
    Sample* GetSample(int id);

    void Unload();
    bool Load();
    bool Save();

private:

    string _Name;
    string _Path;
    vector<Sample*> _Samples;
};

#endif
