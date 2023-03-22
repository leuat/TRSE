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

#ifndef NODESTRING_H
#define NODESTRING_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/codegen/abstractcodegen.h"
/* 
   A Node representing a string. 
    m_left: undefined 
    m_right: undefined
    m_op: the String's token. 
*/
class NodeString : public Node {
public:
    QStringList m_val;
    bool m_isCString = false;
    bool m_hasBeenDefined = false;
    NodeString(Token op, QStringList val, bool isCString):Node() {
        m_op = op;
        m_val = val;
        m_isCString = isCString;
    }

    void ExecuteSym(QSharedPointer<SymbolTable>  symTab) override {
    }
    void Accept(AbstractCodeGen* dispatcher) override {
        dispatcher->dispatch(qSharedPointerDynamicCast<NodeString>(sharedFromThis()));
    }

    QString getValue(Assembler* as) override {
        return Util::toString(m_val);
    }

};

#endif // NODESTRING_H
