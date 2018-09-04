#ifndef NODEUNARYOP_H
#define NODEUNARYOP_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodenumber.h"
#include "source/Compiler/ast/nodevar.h"
#include "source/Compiler/ast/nodebinop.h"

class NodeUnaryOp : public Node {
public:
    NodeUnaryOp(Token t, Node* right);
    PVar Execute(SymbolTable* symTab, uint lvl) override;


    bool isMinusOne() override;

    QString Build(Assembler* as) override;


    void ExecuteSym(SymbolTable* symTab) override {
        m_right->ExecuteSym(symTab);
    }

};

#endif // NODEUNARYOP_H
