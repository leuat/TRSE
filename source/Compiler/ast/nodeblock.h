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

#ifndef NODEBLOCK_H
#define NODEBLOCK_H

#include "source/Compiler/assembler/abstractastdispatcher.h"

#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodevar.h"
//#include "source/Compiler/ast/nodeproceduredecl.h"
#include "source/Compiler/ast/nodevardecl.h"

class NodeBlock : public Node {
public:
    QVector<Node*> m_decl;
    QString forceLabel = "";
    Node* m_compoundStatement = nullptr;
    SymbolTable* m_symTab = nullptr;
    bool m_useOwnSymTab;
    bool m_isMainBlock = false;
    QString m_forceInterupt = "";


    NodeBlock(Token t, QVector<Node*> decl, Node* comp, bool useOwnSymTab = true):Node() {
        m_compoundStatement = comp;
        m_decl = decl;
        m_op = t;
        m_useOwnSymTab = useOwnSymTab;
    }

    void SetParameter(QString name, PVar var);
    void Delete() override;

    void PopZeroPointers(Assembler* as);
    void parseConstants(SymbolTable* symTab) override {
        for (Node* n: m_decl)
            if (n!=nullptr)
            n->parseConstants(symTab);
        if (m_compoundStatement!=nullptr)
            m_compoundStatement->parseConstants(symTab);
    }


    void ExecuteSym(SymbolTable* symTab) override;

    void Accept(AbstractASTDispatcher* dispatcher) override {
        dispatcher->dispatch(this);
    }

};

#endif // NODEBLOCK_H
