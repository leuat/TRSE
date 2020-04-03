#ifndef COMPILERX86_H
#define COMPILERX86_H

#include "compiler.h"

class CompilerX86 : public Compiler
{
public:
    CompilerX86(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni) : Compiler(ini, pIni) {};

    void InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system) override;

    void Connect() override;

};

#endif // COMPILERX86_H
