#ifndef TTRPLAYER_H
#define TTRPLAYER_H

#include <QThread>
#include "source/LeLib/ttrfile.h"
#include <QBuffer>
//#include <QAudioOutput>


class TTRPlayer : public QThread
{
public:
    TTRPlayer();
    ~TTRPlayer();
    TTRFile* m_ttr = nullptr;
    int m_curOrder=0, m_curRow=0;
    bool m_isPlaying = true;

    void Initialize(TTRFile* ttr) {
        m_ttr = ttr;
        m_curRow = 0;
        m_curOrder = 0;
    }

    void Play();

    void run() override;


};

inline void TTRPlayer::Play() {

}

#endif // TTRPLAYER_H
