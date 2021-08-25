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


class SystemX16 : public SystemMOS6502
{
public:
    SystemX16(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemMOS6502(settings, proj) {
        m_processor = MOS6502;
        m_system = X16;
        m_systemColor = QColor(100,60,40);

    }
    virtual QString getEmulatorName() override {



        QString base= m_settingsIni->getString("x16_emulator");
/*
#ifdef __linux__
        base = base.remove("x16emu");
#endif
#ifdef __APPLE__
        base = base.remove("x16emu");
#endif
#ifdef _WIN32
        base = base.toLower();
        base = base.remove("x16emu.exe");
#endif*/
        return base;
    }

    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
/*        QStringList lst = m_settingsIni->getString("x16_emulator_params").trimmed().simplified().split(" ");
        for (QString s: lst) {
            if (s.trimmed()!="") {
                params<<s;
            }
        }*/
        params<< "-run" << "-prg" <<filename+".prg";
    }


};

#endif // SYSTEMC64_H
