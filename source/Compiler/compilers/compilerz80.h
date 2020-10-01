#ifndef COMPILERZ80_H
#define COMPILERZ80_H

#include "compiler.h"

class CompilerZ80 : public Compiler
{
public:
    CompilerZ80(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni) : Compiler(ini, pIni) {};

    void InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system) override;

    void Connect() override;

    void SetupMemoryAnalyzer(QString filename) override;



};

#endif // COMPILERX86_H
