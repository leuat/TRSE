#ifndef NODEBINOP_H
#define NODEBINOP_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodevar.h"
#include "source/Compiler/ast/nodenumber.h"

class NodeBinOP : public Node {
public:

    NodeBinOP(Node* left, Token op, Node* right);
    PVar Execute(SymbolTable* symTab, uint lvl) override;
    void ExecuteSym(SymbolTable* symTab) override;

    QString BothConstants(Assembler* as);

    bool isPureNumeric() override;

    int getShiftCount(int i);






    void EightBitMul(Assembler* as);

    void EightBitDiv(Assembler* as);



    void LoadVariable(Assembler* as) override;


    bool isAddress() override;


    void BothPureNumbersBinOp(Assembler* as);


    void RightIsPureNumericMulDiv8bit(Assembler* as);

    void Mul16x8(Assembler* as);

    void Div16x8(Assembler* as);


    void RightIsPureNumericMulDiv16bit(Assembler* as);

    void HandleMulDiv(Assembler* as);


    bool HandleSingleAddSub(Assembler* as);

    bool isWord(Assembler* as) override;


    void HandleVarBinopB16bit(Assembler* as);

    void HandleGenericBinop16bit(Assembler* as);


    void HandleRestBinOp(Assembler* as);


    int numValue() override;

    QString HexValue() override;

    QString Build(Assembler *as) override;

};


#endif // NODEBINOP_H
