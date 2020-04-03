#ifndef COMPILER6502_H
#define COMPILER6502_H

#include "compiler.h"

class Compiler6502 : public Compiler
{
public:
    Compiler6502(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni) : Compiler(ini, pIni) {};

    void InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system) override;

    void Connect() override;

    void CleanupCycleLinenumbers(QString currentFile, QMap<int,int>& ocycles, QMap<int,int>& retcycles ) override;

    void Init6502Assembler();

};

#endif // COMPILER6502_H
