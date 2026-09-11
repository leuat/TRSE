#ifndef ABSTRACTSYSTEM_H
#define ABSTRACTSYSTEM_H

#include <QFile>
#include <QHash>
#include <QString>
#include <QElapsedTimer>
#include "source/Compiler/token.h"
#include "source/LeLib/util/cinifile.h"

// forwarded classes
class Orgasm;

class SymbolTable;

class Assembler;

// label class (for memory analyser)
class SystemLabel {
public:
  enum Type {
    SCREEN,
    SID,
    REGISTERS,
    ZEROPAGE,
    STACK,
    BASIC,
    FREE,
    VIC,
    COLOUR,
    IO,
    KERNAL,
    UNAVAILABLE,
    ROM
  };
  Type m_type;
  SystemLabel() {}
  QString m_name;
  int m_from, m_to;
  SystemLabel(Type t, QString name, int from, int to) {
    m_type = t;
    m_name = name;
    m_from = from;
    m_to = to;
  }
};

/*
 *  Mother of all system classes. Every system inherits from these class,
 * through the CPU type. Ie, systemmos6502 inherits from this project, and
 * systemC64 inherits from systemmos6502
 *
 */

class AbstractSystem : public QObject {
  Q_OBJECT
public:
  AbstractSystem(QSharedPointer<CIniFile> settings,
                 QSharedPointer<CIniFile> proj);
  AbstractSystem();
  AbstractSystem(AbstractSystem *a);

  // All the different systems and CPUs that TRSE supports
  enum System {
    C64,
    VIC20,
    PET,
    NES,
    C128,
    BBCM,
    AMIGA,
    PLUS4,
    OK64,
    X16,
    X86,
    GAMEBOY,
    SPECTRUM,
    TIKI100,
    ATARI2600,
    ATARI520ST,
    AMSTRADCPC,
    COLECO,
    MEGA65,
    ATARI800,
    MSX,
    APPLEII,
    M1ARM,
    ORIC,
    SNES,
    CUSTOM,
    VZ200,
    ACORN,
    JDH8,
    POKEMONMINI,
    TRS80,
    TRS80COCO,
    WONDERSWAN,
    TIM,
    TVC,
    VECTREX,
    THOMSON,
    CHIP8,
    PCW,
    SCHIP,
    XO_CHIP,
    CHIP8X,
    BK0010,
    DRAGON,
    FOENIX,
    AGON,
    PRIMO
  };
  enum Processor {
    MOS6502,
    M68000,
    PX86,
    GBZ80,
    Z80,
    ARM,
    WDC65C816,
    WDC65C02,
    PJDH8,
    S1C88,
    M6809,
    Z180,
    PCHIP8,
    PDP11
  };

  System m_system = C64;           // default is C64
  Processor m_processor = MOS6502; // default is 6502

  // Sets default values for each system (such as program start address)
  virtual void DefaultValues() {}

  // did we happy?
  bool m_buildSuccess;

  // list of registers for the current system (automatic variable renaming)
  QStringList m_registers;
  static QHash<QString, QString> m_systemParams;
  QHash<int, int> m_addresses;
  QSharedPointer<CIniFile> m_projectIni, m_settingsIni;
  QVector<SystemLabel> m_labels;
  static QHash<SystemLabel::Type, QColor> m_labelColors;
  // Base types allowed for the current system. Overidden by system constructors
  QStringList m_allowedBaseTypes =
      QStringList() << "BYTE" << "ADDRESS" << "INTEGER" << "POINTER" << "ARRAY"
                    << "BOOLEAN" << "STRING";
  QStringList m_renameVariables; // Disallowed variables for a given system -
                                 // will be replaced with a prefix
  QString m_renamedVariablePrefix = "varPrefixed_";
  QElapsedTimer timer;
  QString m_orgOutput;
  QString m_alternateMethods = "";

  // memory/init pos
  bool m_ignoreSys = false;
  bool m_stripPrg = false;
  bool m_canRunAsmFiles = false;
  bool m_supportsInclusiveFor = true;
  bool m_useOctals = false;

  int m_startAddress = 0x800;
  int m_programStartAddress = m_startAddress + 10;
  int m_ramAddress = 0;
  // only used for the vic
  int m_memoryType = 0;
  int m_memorySize = 65536;
  // Some systems can have user-defined palettes, others are fixed
  bool m_hasVariableColorPalette = false;
  bool m_supportsExomizer = false;
  // Only some cpu architectures supports classes (yet)
  bool m_allowClasses = false;
  QString m_exomizerName = "Exomize";
  // Default system color. changed for each system
  QColor m_systemColor = QColor(64, 64, 128);

  QStringList m_allowedGlobalTypeFlags;
  QStringList m_allowedProcedureTypeFlags;
  // Does the emulator require a working directory?
  bool m_requireEmulatorWorkingDirectory = false;
  bool m_isBigEndian = false;
  bool m_usesRom = false;
  bool useZByte = false;
  // Need a copy of orgasm for various z80 shenanigans
  QSharedPointer<Orgasm> m_orgAsm = nullptr;

