#ifndef SYSTEMVIC20_H
#define SYSTEMVIC20_H

#include "systemmos6502.h"

class SystemVIC20 : public SystemMOS6502
{
public:
    QString param;
    SystemVIC20(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);
    void DefaultValues() override;
    int getDefaultBasicAddress() override;;
    bool isCommodoreSystem() override  {return true;}
    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("vic20_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
        params<< "-memory" << pini->getString("vic_memory_config");
        applyEmulatorParametersVICE(params, debugFile,filename);

    }


};

#endif // SYSTEMVIC20_H
