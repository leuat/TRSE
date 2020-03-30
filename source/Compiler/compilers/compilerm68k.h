#ifndef COMPILERM68K_H
#define COMPILERM68K_H

#include "compiler.h"

class CompilerM68K : public Compiler
{
public:
    CompilerM68K(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni) : Compiler(ini, pIni) {};
    void InitAssemblerAndDispatcher(QSharedPointer<AbstractSystem> system) override;

    void Connect() override;


};

#endif // COMPILERM68K_H
