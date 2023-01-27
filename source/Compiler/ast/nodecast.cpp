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

#include "nodecast.h"


NodeCast::NodeCast(Token op, QSharedPointer<Node> right):Node() {
    m_right = right;
    m_op = op;

    ApplyFlags();
}


void NodeCast::ExecuteSym(QSharedPointer<SymbolTable>  symTab) {
    m_right->ExecuteSym(symTab);
}


bool NodeCast::isPureNumeric() {
    if (m_right==nullptr)
        return false;

    return (m_right->isPureNumeric() &&
            !m_right->containsVariables()
            );
}

bool NodeCast::containsVariables() {
    if (m_right==nullptr)
        return false;

    return ((m_right->containsVariables()));
}

bool NodeCast::isPureNumericOrAddress()
{
    if (m_right==nullptr)
        return false;

    return (m_right->isPureNumericOrAddress());
}

bool NodeCast::is8bitValue(Assembler *as)
{
    return (m_op.m_type==TokenType::BYTE);
}

void NodeCast::ApplyFlags()
{
    m_right->ApplyFlags();
//    qDebug() << flags.keys();
}

bool NodeCast::isAddress() {
    return m_right->isAddress();
}




bool NodeCast::isWord(Assembler *as) {
    return ((m_right->isWord(as)) || (m_forceType==TokenType::INTEGER));
}

void NodeCast::setForceType(TokenType::Type t) {
//    qDebug() << "Binop set force type " << TokenType::getType(t);
    m_forceType  =t;
    m_right->setForceType(t);
}

/*void NodeCast::setForceTypeFunctions(TokenType::Type t) {
    m_left->setForceTypeFunctions(t);
    m_right->setForceTypeFunctions(t);
}*/
void NodeCast::setCastType(TokenType::Type t) {
    m_right->setCastType(t);
}

bool NodeCast::containsPointer(Assembler *as)
{
    return m_right->containsPointer(as);
}

bool NodeCast::isReference() {
    return m_right->isReference() || m_op.m_isReference; }

void NodeCast::parseConstants(QSharedPointer<SymbolTable>  symTab) {

 //   qDebug() << "NodeCast :: parse HERE1" << m_left->isPureNumeric() <<m_right->isPureNumeric();
//    int a = m_left->getValueAsInt(nullptr);
//    int b = m_right->getValueAsInt(nullptr);
 //   qDebug() << a << b;

    if (!isPureNumericOrAddress()) {
        if (m_right!=nullptr)
            m_right->parseConstants(symTab);
        return;
    }
//    qDebug() << "NodeCast :: parse HERE2";
//    m_value = numValue();
  //  m_isCollapsed = true;
/*    int a = m_left->getValueAsInt(nullptr);
    int b = m_right->getValueAsInt(nullptr);
*/
}

QString NodeCast::getValue(Assembler *as) {
    QString hash = "";
    if (!isAddress() && as!=nullptr)
        hash=as->m_hash;

    if (isAddress()) return HexValue();
    return hash + HexValue();
}

QString NodeCast::getValue8bit(Assembler *as, int isHi)
{   QString hash = "";
    if (as!=nullptr)
        hash = as->m_hash;

    if (isAddress()) hash="";

    int res = numValue();
    if (isHi==1)
        return hash + Util::numToHex((int)res>>8);
    else
        return hash + Util::numToHex(((int)res)&0xFF);


}

TokenType::Type NodeCast::getType(Assembler *as) {
    TokenType::Type a =m_right->getType(as);

    return m_op.m_type;

}

bool NodeCast::isPure() {
    if (isPureNumeric())
        return true;
    return false;
}

int NodeCast::numValue()
{
    if (m_op.m_type==TokenType::BYTE)
        return m_right->numValue()&0xFF;
    if (m_op.m_type==TokenType::INTEGER)
       return m_right->numValue()&0xFFFF;

    return m_right->numValue();
}





QString NodeCast::HexValue() {
    if (!isPureNumericOrAddress())
        return "";
    int res = numValue();
    //qDebug() << QString::number(res, 16);
    return "$" + QString::number(res, 16);
}



