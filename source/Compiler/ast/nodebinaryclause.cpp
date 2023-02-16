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

#include "nodebinaryclause.h"
#include "nodevar.h"
#include "nodenumber.h"




void NodeBinaryClause::ExecuteSym(QSharedPointer<SymbolTable> sym) {
    if (m_left!=nullptr)
        m_left->ExecuteSym(sym);
    if (m_right!=nullptr)
        m_left->ExecuteSym(sym);
}

bool NodeBinaryClause::cannotBeSimplified(Assembler *as) {
    // can NOT be simplified
    return ((m_op.m_type==TokenType::AND || m_op.m_type == TokenType::OR)
            || m_left->getType(as)==TokenType::INTEGER || m_right->getType(as)==TokenType::INTEGER
            //|| m_left->getType(as)==TokenType::POINTER || m_right->getType(as)==TokenType::POINTER

            );
}


bool NodeBinaryClause::isWord(Assembler *as)
{
    return m_left->isWord(as) | m_right->isWord(as);
}



