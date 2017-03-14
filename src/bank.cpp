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

    string wav_extension = ".wav";
    DIR* dir = opendir(_Path.c_str());
    if (dir!=NULL)
    {
        dirent* entry = readdir(dir);

        while (entry!=NULL)
        {
            if (entry->d_type==DT_REG && entry->d_name[0]!='.' && has_suffix(entry->d_name, wav_extension))
            {
                Sample* sample = new Sample(_Path+entry->d_name);
                if (sample->IsValid()) _Samples.push_back(sample);
                else delete sample;
            }

            entry = readdir(dir);
        }

        closedir(dir);
    }

    Display::Get().SetLoading(false);
    LOG("ok!");

    return _Samples.size()>0;
}

bool Bank::Save()
{
    LOG("save bank %s", _Path.c_str());



    return true;
}
