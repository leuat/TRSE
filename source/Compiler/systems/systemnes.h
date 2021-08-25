#ifndef SYSTEMNES_H
#define SYSTEMNES_H

#include "systemmos6502.h"

class SystemNES : public SystemMOS6502
{
public:
    SystemNES(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);
    void DefaultValues() override;
    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("nes_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
        params<< filename+".nes";
    }

};

#endif // SYSTEMNES_H
