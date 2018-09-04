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
    void Delete() override;

    PVar Execute(SymbolTable* symTab, uint lvl) override {return PVar();}

    void ExecuteSym(SymbolTable* symTab) override;


};

#endif // NODEWHILELOOP_H
