#ifndef SYSTEMC64_H
#define SYSTEMC64_H

#include "systemmos6502.h"

class SystemC64 : public SystemMOS6502
{
public:
    SystemC64(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) ;
    void DefaultValues() override;

    int getDefaultBasicAddress() override {
        return 0x801;
    };
    bool isCommodoreSystem() override  {return true;}

    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
        applyEmulatorParametersVICE(params,debugFile,filename);

    }

};



#endif // SYSTEMC64_H
