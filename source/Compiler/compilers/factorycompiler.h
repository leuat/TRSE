#ifndef FACTORYCOMPILER_H
#define FACTORYCOMPILER_H

#include "compiler6502.h"
#include "compiler65c816.h"
#include "compilerm68k.h"
#include "compilerx86.h"
#include "compilerz80.h"
#include "compilergbz80.h"
#include "compilerarm.h"

class FactoryCompiler
{
public:
    FactoryCompiler();
    static Compiler* CreateCompiler(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni);
};

#endif // FACTORYCOMPILER_H
