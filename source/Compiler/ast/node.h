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
#include "source/Compiler/assembler/mos6502/mos6502.h"
#include "source/LeLib/data.h"
#include "source/Compiler/assembler/abstractastdispatcher.h"


class MemoryBlockInfo {
public:
    QString m_blockPos, m_blockName;
    int m_blockID;
    MemoryBlockInfo() {
        m_blockID = -1;
    }
};

class Node : public QEnableSharedFromThis<Node> {
public:
//    static QMap<QSharedPointer<Node>, QSharedPointer<Node>> s_uniqueSymbols;
    Token m_op;
//    int m_lineNumber;
    uint level = 0;
    static uint s_nodeCount;
    QString m_comment = "";
    BuiltInFunction::Type m_builtInFunctionParameterType = BuiltInFunction::BYTE;
    bool m_isUsed = false;
    QStringList m_isUsedBy;
    bool m_forceAddress = false;
    bool m_ignoreSuccess = false; // Used for binary expressions
    int m_forcePage = 0;
    static QString sForceFlag;
    bool m_isRegister = false;

    // Used to set various states, such as if binary operations are used etc
    static QMap<QString, bool> flags;
    static QSharedPointer<SymbolTable>  parserSymTab;

    QSharedPointer<Node> m_left = nullptr;
    QSharedPointer<Node> m_right = nullptr;

    void SwapNodes();

    bool m_isWord = false;
    static MemoryBlockInfo m_staticBlockInfo;
    static QSharedPointer<MemoryBlock> m_curMemoryBlock;

    virtual bool isReference() { return false;}

    virtual bool containsPointer(Assembler* as) {return false;}

    virtual void setForceType(TokenType::Type t) {
        m_forceType  =t;
    }


    virtual void ReplaceInline(Assembler* as,QMap< QString,QSharedPointer<Node>>& inp);

    virtual void ApplyFlags() {}

    MemoryBlockInfo m_blockInfo;
    Node();

    virtual bool isCompoundClause() { return false; }
    void DispatchConstructor(Assembler* as, AbstractASTDispatcher* dispatcher);

    int MaintainBlocks(Assembler* as);
    virtual bool isPointer(Assembler* as)  { return false;}
    virtual bool isPurePointer(Assembler* as)  { return false;}
    virtual bool is8bitValue(Assembler* as) { return true; }
    TokenType::Type m_forceType = TokenType::NADA;

    int m_cycleCounter;
    static int m_currentLineNumber;
    virtual void ExecuteSym(QSharedPointer<SymbolTable> symTab) = 0;
    virtual bool DataEquals(QSharedPointer<Node> other) { return false;}
    virtual QString HexValue() {return "0";}
    virtual int numValue() { return 0;}
    virtual void ForceAddress() {
        m_forceAddress = true;
        if (m_left!=nullptr)
            m_left->ForceAddress();
        if (m_right!=nullptr)
            m_right->ForceAddress();
    }

    virtual QString getAddress() {return "";}

    virtual void forceWord() {}
    virtual QString getTypeText(Assembler* as) {return "";}
    virtual bool isPure() {
        return isPureNumeric() || isPureVariable();
    }
    virtual bool isRecord(Assembler* as)  {
        return false;
    }
    virtual bool isRecordData(Assembler* as)  {
        return false;
    }

    virtual bool isPureNumeric() {
        return false;
    }
    virtual bool isPureVariable() { // Variable with no expressions
        return false;
    }
    virtual bool isVariable() { // Variable with possible expressions
        return false;
    }
/*    virtual void LoadVariable(AbstractASTDispatcher* dispatcher) {}
    virtual void StoreVariable(AbstractASTDispatcher* dispatcher) {}*/
    virtual TokenType::Type getType(Assembler* as) {
        return m_op.m_type;
    }
    virtual TokenType::Type getArrayType(Assembler* as) {
        return m_op.m_type;
    }
    virtual bool isArrayIndex() { return false; }
    virtual void Accept(AbstractASTDispatcher* dispatcher) = 0;
    virtual QString getLiteral(Assembler* as) {return "";}
    virtual bool isAddress() { return false;}
    virtual void AssignPointer(Assembler* as, QString memoryLocation) {}

    virtual QString getValue(Assembler* as) {return "";}
    virtual QString getValue8bit(Assembler* as, bool isHi) {return "";}
    virtual int getValueAsInt(Assembler* as) {
        return Util::NumberFromStringHex(getValue(as));
    }


    void RequireAddress(QSharedPointer<Node> n,QString name, int ln);

    void RequireNumber(QSharedPointer<Node> n,QString name, int ln) {
        if (!n->isPureNumeric())
            ErrorHandler::e.Error(name + " requires parameter to be pure numeric", ln);
    }

    virtual bool isWord(Assembler* as) { return false;}
    virtual bool isLong(Assembler* as) { return false;}
    virtual bool isByte(Assembler* as) { return false;}


    virtual bool isMinusOne() { return false; }
    virtual bool isOne() { return false; }

    bool verifyBlockBranchSize(Assembler *as, QSharedPointer<Node> testBlockA,QSharedPointer<Node> testBlockB, AbstractASTDispatcher* disp);
    virtual TokenType::Type VerifyAndGetNumericType();


    virtual void parseConstants(QSharedPointer<SymbolTable>  symTab) {

    }
    virtual bool typeIsArray(Assembler* as) {
        return false;
    }
    virtual bool typeIsArrayOfPointer(Assembler* as) {
        return false;
    }

    virtual void VerifyReferences(Assembler* as);
    virtual bool isSigned(Assembler* as);

};


class NoOp : public Node {
    public:
    void ExecuteSym(QSharedPointer<SymbolTable> symTab) override {

    }
    void Accept(AbstractASTDispatcher* dispatcher) override {
        dispatcher->dispatch(sharedFromThis());
    }

};

class NodeComment : public Node {
    public:
    QString m_comment="";
    NodeComment(QString com) {
        m_comment = com;
    }
    void ExecuteSym(QSharedPointer<SymbolTable> symTab) override {

    }
    void Accept(AbstractASTDispatcher* dispatcher) override {
        dispatcher->dispatch(sharedFromThis());
    }


};


#endif // NODE_H
