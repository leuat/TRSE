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

#include "nodeassign.h"


NodeAssign::NodeAssign(Node *left, Token t, Node *r):Node() {
    m_right = r;
    m_op = t;
    m_left = left;
   // qDebug() << "NodeAssign " <<TokenType::getType(m_left->getType(nullptr)) << ((NodeVar*)m_left)->getValue(as);
    if (m_left->getType(nullptr)==TokenType::INTEGER) {// || m_left->getType(nullptr)==TokenType::POINTER) {
 //       qDebug() << "::NodeAssign INTEGER";
//        m_right->setForceType(TokenType::INTEGER);
//        m_right->setForceType(m_left->getType(nullptr));
    }

}

void NodeAssign::Delete()
{
    Node::Delete();
}

void NodeAssign::ExecuteSym(QSharedPointer<SymbolTable>  symTab) {
    QString varName = ((NodeVar*)m_left)->value;
    QSharedPointer<Symbol> varSymbol = symTab->Lookup(varName, m_op.m_lineNumber);
    m_right->ExecuteSym(symTab);

}
