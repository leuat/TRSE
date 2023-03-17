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

#ifndef NODEASM_H
#define NODEASM_H

#include "node.h"


#include "source/Compiler/codegen/abstractcodegen.h"

/* 
    Node for an asm block
    m_left: undefined
    m_right: undefined
    m_op: Token associated with the assembly string.
*/
class NodeAsm : public Node
{
public:
    /* Assembly code associated with the node as a QString */
    QString m_asm;


    NodeAsm(Token t):Node() {
        m_asm = t.m_value;
        m_op = t;
    }

    void ExecuteSym(QSharedPointer<SymbolTable>  symTab) override {

    }
    // This method is used for finding symbols/variables used in the assembly block, so the
    // optimiser won't flag them as "unused"
    void FindPotentialSymbolsInAsmCode(QStringList& lst)  override;

    void Accept(AbstractCodeGen* dispatcher) override {
        dispatcher->dispatch(qSharedPointerDynamicCast<NodeAsm>(sharedFromThis()));
    }

};

#endif // NODEASM_H
