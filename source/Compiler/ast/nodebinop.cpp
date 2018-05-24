#include "nodebinop.h"


PVar NodeBinOP::Execute(SymbolTable *symTab, uint lvl) {
    level = lvl+1;
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
    ErrorHandler::e.DebugLow("Calling BinOP",level);

    if (m_op.m_type==TokenType::PLUS)
        return m_left->Execute(symTab, level) + m_right->Execute(symTab, level);
    if (m_op.m_type==TokenType::MINUS)
        return m_left->Execute(symTab, level) - m_right->Execute(symTab, level);

    if (m_op.m_type==TokenType::DIV)
        return m_left->Execute(symTab, level) / m_right->Execute(symTab, level);
    if (m_op.m_type==TokenType::MUL)
        return m_left->Execute(symTab, level) * m_right->Execute(symTab, level);
    return PVar();

}


