#include "includes.h"
#include "device.h"

int main(int argc, char *argv[])
{
    try
    {
        Device* device = new Device(SAMPLER_DEVICE, SAMPLER_RATE, SAMPLER_CHANNELS, SAMPLER_BUFFER_SIZE, SAMPLER_VOICES);

        while(1)
        {
        }

        delete device;
    }
    catch(Error& error)
    {
        error.Log();
    }
    return 0;
}






/*
snd_pcm_t *playback_handle;

short* sample_data;
long sample_size;
long position = 0;

short buffer[8192];

int playback_callback(snd_pcm_uframes_t nframes)
{
    for (int i=0 ; i<nframes ; ++i)
    {
        short left = sample_data[position];
        short right = sample_data[position+1];

        buffer[i*2] = left;
        buffer[i*2+1] = right;

        position += 2;
        if (position>=sample_size/2)
            position = 0;
    }

    if (snd_pcm_writei(playback_handle, buffer, nframes)<0)
    {
        fprintf (stderr, "write failed");
        return -1;
    }
    return nframes;
}


main(int argc, char *argv[])
{
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_sw_params_t *sw_params;
    snd_pcm_sframes_t frames_to_deliver;
    int nfds;
    int err;
    struct pollfd *pfds;

    // hw:2,0
    if ((err = snd_pcm_open (&playback_handle, "hw:1,0", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        fprintf (stderr, "cannot open audio device %s (%s)\n",
             "pulse",
             snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
        fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
             snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_any (playback_handle, hw_params)) < 0) {
        fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
             snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_set_access (playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        fprintf (stderr, "cannot set access type (%s)\n",
             snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_set_format (playback_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
        fprintf (stderr, "cannot set sample format (%s)\n",
             snd_strerror (err));
        exit (1);
    }

    unsigned int rate = 44100;
    int dir = 0;

    if ((err = snd_pcm_hw_params_set_rate_near (playback_handle, hw_params, &rate, &dir)) < 0) {
        fprintf (stderr, "cannot set sample rate (%s)\n",
             snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_set_channels (playback_handle, hw_params, 2)) < 0) {
        fprintf (stderr, "cannot set channel count (%s)\n",
             snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params (playback_handle, hw_params)) < 0) {
        fprintf (stderr, "cannot set parameters (%s)\n",
             snd_strerror (err));
        exit (1);
    }

    snd_pcm_hw_params_free (hw_params);

    if ((err = snd_pcm_sw_params_malloc (&sw_params)) < 0) {
        fprintf (stderr, "cannot allocate software parameters structure (%s)\n",
             snd_strerror (err));
        exit (1);
    }
    if ((err = snd_pcm_sw_params_current (playback_handle, sw_params)) < 0) {
        fprintf (stderr, "cannot initialize software parameters structure (%s)\n",
             snd_strerror (err));
        exit (1);
    }
    if ((err = snd_pcm_sw_params_set_avail_min (playback_handle, sw_params, 4096)) < 0) {
        fprintf (stderr, "cannot set minimum available count (%s)\n",
             snd_strerror (err));
        exit (1);
    }
    if ((err = snd_pcm_sw_params_set_start_threshold (playback_handle, sw_params, 0U)) < 0) {
        fprintf (stderr, "cannot set start mode (%s)\n",
             snd_strerror (err));
        exit (1);
    }
    if ((err = snd_pcm_sw_params (playback_handle, sw_params)) < 0) {
        fprintf (stderr, "cannot set software parameters (%s)\n",
             snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_prepare (playback_handle)) < 0) {
        fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
             snd_strerror (err));
        exit (1);
    }


    FILE* file = fopen("sample.wav", "rb");
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* wav_data = (char*)malloc(size);
    if (wav_data==NULL)
    {
        fprintf(stderr, "Memory full");
        exit (2);
    }

    long result = fread(wav_data, 1, size, file);
    if (result!=size)
    {
        fprintf(stderr, "File is corrupt");
        exit (3);
    }

    fclose(file);

    if (memcmp(wav_data, "RIFF", 4)!=0)
    {
        fprintf(stderr, "File is corrupt");
        exit (3);
    }

    unsigned int s = ((int*)wav_data)[1];
    if (s+8!=size)
    {
        fprintf(stderr, "File is corrupt");
        exit (3);
    }

    if (memcmp(wav_data+8, "WAVE", 4)!=0)
    {
        fprintf(stderr, "File is corrupt");
        exit (3);
    }

    if (memcmp(wav_data+12, "fmt ", 4)!=0)
    {
        fprintf(stderr, "File is corrupt");
        exit (3);
    }

    unsigned int format_size = *(int*)(wav_data+16);
    if (format_size!=16)
    {
        fprintf(stderr, "Invalid wave");
        exit (3);
    }

    short format = *(short*)(wav_data+20);
    if (format!=1)
    {
        fprintf(stderr, "Supports PCM only");
        exit (3);
    }

    short channel = *(short*)(wav_data+22);
    if (channel!=2)
    {
        fprintf(stderr, "Supports 2 channels only");
        exit (3);
    }

    unsigned int sample_rate = *(int*)(wav_data+24);
    if (sample_rate!=44100)
    {
        fprintf(stderr, "Supports 44100Hz only");
        exit (3);
    }

    unsigned int k = *(int*)(wav_data+28);
    if (k!=44100*16*2/8)
    {
        fprintf(stderr, "Invalid wave");
        exit (3);
    }

    short byte_size = *(short*)(wav_data+34);
    if (byte_size!=16)
    {
        fprintf(stderr, "Supports 16bits only");
        exit (3);
    }

    if (memcmp(wav_data+36, "data", 4)!=0)
    {
        fprintf(stderr, "File is corrupt");
        exit (3);
    }

    sample_size = *(int*)(wav_data+40);
    if (sample_size+44!=size)
    {
        fprintf(stderr, "Invalid wave");
        exit (3);
    }

    sample_data = (short*)malloc(sample_size);
    if (sample_data==NULL)
    {
        fprintf(stderr, "Memory full");
        exit (2);
    }
    memcpy(sample_data, wav_data+44, sample_size);

    free(wav_data);


    while (1) {


        if ((err = snd_pcm_wait (playback_handle, 1000)) < 0) {
                fprintf (stderr, "poll failed (%s)\n", strerror (errno));
                break;
        }

        if ((frames_to_deliver = snd_pcm_avail_update (playback_handle)) < 0) {
            if (frames_to_deliver == -EPIPE) {
                fprintf (stderr, "an xrun occured\n");
                break;
            } else {
                fprintf (stderr, "unknown ALSA avail update return value (%d)\n",
                     frames_to_deliver);
                break;
            }
        }

        frames_to_deliver = frames_to_deliver > 4096 ? 4096 : frames_to_deliver;

        if (frames_to_deliver>0)
        if (playback_callback (frames_to_deliver) != frames_to_deliver) {
                fprintf (stderr, "playback callback failed\n");
            break;
        }
    }


    free(sample_data);

    snd_pcm_close (playback_handle);
    exit (0);
}

*/
