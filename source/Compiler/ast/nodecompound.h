#ifndef NODECOMPOUND_H
#define NODECOMPOUND_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"

class NodeCompound : public Node {
public:
    QVector<Node*> children;
    NodeCompound(Token t):Node() {
        m_op = t;
    }
    PVar Execute(SymbolTable* symTab, uint lvl) override;
    void Delete() override;
    void ExecuteSym(SymbolTable* symTab) override;

    QString Build(Assembler* as) override;

};
#endif // NODECOMPOUND_H
