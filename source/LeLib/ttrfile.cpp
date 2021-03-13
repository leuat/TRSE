#include "ttrfile.h"

TTRFile::TTRFile()
{
    m_header.resize(HEADER_SIZE);
    m_header.fill(0);
}

void TTRFile::AddInstrument()
{
    QByteArray ba;
    ba.resize(INSTRUMENT_SIZE);
    m_instruments.append(ba);
}

void TTRFile::Initialize(int channels, int rows)
{
    m_patterns.clear();
    m_orders.clear();
    m_noChannels = channels;
    m_patternLength = rows;
    m_currentInstrument = 0;
    m_currentOrder = 0;
    InsertPattern(0);
    InsertOrder(0);
}

void TTRFile::InsertPattern(int pos) {
    QByteArray ba;
    ba.resize(m_patternLength*m_noBytesPerLine);
    ba.fill(0);
    m_patterns.insert(pos,ba);
    for (QByteArray& ba: m_orders){
        for (int i=0;i<ba.count();i++)
            if ((uchar)ba[i]>pos)
                ba[i]=ba[i]+1;
    }
}

void TTRFile::DeletePattern(int pos)
{
    if (m_patterns.count()<=1)
        return;

    m_patterns.removeAt(pos);
    for (QByteArray& ba: m_orders){
        for (int i=0;i<ba.count();i++)
            if ((uchar)ba[i]>pos)
                ba[i]=ba[i]-1;
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
    o.append((char)2);
    o.append(m_noChannels);
    o.append(m_patternLength);
    o.append(m_noBytesPerLine);
    o.append(m_header);
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

void TTRFile::Export(QString filename, int type)
{
    if (type==1)
        ExportVIC20(filename);
    if (type==2)
        ExportVIC20C(filename);
}

// exports to the Vic 20 as a note and octave directly from the TRT data
void TTRFile::ExportVIC20(QString filename)
{
    QByteArray o;
    // Header first:
    o.append(m_noChannels);
    o.append(m_patternLength);
    o.append(m_orders.count());
    int pos = m_orders.size()*m_noChannels; // Start of patterns
    Util::appendInt16(o,pos); // pattern start

    for (QByteArray& ba: m_orders)
        o.append(ba);

    for (QByteArray& ba: m_patterns) {
        for (int i=0;i<m_patternLength;i++)
            o.append(ba[i*m_noBytesPerLine]); // Only add first byte - ignore rest
    }


    Util::SaveByteArray(o,filename);
}

// exports to the Vic 20 as a fixed 4 track, with notes converted ready to play
void TTRFile::ExportVIC20C(QString filename)
{

    const unsigned char NOTETABLE[ 37 ] = {
        255, 134, 141, 147, 153, 159, 164, 170, 174, 179, 183, 187,
        191, 195, 198, 201, 204, 207, 210, 212, 215, 217, 219, 221,
        223, 225, 226, 228, 230, 231, 232, 234, 235, 236, 237, 238,
        239
    };

    int noChannels = m_noChannels > 4 ? 4 : m_noChannels;

    QByteArray o;
    // Header first:
    o.append( noChannels ); // fixed at 4 max
    o.append( m_patternLength );
    o.append( m_orders.count() );
    int pos = m_orders.size() * noChannels; // Start of patterns
    Util::appendInt16( o, pos ); // pattern start

    for ( QByteArray& ba: m_orders )
        o.append( ba );

    for ( QByteArray& ba: m_patterns ) {
        for ( int i = 0; i < m_patternLength; i++ ) {
            unsigned char snd = ba[ i * m_noBytesPerLine ];  // get first byte of music data (other 3 are not used for VIC)
            if ( (snd & 0x80) != 0 ) {
                snd = snd & 0x7f;
                //qDebug() << "snd=" << snd;
                unsigned char octave = snd / 12;
                //qDebug() << "oct=" << octave;
                if ( octave < 3 ) {

                    o.append( NOTETABLE[ snd ] );
                    //qDebug() << "con=" << NOTETABLE[ snd ];

                } 
                
            } else {

                snd = 0x00;
                o.append( snd );
                //qDebug() << "con=0";

            }
            //o.append( ba[ i * m_noBytesPerLine ] ); // Only add first byte of music data - ignore rest
        }
    }

    Util::SaveByteArray( o, filename );

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
    m_header = d.mid(7,HEADER_SIZE);
    int d2 = 0; // -64 shift used when upgrading stuff
    int startI = Util::getInt16(d,POS_START_OF_INSTRUMENTS+d2);
    int startSPFX = Util::getInt16(d,POS_START_OF_SPFX+d2);
    int startOrders = Util::getInt16(d,POS_START_OF_ORDERS+d2);
    int startPatterns = Util::getInt16(d,POS_START_OF_PATTERNS+d2);
    int startText = Util::getInt16(d,POS_START_OF_TEXT+d2);


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
        for (int i=0;i<m_noChannels;i++)
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
        if ((uchar)d[cnt]!=0)
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

void TTRFile::InsertOrder(int pos)
{
    QByteArray ba;
    ba.resize(m_noChannels);
    ba.fill(0);
    m_orders.insert(pos,ba);
//    m_currentOrder++;

}

void TTRFile::DeleteOrder()
{
//    qDebug() << "Removing from :"  <<m_currentOrder;
    if (m_orders.count()>1)
        m_orders.removeAt(m_currentOrder);

    if (m_currentOrder>=m_orders.count())
        m_currentOrder = m_orders.count()-1;

}
