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
#include "source/Compiler/codegen/abstractcodegen.h"

/* 
    Binary clause node: (a = b), (c != 2*d), (e>5), (a>b and c!=d) etc
    m_left: left expression. Can be recursive.
    m_right: right expression. Can be recursive.
    m_op: Comparator operator (GREATER, LESS, EQUALS, NOTEQUALS, AND, OR) etc)

    if the comparator is OR or AND, then the binary clause is a *compound clause*, meaning
    that the left/right nodes are sub-binary clause nodes. Example:

    if (a>b) then ... <- simple binary clause, NOT a compound, a "leaf" node.
    if (a>b and c<d) <- compound binary clause (not a leaf), but m_left/m_right is a leaf node "c<d" and "a>b"

*/
class NodeBinaryClause : public Node
{
public:
    NodeBinaryClause(Token op, QSharedPointer<Node> left, QSharedPointer<Node> right):Node() {
        m_right = right;
        m_left = left;
        m_op = op;
    }
    void ExecuteSym(QSharedPointer<SymbolTable> sym) override;

    bool cannotBeSimplified(Assembler* as);



    void Accept(AbstractCodeGen* dispatcher) override {
        dispatcher->dispatch(qSharedPointerDynamicCast<NodeBinaryClause>(sharedFromThis()));
    }

    void parseConstants(QSharedPointer<SymbolTable>  symTab) override {
        if (m_left!=nullptr)
            m_left->parseConstants(symTab);
        if (m_right!=nullptr)
            m_right->parseConstants(symTab);
    }

    bool isCompoundClause() override {
        return (m_op.m_type==TokenType::AND || m_op.m_type == TokenType::OR);

    }

    bool isWord(Assembler *as) override;
    bool isLong(Assembler *as) override;


};

#endif // NODEBINARYCLAUSE_H
