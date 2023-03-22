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
#include "source/Compiler/assembler/asm6502.h"
#include "source/LeLib/data.h"
#include "source/Compiler/codegen/abstractcodegen.h"


class MemoryBlockInfo {
public:
    QString m_blockPos, m_blockName;
    int m_blockID;
    MemoryBlockInfo() {
        m_blockID = -1;
    }
};
/*
 *
 * The main base class for the entire Abstract Syntax Tree (AST) in TRSE
 * Every node must inherit from this base class.
 *
 *
 * */
class Node : public QEnableSharedFromThis<Node> {
protected:
    // Toggle nodes as "used" and "used by" - necessary for the optimizer for
    // automatic removal of nodes
    bool m_hasSwapped = false;
    bool m_isUsed = false;
    QStringList m_isUsedBy;

//    int m_lineNumber;
    uint level = 0;
    // Comments associated with current node.
    QString m_comment = "";

    static Assembler* s_as;
    // Used in x86 to specity if is an index or not
    bool m_isIndex = false;
    // Forced values
    bool m_forceAddress = false;
    static QString sForceFlag;

    static QSharedPointer<SymbolTable>  parserSymTab;

    // Current block information
    static MemoryBlockInfo m_staticBlockInfo;
    // Used to set various states, such as if binary operations are used etc
    static QMap<QString, bool> flags;


    bool m_ignoreSuccess = false; // Used for binary expressions
    bool m_classApplied = false;
    static uint s_nodeCount;
    MemoryBlockInfo m_blockInfo;
    TokenType::Type m_loadType = TokenType::NADA;
    TokenType::Type m_storeType = TokenType::NADA;

public:
    friend class Parser;
    // Token contains node type data and values from the parser
    Token m_op;
    // Base node has 2 children: left and right
    QSharedPointer<Node> m_left = nullptr;
    QSharedPointer<Node> m_right = nullptr;
    // Is the current node a register? (applicable to variables only)
    bool m_isRegister = false;
    static QSharedPointer<MemoryBlock> m_curMemoryBlock;
    bool m_isBoolean = false;
    bool m_isWord = false;
    // Force page for conditionals (while/if/repeat until etc)
    int m_forcePage = 0;
    BuiltInFunction::Type m_builtInFunctionParameterType = BuiltInFunction::BYTE;
    // Line number for keeping track of current cycles
    static int m_currentLineNumber;

    /*
     *
     *  Methods
     *
     * */
    // Swaps left and right nodes
    void SwapNodes();

    Node();
    // called manually on each dispatch visitor
    void DispatchConstructor(Assembler* as, AbstractCodeGen* dispatcher);
    // Makes sure that the node and blocks are in sync
    int MaintainBlocks(Assembler* as);

    virtual TokenType::Type getLoadType() { return m_loadType ;}
    virtual TokenType::Type getStoreType() { return m_storeType ;}
    // And now for a ton of methods that can/should be implemented by all the subclasses


    virtual void ForceAddress();
    // Returns a list of potential symols in asm code. Recursive. Used for preventing removal of unused symbols that are actually used within asm blocks
    virtual void FindPotentialSymbolsInAsmCode(QStringList& lst);

    // returns whether node is a reference # or not
    virtual bool isReference() { return false;}
    virtual void setReference(bool ref);

    // Does the expression contain a pointer? (ie turn 16/32 bit)
    virtual bool containsPointer(Assembler* as) {return false;}

    // Force a specific type to be set for this node
    virtual void setLoadType(TokenType::Type t) {
        m_loadType = t;
    }
    // Force a specific type to be set for this node
    virtual void setStoreType(TokenType::Type t) {
        m_storeType  = t;
    }
    virtual void clearComment();
    // Replaces all variables of a given name within the subnodes with another one
    virtual void ReplaceVariable(Assembler* as, QString name, QSharedPointer<Node> node);
    virtual bool isStackVariable() { return false;}
    virtual int getStackShift() { return 0;}


    // Replaces inline variables with the macro parameter
    virtual void ReplaceInline(Assembler* as,QMap< QString,QSharedPointer<Node>>& inp);

    // Apply type flags to node
    virtual void ApplyFlags() {}

    // Only returns true of is a compound clause
    virtual bool isCompoundClause() { return false; }

