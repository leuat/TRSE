#include "nodeunaryop.h"


NodeUnaryOp::NodeUnaryOp(Token t, Node *right):Node() {
    m_op = t;
    m_right = right;
    m_left = nullptr;
}

PVar NodeUnaryOp::Execute(SymbolTable *symTab, uint lvl) {
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
    level = lvl+1;
    ErrorHandler::e.DebugLow("Calling Unary Op Node",level);

    if (m_op.m_type==TokenType::PLUS)
        return m_right->Execute(symTab, level)*1;
    if (m_op.m_type==TokenType::MINUS)
        return m_right->Execute(symTab, level)*-1;
    return PVar();

}

bool NodeUnaryOp::isMinusOne() {
    NodeNumber* num = dynamic_cast<NodeNumber*>(m_right);
    if (num==nullptr)
        return false;

    if (num->m_val==1 || m_op.m_type==TokenType::MINUS)
        return true;

    return false;

}

QString NodeUnaryOp::Build(Assembler *as) {

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
