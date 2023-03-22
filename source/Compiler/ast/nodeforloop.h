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

#ifndef NODEFORLOOP_H
#define NODEFORLOOP_H

#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodeassign.h"
#include "source/Compiler/ast/nodevar.h"

#include "source/Compiler/codegen/abstractcodegen.h"
/* 
    A node representing a for loop, for example "for i:=0 to 100 do"
    m_left: assign statment, i:=0
    m_right: target value, ie 100
    m_op: undefined
    m_block : the for loop block statement list
*/
class NodeForLoop : public Node {
public:

    QSharedPointer<Node> m_block = nullptr;
    bool m_unroll = false;
    QSharedPointer<Node> m_step = nullptr;
    int m_loopCounter=0;
    bool m_inclusive = false;

    NodeForLoop(QSharedPointer<Node> a, QSharedPointer<Node> b, QSharedPointer<Node> block, QSharedPointer<Node> step, bool unroll, int forcePage, int loopCounter, bool inclusive);

    void clearComment() override {
        m_comment = "";
        if (m_block!=nullptr)
        m_block->clearComment();
    }


    void parseConstants(QSharedPointer<SymbolTable>  symTab) override {
        if (m_left!=nullptr)
            m_left->parseConstants(symTab);
        if (m_right!=nullptr)
            m_right->parseConstants(symTab);
        if (m_block!=nullptr)
            m_block->parseConstants(symTab);
        if (m_step!=nullptr)
            m_step->parseConstants(symTab);
    }
    void FindPotentialSymbolsInAsmCode(QStringList& lst)  override {
        if (m_block!=nullptr)
            m_block->FindPotentialSymbolsInAsmCode(lst);
    }



    void ExecuteSym(QSharedPointer<SymbolTable>  symTab) override {
        m_block->ExecuteSym(symTab);
    }


    void Accept(AbstractCodeGen* dispatcher) override {
        dispatcher->dispatch(qSharedPointerDynamicCast<NodeForLoop>(sharedFromThis()));
    }

};
#endif // NODEFORLOOP_H
