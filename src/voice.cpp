#include "voice.h"

Voice::Voice(string device, unsigned int rate, unsigned int channels, unsigned int buffer_size) :
    _Device(device),
    _Rate(rate),
    _Channels(channels),
    _BufferSize(buffer_size),
     _Quit(false)
{
    if (pthread_create(&_Thread, NULL, _RunThreaded, (void*)this)!=0)
        ERROR("failed to create thread");
}

Voice::~Voice()
{
    _Quit = true;
    pthread_join(_Thread, NULL);
}

void* Voice::_RunThreaded(void* data)
{
    Voice* voice = (Voice*)data;
    voice->_Run();
}

void Voice::_Run()
{
    _Create();

    snd_pcm_sframes_t frames_to_deliver;

    while (!_Quit)
    {

        if (snd_pcm_wait(_PlaybackHandle, 1000)<0)
            ERROR("snd_pcm_wait failed");

        frames_to_deliver = snd_pcm_avail_update(_PlaybackHandle);

        if (frames_to_deliver<0)
            ERROR("snd_pcm_avail_update<0");

        if (frames_to_deliver>0)
        {
            if (frames_to_deliver>_BufferSize)
                frames_to_deliver = _BufferSize;

            _Update(frames_to_deliver);
        }
    }

    _Destroy();
}

void Voice::_Create()
{
    if (snd_pcm_open(&_PlaybackHandle, _Device.c_str(), SND_PCM_STREAM_PLAYBACK, 0)<0)
        ERROR("device not found");

    snd_pcm_hw_params_t* hw_params;
    if (snd_pcm_hw_params_malloc(&hw_params)<0) ERROR("failed to alloc hw_params");
    if (snd_pcm_hw_params_any(_PlaybackHandle, hw_params)<0) ERROR("snd_pcm_hw_params_any");
    if (snd_pcm_hw_params_set_access(_PlaybackHandle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)<0) ERROR("SND_PCM_ACCESS_RW_INTERLEAVED");
    if (snd_pcm_hw_params_set_format(_PlaybackHandle, hw_params, SND_PCM_FORMAT_S16_LE)<0) ERROR("SND_PCM_FORMAT_S16_LE");
    int dir = 0;
    if (snd_pcm_hw_params_set_rate_near(_PlaybackHandle, hw_params, &_Rate, &dir)<0) ERROR("snd_pcm_hw_params_set_rate_near");
    if (snd_pcm_hw_params_set_channels(_PlaybackHandle, hw_params, _Channels)<0) ERROR("snd_pcm_hw_params_set_channels");
    if (snd_pcm_hw_params(_PlaybackHandle, hw_params)<0) ERROR("snd_pcm_hw_params");
    snd_pcm_hw_params_free(hw_params);

    snd_pcm_sw_params_t* sw_params;
    if (snd_pcm_sw_params_malloc(&sw_params)<0) ERROR("failed to alloc sw_params");
    if (snd_pcm_sw_params_current(_PlaybackHandle, sw_params)<0) ERROR("snd_pcm_sw_params_current");
    if (snd_pcm_sw_params_set_avail_min(_PlaybackHandle, sw_params, _BufferSize)<0) ERROR("snd_pcm_sw_params_set_avail_min");
    if (snd_pcm_sw_params_set_start_threshold(_PlaybackHandle, sw_params, 0U)<0) ERROR("snd_pcm_sw_params_set_start_threshold");
    if (snd_pcm_sw_params(_PlaybackHandle, sw_params)<0) ERROR("snd_pcm_sw_params");
    snd_pcm_sw_params_free(sw_params);

    if (snd_pcm_prepare(_PlaybackHandle)<0)
        ERROR("snd_pcm_prepare");

    _Buffer = (short*)malloc(sizeof(short)*_BufferSize*_Channels);
    if (_Buffer==NULL)
        ERROR("failed to alloc buffer");
}

void Voice::_Destroy()
{
    if (_Buffer!=NULL)
        free(_Buffer);

    snd_pcm_close(_PlaybackHandle);
}

void Voice::_Update(snd_pcm_uframes_t frames)
{
    for (int i=0 ; i<frames ; ++i)
    {
        short left = 0;
        short right = 0;

        _Buffer[i*2] = left;
        _Buffer[i*2+1] = right;
    }

    if (snd_pcm_writei(_PlaybackHandle, _Buffer, frames)<0)
        ERROR("update voice failed");
}


