#ifndef NODEASM_H
#define NODEASM_H

#include "node.h"

class NodeAsm : public Node
{
public:
    QString m_asm;


    NodeAsm(Token t):Node() {

        m_asm = t.m_value;
        m_op = t;
    }

    PVar Execute(SymbolTable* symTab, uint lvl) override {
        return PVar();
    }

    QString Build(Assembler *as) override;
    void ExecuteSym(SymbolTable* symTab) override {

    }
};

#endif // NODEASM_H
