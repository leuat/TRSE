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
    int m_totalSize = 0;
    void DefaultValues() override;
    void AddFileToDisk(DiscImage* di, QString filename, QString name, int address);
    bool BuildDiskFiles(DiscImage* di, QString currentDir, QString iniData, QString&text);
    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("bbc_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
        params<< "-0" << filename + ".ssd" <<"-b";

    }

};

#endif // SYSTEMBBCM_H
