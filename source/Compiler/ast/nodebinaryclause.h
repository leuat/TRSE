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

#ifndef NODEBINARYCLAUSE_H
#define NODEBINARYCLAUSE_H

#include "node.h"


class NodeBinaryClause : public Node
{
public:
    NodeBinaryClause(Token op, Node* left, Node* right):Node() {
        m_right = right;
        m_left = left;
        m_op = op;
    }

    PVar Execute(SymbolTable *symTab, uint lvl) override{
        return PVar();
    }

    void ExecuteSym(SymbolTable* sym) override;

    void BuildToCmp(Assembler* as);

    void BuildSimple(Assembler* as, QString failedLabel);


    bool cannotBeSimplified(Assembler* as);

//    void OnlyNumVar(Assembler* as, QString a, QString b);

    void BinaryClause(Assembler* as );
    void BinaryClauseInteger(Assembler* as );
    void LogicalClause(Assembler* as );


    // Input: left, right
    // Output: a = 0, 1
    QString Build(Assembler *as) override;



};

#endif // NODEBINARYCLAUSE_H
