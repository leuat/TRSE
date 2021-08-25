#ifndef SYSTEMATARI800_H
#define SYSTEMATARI800_H

#include "systemmos6502.h"



class SystemAtari800 : public SystemMOS6502
{
public:
    SystemAtari800(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);
    void PostProcess(QString &text, QString filename, QString currentDir) override;

    void DefaultValues() override;
    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("atari800_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
        params<< filename + ".xex";

    }

};

#endif // SYSTEMBBCM_H
