#ifndef ABSTRACTSYSTEM_H
#define ABSTRACTSYSTEM_H

#include <QString>
#include <QFile>
#include "source/LeLib/util/cinifile.h"
#include <QMap>
//#include "source/OrgAsm/orgasm.h"
#include <QElapsedTimer>

class Orgasm;

class SymbolTable;

class SystemLabel {
public:
    enum Type {SCREEN, SID, REGISTERS, ZEROPAGE,STACK, BASIC, FREE, VIC,COLOUR,IO,KERNAL,UNAVAILABLE,ROM};
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

class AbstractSystem : public QObject
{
    Q_OBJECT
public:
    AbstractSystem(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) {
        m_projectIni = proj;
        m_settingsIni = settings;
        if (m_labelColors.count()==0)
            InitLabelColors();
    }
    AbstractSystem() {

    }
    QStringList m_registers;
    AbstractSystem(AbstractSystem* a);
    QMap<QString, QString> m_systemParams;
    QElapsedTimer timer;
    QString m_orgOutput;
    bool m_ignoreSys = false;
    bool m_stripPrg = false;
    bool m_canRunAsmFiles = false;

    QMap<int,int> m_addresses;
    bool m_hasVariableColorPalette = false;
    bool m_supportsExomizer = false;
    QString m_exomizerName = "Exomize";

    QSharedPointer<CIniFile> m_projectIni, m_settingsIni;
    int m_startAddress = 0x800;
    int m_programStartAddress = m_startAddress+10;
//    bool m_hasSysHeader = true;

//    virtual void getEmulatorAndParams(QString emulator, QStringList params) = 0;

    QString CompressLZ4(QString fileName) {
        return CompressLZ4(fileName, fileName +"_c");
    }
    QString CompressLZ4(QString fileName, QString outFileName);


    virtual void InitSystemPreprocessors(QMap<QString, QString>& defines)  {};
    virtual QString CompressFile(QString fileName) {
        return fileName;
    }
    Orgasm* m_orgAsm = nullptr;

    int m_memoryType = 0;
    QVector<SystemLabel> m_labels;
    int m_memorySize = 65536;
    static QMap<SystemLabel::Type, QColor> m_labelColors;
    QColor m_systemColor = QColor(64,64,128);

    QStringList m_allowedGlobalTypeFlags;
    QStringList m_allowedProcedureTypeFlags;

    virtual bool is8bit() {
        return true;
    }

    virtual bool isCommodoreSystem()  {return false;}
    virtual int getDefaultBasicAddress() { return 0;}

    virtual bool is486() { return false;}
    virtual bool is386() { return false;}
    virtual bool is286() { return false;}
    virtual bool is8088() { return false;}


    void StartProcess(QString file, QStringList params, QString& output, bool standardOutput = false);

    static void InitLabelColors();
 //   virtual bool hasFixedProgramAddress() {return true;}

    enum System {C64, VIC20, PET, NES, C128, BBCM, AMIGA, PLUS4, OK64, X16,X86, GAMEBOY, SPECTRUM, TIKI100, ATARI2600, ATARI520ST, AMSTRADCPC464, COLECO, MEGA65, ATARI800, MSX, APPLEII};
    enum Processor {MOS6502, M68000,PX86, GBZ80, Z80};

    static QString StringFromProcessor(Processor s) {
        if (s == MOS6502) return "MOS6502";
        if (s == M68000) return "M68000";
        if (s == PX86) return "PX86";
        if (s == GBZ80) return "GBZ80";
        if (s == Z80) return "Z80";
        qDebug() << "SYSTEM CPU NOT FOUND for system "<<s;
        return "";
    }

    static QString StringFromProcessor(QString s) {
        if (s == "PET" || s == "C64" || s == "ATARI2600" ||s == "VIC20" || s == "NES" || s == "OK64" || s == "C128" || s == "PLUS4" || s == "X16" || s == "MEGA65" || s == "BBCM" || s=="ATARI800"|| s=="APPLEII") return "MOS6502";
        if (s == "AMIGA" || s == "ATARI520ST") return "M68000";
        if (s == "X86") return "PX86";
        if (s == "GAMEBOY") return "GBZ80";
        if (s == "AMSTRADCPC464" || s == "TIKI100" || s == "SPECTRUM" || s =="COLECO" || s == "MSX") return "Z80";
        qDebug() << "SYSTEM STRING NOT FOUND for system "<<s ;
        return "";
    }




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

    static System SystemFromString(QString s);

    static QString StringFromSystem(System s);


    System m_system = C64;
    Processor m_processor = MOS6502;

    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) {}
    virtual void PostProcess(QString& text, QString file, QString currentDir) {}
    virtual void DefaultValues() {}
//    virtual void Execute(QString filename, QString currentDir);
public:
signals:
    void EmitTick(QString val);
public slots:
    void AcceptDispatcherTick(QString val);

};



#endif // ABSTRACTSYSTEM_H
