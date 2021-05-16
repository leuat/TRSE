#ifndef NODECASE_H
#define NODECASE_H

#include "node.h"
#include "nodevar.h"
class NodeCase : public Node
{
public:
    NodeCase(Token t);
    QSharedPointer<NodeVar> m_variable = nullptr;
    QVector<QSharedPointer<Node>> m_conditionals;
    QVector<QSharedPointer<Node>> m_statements;
    QSharedPointer<Node> m_elseBlock = nullptr;

    void Append(QSharedPointer<Node> c, QSharedPointer<Node> b) {
        m_conditionals.append(c);
        m_statements.append(b);
    }


    void ExecuteSym(QSharedPointer<SymbolTable>  symTab) override {
        m_variable->ExecuteSym(symTab);
        for (QSharedPointer<Node> n: m_statements)
            n->ExecuteSym(symTab);
        if (m_elseBlock!=nullptr)
           m_elseBlock->ExecuteSym(symTab);
    }
    void Accept(AbstractCodeGen* dispatcher) override {
        dispatcher->dispatch(qSharedPointerDynamicCast<NodeCase>(sharedFromThis()));
    }
    void parseConstants(QSharedPointer<SymbolTable>  symTab) override {
        if (m_variable!=nullptr)
            m_variable->parseConstants(symTab);
        for (QSharedPointer<Node> n:m_conditionals)
            n->parseConstants(symTab);
        for (QSharedPointer<Node> n: m_statements)
            n->parseConstants(symTab);
    }


};

#endif // NODECASE_H
