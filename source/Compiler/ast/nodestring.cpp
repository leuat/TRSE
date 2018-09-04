#include "nodestring.h"


PVar NodeString::Execute(SymbolTable *symTab, uint lvl) {
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
    level = lvl +1;
    return PVar();
}

QString NodeString::Build(Assembler *as) {
    Node::Build(as);

    as->String(m_val);
    return "";
}
