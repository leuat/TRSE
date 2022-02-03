#ifndef COMPILER6809_H
#define COMPILER6809_H

#include "compiler.h"

class Compiler6809 : public Compiler
{
public:
    Compiler6809(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni) : Compiler(ini, pIni) {};

    void InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system) override;

    void Connect() override;

    void CleanupCycleLinenumbers(QString currentFile, QMap<int,int>& ocycles, QMap<int,int>& retcycles, bool isCycles=true ) override;

    virtual void Init6809Assembler();




    int FindEndSymbol(Orgasm& orgasm);

    QVector<int> FindBlockEndSymbols(Orgasm& orgasm);

    void ConnectBlockSymbols(QVector<int>& blockEndSymbols);

};


#endif // COMPILER6809_H
