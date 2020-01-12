#ifndef LIMAGEFOOTER_H
#define LIMAGEFOOTER_H

#include <QByteArray>
#include <QFile>
#include <QDebug>

class LImageFooter {
private:
    QByteArray m_data;
public:
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
    LImageFooter() {
        m_data.resize(256);
        m_data.fill(0);
        m_data[POS_ID0] = LImageID0;
        m_data[POS_ID1] = LImageID1;
    }

    uchar get(int pos) {
        if (pos<m_data.count())
            return m_data[pos];
        return 0;
    }
    void set(int pos, uchar val) {
        if (pos<m_data.count())
            m_data[pos] = val;
    }

    bool Load(QFile& f) {
        if (f.atEnd()) {
            qDebug() << "No footer in .flf file "<<f.fileName();
            return false;
        }
        QByteArray b = f.readAll();
        if (b.size()!=256) {
            qDebug() << "Incorrect footer size in .flf file "<<f.fileName();
            return false;
        }
        if (b[POS_ID0]!=(char)LImageID0 || b[POS_ID1]!=(char)LImageID1) {
            qDebug() << "Incorrect footer ID";
            return false;
        }
        m_data =b;
        return true;
    }
    bool Save(QFile& f) {
        f.write(m_data);
        return true;
    }

};


#endif // LIMAGEFOOTER_H
