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

#include "nodeconditional.h"
#include "source/Compiler/assembler/mos6502.h"


NodeConditional::NodeConditional(Token op, int forcePage, Node *clause, Node *block, bool isWhile, Node *elseBlock) :Node(){
    m_block = block;
    m_isWhileLoop = isWhile;
    m_elseBlock = elseBlock;
    m_binaryClause = clause;
    m_op = op;
    m_forcePage = forcePage;
}

void NodeConditional::Delete() {
    Node::Delete();
    if (m_block!=nullptr) {
        m_block->Delete();
        delete m_block;
        m_block = nullptr;
    }
    if (m_elseBlock!=nullptr) {
        m_elseBlock->Delete();
        delete m_elseBlock;
        m_elseBlock = nullptr;
    }
    if (m_binaryClause) {
        m_binaryClause->Delete();
        delete m_binaryClause;
        m_binaryClause = nullptr;
    }


}

