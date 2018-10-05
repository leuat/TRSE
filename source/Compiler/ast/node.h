#ifndef NODE_H
#define NODE_H

#include "source/Compiler/token.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/assembler/assembler.h"
#include "source/Compiler/assembler/mos6502.h"


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

    Node* m_left = nullptr, *m_right = nullptr;
    bool m_isWord = false;
    static MemoryBlockInfo m_staticBlockInfo;
    static MemoryBlock* m_curMemoryBlock;

    MemoryBlockInfo m_blockInfo;
    Node() {
        m_blockInfo = m_staticBlockInfo;
    }

    int MaintainBlocks(Assembler* as);

    TokenType::Type m_forceType = TokenType::NADA;

    int m_cycleCounter;
    static int m_currentLineNumber;
    virtual PVar Execute(SymbolTable* symTab, uint lvl) = 0;
    virtual void ExecuteSym(SymbolTable* symTab) = 0;
    virtual bool DataEquals(Node* other) { return false;}
    virtual QString HexValue() {return "0";}
    virtual int numValue() { return 0;}
    virtual void Delete();
    virtual bool isPureNumeric() {
        return false;
    }
    virtual QString Build(Assembler* as);
    virtual void LoadVariable(Assembler* a) {}
    virtual void StoreVariable(Assembler* a) {}
    virtual TokenType::Type getType(Assembler* as) {
        return m_op.m_type;
    }

    virtual bool isAddress() { return false;}
    virtual void AssignPointer(Assembler* as, QString memoryLocation) {}

    void RequireAddress(Node* n,QString name, int ln);

    void RequireNumber(Node* n,QString name, int ln) {
        if (!n->isPureNumeric())
            ErrorHandler::e.Error(name + " requires parameter to be pure numeric", ln);
    }

    virtual bool isWord(Assembler* as) { return false;}

   virtual bool isMinusOne() { return false; }
    virtual bool isOne() { return false; }

    bool verifyBlockBranchSize(Assembler *as, Node* testBlock);


};


class NoOp : public Node {
    public:
    PVar Execute(SymbolTable* symTab, uint lvl) override {
        level = lvl+1;
        return PVar();
    }
    void ExecuteSym(SymbolTable* symTab) override {

    }
};

#endif // NODE_H
