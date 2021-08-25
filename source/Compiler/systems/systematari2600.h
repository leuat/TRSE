#ifndef SYSTEMATARI2600_H
#define SYSTEMATARI2600_H

#include "systemmos6502.h"

class SystemAtari2600 : public SystemMOS6502
{
public:
    SystemAtari2600(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);
    void DefaultValues() override;
    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("atari2600_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
        params<< "-f"<<filename+".prg";
//        qDebug() << filename;

    }

};


#endif // SYSTEMATARI2600_H
