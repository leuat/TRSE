#ifndef SYSTEMACORN_H
#define SYSTEMACORN_H

#include "systemmos6502.h"

class SystemAcorn : public SystemMOS6502
{
public:
    SystemAcorn(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);
    void PostProcess(QString &text, QString filename, QString currentDir) override;
    int m_totalSize = 0;
    void DefaultValues() override;
   /// void AddFileToDisk(DiscImage* di, QString filename, QString name, int address);
  //  bool BuildDiskFiles(DiscImage* di, QString currentDir, QString iniData, QString&text);
    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("acorn_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
//        params<< "-0" << filename + ".ssd" <<"-b";
        params<< "-autoboot" <<filename + ".atm";

    }

};

#endif // SYSTEMACORN_H
