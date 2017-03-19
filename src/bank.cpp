#include "bank.h"
#include "display.h"

vector<Bank*> Bank::List()
{
    vector<Bank*> banks;

    DIR* dir = opendir(SAMPLER_BANKS);
    if (dir!=NULL)
    {
        dirent* entry = readdir(dir);

        while (entry!=NULL)
        {
            if (entry->d_type==DT_DIR && entry->d_name[0]!='.')
            {
                Bank* bank = new Bank(string(SAMPLER_BANKS)+entry->d_name+"/");
                banks.push_back(bank);
            }

            entry = readdir(dir);
        }

        closedir(dir);
    }

    if (banks.size()==0)
        LOG("no banks!");

    return banks;
}

void Bank::Destroy(vector<Bank*>& banks)
{
    for (int i=0 ; i<banks.size() ; ++i)
        delete banks[i];
    banks.clear();
}

Bank::Bank(string path) :
    _Path(path)
{
}

Bank::~Bank()
{
    Unload();
}

int Bank::GetSampleCount()
{
    return _Samples.size();
}

Sample* Bank::GetSample(int id)
{
    if (id>=0 && id<_Samples.size())
        return _Samples[id];
    return NULL;
}

void Bank::Unload()
{
    LOG("unload bank %s", _Path.c_str());

    for (int i=0 ; i<_Samples.size() ; ++i)
        delete _Samples[i];
}

bool Bank::Load()
{
    LOG("load bank %s", _Path.c_str());
    Display::Get().SetLoading(true);

    _Samples.clear();

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

    Display::Get().SetLoading(false);
    LOG("ok!");

    return _Samples.size()>0;
}

bool Bank::Save()
{
    LOG("save bank %s", _Path.c_str());

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
    return true;
}
