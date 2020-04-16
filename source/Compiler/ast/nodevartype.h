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

#ifndef NODEVARTYPE_H
#define NODEVARTYPE_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/assembler/abstractastdispatcher.h"

class NodeVarType : public Node {
public:
    QString value;
    Token m_arrayVarType;
    QString m_arrayVarTypeText = "";
    QStringList m_data;
    QStringList m_flags;
    int m_bank=0;
    QString m_filename, m_position="";
    QString initVal ="0";
    int m_flag = 0;

    NodeVarType(Token t, QString position,   Token arrayVarType,QStringList data);
    NodeVarType(Token t, QStringList data);
    NodeVarType(Token t,  QString filename, QString position );
    NodeVarType(Token t,  QString initvalue );

    QString getValue(Assembler* as) override {
        return value;
    }

    void ExecuteSym(QSharedPointer<SymbolTable> symTab) override {};


    void Accept(AbstractASTDispatcher* dispatcher) override {
        dispatcher->dispatch(qSharedPointerDynamicCast<NodeVarType>(sharedFromThis()));
    }

};
#endif // NODEVARTYPE_H
