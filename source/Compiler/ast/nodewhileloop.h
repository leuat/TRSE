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

#ifndef NODEWHILELOOP_H
#define NODEWHILELOOP_H

#include "source/Compiler/ast/node.h"

class NodeWhileLoop : public Node
{
public:
    NodeWhileLoop();
    Node* m_block = nullptr;
    NodeWhileLoop(Node* block) {
        m_block = block;
    }
    void Delete() override;

    PVar Execute(SymbolTable* symTab, uint lvl) override {return PVar();}

    void ExecuteSym(SymbolTable* symTab) override;


};

#endif // NODEWHILELOOP_H
