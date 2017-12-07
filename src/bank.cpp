#include "bank.h"
#include "display.h"

struct bank_sort
{
    inline bool operator() (const Bank* b1, const Bank* b2)
    {
        return strcmp(b1->GetName().c_str(), b2->GetName().c_str())<0;
    }
};

struct sample_sort
{
    inline bool operator() (const Sample* s1, const Sample* s2)
    {
        return strcmp(s1->GetName().c_str(), s2->GetName().c_str())<0;
    }
};

vector<Bank*> Bank::_Banks;
Bank* Bank::_PlayBank = NULL;

void Bank::UpdatePlayBank()
{
    _PlayBank->_Samples.erase(_PlayBank->_Samples.begin()+1, _PlayBank->_Samples.end());

    for (int i=1 ; i<_Banks.size() ; ++i)
    {
        if (_Banks[i]->IsLoaded())
        {
            for (int j=0 ; j<_Banks[i]->_Samples.size() ; ++j)
            {
                Sample* sample = _Banks[i]->_Samples[j];
                if (!sample->GetMidiKey().IsNull() || (sample->IsPlaying() && sample->IsLooping()))
                    _PlayBank->_Samples.push_back(sample);
            }
        }
    }
}

void Bank::DetachAll()
{
    UpdatePlayBank();

    for (int i=1 ; i<_PlayBank->_Samples.size() ; ++i)
        _PlayBank->_Samples[i]->GetMidiKey().SetNull();
}

vector<Bank*> Bank::List()
{
    if (_PlayBank==NULL)
    {
        _PlayBank = new Bank();
        _PlayBank->_Loaded = true;
        Sample* global = new Sample();
        _PlayBank->_Samples.push_back(global);

        _Banks.push_back(_PlayBank);
    }

    DIR* dir = opendir(SAMPLER_FOLDER);
    if (dir!=NULL)
    {
        dirent* entry = readdir(dir);

        while (entry!=NULL)
        {
            if (entry->d_type==DT_DIR && entry->d_name[0]!='.')
            {
                Bank* bank = new Bank(entry->d_name, string(SAMPLER_FOLDER)+entry->d_name+"/");
                _Banks.push_back(bank);
            }

            entry = readdir(dir);
        }

        closedir(dir);
    }

    sort(_Banks.begin()+1, _Banks.end(), bank_sort());
    return _Banks;
}

void Bank::Destroy(vector<Bank*>& banks)
{
    for (int i=1 ; i<banks.size() ; ++i)
        delete banks[i];
    banks.clear();
    banks.push_back(_PlayBank);
    _Banks.clear();
    _Banks.push_back(_PlayBank);
}

void Bank::Finalize()
{
    delete _PlayBank;
}

Bank::Bank() :
    _Loaded(false)
{
}

Bank::Bank(string name, string path) :
    _Name(name),
    _Path(path),
    _Loaded(false)
{
}

Bank::~Bank()
{
    Unload();
}

Sample* Bank::GetSample(int id)
{
    if (!_Loaded)
        return NULL;

    if (id>=0 && id<_Samples.size())
        return _Samples[id];
    return NULL;
}

Sample* Bank::GetSample(const MidiKey& key)
{
    if (!_Loaded)
        return NULL;

    for (int i=0 ; i<_Samples.size() ; ++i)
    {
        if ((int)_Samples[i]->IsInstru())
        {
            if (_Samples[i]->GetMidiKey().IsOnDevice(key))
                return _Samples[i];
        }
        else if (_Samples[i]->GetMidiKey()==key)
        {
            return _Samples[i];
        }
    }
    return NULL;
}

bool Bank::HasSample(Sample* sample) const
{
    if (!_Loaded)
        return false;

    for (int i=0 ; i<_Samples.size() ; ++i)
    {
        if (_Samples[i]==sample)
            return true;
    }
    return false;
}

void Bank::Unload()
{
    if (_Loaded)
    {
        LOG("unload bank %s", _Path.c_str());

        if (this==_PlayBank)
        {
            delete _Samples[0];
        }
        else
        {
            for (int i=0 ; i<_Samples.size() ; ++i)
                delete _Samples[i];
        }

        _Samples.clear();
        _Loaded = false;
    }
}

bool Bank::Load()
{
    Unload();

    LOG("load bank %s", _Path.c_str());
    Display::Get().SetLoading(true);

    map<string, pugi::xml_node> samples;
    string xml_path = _Path+"bank.xml";
    ifstream iss(xml_path.c_str());
    pugi::xml_document document;
    if (iss.is_open() && document.load(iss))
    {
        pugi::xml_node xml_root = document.child("Bank");
        if (xml_root)
        {
            pugi::xml_node xml_samples = xml_root.child("Samples");
            pugi::xml_node xml_sample = xml_samples.first_child();
            while (xml_sample)
            {
                string sample_name;
                if (XML_Read(xml_sample, "Name", sample_name))
                    samples[sample_name] = xml_sample;

                xml_sample = xml_sample.next_sibling();
            }
        }
    }

    string wav_extension = ".wav";
    DIR* dir = opendir(_Path.c_str());
    if (dir!=NULL)
    {
        dirent* entry = readdir(dir);

        while (entry!=NULL)
        {
            if (entry->d_type==DT_REG && entry->d_name[0]!='.' && has_suffix(entry->d_name, wav_extension))
            {
                string sample_name = string(entry->d_name);
                sample_name.erase(sample_name.end()-4, sample_name.end());

                Sample* sample = new Sample(sample_name, _Path+entry->d_name);
                pugi::xml_node xml_sample = samples[sample_name];
                if (xml_sample)
                    sample->Load(xml_sample);

                _Samples.push_back(sample);
            }

            entry = readdir(dir);
        }

        closedir(dir);
    }

    if (iss.is_open())
        iss.close();

    sort(_Samples.begin(), _Samples.end(), sample_sort());

    Display::Get().SetLoading(false);
    LOG("ok!");

    return _Samples.size()>0;
}

void Bank::SetLoaded()
{
    _Loaded = true;
}

bool Bank::Save()
{
    LOG("save bank %s", _Path.c_str());
    Display::Get().SetLoading(true);

    string xml_path = _Path+"bank.xml";
    ofstream oss(xml_path.c_str());
    pugi::xml_document document;

    pugi::xml_node xml_root = document.append_child("Bank");

    pugi::xml_node xml_samples = xml_root.append_child("Samples");
    for (int i=0 ; i<_Samples.size() ; ++i)
    {
        pugi::xml_node xml_sample = xml_samples.append_child("Sample");
        _Samples[i]->Save(xml_sample);
    }

    document.save(oss);

    Display::Get().SetLoading(false);
    LOG("ok!");

    return true;
}
