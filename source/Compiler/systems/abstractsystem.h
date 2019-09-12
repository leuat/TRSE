#ifndef ABSTRACTSYSTEM_H
#define ABSTRACTSYSTEM_H

#include <QString>
#include <QFile>
#include "source/LeLib/util/cinifile.h"

#include <QElapsedTimer>
class SymbolTable;

class AbstractSystem
{
public:
    AbstractSystem(CIniFile* settings, CIniFile* proj) {
        m_projectIni = proj;
        m_settingsIni = settings;
    }
    QElapsedTimer timer;
    QString m_orgOutput;
    CIniFile* m_projectIni, *m_settingsIni;
    enum System {C64, VIC20, PET, NES, C128, BBCM, AMIGA, PLUS4, OK64, X16};
    enum Processor {MOS6502, M68000};

    bool m_buildSuccess;
    static bool isSupported(System currentSystem, QString list) {
        return isSupported(StringFromSystem(currentSystem), list);
    }
    static bool isSupported(QString currentSystem, QString list) {
        QStringList lst  = list.toLower().trimmed().simplified().split(",");
        for (QString s : lst)
            if (s == currentSystem.toLower())
                return true;

        return false;
    }

    static System SystemFromString(QString s) {
        if (s.toLower()=="c64")
            return C64;
        if (s.toLower()=="c128")
            return C128;
        if (s.toLower()=="pet")
            return PET;
        if (s.toLower()=="vic20")
            return VIC20;
        if (s.toLower()=="nes")
            return NES;
        if (s.toLower()=="bbcm")
            return BBCM;
        if (s.toLower()=="amiga")
            return AMIGA;
        if (s.toLower()=="plus4")
            return PLUS4;
        if (s.toLower()=="ok64")
            return OK64;
        if (s.toLower()=="x16")
            return X16;
    }

    static QString StringFromSystem(System s) {
        if (s == C64) return "C64";
        if (s == PET) return "PET";
        if (s == VIC20) return "VIC20";
        if (s == NES) return "NES";
        if (s == C128) return "C128";
        if (s == BBCM) return "BBCM";
        if (s == AMIGA) return "AMIGA";
        if (s == PLUS4) return "PLUS4";
        if (s == OK64) return "OK64";
        if (s == X16) return "X16";
    }


    System m_system = C64;
    Processor m_processor = MOS6502;

    virtual void Assemble(QString& text, QString file, QString currentDir, SymbolTable* symTab) {}
    virtual void PostProcess(QString& text, QString file, QString currentDir) {}

};



#endif // ABSTRACTSYSTEM_H
