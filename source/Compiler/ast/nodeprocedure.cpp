#include "nodeprocedure.h"


NodeProcedure::NodeProcedure(NodeProcedureDecl *proc, QVector<Node *> params, Token t) {
    m_parameters = params;
    m_procedure = proc;
    m_op = t;
}

void NodeProcedure::Delete() {
    ErrorHandler::e.DebugLow("Memory: Deleting in NodeProcedure", level);
    if (m_procedure) {
        //m_procedure->Delete();
        //delete m_procedure;
        //m_procedure = nullptr;
    }
    for (Node* n : m_parameters) {
        n->Delete();
        delete n;
    }
    m_parameters.clear();
    ErrorHandler::e.DebugLow("Memory DONE: Deleting in NodeProcedure", level);
}

QString NodeProcedure::Build(Assembler *as) {
    Node::Build(as);

    if (m_parameters.count()!=m_procedure->m_paramDecl.count())
        ErrorHandler::e.Error("Procedure '" + m_procedure->m_procName+"' requires "
                              + QString::number(m_procedure->m_paramDecl.count()) +" parameters, not "
                              + QString::number(m_parameters.count()) + ".", m_op.m_lineNumber);

    for (int i=0; i<m_parameters.count();i++) {
        // Assign all variables
        NodeVarDecl* vd = (NodeVarDecl*)m_procedure->m_paramDecl[i];
        NodeAssign* na = new NodeAssign(vd->m_varNode, m_parameters[i]->m_op, m_parameters[i]);
        na->Build(as);
    }

    as->Asm("jsr " + m_procedure->m_procName);
    return "";
}

PVar NodeProcedure::Execute(SymbolTable *symTab, uint lvl) {
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
    QVector<PVar> vars;
    for (Node* n: m_parameters) {
        vars.append(n->Execute(symTab, lvl));
    }
    m_procedure->SetParametersValue(vars);
    return m_procedure->Execute(symTab, lvl);
}

void NodeProcedure::ExecuteSym(SymbolTable *symTab) {
    m_procedure ->ExecuteSym(symTab);
}
