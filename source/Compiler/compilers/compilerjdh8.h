#ifndef COMPILERJDH8_H
#define COMPILERJDH8_H

#include "compiler.h"
#include "source/Compiler/assembler/asmjdh8.h"

class CompilerJDH8 : public Compiler
{
public:
    CompilerJDH8(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni);

    void InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system) override;

    void Connect() override;

    bool SetupMemoryAnalyzer(QString filename, Orgasm* orgAsm = nullptr) override;




};

#endif // COMPILERJDH8_H
