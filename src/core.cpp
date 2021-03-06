#include "core.h"

Core* Core::_Instance = NULL;

Core::Core() :
    _Ready(false),
    _Quit(false)
{
    if (pthread_mutex_init(&_Lock, NULL)!=0)
        ERROR("failed to create mutex");

    pthread_attr_t tattr;
    sched_param param;

    if (pthread_attr_init(&tattr)!=0)
        ERROR("pthread_attr_init failed");
    if (pthread_attr_getschedparam(&tattr, &param)!=0)
        ERROR("pthread_attr_getschedparam failed");
    if (pthread_attr_setschedpolicy(&tattr, SCHED_RR)!=0)
        ERROR("pthread_attr_setschedpolicy failed");
    param.sched_priority = 40;
    if (pthread_attr_setschedparam(&tattr, &param)!=0)
        ERROR("pthread_attr_setschedparam failed");

    if (pthread_create(&_Thread, &tattr, _RunThreaded, (void*)this)!=0)
        ERROR("failed to create thread");

    while (!_Ready)
        usleep(10);
}

Core::~Core()
{
    _Quit = true;
    pthread_join(_Thread, NULL);
    pthread_mutex_destroy(&_Lock);
}

bool Core::PlayMono(Sample* sample, int note, int velocity)
{
    Voice* voice = NULL;

    pthread_mutex_lock(&_Lock);

    for (int i=0 ; i<_Voices.size() ; ++i)
    {
        if (_Voices[i]->IsPlaying(sample))
        {
            voice = _Voices[i];
            break;
        }
    }

    pthread_mutex_unlock(&_Lock);

    if (voice!=NULL)
    {
        pthread_mutex_lock(&_Lock);

        if (sample->IsLooping() && voice->IsBusy())
        {
            voice->Stop(sample, note);
        }
        else
        {
            voice->Play(sample, note, velocity);
        }

        pthread_mutex_unlock(&_Lock);
        return true;
    }
    return false;
}

bool Core::Play(Sample* sample, int note, int velocity)
{
    Voice* voice = NULL;

    pthread_mutex_lock(&_Lock);

    for (int i=0 ; i<_Voices.size() ; ++i)
    {
        if (!_Voices[i]->IsBusy())
        {
            voice = _Voices[i];
            break;
        }
    }

    pthread_mutex_unlock(&_Lock);

    if (voice!=NULL)
    {
        pthread_mutex_lock(&_Lock);
        voice->Play(sample, note, velocity);
        pthread_mutex_unlock(&_Lock);
        return true;
    }
    return false;
}

bool Core::Stop(Sample* sample, int note)
{
    for (int i=0 ; i<_Voices.size() ; ++i)
    {
        if (_Voices[i]->IsPlaying(sample, note))
        {
            pthread_mutex_lock(&_Lock);
            _Voices[i]->Stop(sample, note);
            pthread_mutex_unlock(&_Lock);
            return true;
        }
    }
    return false;
}

void Core::StopAll()
{
    pthread_mutex_lock(&_Lock);
    for (int i=0 ; i<_Voices.size() ; ++i)
        _Voices[i]->ForceStop();
    pthread_mutex_unlock(&_Lock);
}

void Core::OnUnloadBank(Bank& bank)
{
    pthread_mutex_lock(&_Lock);
    for (int i=0 ; i<_Voices.size() ; ++i)
    {
        if (bank.HasSample(_Voices[i]->GetSample()))
            _Voices[i]->ForceStop();
    }
    pthread_mutex_unlock(&_Lock);
}

void* Core::_RunThreaded(void* data)
{
    Core* core = (Core*)data;
    core->_Run();
}

void Core::_Run()
{
    _Create();
    LOG("core ready");
    _Ready = true;

    snd_pcm_sframes_t frames_to_deliver;

    while (!_Quit)
    {
        snd_pcm_wait(_PlaybackHandle, 10);
        frames_to_deliver = snd_pcm_avail_update(_PlaybackHandle);

        if (frames_to_deliver>0)
        {
            if (frames_to_deliver>SAMPLER_BUFFER_SIZE)
                frames_to_deliver = SAMPLER_BUFFER_SIZE;

            _Update(frames_to_deliver);
        }
    }

    _Destroy();
    LOG("core destroyed");
}

void Core::_Create()
{
    int result = snd_pcm_open(&_PlaybackHandle, SAMPLER_DEVICE, SND_PCM_STREAM_PLAYBACK, 0);
    if (result<0)
        ERROR(string("device is not available :")+snd_strerror(result));

    snd_pcm_hw_params_t* hw_params;
    if (snd_pcm_hw_params_malloc(&hw_params)<0) ERROR("failed to alloc hw_params");
    if (snd_pcm_hw_params_any(_PlaybackHandle, hw_params)<0) ERROR("snd_pcm_hw_params_any");
    if (snd_pcm_hw_params_set_access(_PlaybackHandle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)<0) ERROR("SND_PCM_ACCESS_RW_INTERLEAVED");
    if (snd_pcm_hw_params_set_format(_PlaybackHandle, hw_params, SND_PCM_FORMAT_S16_LE)<0) ERROR("SND_PCM_FORMAT_S16_LE");
    int dir = 0;
    unsigned int rate = SAMPLER_RATE;
    if (snd_pcm_hw_params_set_rate_near(_PlaybackHandle, hw_params, &rate, &dir)!=0) ERROR("snd_pcm_hw_params_set_rate_near");
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

    _Buffer = (short int*)malloc(sizeof(short)*buffer_size*SAMPLER_CHANNELS);
    if (_Buffer==NULL)
        ERROR("failed to alloc buffer");

    for (int i=0 ; i<SAMPLER_VOICES_PER_CORE ; ++i)
        _Voices.push_back(new Voice());
}

void Core::_Destroy()
{
    for (int i=0 ; i<_Voices.size() ; ++i)
        delete _Voices[i];

    if (_Buffer!=NULL)
        free(_Buffer);

    snd_pcm_close(_PlaybackHandle);
}

void Core::_Update(snd_pcm_uframes_t frames)
{
    const vector<int>& params = Bank::GetGlobalParams();

    short int* out = _Buffer;
    for (int i=0 ; i<frames ; ++i)
    {
        int left = 0;
        int right = 0;

        pthread_mutex_lock(&_Lock);

        for (int j=0 ; j<_Voices.size() ; ++j)
        {
            int l=0,r=0;
            _Voices[j]->Update(l, r);
            left += l;
            right += r;
        }

        pthread_mutex_unlock(&_Lock);

        _LeftFilters.Compute(left, params);
        _RightFilters.Compute(right, params);
        _LeftFilters.ComputeStereo(left, right, params);

        if (left<-32767) left = -32767;
        else if (left>32767) left = 32767;
        if (right<-32767) right = -32767;
        else if (right>32767) right = 32767;

        *out++ = left;
        *out++ = right;
    }

    if (snd_pcm_writei(_PlaybackHandle, _Buffer, frames)<0)
    {
        LOG("buffer underrun");
        snd_pcm_prepare(_PlaybackHandle);
    }
}
