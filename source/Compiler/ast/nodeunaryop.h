#ifndef NODEUNARYOP_H
#define NODEUNARYOP_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodenumber.h"
#include "source/Compiler/ast/nodevar.h"
#include "source/Compiler/ast/nodebinop.h"

class NodeUnaryOp : public Node {
public:
    NodeUnaryOp(Token t, Node* right) {
        m_op = t;
        m_right = right;
        m_left = nullptr;
        qDebug() << "NU:";
    }
    PVar Execute(SymbolTable* symTab, uint lvl) override {
        Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
        level = lvl+1;
        ErrorHandler::e.DebugLow("Calling Unary Op Node",level);

        if (m_op.m_type==TokenType::PLUS)
            return m_right->Execute(symTab, level)*1;
        if (m_op.m_type==TokenType::MINUS)
            return m_right->Execute(symTab, level)*-1;
        return PVar();

    }


    bool isMinusOne() override {
        NodeNumber* num = dynamic_cast<NodeNumber*>(m_right);
        if (num==nullptr)
            return false;

        if (num->m_val==1 || m_op.m_type==TokenType::MINUS)
            return true;

        return false;

    }

    QString Build(Assembler* as) {

        NodeNumber* num = dynamic_cast<NodeNumber*>(m_right);

        if (num!=nullptr) {
            int s = num->m_val;

            if (m_op.m_type==TokenType::MINUS) {
                if (num->m_op.m_type<256)
                    num->m_val=256-num->m_val;
                else
                    if (num->m_op.m_type<65536)
                        num->m_val=65536-num->m_val;

            }
            QString ss= num->Build(as);
            num->m_val = s;
            return ss;

        }


        return "";
    }


    void ExecuteSym(SymbolTable* symTab) override {
        m_right->ExecuteSym(symTab);
    }

};

#endif // NODEUNARYOP_H
