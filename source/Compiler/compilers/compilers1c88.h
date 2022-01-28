#ifndef COMPILERS1C88_H
#define COMPILERS1C88_H

#include "compiler.h"

class CompilerS1C88 : public Compiler
{
public:
    CompilerS1C88(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni) : Compiler(ini, pIni) {};

    void InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system) override;

    void Connect() override;

    bool SetupMemoryAnalyzer(QString filename, Orgasm* orgAsm = nullptr) override;



};

#endif
