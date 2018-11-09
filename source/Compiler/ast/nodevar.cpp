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

#include "nodevar.h"


NodeVar::NodeVar(Token t):Node() {
    m_op = t;
    value = t.m_value;
}

NodeVar::NodeVar(Token t, Node *expr) : Node() {
    m_op = t;
    value = t.m_value;
    m_expr = expr;
    //  if (m_op.m_type==TokenType::INTEGER)
    //     m_isWord = true;

}


TokenType::Type NodeVar::getType(Assembler *as) {
    if (m_forceType!=TokenType::NADA)
        return m_forceType;
    if (as->m_symTab->Lookup(value, m_op.m_lineNumber)!=nullptr)
        return as->m_symTab->Lookup(value, m_op.m_lineNumber)->getTokenType();
    return m_op.m_type;
}

bool NodeVar::DataEquals(Node *other) {
    NodeVar* var = dynamic_cast<NodeVar*>(other);
    if (var==nullptr)
        return false;
    return var->value==value;
}

bool NodeVar::isWord(Assembler *as) {


    return getType(as)==TokenType::INTEGER;
    return m_op.m_type==TokenType::INTEGER;
    Symbol* s = as->m_symTab->Lookup(value, m_op.m_lineNumber);

    return s->m_type.toLower()=="integer";
}


bool NodeVar::isAddress() {
    return true;
}


void NodeVar::ExecuteSym(SymbolTable *symTab) {
    QString varName = m_op.m_value;
    Symbol* varSymbol = symTab->Lookup(varName, m_op.m_lineNumber);

}
