#ifndef LIMAGEFOOTER_H
#define LIMAGEFOOTER_H

#include <QByteArray>
#include <QFile>
#include <QDebug>

class LImageFooter {
private:
public:
    QByteArray m_data;
    const static uchar LImageID0 = 64;
    const static uchar LImageID1 = 69;
    const static uchar POS_ID0 = 0;
    const static uchar POS_ID1 = 1;
    const static uchar POS_ASPECT = 2;
    const static uchar POS_MAIN_COLOR = 3;
    const static uchar POS_DISPLAY_GRID = 4;
    const static uchar POS_DISPLAY_MULTICOLOR = 5;
    const static uchar POS_DOUBLE_PAINT = 6;
    const static uchar POS_CURRENT_MODE = 7;
    const static uchar POS_KEEP_MODE = 8;
    const static uchar POS_CURRENT_PALETTE = 9;
    const static uchar POS_CURRENT_BANK = 10;
    const static uchar POS_CURRENT_DISPLAY_X = 11;
    const static uchar POS_CURRENT_DISPLAY_Y = 12;
    const static uchar POS_CURRENT_DISPLAY_MIRROR = 13;
    const static uchar POS_CURRENT_DISPLAY_REPEAT = 14;
    const static uchar POS_DISPLAY_CHAR = 15;
    const static uchar POS_CLEAR_VALUE = 16;
    const static uchar POS_CHARSET_WIDTH = 17;
    const static uchar POS_DISPLAY_HYBRID = 18;
    const static uchar POS_GRID_SCALE_X = 19;
    const static uchar POS_GRID_SCALE_Y = 20;
    const static uchar POS_CURRENT_STAMP_X = 21;
    const static uchar POS_CURRENT_STAMP_Y = 22;
    const static uchar POS_D800_FIXED = 23;
    const static uchar POS_PEN_START = 256-64;


    bool isFullscreen() {
        return get(POS_DISPLAY_CHAR)==0;
    }

    LImageFooter();

    uchar get(int pos);
    void set(int pos, uchar val);
    void toggle(int pos);

    bool Load(QFile& f);
    bool Save(QFile& f);

};


#endif // LIMAGEFOOTER_H
