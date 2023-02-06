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

#ifndef NODEBUILTINMETHOD_H
#define NODEBUILTINMETHOD_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/codegen/abstractcodegen.h"

class NodeBuiltinMethod : public Node {
public:
    QString m_procName;
    QVector<QSharedPointer<Node>> m_params;
    static QMap<QString, bool> m_isInitialized;
    BuiltInFunction* m_function = nullptr;

    void VerifyParams(Assembler* as);

    NodeBuiltinMethod(QString m, QVector<QSharedPointer<Node>> params, BuiltInFunction* bf):Node() {
        m_procName = m;
        m_params = params;
        m_op.m_type = TokenType::BYTE;
        m_function= bf;
        m_op.m_lineNumber--;
    }
    ~NodeBuiltinMethod() {

    }

    void parseConstants(QSharedPointer<SymbolTable>  symTab) override {
        for (QSharedPointer<Node> n:m_params)
            n->parseConstants(symTab);
    }
    bool isPureVariable() override;


    virtual void ReplaceInline(Assembler* as,QMap< QString,QSharedPointer<Node>>& inp) override;

    void ExecuteSym(QSharedPointer<SymbolTable>  symTab) override {

    }

    void Accept(AbstractCodeGen* dispatcher) override {
        dispatcher->dispatch(qSharedPointerDynamicCast<NodeBuiltinMethod>(sharedFromThis()));
    }

};


#endif // NODEBUILTINMETHOD_H
