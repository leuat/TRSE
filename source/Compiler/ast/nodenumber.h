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

#ifndef NODENUMBER_H
#define NODENUMBER_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/codegen/abstractcodegen.h"
#include "source/Compiler/ast/node.h"


class NodeNumber : public Node {
public:
    long m_val;
    NodeNumber(Token op, long val);
    QString m_strVal = "";
    void ExecuteSym(QSharedPointer<SymbolTable> symTab) override {
    }

    bool isAddress() override;
    bool isPureNumeric() override;
    bool isPureNumericOrAddress() override;
    bool is8bitValue(Assembler* as) override;

    bool isWord(Assembler* as) override;

    QString getValue(Assembler* as) override;
    virtual bool isReference() override;

    QString getValue8bit(Assembler* as, int isHi) override;

    void forceWord() override {
        m_op.m_type = TokenType::INTEGER_CONST;
    }

    //void LoadVariable(AbstractCodeGen* dispatcher) override;

    int numValue() override { return m_val;}

    QString getAddress() override {
        return HexValue();
    }
    bool containsVariables() override {return false;}

    QString getLiteral(Assembler* as) override {
        return HexValue();
    }



    QString HexValue() override;

    bool DataEquals(QSharedPointer<Node> other) override;



    QString StringValue();

    bool isMinusOne() override;

    bool isOne() override;
    void Accept(AbstractCodeGen* dispatcher) override {
        dispatcher->dispatch(qSharedPointerDynamicCast<NodeNumber>(sharedFromThis()));
    }


};

#endif // NODENUMBER_H
