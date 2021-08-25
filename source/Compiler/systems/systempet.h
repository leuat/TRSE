#ifndef SYSTEMPET_H
#define SYSTEMPET_H

#include "systemmos6502.h"

class SystemPET : public SystemMOS6502
{
public:
    SystemPET(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);
    void DefaultValues() override;
    void InitSystemPreprocessors(QMap<QString, QString>& defines) override;;

    int getDefaultBasicAddress() override {
        return 0x401;
    };
    bool isCommodoreSystem() override  {return true;}

    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("pet_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
        QString petmodel = pini->getString("petmodel");
        if (petmodel!="") {
            params << "-model" << petmodel;
        }
        applyEmulatorParametersVICE(params, debugFile, filename);
    }

};


#endif // SYSTEMPET_H