    bool m_isCollapsed = false;

    virtual bool isPointer(Assembler* as)  { return false;}
    virtual bool isPurePointer(Assembler* as)  { return false;}
    virtual bool is8bitValue(Assembler* as) { return true; }
    virtual void ExecuteSym(QSharedPointer<SymbolTable> symTab) = 0;
    virtual bool DataEquals(QSharedPointer<Node> other) { return false;}
    virtual QString HexValue() {return "0";}
    virtual int numValue() { return 0;}

    virtual QString getAddress() {return "";}
    // Writetype is used for writing data to class objects
    virtual TokenType::Type getClassvariableType();

    virtual void forceWord() {}
    virtual QString getTypeText(Assembler* as) {return "";}
    // Is the node a pure variable or number?
    virtual bool isPure();
    // Is ita record (or a class)
    virtual bool isRecord(Assembler* as)  {
        return false;
    }
    virtual bool isRecord(QSharedPointer<SymbolTable> s, QString& typ) {typ=""; return false;}
    virtual bool isClass(Assembler* as)  {
        return false;
    }
    virtual bool isRecordData(Assembler* as)  {
        return false;
    }
    virtual bool isBool(Assembler* as){
        return false;
    }

    virtual bool isPureNumeric() {
        return false;
    }
    virtual bool containsVariables() {
        return false;
    }
    virtual bool isPureNumericOrAddress() {
        return false;
    }
    virtual bool isPureVariable() { // Variable with no expressions
        return false;
    }
    virtual bool isVariable() { // Variable with possible expressions
        return false;
    }
    virtual TokenType::Type getType(Assembler* as);
    virtual TokenType::Type getArrayType(Assembler* as) {
        return m_op.m_type;
    }
    virtual bool hasArrayIndex() { return false; }
    virtual void Accept(AbstractCodeGen* dispatcher) = 0;
    virtual QString getLiteral(Assembler* as) {return "";}
    virtual bool isAddress() { return false;}
    virtual void AssignPointer(Assembler* as, QString memoryLocation) {}

    virtual QString getValue(Assembler* as) {return "";}
    virtual QString getValue8bit(Assembler* as, int isHi) {return "";}
    virtual ulong getValueAsInt(Assembler* as) {
        return Util::NumberFromStringHex(getValue(as));
    }
    virtual int getArrayDataSize(Assembler* as);

    virtual TokenType::Type getOrgType(Assembler *as) {
        return m_op.m_type;
    }

    void RequireAddress(QSharedPointer<Node> n,QString name, int ln);

    void RequireNumber(QSharedPointer<Node> n,QString name, int ln);

    virtual bool isWord(Assembler* as) { return false;}
    virtual bool isStringList(Assembler* as) {return false;}
    virtual bool isLong(Assembler* as) { return false;}
    virtual bool isByte(Assembler* as) { return false;}


    virtual bool isMinusOne() { return false; }
    virtual bool isOne() { return false; }

    bool verifyBlockBranchSize(Assembler *as, QSharedPointer<Node> testBlockA,QSharedPointer<Node> testBlockB, AbstractCodeGen* disp);
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

    virtual bool hasFlag(Assembler* as, QString flag);
    inline static void setAssembler(Assembler * as){s_as = as;}
    inline QList<QString> getFlagKeys(){return flags.keys();}
    inline void ignoreSuccess(){m_ignoreSuccess=false;}
    inline void dontIgnoreSuccess(){m_ignoreSuccess=true;}
    inline static uint getNodeCount(){return s_nodeCount;}
    inline bool isClassApplied(){return m_classApplied;}
    inline MemoryBlockInfo getBlockInfo(){return m_blockInfo;}
    
};

inline TokenType::Type Node::getType(Assembler *as) {
    if (m_op.m_isBoolean)
        return TokenType::BOOLEAN;
    return m_op.m_type;
}



class NoOp : public Node {
public:
    void ExecuteSym(QSharedPointer<SymbolTable> symTab) override {

    }
    void Accept(AbstractCodeGen* dispatcher) override {
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
    void Accept(AbstractCodeGen* dispatcher) override {
        dispatcher->dispatch(sharedFromThis());
    }


};


#endif // NODE_H
