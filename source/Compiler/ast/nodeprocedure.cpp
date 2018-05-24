#include "nodeprocedure.h"


PVar NodeProcedure::Execute(SymbolTable *symTab, uint lvl) {
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
    QVector<PVar> vars;
    for (Node* n: m_parameters) {
        vars.append(n->Execute(symTab, lvl));
    }
    m_procedure->SetParametersValue(vars);
    return m_procedure->Execute(symTab, lvl);
}
