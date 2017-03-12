#include "device.h"

Device* Device::_Instance = NULL;

Device::Device() :
    _Ready(false),
    _Quit(false),
    _Sample(NULL)
{
    _Instance = this;

    if (pthread_mutex_init(&_Lock, NULL)!=0)
        ERROR("failed to create mutex");

    if (pthread_create(&_Thread, NULL, _RunThreaded, (void*)this)!=0)
        ERROR("failed to create thread");

    while (!_Ready)
        usleep(10);

    pthread_mutex_lock(&_Lock);
    _Sample = new Sample("../samples/sample.wav");
    pthread_mutex_unlock(&_Lock);
}

Device::~Device()
{
    _Quit = true;
    pthread_join(_Thread, NULL);
    pthread_mutex_destroy(&_Lock);

    delete _Sample;
}

void Device::OnNoteOn(int device_id, int channel, int note, int velocity)
{
    if (_Sample!=NULL)
    {
        for (int i=0 ; i<_Voices.size() ; ++i)
        {
            if (!_Voices[i]->IsBusy())
            {
                pthread_mutex_lock(&_Lock);
                _Voices[i]->OnNoteOn(_Sample, device_id, channel, note, velocity);
                pthread_mutex_unlock(&_Lock);
                return;
            }
        }
    }
}

void Device::OnNoteOff(int device_id, int channel, int note, int velocity)
{
    if (_Sample!=NULL)
    {
        for (int i=0 ; i<_Voices.size() ; ++i)
        {
            if (_Voices[i]->IsPlaying(device_id, channel, note))
            {
                pthread_mutex_lock(&_Lock);
                _Voices[i]->OnNoteOff(velocity);
                pthread_mutex_unlock(&_Lock);
                return;
            }
        }
    }
}

void* Device::_RunThreaded(void* data)
{
    Device* device = (Device*)data;
    device->_Run();
}

void Device::_Run()
{
    _Create();
    _Ready = true;
    LOG("device ready");

    snd_pcm_sframes_t frames_to_deliver;

    while (!_Quit)
    {
        usleep(1);

        if (snd_pcm_wait(_PlaybackHandle, 10)<0)
            ERROR("snd_pcm_wait failed");

        frames_to_deliver = snd_pcm_avail_update(_PlaybackHandle);

        if (frames_to_deliver<0)
            ERROR("snd_pcm_avail_update<0");

        if (frames_to_deliver>0)
        {
            if (frames_to_deliver>SAMPLER_BUFFER_SIZE)
                frames_to_deliver = SAMPLER_BUFFER_SIZE;

            pthread_mutex_lock(&_Lock);
            _Update(frames_to_deliver);
            pthread_mutex_unlock(&_Lock);
        }
    }

    _Destroy();
    LOG("device destroyed");
}

void Device::_Create()
{
    if (snd_pcm_open(&_PlaybackHandle, SAMPLER_DEVICE, SND_PCM_STREAM_PLAYBACK, 0)<0)
        ERROR("device is not available");

    snd_pcm_hw_params_t* hw_params;
    if (snd_pcm_hw_params_malloc(&hw_params)<0) ERROR("failed to alloc hw_params");
    if (snd_pcm_hw_params_any(_PlaybackHandle, hw_params)<0) ERROR("snd_pcm_hw_params_any");
    if (snd_pcm_hw_params_set_access(_PlaybackHandle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)<0) ERROR("SND_PCM_ACCESS_RW_INTERLEAVED");
    if (snd_pcm_hw_params_set_format(_PlaybackHandle, hw_params, SND_PCM_FORMAT_S16_LE)<0) ERROR("SND_PCM_FORMAT_S16_LE");
    int dir = 0;
    unsigned int rate = SAMPLER_RATE;
    if (snd_pcm_hw_params_set_rate_near(_PlaybackHandle, hw_params, &rate, &dir)<0) ERROR("snd_pcm_hw_params_set_rate_near");
    if (snd_pcm_hw_params_set_channels(_PlaybackHandle, hw_params, SAMPLER_CHANNELS)<0) ERROR("snd_pcm_hw_params_set_channels");

    snd_pcm_uframes_t buffer_size = SAMPLER_BUFFER_SIZE;
    snd_pcm_uframes_t period_size = SAMPLER_PERIOD_SIZE;
    //if (snd_pcm_hw_params_set_buffer_size_near(_PlaybackHandle, hw_params, &buffer_size)<0) ERROR("snd_pcm_hw_params_set_buffer_size_near");
    if (snd_pcm_hw_params_set_period_size_near(_PlaybackHandle, hw_params, &period_size, NULL)<0) ERROR("snd_pcm_hw_params_set_period_size_near");

    if (snd_pcm_hw_params(_PlaybackHandle, hw_params)<0) ERROR("snd_pcm_hw_params");
    snd_pcm_hw_params_free(hw_params);

    snd_pcm_sw_params_t* sw_params;
    if (snd_pcm_sw_params_malloc(&sw_params)<0) ERROR("failed to alloc sw_params");
    if (snd_pcm_sw_params_current(_PlaybackHandle, sw_params)<0) ERROR("snd_pcm_sw_params_current");
    if (snd_pcm_sw_params_set_avail_min(_PlaybackHandle, sw_params, SAMPLER_BUFFER_SIZE)<0) ERROR("snd_pcm_sw_params_set_avail_min");
    if (snd_pcm_sw_params_set_start_threshold(_PlaybackHandle, sw_params, 0U)<0) ERROR("snd_pcm_sw_params_set_start_threshold");
    if (snd_pcm_sw_params(_PlaybackHandle, sw_params)<0) ERROR("snd_pcm_sw_params");
    snd_pcm_sw_params_free(sw_params);

    if (snd_pcm_prepare(_PlaybackHandle)<0)
        ERROR("snd_pcm_prepare");

    _Buffer = (short*)malloc(sizeof(short)*SAMPLER_BUFFER_SIZE*SAMPLER_CHANNELS);
    if (_Buffer==NULL)
        ERROR("failed to alloc buffer");

    for (int i=0 ; i<SAMPLER_VOICES ; ++i)
        _Voices.push_back(new Voice());
}

void Device::_Destroy()
{
    for (int i=0 ; i<_Voices.size() ; ++i)
        delete _Voices[i];

    if (_Buffer!=NULL)
        free(_Buffer);

    snd_pcm_close(_PlaybackHandle);
}

void Device::_Update(snd_pcm_uframes_t frames)
{
    for (int i=0 ; i<frames ; ++i)
    {
        int left = 0;
        int right = 0;

        for (int j=0 ; j<_Voices.size() ; ++j)
        {
            Voice* voice = _Voices[j];
            if (voice->IsBusy())
            {
                int l, r;
                voice->Update(l, r);
                left += l;
                right += r;
            }
        }

        if (left>32767) left = 32767;
        else if (left<-32768) left = -32768;
        if (right>32767) right = 32767;
        else if (right<-32768) right = -32768;

        _Buffer[i*2] = (short)(left-1);
        _Buffer[i*2+1] = (short)(right-1);
    }

    if (snd_pcm_writei(_PlaybackHandle, _Buffer, frames)!=frames)
        ERROR("update voice failed");
}


