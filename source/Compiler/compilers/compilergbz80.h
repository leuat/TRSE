#ifndef COMPILERGBZ80_H
#define COMPILERGBZ80_H

#include "compiler.h"

class CompilerGBZ80 : public Compiler
{
public:
    CompilerGBZ80(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni) : Compiler(ini, pIni) {};

    void InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system) override;

    void Connect() override;

    void SetupMemoryAnalyzer(QString filename) override;

};

#endif // COMPILERX86_H
