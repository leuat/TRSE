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

#include "nodeblock.h"

void NodeBlock::SetParameter(QString name, PVar var) {
    QSharedPointer<Symbol> s = m_symTab->Lookup(name, m_op.m_lineNumber);
    //        if (s==nullptr)
    //          ErrorHandler::e.Error("Could not find variable :" + name,m_op.m_lineNumber);

    s->m_value = QSharedPointer<PVar>(new PVar(var));
}

void NodeBlock::Delete() {
    Node::Delete();
//    qDebug()<< "NODEBLOCK DETELET";
    if (m_useOwnSymTab && m_symTab!=nullptr) {
        m_symTab->Delete();
//        delete m_symTab;
    }
    if (m_compoundStatement!=nullptr) {
        m_compoundStatement->Delete();
        m_compoundStatement = nullptr;
    }
    for (Node* n : m_decl) {
//        qDebug() << "Deleting : "<< (dynamic_cast<NodeVarDecl*>(n)!=nullptr) <<m_decl.count();
       // n->Delete();
  //      qDebug() << "DONE";
       // delete n;
        s_uniqueSymbols[n] = n;

    }
    m_decl.clear();

}

void NodeBlock::PopZeroPointers(Assembler *as) {
    for (Node* n: m_decl) {
        NodeVarDecl* nv = dynamic_cast<NodeVarDecl*>(n);
        if (nv!=nullptr) {
            for (int i=0;i<nv->m_pushedPointers;i++)
                as->PopZeroPointer();
        }
    }

}



void NodeBlock::ExecuteSym(QSharedPointer<SymbolTable> symTab) {
    m_useOwnSymTab = false;

    if (m_useOwnSymTab) {
        if (m_symTab == nullptr)
            m_symTab = QSharedPointer<SymbolTable>(new SymbolTable());
    }
    else m_symTab = symTab;

    for (Node* n: m_decl)
    {
        n->ExecuteSym(m_symTab);
    }
    m_compoundStatement->ExecuteSym(m_symTab);
}
