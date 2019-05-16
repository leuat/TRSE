#ifndef METHODS68000_H
#define METHODS68000_H

#include "abstractastdispatcher.h"
#include "source/Compiler/ast/nodebuiltinmethod.h"

class Methods68000
{
public:
    Methods68000();
    AbstractASTDispatcher* m_dispatcher;
    NodeBuiltinMethod* m_node = nullptr;

    void Assemble(Assembler* as, AbstractASTDispatcher* dispenser);


    bool Command(QString name);
    void Asm(Assembler* as, QString cmd, QString a, QString b) {
        as->Asm(cmd + " " + a +","+b);
    }

    void Poke(Assembler* as, QString bb);
    void SetCopperList32(Assembler* as);
};

#endif // METHODS68000_H
