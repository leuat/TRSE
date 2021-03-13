#ifndef TTRPLAYER_H
#define TTRPLAYER_H

#include <QThread>
#include "source/LeLib/ttrfile.h"
#include <QBuffer>
#define MA_NO_DECODING
#define MA_NO_ENCODING
#define MINIAUDIO_IMPLEMENTATION
#include "source/LeLib/miniaudio.h"
//#include <QAudioOutput>


class TTRPlayer : public QThread
{
public:
    TTRPlayer();
    ~TTRPlayer();
    TTRFile* m_ttr = nullptr;
    int m_curOrder=0, m_curRow=0;
    bool m_isPlaying = false;
    static const int size = 102400;
    char m_data[size];

    ma_waveform sineWave;
    ma_device_config deviceConfig;
    ma_device device;
    ma_waveform_config sineWaveConfig;
    ma_allocation_callbacks allocationCallbacks;
    ma_audio_buffer buffer;
    ma_audio_buffer_config bufferConfig;

    void Initialize(TTRFile* ttr) {
        m_ttr = ttr;
        m_curRow = 0;
        m_curOrder = 0;
    }

    void Play();
    void Stop();

    void run() override;


};


#endif // TTRPLAYER_H
