#include "wav.h"

Wav::Wav(string path) :
    _FileContent(NULL),
    _Data(NULL),
    _Length(0)
{
    _Path = path;
    _Valid = _Load(path);
}

Wav::~Wav()
{
    if (_FileContent!=NULL)
        free(_FileContent);
}

bool Wav::_Load(string path)
{
    LOG("loading %s", path.c_str());

    FILE* file = fopen(path.c_str(), "rb");
    if (file==NULL)
    {
        LOG("wav not found");
        return false;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    _FileContent = (char*)malloc(size);
    if (_FileContent==NULL)
    {
        fclose(file);
        LOG("wav is too big");
        return false;
    }

    long result = fread(_FileContent, 1, size, file);
    fclose(file);
    if (result!=size)
    {
        free(_FileContent);
        LOG("wav is corrupted");
        return false;
    }

    unsigned int s = ((int*)_FileContent)[1];
    if ((memcmp(_FileContent, "RIFF", 4)!=0)
        || (s+8!=size)
        || (memcmp(_FileContent+8, "WAVE", 4)!=0)
        || (memcmp(_FileContent+12, "fmt ", 4)!=0))
    {
        free(_FileContent);
        LOG("wav is corrupted");
        return false;
    }

    unsigned int format_size = *(int*)(_FileContent+16);
    short format = *(short*)(_FileContent+20);
    short channel = *(short*)(_FileContent+22);
    unsigned int sample_rate = *(int*)(_FileContent+24);
    unsigned int k = *(int*)(_FileContent+28);
    short byte_size = *(short*)(_FileContent+34);
    if ((format_size!=16)
        || (format!=1)              // raw
        || (channel!=2)             // stereo
        || (sample_rate!=44100)     // 44100Hz
        || (k!=44100*16*2/8)
        || (byte_size!=16))         // 16bits
    {
        free(_FileContent);
        LOG("wav is unsupported");
        return false;
    }

    unsigned int sample_size = *(unsigned int*)(_FileContent+40);
    if ((memcmp(_FileContent+36, "data", 4)!=0)
        || (sample_size+44!=size))
    {
        free(_FileContent);
        LOG("wav is corrupted");
        return false;
    }

    _Data = (short*)(_FileContent+44);
    _Length = sample_size/4;

    return _Length>0;
}

