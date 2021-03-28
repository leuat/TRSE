#ifndef TTRPLAYER_H
#define TTRPLAYER_H

#include <QThread>
#include "source/LeLib/ttrfile.h"
#include <QBuffer>
#include "source/mynth/mynthesizer.h"
#include "source/LeLib/util/util.h"

struct Mynthesizer {
    mynth::Voice voices[4];
};

class TRSEInstrument {
public:
    QString name;
    float a=0.1,d=0.1,s=0.1,r=0.1;
    float l0=0.3, l1=0, l2=1;
    float w0=0,w1=0,w2=1,w3=0;
    int oct =0;
    float vibrato=0;
    TRSEInstrument() {

    }
};


class TRSEInstruments {
public:
    QVector<QSharedPointer<TRSEInstrument>> m_instruments;
    QString m_fileName = "";
    void Load(QString fname);
    void Save();
    void New();
};


class TTRPlayer : public QThread
{
    Q_OBJECT
public:
    TTRPlayer();
    ~TTRPlayer();
    TTRFile* m_ttr = nullptr;
    int m_curOrder=0, m_curRow=0;
    bool m_isPlaying = false;
    static const int size = 102400;
    char m_data[size];
    Mynthesizer mynthesizer;
    TRSEInstruments m_instruments;
    static bool m_initialized;// = false;
    QVector<bool> m_silentChannels;

    enum class JamMode {
        Mono
    };

    struct JamContext {
        JamMode mode = JamMode::Mono;
        float mono_last_frequency = -1;
        int mono_last_note = -1;
        int mono_channel = 0;
        mynth::ADSRSettings adsr;
//        sine, square, saw, triangle.
        float wavemixer[4] = { 0, 1, 0, 0 };
        mynth::LFO vibrato;
    } jam_context;


    void Initialize(TTRFile* ttr);

    void Play();
    void PlayNote(int channel, QByteArray data);
    void PlayNote(int channel, int midi_note, int velocity, QSharedPointer<TRSEInstrument> ins);
    void Stop();
    void StartPlaying();
    void StopSound() {
        for (auto& voc : mynthesizer.voices) {
            ma_sound_stop(&voc.sound);
        }

    }

    void StartSound(int channel) {
        ma_sound_start(&mynthesizer.voices[channel].sound);
    }
    void StartSound() {
        for (auto& voc : mynthesizer.voices) {
            ma_sound_start(&voc.sound);
        }

    }

    void run() override;
signals:
    void emitUpdateRow(int row);

};


#endif // TTRPLAYER_H
