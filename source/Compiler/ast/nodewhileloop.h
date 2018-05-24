#ifndef NODEWHILELOOP_H
#define NODEWHILELOOP_H

#include "source/Compiler/ast/node.h"

class NodeWhileLoop : public Node
{
public:
    NodeWhileLoop();
    Node* m_block = nullptr;
    NodeWhileLoop(Node* block) {
        m_block = block;
    }
    void Delete() override {
        if (m_block) {
            m_block->Delete();
            delete m_block;
            m_block = nullptr;
        }


    }

    PVar Execute(SymbolTable* symTab, uint lvl) override {return PVar();}

    void ExecuteSym(SymbolTable* symTab) override {
       m_block->ExecuteSym(symTab);
    }


};

#endif // NODEWHILELOOP_H
