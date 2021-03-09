#ifndef TTRFILE_H
#define TTRFILE_H

#include <QByteArray>
#include <QString>
#include "util/util.h"

class TTRFile
{
public:
    TTRFile();

    int m_noChannels = 4;
    QByteArray m_header;
    QVector<QByteArray> m_patterns;
    QVector<QByteArray> m_instruments;
    QVector<QByteArray> m_orders;
    QString m_text="";
    int m_currentOrder = 0;
    int m_currentInstrument = 0;
    int m_noBytesPerLine = 4;
    int m_patternLength = 0x20;
    int m_version = 0x1;

//    const int POS_START_OF_INSTRUMENTS = 0x23;
    const int POS_START_OF_INSTRUMENTS = 0x7;
    const int SIZE_OF_METADATA = 0x2*5;
    const int POS_START_OF_SPFX = POS_START_OF_INSTRUMENTS+2;
    const int POS_START_OF_ORDERS = POS_START_OF_SPFX+2;
    const int POS_START_OF_PATTERNS = POS_START_OF_ORDERS+2;
    const int POS_START_OF_TEXT = POS_START_OF_PATTERNS+2;
    const int INSTRUMENT_SIZE = 16;

    void InsertPattern(int pos);

    QVector<QByteArray> getCurrentPattern();
    void Save(QString filename);
    void Load(QString filename);
    void LoadPSF(QString filename);
};

#endif // TTRFILE_H
