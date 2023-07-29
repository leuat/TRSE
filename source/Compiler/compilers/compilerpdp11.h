#ifndef COMPILERPDP11_H
#define COMPILERPDP11_H

#include "compiler.h"

class CompilerPDP11 : public Compiler
{
public:
    CompilerPDP11(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni) : Compiler(ini, pIni) {};
    void InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system) override;

    void Connect() override;


};

#endif // COMPILERM68K_H
