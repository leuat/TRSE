#ifndef SYSTEM65C816_H
#define SYSTEM65C816_H

#include "source/Compiler/systems/abstractsystem.h"
#include "source/Compiler/misc/machinecodeanalyzer.h"
#include <QMessageBox>
#include <QProcess>
#include "source/messages.h"



class System65C816 : public AbstractSystem
{
public:
    System65C816(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);

    MachineCodeAnalyzer m_mca;


    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;
    virtual void PostProcess(QString& text, QString file, QString currentDir) override;

    virtual bool CL65Syntax() override {return true;}

    virtual bool AllowPointerInStructs() override {return false;}


    void DefaultValues() override;

    QString CompressFile(QString fileName) override
    {
        return CompressLZ4(fileName);
    }
    virtual int addressBusBits() override {
        return 24;
    }
    virtual QString getCPUAssemblerString() override {
        return ".p4510   ; 65816 processor";

    }

};

#endif // SYSTEM65C816_H
