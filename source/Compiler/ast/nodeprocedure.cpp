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


NodeProcedure::NodeProcedure(QSharedPointer<NodeProcedureDecl> proc, QVector<QSharedPointer<Node> > params, Token t):Node() {
    m_parameters = params;
    m_procedure = proc;
    m_op = t;
}


void NodeProcedure::ExecuteSym(QSharedPointer<SymbolTable>  symTab) {
    m_procedure ->ExecuteSym(symTab);
}

QString NodeProcedure::getValue(Assembler *as)
{
    if (m_op.m_isReference)
        return "#" + m_procedure->m_procName;
    else
        return m_procedure->m_procName;

}
