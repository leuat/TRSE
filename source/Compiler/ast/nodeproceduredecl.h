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
#include "source/Compiler/codegen/abstractcodegen.h"

class NodeProcedureDecl : public Node {
public:
    QString m_procName, m_fileName;
    bool m_isStatic = true;
    QString m_class;

    QStringList m_flags;
    QVector<QSharedPointer<Node>> m_paramDecl;
    QSharedPointer<Node> m_returnValue;
    QSharedPointer<Node> m_returnType;
    bool m_isFunction = false;
    bool m_isRecursive = false;
    int m_type;
    bool m_isInline = false;
    bool m_isForward = false;
    QSharedPointer<Node> m_block = nullptr;


    void FindPotentialSymbolsInAsmCode(QStringList& lst)  override;

    NodeProcedureDecl(Token t, QString m);
    void parseConstants(QSharedPointer<SymbolTable>  symTab) override {
        if (m_block!=nullptr)
            m_block->parseConstants(symTab);
        for (QSharedPointer<Node> n:m_paramDecl)
            n->parseConstants(symTab);
    }



    NodeProcedureDecl(Token t, QString m, QVector<QSharedPointer<Node>> paramDecl, QSharedPointer<Node> block, int type);


    void AppendBlock(QSharedPointer<Node> block);

    void SetParametersValue(QVector<PVar>& lst);


    void ExecuteSym(QSharedPointer<SymbolTable>  symTab) override;

    void Accept(AbstractCodeGen* dispatcher) override {
        dispatcher->dispatch(qSharedPointerDynamicCast<NodeProcedureDecl>(sharedFromThis()));
    }

};

#endif // NODEPROCEDUREDECL_H
