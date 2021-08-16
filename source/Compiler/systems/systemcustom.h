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

};
#endif // SYSTEMCUSTOM_H
