#ifndef ABSTRACTSYSTEM_H
#define ABSTRACTSYSTEM_H

#include <QString>
#include <QFile>
#include "source/LeLib/util/cinifile.h"
#include <QMap>

#include <QElapsedTimer>

class SymbolTable;

class SystemLabel {
public:
    enum Type {SCREEN, SID, REGISTERS, ZEROPAGE,STACK, BASIC, FREE, VIC,COLOUR,IO,KERNAL,UNAVAILABLE};
    Type m_type;
    SystemLabel() {}
    QString m_name;
    int m_from, m_to;
    SystemLabel(Type t, QString name, int from, int to) {
        m_type =t;
        m_name =name;
        m_from = from;
        m_to = to;
    }
};

class AbstractSystem
{
public:
    AbstractSystem(CIniFile* settings, CIniFile* proj) {
        m_projectIni = proj;
        m_settingsIni = settings;
        if (m_labelColors.count()==0)
            InitLabelColors();
    }
    QMap<QString, QString> m_systemParams;
    QElapsedTimer timer;
    QString m_orgOutput;
    CIniFile* m_projectIni, *m_settingsIni;
    int m_startAddress = 0x800;
    int m_programStartAddress = m_startAddress+10;
//    bool m_hasSysHeader = true;

    int m_memoryType = 0;
    QVector<SystemLabel> m_labels;
    int m_memorySize = 65536;
    static QMap<SystemLabel::Type, QColor> m_labelColors;
    QColor m_systemColor = QColor(64,64,128);

    static void InitLabelColors();

    enum System {C64, VIC20, PET, NES, C128, BBCM, AMIGA, PLUS4, OK64, X16,X86};
    enum Processor {MOS6502, M68000,PX86};

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
        if (s.toLower()=="x86")
            return X86;
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
        if (s == X86) return "X86";
    }


    System m_system = C64;
    Processor m_processor = MOS6502;

    virtual void Assemble(QString& text, QString file, QString currentDir, SymbolTable* symTab) {}
    virtual void PostProcess(QString& text, QString file, QString currentDir) {}
    virtual void DefaultValues() {}
};



#endif // ABSTRACTSYSTEM_H
