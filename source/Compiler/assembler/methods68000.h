#ifndef METHODS68000_H
#define METHODS68000_H

#include "abstractastdispatcher.h"
//#include "astdispatcher68000.h"
#include "source/Compiler/ast/nodebuiltinmethod.h"
#include "source/Compiler/ast/nodenumber.h"

class Methods68000
{
public:
    Methods68000();
    AbstractASTDispatcher* m_dispatcher;
    NodeBuiltinMethod* m_node = nullptr;

    void Assemble(Assembler* as, AbstractASTDispatcher* dispenser);


    bool Command(QString name);
    void Asm(Assembler* as, QString cmd, QString a, QString b, Node* n) {
        as->Asm(cmd + m_dispatcher->getEndType(as,n) + " " + a +","+b);
    }
    void Asm(Assembler* as, QString cmd, QString a, QString b) {
        as->Asm(cmd + " " + a +","+b);
    }

    void LoadVariable(Assembler* as, QString cmd, Node* n, QString d0);
    void LoadAddress(Assembler* as, Node* n, QString d0);

    void Poke(Assembler* as, QString bb);
    void SetCopperList32(Assembler* as);
    void Memcpy(Assembler *as);
    void Setpalette(Assembler *as);
    void ABlit(Assembler* as);
    void AddCopperCommand(Assembler* as);
    void SkipCopperCommands(Assembler* as);
};

#endif // METHODS68000_H
