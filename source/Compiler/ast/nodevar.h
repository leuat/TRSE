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

#pragma once

#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodenumber.h"
#include "source/Compiler/ast/nodevartype.h"

class NodeVar : public Node {
public:
    QString value;
    Node* m_expr = nullptr;
    //NodeVarType* m_type;


    NodeVar(Token t);

    NodeVar(Token t, Node* expr);

    PVar Execute(SymbolTable* symTab, uint lvl) override;


    TokenType::Type getType(Assembler* as) override;


    bool DataEquals(Node *other) override;
    bool isWord(Assembler* as) override;

    void LoadPointer(Assembler* as);

    virtual void AssignPointer(Assembler *as, QString memoryLocation);

    bool LoadXYVarOrNum(Assembler* as, Node* node, bool isx);

    void LoadByteArray(Assembler *as);

    bool isAddress() override;

    void LoadVariable(Assembler* as) override;

    void StoreVariable(Assembler* as) override;

    QString Build(Assembler *as) override;
    void ExecuteSym(SymbolTable* symTab) override;

};
