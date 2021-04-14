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
#include "source/Compiler/ast/nodevartype.h"
#include "source/Compiler/assembler/abstractastdispatcher.h"

class NodeVar : public Node {
private:
public:
    QString value;
    QSharedPointer<Node> m_expr = nullptr;
    //QSharedPointer<NodeVarType> m_type;
    bool m_fake16bit = false;
    QSharedPointer<Node> m_subNode = nullptr;
    bool m_ignoreRecordExpr = false;
    bool m_ignoreLookup = false;
    NodeVar(Token t);

    NodeVar(Token t, QSharedPointer<Node> expr);

    bool m_isGlobal = false;
    void ReplaceInline(Assembler* as,QMap<QString, QSharedPointer<Node> >& inp) override;


    TokenType::Type getOrgType(Assembler *as);

    TokenType::Type getType(Assembler* as) override;
    QString getTypeText(Assembler* as) override;
    TokenType::Type getArrayType(Assembler* as) override;

    bool isPointer(Assembler* as) override;
    bool isPurePointer(Assembler* as) override;


    bool DataEquals(QSharedPointer<Node> other) override;
    bool isWord(Assembler* as) override;
    bool isLong(Assembler* as) override;
    bool isByte(Assembler* as) override;
    bool containsPointer(Assembler* as) override;
    bool isRecord(Assembler* as) override;
    bool isClass(Assembler* as) override;
    bool isRecordData(Assembler* as) override;

    virtual bool isReference() override { return m_op.m_isReference; }

    void forceWord() override {
        m_fake16bit = true;
    }

    QString getAddress() override {return value;}

    void parseConstants(QSharedPointer<SymbolTable>  symTab) override {
        if (m_expr!=nullptr)
            m_expr->parseConstants(symTab);
    }


    bool isVariable() override {
        return true;
    }

    bool isPureVariable() override {

        return m_expr==nullptr; // only return true if there are no array expressions
    }
    bool is8bitValue(Assembler* as) override {
        return getType(as)==TokenType::BYTE
                ||(getType(as)==TokenType::POINTER && m_expr!=nullptr )
                ||(getType(as)==TokenType::ADDRESS && m_expr!=nullptr )
                || getType(as)==TokenType::ARRAY
                ;
    }

    bool isArrayIndex() override { return m_expr!=nullptr; }

    bool typeIsArray(Assembler* as) override;
    QString getValue8bit(Assembler* as, bool isHi) override;



    QString getValue(Assembler* as) override;
    QString getLiteral(Assembler* as) override {
        return getValue(as);
        //if (m_forceAddress) return "#" + value;
        //return value;
    }

    bool isAddress() override;

    bool isSigned(Assembler* as) override;
/*    void LoadVariable(AbstractASTDispatcher* dispatcher) override;
    void LoadPointer(Assembler* as);

    void StoreVariable(AbstractASTDispatcher* dispatcher) override;
*/
    void ExecuteSym(QSharedPointer<SymbolTable> symTab) override;
    void Accept(AbstractASTDispatcher* dispatcher) override {
        dispatcher->dispatch(qSharedPointerDynamicCast<NodeVar>(sharedFromThis()));
    }

    void VerifyReferences(Assembler* as) override;

};
