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
#include "source/Compiler/assembler/abstractastdispatcher.h"


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



    void Accept(AbstractASTDispatcher* dispatcher) override {
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


};

#endif // NODEBINARYCLAUSE_H
