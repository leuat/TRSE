#ifndef COMPILERX86_H
#define COMPILERX86_H

#include "compiler.h"

class CompilerX86 : public Compiler
{
public:
    CompilerX86(CIniFile* ini, CIniFile* pIni) : Compiler(ini, pIni) {};

    void InitAssemblerAndDispatcher(AbstractSystem* system) override;

    void Connect() override;

};

#endif // COMPILERX86_H
