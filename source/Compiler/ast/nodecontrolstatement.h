#ifndef NODECONTROLSTATEMENT_H
#define NODECONTROLSTATEMENT_H

#include "source/Compiler/ast/node.h"

/* 
    ?
    m_left: ?
    m_right: ?
    m_op: ?
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
