#ifndef COMPILERARM_H
#define COMPILERARM_H

#include "compiler.h"

class CompilerArm : public Compiler
{
public:
    CompilerArm(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni) : Compiler(ini, pIni) {};

    void InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system) override;

    void Connect() override;

    bool SetupMemoryAnalyzer(QString filename, Orgasm* orgAsm = nullptr) override;
};

#endif // COMPILERARM_H
