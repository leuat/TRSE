#ifndef NODECONTROLSTATEMENT_H
#define NODECONTROLSTATEMENT_H

#include "source/Compiler/ast/node.h"

/* 
    Control statements in for loops, ie "break", "continue" and "return
    m_left: not used
    m_right: not used
    m_op: control type: BREAK, CONTINUE or RETURN
*/
class NodeControlStatement : public Node
{
public:
    enum Type {BREAK, CONTINUE};

    NodeControlStatement(Token op);


    void parseConstants(QSharedPointer<SymbolTable>  symTab) override {
    }


    void ExecuteSym(QSharedPointer<SymbolTable>  symTab) override {
        m_right->ExecuteSym(symTab);
    }
    void Accept(AbstractCodeGen* dispatcher) override {
        dispatcher->dispatch(qSharedPointerDynamicCast<NodeControlStatement>(sharedFromThis()));
    }


};

#endif // NODECONTROLSTATEMENT_H
