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

#include "nodeprocedure.h"


NodeProcedure::NodeProcedure(QSharedPointer<NodeProcedureDecl> proc, QVector<QSharedPointer<Node> > params, Token t):Node() {
    m_parameters = params;
    m_procedure = proc;
    m_op = t;
}

void NodeProcedure::ReplaceVariable(Assembler *as, QString name, QSharedPointer<Node> node) {
    int i=0;
    for (auto p:m_parameters) {
        p->ReplaceVariable(as,name,node);
        if (p->isPureVariable() && p->getValue(as)==name)
            m_parameters[i] = node;

        i++;
    }
}

bool NodeProcedure::isAddress() {
    return m_op.m_isReference;
}


void NodeProcedure::ExecuteSym(QSharedPointer<SymbolTable>  symTab) {
    m_procedure ->ExecuteSym(symTab);
}

QString NodeProcedure::getValue(Assembler *as)
{

    if (m_procedure==nullptr)
        ErrorHandler::e.Error("NodeProcedure Getvalue m_procedure is empty!",m_op.m_lineNumber);

    if (m_op.m_isReference)
        return "#" + m_procedure->m_procName;
    else
        return m_procedure->m_procName;

}

QString NodeProcedure::getValue8bit(Assembler *as,  int isHi)
{
    QString v = "<";
    if (isHi==1) v=">";
    if (isHi==2) v="^";

    if (m_op.m_isReference)
        return v+"#" + m_procedure->m_procName;
    else
        return v+m_procedure->m_procName;

}

bool NodeProcedure::isWord(Assembler *as)
{
    if (m_procedure->m_returnType!=nullptr)
        return m_procedure->m_returnType->getType(as)==TokenType::INTEGER;

    return false;
}

bool NodeProcedure::isLong(Assembler *as)
{
    if (m_procedure->m_returnType!=nullptr)
        return m_procedure->m_returnType->getType(as)==TokenType::LONG;

    return false;

}

bool NodeProcedure::isByte(Assembler *as)
{
    if (m_procedure->m_returnType!=nullptr)
        return m_procedure->m_returnType->getType(as)==TokenType::BYTE;

    return true;

}

void NodeProcedure::ReplaceInline(Assembler* as,QMap<QString, QSharedPointer<Node> > &inp)
{
    m_procedure->ReplaceInline(as,inp);
}

bool NodeProcedure::isPureNumeric() {
    return false;
    return isReference();
}
