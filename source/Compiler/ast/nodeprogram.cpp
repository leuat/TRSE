#include "nodeprogram.h"


void NodeProgram::Delete() {
    if (m_NodeBlock) {
        m_NodeBlock->Delete();
        delete m_NodeBlock;
        m_NodeBlock = nullptr;
    }
}

PVar NodeProgram::Execute(SymbolTable *symTab, uint lvl) {
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
    level = lvl+1;
    ErrorHandler::e.DebugLow("Calling Program Node",level);

    m_NodeBlock->Execute(symTab, level);
    return PVar();

}

void NodeProgram::ExecuteSym(SymbolTable *symTab) {
    m_NodeBlock->ExecuteSym(symTab);
}

QString NodeProgram::Build(Assembler *a) {
    Node::Build(a);
    a->EndMemoryBlock();
    NodeBuiltinMethod::m_isInitialized.clear();
    a->Program(m_name);
    a->m_source << m_initJumps;
    m_NodeBlock->Build(a);
    a->EndProgram();
    return "";
}
