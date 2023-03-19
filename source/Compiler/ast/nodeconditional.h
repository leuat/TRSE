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

#ifndef NODECONDITIONAL_H
#define NODECONDITIONAL_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodebinaryclause.h"
#include <QVector>
#include "source/Compiler/codegen/abstractcodegen.h"
/* 
    Node representing a full conditional statement + block, ie
    if (a>b and c!=d*2) then
    begin
        ...
    end
    else
    begin
        ..
    end;


    m_left: unused
    m_right: unused
    m_block : the main block if the conditional is true
    m_elseBlock : the block if the conditional fails
    m_binaryClause : the actual binary clause (a>b and c!=d*2)
*/
class NodeConditional : public Node {
public:


    QSharedPointer<Node> m_block = nullptr;
    QSharedPointer<Node> m_binaryClause = nullptr;

    QSharedPointer<Node> m_elseBlock = nullptr;
    bool m_isWhileLoop;



    NodeConditional(Token op, int forcePage, QSharedPointer<Node> clause, QSharedPointer<Node> block, bool isWhile, QSharedPointer<Node> elseBlock=nullptr);

    void ReplaceVariable(Assembler *as, QString name, QSharedPointer<Node> node) override;

    void parseConstants(QSharedPointer<SymbolTable>  symTab) override {
        if (m_block!=nullptr)
            m_block->parseConstants(symTab);
        if (m_elseBlock!=nullptr)
            m_elseBlock->parseConstants(symTab);
        if (m_binaryClause!=nullptr)
            m_binaryClause->parseConstants(symTab);
    }


    void FindPotentialSymbolsInAsmCode(QStringList& lst)  override {
        if (m_block!=nullptr)
            m_block->FindPotentialSymbolsInAsmCode(lst);
        if (m_elseBlock!=nullptr)
            m_elseBlock->FindPotentialSymbolsInAsmCode(lst);
    }







    void ExecuteSym(QSharedPointer<SymbolTable>  symTab) override {
       m_block->ExecuteSym(symTab);
    }

    void Accept(AbstractCodeGen* dispatcher) override {
        dispatcher->dispatch(qSharedPointerDynamicCast<NodeConditional>(sharedFromThis()));
    }

};

#endif // NODECONDITIONAL_H
