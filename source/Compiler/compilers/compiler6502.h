#ifndef COMPILER6502_H
#define COMPILER6502_H

#include "compiler.h"
#include "source/OrgAsm/orgasm.h"

class Compiler6502 : public Compiler
{
public:
    Compiler6502(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni) : Compiler(ini, pIni) {};

    void InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system) override;

    void Connect() override;

    void CleanupCycleLinenumbers(QString currentFile, QMap<int,int>& ocycles, QMap<int,int>& retcycles, bool isCycles=true ) override;

    void Init6502Assembler();

    bool SetupMemoryAnalyzer(QString filename, Orgasm* orgAsm = nullptr) override;


    int FindEndSymbol(Orgasm& orgasm);

    QVector<int> FindBlockEndSymbols(Orgasm& orgasm);

    void ConnectBlockSymbols(QVector<int>& blockEndSymbols);

};

#endif // COMPILER6502_H
