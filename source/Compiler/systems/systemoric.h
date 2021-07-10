#ifndef SYSTEMORIC_H
#define SYSTEMORIC_H

#include "systemmos6502.h"

class SystemOric : public SystemMOS6502
{
public:
    SystemOric(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) ;
    void DefaultValues() override;

    int getDefaultBasicAddress() override {
        return 0x500;
    };
    bool isCommodoreSystem() override  {return false;}

    void Assemble(QString& text, QString filename, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;




};

#endif // SYSTEMORIC_H
