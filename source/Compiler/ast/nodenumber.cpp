#include "nodenumber.h"


PVar NodeNumber::Execute(SymbolTable *symTab, uint lvl) {
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
    level = lvl +1;
    return PVar(m_val);

}
