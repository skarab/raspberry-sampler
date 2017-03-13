#include "bank.h"

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
                Bank* bank = new Bank(string(SAMPLER_BANKS)+entry->d_name);
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
}

