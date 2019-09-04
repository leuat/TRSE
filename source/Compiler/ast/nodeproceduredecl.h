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

#ifndef NODEPROCEDUREDECL_H
#define NODEPROCEDUREDECL_H

#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodeblock.h"
#include "source/Compiler/ast/nodevardecl.h"
#include "source/Compiler/assembler/abstractastdispatcher.h"

class NodeProcedureDecl : public Node {
public:
    QString m_procName;
    QVector<Node*> m_paramDecl;
    int m_type;
    Node* m_block = nullptr;


    NodeProcedureDecl(Token t, QString m);
    void parseConstants(SymbolTable* symTab) override {
        if (m_block!=nullptr)
            m_block->parseConstants(symTab);
        for (Node* n:m_paramDecl)
            n->parseConstants(symTab);
    }



    NodeProcedureDecl(Token t, QString m, QVector<Node*> paramDecl, Node* block, int type);


    void AppendBlock(Node* block);
    void Delete() override;

    void SetParametersValue(QVector<PVar>& lst);


    void ExecuteSym(SymbolTable* symTab) override;

    void Accept(AbstractASTDispatcher* dispatcher) override {
        dispatcher->dispatch(this);
    }

};

#endif // NODEPROCEDUREDECL_H
