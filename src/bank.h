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

    bool IsLoaded() const { return _Loaded; }

    const string& GetName() const { return _Name; }
    int GetSampleCount() const { return _Samples.size(); }

    Sample* GetSample(int id);
    Sample* GetSample(const MidiKey& key);
    bool HasSample(Sample* sample) const;

    void Unload();
    bool Load();
    void SetLoaded();

    bool Save();

private:

    string _Name;
    string _Path;
    vector<Sample*> _Samples;
    bool _Loaded;
};

#endif
