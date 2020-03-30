#ifndef METHODSX86_H
#define METHODSX86_H

#include "abstractastdispatcher.h"
#include "astdispatcherx86.h"
//#include "astdispatcher68000.h"
#include "source/Compiler/ast/nodebuiltinmethod.h"
#include "source/Compiler/ast/nodenumber.h"
#include "source/Compiler/ast/nodeprocedure.h"

class MethodsX86
{
public:
    MethodsX86();
    AbstractASTDispatcher* m_dispatcher;
    QSharedPointer<NodeBuiltinMethod> m_node = nullptr;


    void Assemble(Assembler* as, AbstractASTDispatcher* dispenser);




    bool Command(QString name);

    void LoadVar(Assembler *as, int paramNo);
    void LoadAddress(Assembler *as, int paramNo, bool isSource);

    void ClearScreen(Assembler* as, int bits);

    void WaitForVerticalBlank(Assembler* as);

    void ScrollX(Assembler* as);

    void AddInitMethod(Assembler* as, QString name, QString file);

    void MemCpy(Assembler* as, QString type);

    void InitPlaynote(Assembler* as);

};

#endif // METHODSX86_H
