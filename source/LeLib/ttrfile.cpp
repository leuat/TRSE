#include "ttrfile.h"

TTRFile::TTRFile()
{
}

void TTRFile::InsertPattern(int pos) {
    QByteArray ba;
    ba.resize(m_patternLength*m_noChannels);
    ba.fill(0);
    m_patterns.insert(pos,ba);
    for (QByteArray& ba: m_orders){
        for (int i=0;i<ba.count();i++)
            if ((uchar)ba[i]>=pos)
                ba[i]++;
    }
}

void TTRFile::DeletePattern(int pos)
{
    if (m_patterns.count()<=1)
        return;

    m_patterns.removeAt(pos);
    for (QByteArray& ba: m_orders){
        for (int i=0;i<ba.count();i++)
            if ((uchar)ba[i]>=pos)
                ba[i]--;
    }

}

QVector<QByteArray> TTRFile::getCurrentPattern() {
/*    QByteArray test;
    test.resize(100);
    test.fill(0x10);
    return test;
*/
    QVector<QByteArray> patt;
    if (m_currentOrder>=m_orders.count())
        return patt;
   if (m_currentOrder==-1)
       m_currentOrder=0;
    for (uchar c: m_orders[m_currentOrder])
        patt.append(m_patterns[c]);
    return patt;
}

void TTRFile::Save(QString filename) {
    QByteArray o;
    // Header first:
    o.append("TRT");
    o.append((char)1);
    o.append(m_noChannels);
    o.append(m_patternLength);
    o.append(m_noBytesPerLine);
    // Name and artist - to do
//    for (int i=0;i<0x20;i++) o.append((char)0);
    int startI = POS_START_OF_INSTRUMENTS+SIZE_OF_METADATA;
   // startI = 7+SIZE_OF_METADATA;
    int pos = startI;
    Util::appendInt16(o,pos); // Instrument start
    pos+=m_instruments.size()*INSTRUMENT_SIZE;
    Util::appendInt16(o,pos); // spfx (not used)
    pos+=0;
    Util::appendInt16(o,pos); // order
    pos+=m_orders.size()*m_noChannels;
    Util::appendInt16(o,pos); // pattern start
    pos+=m_patterns.count()*m_patternLength*m_noBytesPerLine;
    Util::appendInt16(o,pos); // TEXT start
//    qDebug() << "TEXT start : " <<pos;

    for (QByteArray& ba: m_instruments)
        o.append(ba);
//    qDebug() << "Saved: " << m_instruments.count()<<m_instruments[0].size();
    for (QByteArray& ba: m_orders)
        o.append(ba);
//    o.append(m_spfx); // not implemented
    for (QByteArray& ba: m_patterns)
        o.append(ba);

    for (QChar c: m_text)
        o.append(c.toLatin1());

    o.append((char)0);


    Util::SaveByteArray(o,filename);
}

