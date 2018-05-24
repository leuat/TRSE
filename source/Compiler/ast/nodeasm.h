#ifndef NODEASM_H
#define NODEASM_H

#include "node.h"

class NodeAsm : public Node
{
public:
    QString m_asm;


    NodeAsm(Token t) {
        m_asm = t.m_value;
        m_op = t;
    }

    PVar Execute(SymbolTable* symTab, uint lvl) override {
        return PVar();
    }

    QString Build(Assembler *as) override {
        QStringList txt = m_asm.split("\n");
        as->Comment("");
        as->Comment("****** Inline assembler section");
        for (QString t: txt) {
            as->Write(t,0);
        }
        as->Asm("");
        return "";
    }
    void ExecuteSym(SymbolTable* symTab) override {

    }
};

#endif // NODEASM_H
