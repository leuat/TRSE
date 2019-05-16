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
#include "source/Compiler/assembler/abstractastdispatcher.h"
#include "source/Compiler/ast/node.h"


class NodeNumber : public Node {
public:
    float m_val;
    NodeNumber(Token op, int val);
    QString m_strVal = "";
    void ExecuteSym(SymbolTable* symTab) override {
    }

    bool isAddress() override;

    bool isPureNumeric() override;

    bool isWord(Assembler* as) override;

    QString getValue() override {
        if (isAddress()) return HexValue(); else return "#" + HexValue();
    }


    //void LoadVariable(AbstractASTDispatcher* dispatcher) override;

    int numValue() { return m_val;}

    QString getAddress() override {
        return HexValue();
    }



    QString HexValue() override;

    bool DataEquals(Node *other) override;

    int getInteger() override {
        return m_val;
    }


    QString StringValue();

    bool isMinusOne() override;

    bool isOne() override;
    void Accept(AbstractASTDispatcher* dispatcher) override {
        dispatcher->dispatch(this);
    }


};

#endif // NODENUMBER_H
