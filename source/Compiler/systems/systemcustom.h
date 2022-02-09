#ifndef SYSTEMCUSTOM_H
#define SYSTEMCUSTOM_H

#include "abstractsystem.h"


class SystemCustom : public AbstractSystem
{
public:
    SystemCustom(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);
    bool isCustom() override { return true; }

    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;
    virtual void PostProcess(QString& text, QString file, QString currentDir) override;
    virtual bool CL65Syntax() override;

    virtual QString getEmulatorName() override {
        return "";
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
        QString p = pini->getString("custom_system_emulator_parameters").trimmed().simplified().replace("@prg",filename);
        params << p.split(" ");
    }
    virtual int addressBusBits() override;


    QString getCPUAssemblerString();

    void PrepareInitialAssembler(Assembler* as) override;

};
#endif // SYSTEMCUSTOM_H
