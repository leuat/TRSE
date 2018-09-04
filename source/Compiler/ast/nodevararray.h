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

    NodeVarArray(Token token, Node* expr);

    PVar Execute(SymbolTable* symTab, uint lvl) override;

    QString Build(Assembler *as) override;

    void StoreAcc(Assembler* as);


    void ExecuteSym(SymbolTable* symTab);


};

#endif // NODEVARARRAY_H
