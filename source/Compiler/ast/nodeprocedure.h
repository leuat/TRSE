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

#ifndef NODEPROCEDURE_H
#define NODEPROCEDURE_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodeproceduredecl.h"
#include "source/Compiler/ast/nodevar.h"
#include "source/Compiler/ast/nodeassign.h"
#include "source/Compiler/codegen/abstractcodegen.h"

class NodeProcedure : public Node {
public:
    QSharedPointer<NodeProcedureDecl> m_procedure;
    QVector<QSharedPointer<Node>> m_parameters;

    NodeProcedure(QSharedPointer<NodeProcedureDecl> proc, QVector<QSharedPointer<Node>> params, Token t );

    virtual bool isReference() override { return m_op.m_isReference; }

    bool isAddress() override;
    bool m_classTagged = false;
    void parseConstants(QSharedPointer<SymbolTable>  symTab) override {
        if (m_procedure!=nullptr)
            m_procedure->parseConstants(symTab);
        for (QSharedPointer<Node> n : m_parameters)
            n->parseConstants(symTab);
    }

    void ExecuteSym(QSharedPointer<SymbolTable>  symTab) override;
/*    void setForceTypeFunctions(TokenType::Type t) {
        if (m_procedure->m_returnValue!=nullptr)
            m_forceType = t;
    }
*/
    QString getValue(Assembler* as) override;
    QString getValue8bit(Assembler* as, int isHi) override;

    bool isWord(Assembler* as) override;
    bool isLong(Assembler* as) override;
    bool isByte(Assembler* as) override;


    void Accept(AbstractCodeGen* dispatcher) override {
        dispatcher->dispatch(qSharedPointerDynamicCast<NodeProcedure>(sharedFromThis()));
    }


    virtual void ReplaceInline(Assembler* as,QMap< QString,QSharedPointer<Node>>& inp) override;
    bool isPureNumeric() override;

};


#endif // NODEPROCEDURE_H
