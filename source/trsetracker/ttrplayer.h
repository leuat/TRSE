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
    float a,d,s,r;
    float l0, l1, l2;
    float w0,w1,w2,w3;
    int oct;
    float vibrato;
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

    void run() override;
signals:
    void emitUpdateRow(int row);

};


#endif // TTRPLAYER_H
