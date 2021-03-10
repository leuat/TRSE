#include "ttrplayer.h"



TTRPlayer::TTRPlayer()
{

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
