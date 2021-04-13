#ifndef ABSTRACTMETHODS_H
#define ABSTRACTMETHODS_H

#include <QSharedPointer>
#include "source/Compiler/ast/nodebuiltinmethod.h"



class AbstractMethods
{
public:
    AbstractMethods();


    AbstractASTDispatcher* m_dispatcher;
    QSharedPointer<NodeBuiltinMethod> m_node = nullptr;
    virtual void Assemble(Assembler *as, AbstractASTDispatcher *dispatcher) = 0;

    virtual bool Command(QString name) = 0;

    // Platform independent
    void IncDec(Assembler *as, QString cmd);


};

#endif // ABSTRACTMETHODS_H
