asm(";here");
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


class NodeBinOP : public Node {
public:
    QVector<int> power2 = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192, 8192*2,8192*4,8192*8};
    NodeBinOP(QSharedPointer<Node> left, Token op, QSharedPointer<Node> right);
    void ExecuteSym(QSharedPointer<SymbolTable>  symTab) override;

    QString BothConstants(Assembler* as);

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
    void setCastType(TokenType::Type t) override;

    bool isPurePointer(Assembler *as) override {
        return m_left->isPurePointer(as) && m_right->isPurePointer(as);
    }

    bool isPointer(Assembler *as) override
    {
        return m_left->isPointer(as) | m_right->isPointer(as);

    }
    TokenType::Type getOrgType(Assembler *as) override {
        auto t1 = m_right->getOrgType(as);
        auto t2 = m_left->getOrgType(as);
        if (t1==TokenType::LONG || t2==TokenType::LONG) return TokenType::LONG;
        if (t1==TokenType::INTEGER || t2==TokenType::INTEGER) return TokenType::INTEGER;
        return t1;
     }

    QString getAddress() override {
        return HexValue();
    }

    void forceWord() override {
       m_left->forceWord();
       m_right->forceWord();
    }
    bool containsPointer(Assembler* as) override;

    void SwapVariableFirst();

    virtual bool isReference() override;

    bool ContainsVariable(Assembler* as, QString var);


    void parseConstants(QSharedPointer<SymbolTable>  symTab) override;


    QString getValue(Assembler* as)  override;
    QString getValue8bit(Assembler* as, int isHi) override;

    TokenType::Type getType(Assembler *as) override;
    QString getTypeText(Assembler* as) override;

    bool isPure() override;

    TokenType::Type VerifyAndGetNumericType() override;


    QString getStringOperation();

    QString getLiteral(Assembler* as) override;


    int numValue() override;

    QString HexValue() override;

    int BothPureNumbersBinOp(Assembler *as);


    void Accept(AbstractCodeGen* dispatcher) override {
        dispatcher->dispatch(qSharedPointerDynamicCast<NodeBinOP>(sharedFromThis()));
    };

};


