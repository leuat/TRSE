#ifndef NODECASE_H
#define NODECASE_H

#include "node.h"
#include "nodevar.h"
class NodeCase : public Node
{
public:
    NodeCase(Token t);
    NodeVar* m_variable = nullptr;
    QVector<Node*> m_conditionals;
    QVector<Node*> m_statements;
    Node* m_elseBlock = nullptr;

    void Append(Node* c, Node* b) {
        m_conditionals.append(c);
        m_statements.append(b);
    }

    void ExecuteSym(SymbolTable* symTab) override {
        m_variable->ExecuteSym(symTab);
        for (Node* n: m_statements)
            n->ExecuteSym(symTab);
        if (m_elseBlock!=nullptr)
           m_elseBlock->ExecuteSym(symTab);
    }
    void Accept(AbstractASTDispatcher* dispatcher) override {
        dispatcher->dispatch(this);
    }

};

#endif // NODECASE_H
