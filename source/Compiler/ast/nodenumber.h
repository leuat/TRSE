#ifndef NODENUMBER_H
#define NODENUMBER_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"


class NodeNumber : public Node {
public:
    float m_val;
    NodeNumber(Token op, int val);
    PVar Execute(SymbolTable* symTab, uint lvl) override;
    void ExecuteSym(SymbolTable* symTab) override {
    }


    bool isAddress() override;

    bool isPureNumeric() override;

    bool isWord(Assembler* as) override;

    void LoadVariable(Assembler* as) override;

    int numValue() { return m_val;}

    QString HexValue() override;

    bool DataEquals(Node *other) override;



    QString StringValue();

    bool isMinusOne() override;

    bool isOne() override;
    QString Build(Assembler *as) override;


};

#endif // NODENUMBER_H
