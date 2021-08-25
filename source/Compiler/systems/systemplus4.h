#ifndef SYSTEMPLUS4_H
#define SYSTEMPLUS4_H


#include "systemmos6502.h"

class SystemPlus4 : public SystemMOS6502
{
public:
    SystemPlus4(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemMOS6502(settings, proj) {
        m_processor = MOS6502;
        m_system = PLUS4;
        DefaultValues();
    }
    int getDefaultBasicAddress() override {
        return 0x1001;
    };

    void DefaultValues() override {
        m_startAddress = getDefaultBasicAddress();
        m_programStartAddress = 0x1010;
        m_ignoreSys = false;
        m_stripPrg = false;

    }
    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("plus4_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
        applyEmulatorParametersVICE(params, debugFile,filename);

    }

};


#endif // SYSTEMPLUS4_H
