#include "limagefooter.h"


LImageFooter::LImageFooter() {
    m_data.resize(256);
    m_data.fill(0);
    m_data[POS_ID0] = LImageID0;
    m_data[POS_ID1] = LImageID1;
    m_data[POS_CURRENT_DISPLAY_X] = 1;
    m_data[POS_CURRENT_DISPLAY_Y] = 1;
    m_data[POS_KEEP_MODE] = 1;
}

uchar LImageFooter::get(int pos) {
    if (pos<m_data.count())
        return m_data[pos];
    return 0;
}

void LImageFooter::set(int pos, uchar val) {
    if (pos<m_data.count())
        m_data[pos] = val;
}

void LImageFooter::toggle(int pos) {
    if (pos<m_data.count()) {
        m_data[pos] = !((bool)m_data[pos]);
    }
}

bool LImageFooter::Load(QFile &f) {
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
    if (get(POS_CURRENT_DISPLAY_X)==0)
        set(POS_CURRENT_DISPLAY_X,1);
    if (get(POS_CURRENT_DISPLAY_Y)==0)
        set(POS_CURRENT_DISPLAY_Y,1);
    return true;
}

bool LImageFooter::Save(QFile &f) {
    f.write(m_data);
    return true;
}
