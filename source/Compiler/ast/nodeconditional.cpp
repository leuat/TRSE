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
#include "source/Compiler/assembler/mos6502/mos6502.h"


NodeConditional::NodeConditional(Token op, int forcePage, QSharedPointer<Node> clause, QSharedPointer<Node> block, bool isWhile, QSharedPointer<Node> elseBlock) :Node(){
    m_block = block;
    m_isWhileLoop = isWhile;
    m_elseBlock = elseBlock;
    m_binaryClause = clause;
    m_op = op;
    m_forcePage = forcePage;
}




