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

#include "nodevararray.h"

NodeVarArray::NodeVarArray(Token token, Node *expr):Node() {
    m_var = new NodeVar(token);
    m_expr = expr;
}

PVar NodeVarArray::Execute(SymbolTable *symTab, uint lvl) {
    return PVar();
}

QString NodeVarArray::Build(Assembler *as) {
    return "";

    m_expr->Build(as);
    as->Asm("tax");
    as->Term("lda ");
    m_var->Build(as);
    as->Term(",x",true);
    qDebug() << "end";
    return "";

}

void NodeVarArray::StoreAcc(Assembler *as) {
    as->Asm("tay");
    m_expr->Build(as);
    as->Asm("tax");
    as->Asm("tya");
    as->Term("sta ");
    m_var->Build(as);
    as->Term(",x",true);



}

void NodeVarArray::ExecuteSym(SymbolTable *symTab) {
    if (m_var!=nullptr)
        m_var->ExecuteSym(symTab);
    if (m_expr!=nullptr)
        m_expr->ExecuteSym(symTab);

}
