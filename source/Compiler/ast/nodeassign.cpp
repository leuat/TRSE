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


NodeAssign::NodeAssign(QSharedPointer<Node> left, Token t, QSharedPointer<Node> r):Node() {
    m_right = r;
    m_op = t;
    m_left = left;


}


void NodeAssign::ExecuteSym(QSharedPointer<SymbolTable>  symTab) {
    return;
    QString varName = qSharedPointerDynamicCast<NodeVar>(m_left)->value;
    QSharedPointer<Symbol> varSymbol = symTab->Lookup(varName, m_op.m_lineNumber);
    m_right->ExecuteSym(symTab);

}

bool NodeAssign::isDead() {
    if ((!m_left) || (!m_right))
        return false;

    if (m_left->rawValue() == m_right->rawValue() && m_right->rawValue()!="") {
        qDebug() << m_left->rawValue() << m_right->rawValue();
        if (m_left->getIndex()!=nullptr) {
            if (m_right->getIndex()!=nullptr)
                if (m_left->getIndex()->rawValue() == m_right->getIndex()->rawValue() && m_right->getIndex()->rawValue()!="")
                    // i[j] := i[j];
                    return true;
        }
        else
            // i := i
            if (m_right->getIndex()==nullptr) {
                return true;
            }

    }
    return false;
}

bool NodeAssign::Optimize() {
    if (isDead()) return true;
    return false;
}
