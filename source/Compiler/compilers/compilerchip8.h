#ifndef COMPILERCHIP8_H
#define COMPILERCHIP8_H

#include "compiler.h"
#include "source/Compiler/assembler/asmjdh8.h"
#include "source/Compiler/codegen/codegen_chip8.h"

class CompilerChip8 : public Compiler
{
public:
    CompilerChip8(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni);

    void InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system) override;

    void Connect() override;

    bool SetupMemoryAnalyzer(QString filename, Orgasm* orgAsm = nullptr) override;




};

#endif // COMPILERJDH8_H
