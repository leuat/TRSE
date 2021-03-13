#include "ttrplayer.h"

#define DEVICE_FORMAT       ma_format_f32
#define DEVICE_CHANNELS     2
#define DEVICE_SAMPLE_RATE  48000

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_audio_buffer* pSineWave;
    pSineWave = (ma_audio_buffer*)pDevice->pUserData;

    //ma_waveform_read_pcm_frames(pSineWave, pOutput, frameCount);
    qDebug() << pSineWave;
     ma_data_source_read_pcm_frames(pSineWave, pOutput, frameCount, NULL, true);

//     ma_decoder_read_pcm_frames(pSineWave, pOutput, frameCount);
    (void)pInput;   /* Unused. */
}


TTRPlayer::TTRPlayer()
{

    sineWaveConfig = ma_waveform_config_init(DEVICE_FORMAT, DEVICE_CHANNELS, DEVICE_SAMPLE_RATE, ma_waveform_type_sine, 0.2, 220);
    ma_waveform_init(&sineWaveConfig, &sineWave);

//    allocationCallbacks = ma_allocation_callbacks_init_default();
    for (int i=0;i<size;i++)
        m_data[i] = rand()%10000;

    bufferConfig = ma_audio_buffer_config_init(
        DEVICE_FORMAT,
        DEVICE_CHANNELS,
        1,
        m_data,
        &allocationCallbacks);




    ma_result result = ma_audio_buffer_init(&bufferConfig, &buffer);
    if (result!=MA_SUCCESS) {
        qDebug() << "COULD not initialize bufferconfig";
    }

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = DEVICE_FORMAT;
    deviceConfig.playback.channels = DEVICE_CHANNELS;
    deviceConfig.sampleRate        = DEVICE_SAMPLE_RATE;
    deviceConfig.dataCallback      = data_callback;
//    deviceConfig.pUserData         = &sineWave;
    deviceConfig.pUserData         = &bufferConfig;



}

TTRPlayer::~TTRPlayer()
{

}


void TTRPlayer::run()
{
    while (m_isPlaying){
        msleep(1000);
        if (!m_isPlaying)
            return;
        Play();
    }
}

void TTRPlayer::Play() {
    if (m_isPlaying)
        return;
    ma_device_init(NULL, &deviceConfig, &device);
    ma_device_start(&device);
    m_isPlaying = true;
}

void TTRPlayer::Stop()
{
    if (!m_isPlaying)
        return;
    ma_device_uninit(&device);
    m_isPlaying = false;
}
