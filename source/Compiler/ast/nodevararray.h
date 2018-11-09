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

#ifndef NODEVARARRAY_H
#define NODEVARARRAY_H

#include "node.h"
#include "source/Compiler/symboltable.h"
#include "nodevar.h"

class NodeVarArray : public Node
{
public:
    NodeVar* m_var;
    Node* m_expr;

    NodeVarArray(Token token, Node* expr);

    void StoreAcc(Assembler* as);


    void ExecuteSym(SymbolTable* symTab);


};

#endif // NODEVARARRAY_H
