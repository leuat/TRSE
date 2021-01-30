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

#ifndef NODECOMPOUND_H
#define NODECOMPOUND_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/assembler/abstractastdispatcher.h"
#include "source/Compiler/ast/node.h"

class NodeCompound : public Node {
public:
    QVector<QSharedPointer<Node>> children;
    NodeCompound(Token t):Node() {
        m_op = t;
    }
    void ExecuteSym(QSharedPointer<SymbolTable>  symTab) override;

    void Accept(AbstractASTDispatcher* dispatcher) override {
        dispatcher->dispatch(qSharedPointerDynamicCast<NodeCompound>(sharedFromThis()));
    }
    void parseConstants(QSharedPointer<SymbolTable>  symTab) override {
        for (QSharedPointer<Node> n: children)
            n->parseConstants(symTab);
    }

    void ReplaceInline(Assembler* as,QMap< QString,QSharedPointer<Node>>& inp) override;


};
#endif // NODECOMPOUND_H
