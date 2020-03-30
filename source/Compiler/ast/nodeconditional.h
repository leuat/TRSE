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
#include "source/Compiler/assembler/abstractastdispatcher.h"

class NodeConditional : public Node {
public:

//    QVector<Node*> m_a, m_b;

    Node* m_block = nullptr;
    Node* m_elseBlock = nullptr;
    Node* m_binaryClause = nullptr;
/*    QVector<Token> m_compares;
    QVector<Token> m_conditionals;*/
    bool m_isWhileLoop;


    int m_forcePage = 0;
/*    NodeConditional(QVector<Token> op, QVector<Node*> a, QVector<Node*> b, Node* block, bool isWhile, QVector<Token> conditionals, Node* elseBlock=nullptr) {
        m_a = a;
        m_b = b;
        m_block = block;
        m_compares = op;
        m_isWhileLoop = isWhile;
        m_elseBlock = elseBlock;
        m_conditionals = conditionals;
    }*/
    NodeConditional(Token op, int forcePage, Node* clause, Node* block, bool isWhile, Node* elseBlock=nullptr);


    void parseConstants(QSharedPointer<SymbolTable>  symTab) override {
        if (m_block!=nullptr)
            m_block->parseConstants(symTab);
        if (m_elseBlock!=nullptr)
            m_elseBlock->parseConstants(symTab);
        if (m_binaryClause!=nullptr)
            m_binaryClause->parseConstants(symTab);
    }



/*    void ConditionalTryFail(Assembler* , QString labelFail, int i);
    void ConditionalTrySuccess(Assembler* , QString labelFail, int i);
*/



    void Delete() override;


    void ExecuteSym(QSharedPointer<SymbolTable>  symTab) override {
       m_block->ExecuteSym(symTab);
    }

    void Accept(AbstractASTDispatcher* dispatcher) override {
        dispatcher->dispatch(this);
    }

};

#endif // NODECONDITIONAL_H
