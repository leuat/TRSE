#ifndef COMPILERGBZ80_H
#define COMPILERGBZ80_H

#include "compiler.h"

class CompilerGBZ80 : public Compiler
{
public:
    CompilerGBZ80(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni) : Compiler(ini, pIni) {};

    void InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system) override;

    void Connect() override;

    bool SetupMemoryAnalyzer(QString filename, Orgasm* orgAsm = nullptr) override;

};

#endif // COMPILERX86_H
