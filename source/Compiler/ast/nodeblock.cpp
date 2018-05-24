#include "nodeblock.h"

PVar NodeBlock::Execute(SymbolTable *symTab, uint lvl) {
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
    level = lvl+1;
    ErrorHandler::e.DebugLow("Calling BlockNode",level);

    for (Node* n: m_decl)
        n->Execute(m_symTab, level);
    ErrorHandler::e.DebugLow("Calling Compound" ,level);
    m_compoundStatement->Execute(m_symTab, level);
    Symbol* s = m_symTab->Lookup("return", m_op.m_lineNumber);
    if (s->m_value)
        return *s->m_value;
    return PVar();

}

void NodeBlock::ExecuteSym(SymbolTable *symTab) {
    m_useOwnSymTab = false;

    if (m_useOwnSymTab) {
        if (m_symTab == nullptr)
            m_symTab = new SymbolTable();
    }
    else m_symTab = symTab;

    for (Node* n: m_decl)
    {
        n->ExecuteSym(m_symTab);
    }
    m_compoundStatement->ExecuteSym(m_symTab);
}