  // emulator stuff
  virtual QString getEmulatorName() = 0;
  virtual void applyEmulatorParameters(QStringList &params, QString debugFile,
                                       QString baseFile, CIniFile *pini) = 0;
  QStringList ApplyDefaultMameParams();
  virtual void ExtraEmulatorCommands() {}

  // Global compression methods
  QString CompressLZ4(QString fileName);
  QString CompressLZ4(QString fileName, QString outFileName);
  QString CompressZX0(QString fileName);
  QString CompressZX0(QString fileName, QString outFileName);
  virtual QString CompressFile(QString fileName);
  virtual QStringList CompressData(QStringList &inData, QString &string);

  // Disk methods
  bool CreateDiskInternal(QString currentDir, QString disk, QString filename,
                          bool addPrg, QString &text, int track);
  bool BuildDiskFilesCC1541(QString currentDir, QStringList &d64Params,
                            QString iniData, QString &text);
  void ApplyDirArt(QString currentDir, QString dirart, QString disk,
                   QString &text);

  virtual QString getArkosTrackerSystemString() { return ""; }

  virtual void InitSystemPreprocessors(QHash<QString, QString> &defines) {};
  // Different CPUS come in various flavors, like the Z80 vs the gameboy Z80
  virtual QString getCPUFlavor() { return ""; }

  // various "is"-methods for handy codegen
  virtual bool is8bit() { return true; }

  virtual bool isCommodoreSystem() { return false; }
  virtual int getDefaultBasicAddress() { return 0; }

  virtual bool is186() { return false; }
  virtual bool is486() { return false; }
  virtual bool is386() { return false; }
  virtual bool is286() { return false; }
  virtual bool is8088() { return false; }
  virtual bool is6809() { return false; }
  virtual bool isZ80() { return false; }
  virtual bool isCustom() { return false; }
  // used for z80 codegen
  bool isGB() { return m_system == AbstractSystem::GAMEBOY; }
  bool isWDC65() { return m_processor == WDC65C816; }
  bool iseZ80() { return m_system == AGON; }


  // Default pointer size is integer
  virtual int getPointerSize() { return 2; }
  // Different systems have different pointers, ie 16, 24 or 32 bit pointers
  virtual TokenType::Type getPointerType() { return TokenType::Type::INTEGER; }
  virtual TokenType::Type getSystemPointerArrayType();

  virtual bool AllowPointerInStructs() { return true; }

  virtual void PrepareInitialAssembler(Assembler *as) {}

  virtual bool CL65Syntax() { return false; }
  // starts process (such as calling external disk creation or assemblers)
  void StartProcess(QString file, QStringList params, QString &output,
                    bool standardOutput = false, QString currentDir = "");

  // set up specific colors for the memory analyse for each system
  static void InitLabelColors();
  // string 2 enums
  static QString StringFromProcessor(Processor s);
  static Processor ProcessorFromString(QString s);
  static QString StringFromProcessor(QString s);
  static System SystemFromString(QString s);
  static QString StringFromSystem(System s);
  bool systemIsOfType(QString val);
  bool systemIsOfType(QStringList val);

  virtual int addressBusBits() { return 16; }

  virtual bool HasCycleCounter() { return false; }
  virtual bool HasAddressCounter() { return false; }

  virtual bool useZeroPagePointers() { return false; }

  virtual QString getTripePointerType() { return "uint64"; }

  virtual void AssembleTripe(QString &text, QString file, QString currentDir,
                             QSharedPointer<SymbolTable> symTab);

  virtual QString getCPUAssemblerString() { return ""; }

  virtual int getCPUFlavorint() { return 0; }

  // Assembling
  void AssembleOrgasm(QString &output, QString &text, QString filename,
                      QString currentDir, QSharedPointer<SymbolTable> symTab);
  void AssembleZOrgasm(QString &output, QString &text, QString filename,
                       QString currentDir, QSharedPointer<SymbolTable> symTab,
                       int orgType = 0);
  void AssembleCL65(QString &text, QString filename, QString currentDir,
                    QSharedPointer<SymbolTable> symTab, QString ending,
                    QStringList params);

  virtual void Assemble(QString &text, QString file, QString currentDir,
                        QSharedPointer<SymbolTable> symTab) {}
  virtual void PostProcess(QString &text, QString file, QString currentDir) {}

  void Sparkle(QString &text, QString filename, QString currentDir);
  bool GenericAssemble(QString assembler, QStringList parameters, QString error,
                       QString &output, QString workingDir = "");

  virtual QStringList AnalyseForPotentialVariables(QString asmCode);
  virtual QStringList Wash(QString s);

public:
signals:
  void EmitTick(QString val);
public slots:
  void AcceptDispatcherTick(QString val);
};

#endif // ABSTRACTSYSTEM_H
