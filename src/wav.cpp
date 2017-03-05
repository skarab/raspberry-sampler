#include "wav.h"

Wav::Wav(string path) :
    _Data(NULL),
    _Length(0)
{
    _Path = path;
    _Valid = _Load(path);
}

Wav::~Wav()
{
    if (_Data!=NULL)
        free(_Data);
}

bool Wav::_Load(string path)
{
    FILE* file = fopen(path.c_str(), "rb");
    if (file==NULL)
    {
        printf("wav not found\n");
        return false;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* wav_data = (char*)malloc(size);
    if (wav_data==NULL)
    {
        fclose(file);
        printf("wav is too big\n");
        return false;
    }

    long result = fread(wav_data, 1, size, file);
    fclose(file);
    if (result!=size)
    {
        free(wav_data);
        printf("wav is corrupted\n");
        return false;
    }

    unsigned int s = ((int*)wav_data)[1];
    if ((memcmp(wav_data, "RIFF", 4)!=0)
        || (s+8!=size)
        || (memcmp(wav_data+8, "WAVE", 4)!=0)
        || (memcmp(wav_data+12, "fmt ", 4)!=0))
    {
        free(wav_data);
        printf("wav is corrupted\n");
        return false;
    }

    unsigned int format_size = *(int*)(wav_data+16);
    short format = *(short*)(wav_data+20);
    short channel = *(short*)(wav_data+22);
    unsigned int sample_rate = *(int*)(wav_data+24);
    unsigned int k = *(int*)(wav_data+28);
    short byte_size = *(short*)(wav_data+34);
    if ((format_size!=16)
        || (format!=1)              // raw
        || (channel!=2)             // stereo
        || (sample_rate!=44100)     // 44100Hz
        || (k!=44100*16*2/8)
        || (byte_size!=16))         // 16bits
    {
        free(wav_data);
        printf("wav is unsupported\n");
        return false;
    }

    unsigned int sample_size = *(unsigned int*)(wav_data+40);
    if ((memcmp(wav_data+36, "data", 4)!=0)
        || (sample_size+44!=size))
    {
        free(wav_data);
        printf("wav is corrupted\n");
        return false;
    }

    _Data = (short*)malloc(sample_size);
    if (_Data==NULL)
    {
        free(wav_data);
        printf("wav is too big\n");
        return false;
    }
    memcpy(_Data, wav_data+44, sample_size);

    _Length = sample_size/4;

    free(wav_data);
    return _Length>0;
}

