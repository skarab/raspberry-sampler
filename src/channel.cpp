#include "channel.h"

Channel::Channel(string device, unsigned int rate, unsigned int channels, unsigned int buffer_size) :
    _Device(device),
    _Rate(rate),
    _Channels(channels),
    _BufferSize(buffer_size),
    _Error(false),
    _Quit(false)
{
    if (pthread_create(&_Thread, NULL, _RunThreaded, (void*)this)!=0)
    {
        _Thread = NULL;
        _Error = true;
    }
}

Channel::~Channel()
{
    if (_Thread!=NULL)
    {
        _Quit = true;
        pthread_join(_Thread, NULL);
    }
}

bool Channel::_Update(snd_pcm_uframes_t frames)
{
    for (int i=0 ; i<frames ; ++i)
    {
        short left = 0;
        short right = 0;

        _Buffer[i*2] = left;
        _Buffer[i*2+1] = right;
    }

    return snd_pcm_writei(_PlaybackHandle, _Buffer, frames)>=0;
}

void Channel::_Run()
{
    if (!_Create())
    {
        _Error = true;
        return;
    }

    snd_pcm_sframes_t frames_to_deliver;

    while (!_Quit)
    {

        if (snd_pcm_wait(_PlaybackHandle, 1000)<0)
        {
            _Error = true;
            break;
        }

        frames_to_deliver = snd_pcm_avail_update(_PlaybackHandle);

        if (frames_to_deliver<0)
        {
            _Error = true;
            break;
        }

        if (frames_to_deliver>0)
        {
            if (frames_to_deliver>_BufferSize)
                frames_to_deliver = _BufferSize;

            if (!_Update(frames_to_deliver))
            {
                _Error = true;
                break;
            }
        }
    }

    _Destroy();
}

bool Channel::_Create()
{
    if (snd_pcm_open(&_PlaybackHandle, _Device.c_str(), SND_PCM_STREAM_PLAYBACK, 0)<0)
        return false;

    snd_pcm_hw_params_t* hw_params;
    if (snd_pcm_hw_params_malloc(&hw_params)<0) return false;
    if (snd_pcm_hw_params_any(_PlaybackHandle, hw_params)<0) return false;
    if (snd_pcm_hw_params_set_access(_PlaybackHandle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)<0) return false;
    if (snd_pcm_hw_params_set_format(_PlaybackHandle, hw_params, SND_PCM_FORMAT_S16_LE)<0) return false;
    int dir = 0;
    if (snd_pcm_hw_params_set_rate_near(_PlaybackHandle, hw_params, &_Rate, &dir)<0) return false;
    if (snd_pcm_hw_params_set_channels(_PlaybackHandle, hw_params, _Channels)<0) return false;
    if (snd_pcm_hw_params(_PlaybackHandle, hw_params)<0) return false;
    snd_pcm_hw_params_free(hw_params);

    snd_pcm_sw_params_t* sw_params;
    if (snd_pcm_sw_params_malloc(&sw_params)<0) return false;
    if (snd_pcm_sw_params_current(_PlaybackHandle, sw_params)<0) return false;
    if (snd_pcm_sw_params_set_avail_min(_PlaybackHandle, sw_params, _BufferSize)<0) return false;
    if (snd_pcm_sw_params_set_start_threshold(_PlaybackHandle, sw_params, 0U)<0) return false;
    if (snd_pcm_sw_params(_PlaybackHandle, sw_params)<0) return false;
    snd_pcm_sw_params_free(sw_params);

    if (snd_pcm_prepare(_PlaybackHandle)<0)
        return false;

    _Buffer = (short*)malloc(sizeof(short)*_BufferSize*_Channels);
    if (_Buffer==NULL)
        return false;

    return true;
}

void Channel::_Destroy()
{
    if (_Buffer!=NULL)
        free(_Buffer);

    snd_pcm_close(_PlaybackHandle);
}

void* Channel::_RunThreaded(void* data)
{
    Channel* channel = (Channel*)data;
    channel->_Run();
}
