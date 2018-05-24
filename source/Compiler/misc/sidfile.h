#ifndef SIDFILE_H
#define SIDFILE_H

#include <QString>
#include <QFile>
#include <QByteArray>
#include "source/Compiler/errorhandler.h"

class SidFile
{
public:
    SidFile();
    QByteArray m_blob, m_stripped;
    unsigned short m_loadAddress, m_initAddress, m_playAddress;
    QString m_outFile, m_fileName, m_path;

    void Load(QString filename, QString path);
    void Convert(int headerShift);
};

#endif // SID_H

