#include "nodecompound.h"

PVar NodeCompound::Execute(SymbolTable *symTab, uint lvl) {
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);

    level = lvl+1;
    for (Node* n: children)
        n->Execute(symTab, level);
    return PVar();

}

void NodeCompound::Delete() {
    for (Node* n: children) {
        n->Delete();
        delete n;
    }
    children.clear();
}

void NodeCompound::ExecuteSym(SymbolTable *symTab) {
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
    for (Node* n:children) {
        ErrorHandler::e.DebugLow("Calling Compound Node",level);
        n->ExecuteSym(symTab);
    }
}

QString NodeCompound::Build(Assembler *as) {
    Node::Build(as);

    as->BeginBlock();
    for (Node* n: children)
        n->Build(as);
    as->EndBlock();
    return "";
}
