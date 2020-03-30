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

#ifndef NODEUNARYOP_H
#define NODEUNARYOP_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodenumber.h"
#include "source/Compiler/ast/nodevar.h"
#include "source/Compiler/ast/nodebinop.h"
#include "source/Compiler/assembler/abstractastdispatcher.h"

class NodeUnaryOp : public Node {
public:
    NodeUnaryOp(Token t, QSharedPointer<Node> right);


    bool isMinusOne() override;


    void parseConstants(QSharedPointer<SymbolTable>  symTab) override {
        if (m_left!=nullptr)
            m_left->parseConstants(symTab);
        if (m_right!=nullptr)
            m_right->parseConstants(symTab);
    }


    void ExecuteSym(QSharedPointer<SymbolTable>  symTab) override {
        m_right->ExecuteSym(symTab);
    }
    void Accept(AbstractASTDispatcher* dispatcher) override {
        dispatcher->dispatch(qSharedPointerDynamicCast<NodeUnaryOp>(sharedFromThis()));
    }

};

#endif // NODEUNARYOP_H
