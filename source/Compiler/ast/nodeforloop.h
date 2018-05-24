#ifndef NODEFORLOOP_H
#define NODEFORLOOP_H

#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodeassign.h"
#include "source/Compiler/ast/nodevar.h"


class NodeForLoop : public Node {
public:

    Node* m_a, *m_b;
    Node* m_block;
    int m_forcePage;
    bool m_unroll = false;
    Node* m_step = nullptr;
    int m_loopCounter=0;

    NodeForLoop(Node* a, Node* b, Node* block, Node* step, bool unroll, int forcePage, int loopCounter) {
        m_a = a;
        m_b = b;
        m_block = block;
        m_forcePage = forcePage;
        m_step = step;
        m_loopCounter = loopCounter;
//        m_op = op;
    }


    void Compare(Assembler* as);

    void IncreaseCounter(Assembler* as);


    PVar Execute(SymbolTable* symTab, uint lvl) override;
    void Delete() override {
        if (m_a) {
            m_a->Delete();
            delete m_a;
            m_a = nullptr;
        }
        if (m_b) {
            m_b->Delete();
            delete m_b;
            m_b = nullptr;
        }
        if (m_block) {
            m_block->Delete();
            delete m_block;
            m_block = nullptr;
        }


    }
    void ExecuteSym(SymbolTable* symTab) override {
        m_block->ExecuteSym(symTab);
    }


    void LargeLoop(Assembler* as);
    void SmallLoop(Assembler* as);

    QString Build(Assembler *as) override;

};
#endif // NODEFORLOOP_H
