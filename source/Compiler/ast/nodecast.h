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

#pragma once

#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"


#include "source/Compiler/ast/nodenumber.h"
#include "source/Compiler/ast/nodeunaryop.h"

#include "source/Compiler/codegen/abstractcodegen.h"


class NodeCast : public Node {
public:
    NodeCast(Token op, QSharedPointer<Node> right);
    void ExecuteSym(QSharedPointer<SymbolTable>  symTab) override;


    bool containsVariables() override;
    bool isPureNumeric() override;
    bool isPureNumericOrAddress() override;
    bool is8bitValue(Assembler* as) override;

    void ApplyFlags() override;
    bool isAddress() override;

    int m_value = 0;

    bool isWord(Assembler* as) override;

    void setForceType(TokenType::Type t) override;
//    void setForceTypeFunctions(TokenType::Type t);
    void setCastType(TokenType::Type t);

    bool isPurePointer(Assembler *as) override {
        return m_right->isPurePointer(as);
    }

    bool isPointer(Assembler *as) override
    {
        return m_right->isPointer(as);

    }

    QString getAddress() override {
        return HexValue();
    }

    void forceWord() override {
       m_right->forceWord();
    }
    bool containsPointer(Assembler* as) override;


    virtual bool isReference() override;

    void parseConstants(QSharedPointer<SymbolTable>  symTab) override;


    QString getValue(Assembler* as)  override;
    QString getValue8bit(Assembler* as, int isHi) override;

    TokenType::Type getType(Assembler *as) override;

    bool isPure() override;






    int numValue() override;

    QString HexValue() override;



    void Accept(AbstractCodeGen* dispatcher) override {
        dispatcher->dispatch(qSharedPointerDynamicCast<NodeCast>(sharedFromThis()));
    }

};


