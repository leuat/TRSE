#ifndef SYSTEMMOS6502_H
#define SYSTEMMOS6502_H

#include "source/Compiler/systems/abstractsystem.h"
#include "source/Compiler/misc/machinecodeanalyzer.h"
#include <QMessageBox>
#include "source/messages.h"


class SystemMOS6502 : public AbstractSystem
{
public:
    SystemMOS6502(CIniFile* settings, CIniFile* proj) : AbstractSystem(settings, proj) {

    }

    bool m_ignoreSys = false;
    bool m_stripPrg = false;
    MachineCodeAnalyzer m_mca;


    virtual void Assemble(QString& text, QString file, QString currentDir, SymbolTable* symTab);
    virtual void PostProcess(QString& text, QString file, QString currentDir);


    bool VerifyMachineCodeZP(QString fname);

    void CreateDisk(QString currentDir, QString filename, QString iniData, bool addPrg);
    bool BuildDiskFiles(QString currentDir, QStringList& d64Params, QString iniData);

    void DefaultValues() override;


};

#endif // SYSTEMMOS6502_H
