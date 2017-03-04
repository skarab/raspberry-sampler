#include "sample.h"

Sample::Sample(string path)
{
    _Wav = new Wav(path);
}

Sample::~Sample()
{
    if (_Wav!=NULL)
        delete _Wav;
}
