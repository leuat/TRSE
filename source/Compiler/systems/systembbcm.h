#ifndef SYSTEMBBCM_H
#define SYSTEMBBCM_H

#include "systemmos6502.h"
#include "source/LeLib/bbc/globaldata.h"
#include "source/LeLib/bbc/discimage.h"



class SystemBBCM : public SystemMOS6502
{
public:
    SystemBBCM(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);
    void PostProcess(QString &text, QString filename, QString currentDir) override;

    void DefaultValues() override;
    void AddFileToDisk(DiscImage* di, QString filename, QString name, int address);
    bool BuildDiskFiles(DiscImage* di, QString currentDir, QString iniData);
};

#endif // SYSTEMBBCM_H
