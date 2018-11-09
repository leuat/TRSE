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


#include "source/Compiler/ast/nodenumber.h"
#include "source/Compiler/ast/nodeunaryop.h"

#include "source/Compiler/assembler/abstractastdispatcher.h"


class NodeBinOP : public Node {
public:

    NodeBinOP(Node* left, Token op, Node* right);
    void ExecuteSym(SymbolTable* symTab) override;

    QString BothConstants(Assembler* as);

    bool isPureNumeric() override;


    bool isAddress() override;


    bool isWord(Assembler* as) override;



    int numValue() override;

    QString HexValue() override;

    int BothPureNumbersBinOp(Assembler *as);


    void Accept(AbstractASTDispatcher* dispatcher) override {
        dispatcher->dispatch(this);
    };

};


#endif // NODEBINOP_H
