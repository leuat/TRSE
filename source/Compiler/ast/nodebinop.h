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

#ifndef NODEBINOP_H
#define NODEBINOP_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodevar.h"
#include "source/Compiler/ast/nodenumber.h"
#include "source/Compiler/ast/nodeunaryop.h"
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


    int BothPureNumbersBinOp(Assembler* as);
//    int BothPureNumbersBinOpOld(Assembler* as);


    void RightIsPureNumericMulDiv8bit(Assembler* as);

    void Mul16x8(Assembler* as);

    void Div16x8(Assembler* as);


    void RightIsPureNumericMulDiv16bit(Assembler* as);
    void HandleMulDiv(Assembler* as);


    bool HandleSingleAddSub(Assembler* as);

    bool isWord(Assembler* as) override;


    void HandleVarBinopB16bit(Assembler* as);
//    void HandleGenericBinOp16BitPureVar(Assembler* as);

    void HandleGenericBinop16bit(Assembler* as);


    void HandleRestBinOp(Assembler* as);


    int numValue() override;

    QString HexValue() override;

    QString Build(Assembler *as) override;

};


#endif // NODEBINOP_H
