#ifndef ABSTRACTMETHODS_H
#define ABSTRACTMETHODS_H

#include <QSharedPointer>
#include "source/Compiler/ast/nodebuiltinmethod.h"



class AbstractMethods
{
public:
    AbstractMethods();


    AbstractCodeGen* m_codeGen;
    QSharedPointer<NodeBuiltinMethod> m_node = nullptr;
    virtual void Assemble(Assembler *as, AbstractCodeGen *dispatcher) = 0;

    virtual bool Command(QString name) = 0;

    // Platform independent
    void IncDec(Assembler *as, QString cmd);

    void Nop(Assembler* as) {
        if (!m_node->m_params[0]->isPureNumeric())
            ErrorHandler::e.Error("Nop() requires a pure numeric value.",m_node->m_op.m_lineNumber);
        int val = Util::NumberFromStringHex(m_node->m_params[0]->getValue(as).remove("#"));
        for (int i=0;i<val;i++)
            as->Asm("nop");
    }





};

#endif // ABSTRACTMETHODS_H
