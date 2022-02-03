#ifndef ABSTRACTSYSTEM_H
#define ABSTRACTSYSTEM_H

#include <QString>
#include <QFile>
#include "source/LeLib/util/cinifile.h"
#include <QMap>
//#include "source/OrgAsm/orgasm.h"
#include <QElapsedTimer>
#include "source/Compiler/token.h"

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
    // Base types allowed for the current system
    QStringList m_allowedBaseTypes = QStringList() <<"BYTE"<<"ADDRESS"<<"INTEGER"<<"POINTER"<<"ARRAY"<<"BOOLEAN"<<"STRING";
    QStringList m_renameVariables; // Disallowed variables for a given system - will be replaced with a prefix
    QString m_renamedVariablePrefix = "varPrefixed_";
    QElapsedTimer timer;
    QString m_orgOutput;
    QString m_alternateMethods="";
    bool m_ignoreSys = false;
    bool m_stripPrg = false;
    bool m_canRunAsmFiles = false;

    virtual TokenType::Type getSystemPointerArrayType() {
        return TokenType::POINTER;
    }
    QMap<int,int> m_addresses;
    bool m_hasVariableColorPalette = false;
    bool m_supportsExomizer = false;
    bool m_allowClasses = false;
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


    virtual QString getArkosTrackerSystemString() {return "";}
    virtual void InitSystemPreprocessors(QMap<QString, QString>& defines)  {};
    virtual QString CompressFile(QString fileName) {
        return fileName;
    }
    virtual QStringList CompressData(QStringList& inData, QString& string) {
        return QStringList();
    }
    QSharedPointer<Orgasm> m_orgAsm = nullptr;

    virtual QString getCPUFlavor() {
        return "";
    }
    int m_memoryType = 0;
    QVector<SystemLabel> m_labels;
    int m_memorySize = 65536;
    static QMap<SystemLabel::Type, QColor> m_labelColors;
    QColor m_systemColor = QColor(64,64,128);

    QStringList m_allowedGlobalTypeFlags;
    QStringList m_allowedProcedureTypeFlags;

    bool m_requireEmulatorWorkingDirectory = false;

    virtual bool is8bit() {
        return true;
    }

    virtual bool isCommodoreSystem()  {return false;}
    virtual int getDefaultBasicAddress() { return 0;}

    virtual bool is486() { return false;}
    virtual bool is386() { return false;}
    virtual bool is286() { return false;}
    virtual bool is8088() { return false;}
    virtual bool is6809() { return false;}
    virtual int getPointerSize() {
        return 2;
    }
    virtual TokenType::Type getPointerType() { return TokenType::Type::INTEGER;}

    virtual bool AllowPointerInStructs() {return true;}




    void StartProcess(QString file, QStringList params, QString& output, bool standardOutput = false, QString currentDir = "");

    static void InitLabelColors();
 //   virtual bool hasFixedProgramAddress() {return true;}

    enum System {C64, VIC20, PET, NES, C128, BBCM, AMIGA, PLUS4, OK64, X16,X86, GAMEBOY, SPECTRUM, TIKI100, ATARI2600, ATARI520ST, AMSTRADCPC, COLECO, MEGA65, ATARI800, MSX, APPLEII, M1ARM, ORIC, SNES, CUSTOM, VZ200, ACORN, JDH8, POKEMONMINI, TRS80, TRS80COCO};
    enum Processor {MOS6502, M68000,PX86, GBZ80, Z80, ARM, WDC65C816, WDC65C02, PJDH8, S1C88, M6809};

    static QString StringFromProcessor(Processor s);
    static Processor ProcessorFromString(QString s);
    virtual bool CL65Syntax() {return false;}
    static QString StringFromProcessor(QString s);


    virtual int addressBusBits() {
        return 16;
    }

    virtual QString getEmulatorName() = 0;
    virtual void applyEmulatorParameters(QStringList& params, QString debugFile, QString baseFile, CIniFile* pini) = 0;

    virtual bool useZeroPagePointers() { return false;}

    virtual QString getTripePointerType() { return "uint64";}

    virtual void AssembleTripe(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab);

    bool m_buildSuccess;
/*    static bool isSupported(System currentSystem, QString list) {
        return isSupported(StringFromSystem(currentSystem), list);
    }
    static bool isSupported(QString currentSystem, QString list);
*/
    static System SystemFromString(QString s);

    static QString StringFromSystem(System s);

    bool isWDC65();


    bool systemIsOfType(QString val);
    bool systemIsOfType(QStringList val);


    void AssembleOrgasm(QString& output,QString& text, QString filename, QString currentDir, QSharedPointer<SymbolTable>  symTab);
    void AssembleZOrgasm(QString& output,QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab);
    void AssembleCL65(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab, QString ending, QStringList params);

    virtual QString getCPUAssemblerString() {
        return "";
    }

    virtual int getCPUFlavorint() {
        return 0;
    }

    virtual bool isCustom() { return false; }
    System m_system = C64;
    Processor m_processor = MOS6502;

    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) {}
    virtual void PostProcess(QString& text, QString file, QString currentDir) {}
    virtual void DefaultValues() {}

    bool GenericAssemble(QString assembler, QStringList parameters, QString error, QString& output, QString workingDir="");
    bool useZByte = false;

//    virtual void Execute(QString filename, QString currentDir);
public:
signals:
    void EmitTick(QString val);
public slots:
    void AcceptDispatcherTick(QString val);

};



#endif // ABSTRACTSYSTEM_H
