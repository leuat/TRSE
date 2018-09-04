#pragma once

#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodenumber.h"
#include "source/Compiler/ast/nodevartype.h"

class NodeVar : public Node {
public:
    QString value;
    Node* m_expr = nullptr;
    //NodeVarType* m_type;


    NodeVar(Token t);

    NodeVar(Token t, Node* expr);

    PVar Execute(SymbolTable* symTab, uint lvl) override;


    TokenType::Type getType(Assembler* as) override;


    bool DataEquals(Node *other) override;
    bool isWord(Assembler* as) override;

    void LoadPointer(Assembler* as);

    virtual void AssignPointer(Assembler *as, QString memoryLocation);

    bool LoadXYVarOrNum(Assembler* as, Node* node, bool isx);

    void LoadByteArray(Assembler *as);

    bool isAddress() override;

    void LoadVariable(Assembler* as) override;

    void StoreVariable(Assembler* as) override;

    QString Build(Assembler *as) override;
    void ExecuteSym(SymbolTable* symTab) override;

};
