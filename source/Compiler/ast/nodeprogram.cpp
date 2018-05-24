#include "nodeprogram.h"


PVar NodeProgram::Execute(SymbolTable *symTab, uint lvl) {
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
    level = lvl+1;
    ErrorHandler::e.DebugLow("Calling Program Node",level);

    m_NodeBlock->Execute(symTab, level);
    return PVar();

}