void TTRFile::Load(QString filename) {
    QByteArray d = Util::loadBinaryFile(filename);
    if (!((char)d[0]=='T' && (char)d[1]=='R' && (char)d[2]=='T')) {
        qDebug() << "TTRFile::Load NOT a TTR file";
        return;
    }
//    for (int i=0;i<20;i++)
  //      qDebug() << i << QString::number((uchar)d[i]);
    m_version = (uchar)d[3];
    m_noChannels = (uchar)d[4];
    m_patternLength = (uchar)d[5];
    m_noBytesPerLine = (uchar)d[6];

    int startI = Util::getInt16(d,POS_START_OF_INSTRUMENTS);
    int startSPFX = Util::getInt16(d,POS_START_OF_SPFX);
    int startOrders = Util::getInt16(d,POS_START_OF_ORDERS);
    int startPatterns = Util::getInt16(d,POS_START_OF_PATTERNS);
    int startText = Util::getInt16(d,POS_START_OF_TEXT);


//    startText = 1505;
  //  qDebug() << Util::numToHex(startI)<<Util::numToHex(startSPFX)<< Util::numToHex(startOrders) <<Util::numToHex(startPatterns) <<Util::numToHex(startText);;
  //  qDebug() << "NO instruments: " << (startOrders-startI)/16.0;
    m_instruments.clear();
    int noInstruments = (startOrders-startI)/INSTRUMENT_SIZE;
    int c = startI;
    for (int j=0;j<noInstruments;j++) {
        QByteArray ib;
        for (int i=0;i<INSTRUMENT_SIZE;i++)
            ib.append(d[c++]);

        m_instruments.append(ib);
    }
    m_orders.clear();
    int noOrders = (startPatterns-startOrders)/m_noChannels;
    c = startOrders;
    for (int j=0;j<noOrders;j++) {
        QByteArray ib;
        for (int i=0;i<4;i++)
            ib.append(d[c++]);
        m_orders.append(ib);
    }

    m_patterns.clear();
    QByteArray cur;
    int cnt = 0;
    for (int i=startPatterns;i<startText;i++) {
        cur.append(d[i]);
//        qDebug() << Util::numToHex(d[i]);
        cnt++;
        if (cnt==m_patternLength*m_noBytesPerLine || i==d.size()-1) {
            m_patterns.append(cur);
            cur.clear();
            cnt=0;
        }
    }
    m_text ="";
    cnt = startText;
    bool done = false;
//    qDebug() << cnt << d.count();;
    while (!done && cnt<d.count()) {
        if (d[cnt]!=0)
            m_text +=QChar(d[cnt]);
        else done = true;
        if (cnt++>=d.count())
            done = true;
    }
    //qDebug() << "Patterns: " << m_patterns.count();
//    qDebug() << m_text;


}


void TTRFile::LoadPSF(QString filename) {
    QByteArray d = Util::loadBinaryFile(filename);
    if ((char)d[0]!='X') {
        qDebug() << "TTRFile::Load NOT a TTR file";
        return;
    }
    m_noChannels = d[2];
    m_text = "";
    for (int i=0;i<0x20;i++)
        m_text +=d[i+3];

    int startI = Util::getInt16(d,0x23);
    int startSPFX = Util::getInt16(d,0x25);
    int startOrders = Util::getInt16(d,0x27);
    int startPatterns = Util::getInt16(d,0x29);
//    qDebug() << Util::numToHex(startI)<<Util::numToHex(startSPFX)<< Util::numToHex(startOrders) <<Util::numToHex(startPatterns);;
  //  qDebug() << "NO instruments: " << (startOrders-startI)/16.0;
    m_instruments.clear();
    int noInstruments = (startOrders-startI)/INSTRUMENT_SIZE;
    int c = startI;
    for (int j=0;j<noInstruments;j++) {
        QByteArray ib;
        for (int i=0;i<INSTRUMENT_SIZE;i++)
            ib.append(d[c++]);

        m_instruments.append(ib);
    }
    m_orders.clear();
    int noOrders = (startPatterns-startOrders)/m_noChannels;
    c = startOrders;
    for (int j=0;j<noOrders;j++) {
        QByteArray ib;
        for (int i=0;i<4;i++)
            ib.append(d[c++]);
        m_orders.append(ib);
    }

    m_patterns.clear();
    QByteArray cur;
    int cnt = 0;
    for (int i=startPatterns;i<d.size();i++) {
        cur.append(d[i]);
//        qDebug() << Util::numToHex(d[i]);
        cnt++;
        if (cnt==m_patternLength*m_noBytesPerLine || i==d.size()-1) {
            m_patterns.append(cur);
            cur.clear();
            cnt=0;
        }
    }
    qDebug() << "Patterns: " << m_patterns.count();


}

void TTRFile::InsertOrder()
{
    QByteArray ba;
    ba.resize(m_noChannels);
    ba.fill(0);
    m_orders.insert(m_currentOrder,ba);
//    m_currentOrder++;

}

void TTRFile::DeleteOrder()
{
    qDebug() << "Removing from :"  <<m_currentOrder;
    if (m_orders.count()>1)
        m_orders.removeAt(m_currentOrder);

    if (m_currentOrder>=m_orders.count())
        m_currentOrder = m_orders.count()-1;

}
