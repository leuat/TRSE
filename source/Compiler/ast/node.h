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

#ifndef NODE_H
#define NODE_H

#include "source/Compiler/token.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/assembler/assembler.h"
#include "source/Compiler/assembler/mos6502.h"

#include "source/Compiler/assembler/abstractastdispatcher.h"


class MemoryBlockInfo {
public:
    QString m_blockPos, m_blockName;
    int m_blockID;
    MemoryBlockInfo() {
        m_blockID = -1;
    }
};

class Node {
public:
    Token m_op;
    int m_lineNumber;
    uint level = 0;
    bool m_isUsed = false;
    bool m_forceAddress = false;



    Node* m_left = nullptr, *m_right = nullptr;
    bool m_isWord = false;
    static MemoryBlockInfo m_staticBlockInfo;
    static MemoryBlock* m_curMemoryBlock;

    MemoryBlockInfo m_blockInfo;
    Node() {
        m_blockInfo = m_staticBlockInfo;
    }


    void DispatchConstructor() {
//        m_blockInfo = m_staticBlockInfo;
        m_currentLineNumber = m_op.m_lineNumber;
    }

    int MaintainBlocks(Assembler* as);
    virtual bool isPointer(Assembler* as)  { return false;}

    TokenType::Type m_forceType = TokenType::NADA;

    int m_cycleCounter;
    static int m_currentLineNumber;
    virtual void ExecuteSym(SymbolTable* symTab) = 0;
    virtual bool DataEquals(Node* other) { return false;}
    virtual QString HexValue() {return "0";}
    virtual int numValue() { return 0;}
    virtual void Delete();
    virtual void ForceAddress() {
        m_forceAddress = true;
        if (m_left!=nullptr)
            m_left->ForceAddress();
        if (m_right!=nullptr)
            m_right->ForceAddress();
    }

    virtual QString getAddress() {return "";}
    virtual int getInteger() {
   //     ErrorHandler::e.Error("Parameter is required to be pure number",m_op.m_lineNumber);
        return 0;
    }

    virtual void forceWord() {}

    virtual bool isPure() {
        return isPureNumeric() || isPureVariable();
    }

    virtual bool isPureNumeric() {
        return false;
    }
    virtual bool isPureVariable() {
        return false;
    }
/*    virtual void LoadVariable(AbstractASTDispatcher* dispatcher) {}
    virtual void StoreVariable(AbstractASTDispatcher* dispatcher) {}*/
    virtual TokenType::Type getType(Assembler* as) {
        return m_op.m_type;
    }
    virtual bool isArrayIndex() { return false; }
    virtual void Accept(AbstractASTDispatcher* dispatcher) = 0;
    virtual QString getLiteral(Assembler* as) {return "";}
    virtual bool isAddress() { return false;}
    virtual void AssignPointer(Assembler* as, QString memoryLocation) {}

    virtual QString getValue(Assembler* as) {return "";}

    void RequireAddress(Node* n,QString name, int ln);

    void RequireNumber(Node* n,QString name, int ln) {
        if (!n->isPureNumeric())
            ErrorHandler::e.Error(name + " requires parameter to be pure numeric", ln);
    }

    virtual bool isWord(Assembler* as) { return false;}
    virtual bool isLong(Assembler* as) { return false;}
    virtual bool isByte(Assembler* as) { return false;}

   virtual bool isMinusOne() { return false; }
    virtual bool isOne() { return false; }

    bool verifyBlockBranchSize(Assembler *as, Node* testBlock);


};


class NoOp : public Node {
    public:
    void ExecuteSym(SymbolTable* symTab) override {

    }
    void Accept(AbstractASTDispatcher* dispatcher) override {
        dispatcher->dispatch(this);
    }

};

#endif // NODE_H
