#ifndef NODEVARARRAY_H
#define NODEVARARRAY_H

#include "node.h"
#include "source/Compiler/symboltable.h"
#include "nodevar.h"

class NodeVarArray : public Node
{
public:
    NodeVar* m_var;
    Node* m_expr;

    NodeVarArray(Token token, Node* expr) {
        m_var = new NodeVar(token);
        m_expr = expr;
    }

    PVar Execute(SymbolTable* symTab, uint lvl) override {
        return PVar();
    }

    QString Build(Assembler *as) override {
        return "";

        m_expr->Build(as);
        as->Asm("tax");
        as->Term("lda ");
        m_var->Build(as);
        as->Term(",x",true);
        qDebug() << "end";
        return "";

    }

    void StoreAcc(Assembler* as) {
        as->Asm("tay");
        m_expr->Build(as);
        as->Asm("tax");
        as->Asm("tya");
        as->Term("sta ");
        m_var->Build(as);
        as->Term(",x",true);



    }


    void ExecuteSym(SymbolTable* symTab) {
        if (m_var!=nullptr)
            m_var->ExecuteSym(symTab);
        if (m_expr!=nullptr)
            m_expr->ExecuteSym(symTab);

    }


};

#endif // NODEVARARRAY_H
