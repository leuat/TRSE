#ifndef SYSTEMX16_H
#define SYSTEMX16_H

#include "system65c816.h"

class SystemX16 : public System65C816
{
public:
    SystemX16(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);
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

#endif // SYSTEMX16_H
