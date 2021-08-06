#ifndef COMPILER65C816_H
#define COMPILER65C816_H

#include "compiler.h"
#include "compiler6502.h"
#include "source/OrgAsm/orgasm.h"

class Compiler65C816 : public Compiler6502
{
public:
    AbstractSystem::Processor m_processor;
    Compiler65C816(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni, AbstractSystem::Processor p) : Compiler6502(ini, pIni) {
        m_processor = p;
    };

    void InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system) override;
    void Init6502Assembler() override;

    void Connect() override;

    void ConnectBlockSymbols(QVector<int>& blockEndSymbols);

};


#endif // COMPILER65C816_H
