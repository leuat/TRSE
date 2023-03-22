#ifndef NODECASE_H
#define NODECASE_H

#include "node.h"
#include "nodevar.h"
/* 
    Case node. Handles case blocks such as
    case val :
        1 : DoSomething();
        b : val:=0;
    else
       doSomethingElse();
    end;

    m_left: unused
    m_right: unused
    m_op: unused
    m_variable : variable to be tested
    m_coniditionals : (val = 1), (val = b) etc
    m_statements : the individual statements for each of the cases
    m_elseblock : executed if none of the conditionals are true
*/
class NodeCase : public Node
{
public:
    NodeCase(Token t);
    QSharedPointer<Node> m_variable = nullptr;
    QVector<QSharedPointer<Node>> m_conditionals;
    QVector<QSharedPointer<Node>> m_statements;
    QSharedPointer<Node> m_elseBlock = nullptr;

    void Append(QSharedPointer<Node> c, QSharedPointer<Node> b) {
        m_conditionals.append(c);
        m_statements.append(b);
    }
    void ReplaceVariable(Assembler* as, QString name, QSharedPointer<Node> node) override {
        Node::ReplaceVariable(as,name,node);
        for (auto p:m_conditionals)
            p->ReplaceVariable(as,name,node);
        for (auto p:m_statements)
            p->ReplaceVariable(as,name,node);
        if (m_elseBlock!=nullptr)
            m_elseBlock->ReplaceVariable(as,name,node);
    }

    void FindPotentialSymbolsInAsmCode(QStringList& lst)  override {
        for (auto p:m_statements)
            p->FindPotentialSymbolsInAsmCode(lst);
        if (m_elseBlock!=nullptr)
            m_elseBlock->FindPotentialSymbolsInAsmCode(lst);

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
