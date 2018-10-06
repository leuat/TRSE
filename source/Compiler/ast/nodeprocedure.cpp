/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

#include "nodeprocedure.h"


NodeProcedure::NodeProcedure(NodeProcedureDecl *proc, QVector<Node *> params, Token t):Node() {
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
