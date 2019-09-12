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

#include "nodebinop.h"


NodeBinOP::NodeBinOP(Node *left, Token op, Node *right):Node() {
    m_right = right;
    m_left = left;
    m_op = op;
    ApplyFlags();
}


void NodeBinOP::ExecuteSym(SymbolTable *symTab) {
    m_left->ExecuteSym(symTab);
    m_right->ExecuteSym(symTab);
}


bool NodeBinOP::isPureNumeric() {
    if (m_left==nullptr || m_right==nullptr)
        return false;

    return (m_left->isPureNumeric() && m_right->isPureNumeric());
}

bool NodeBinOP::is8bitValue(Assembler *as)
{
    return (m_right->is8bitValue(as) && m_left->is8bitValue(as));
}

void NodeBinOP::ApplyFlags()
{
//    qDebug() << "::NodeBinop Applying Flags";
    bool a = m_left->isWord(nullptr);
    bool b = m_right->isWord(nullptr);

   // qDebug() << "::NodeBinop a b " << a << b;

    if (m_op.m_type==TokenType::MUL) {
     //   if (a || b)
            flags["mul16"] = true;
       // if (!a && !b)
            flags["mul8"] = true;
    }
    if (m_op.m_type==TokenType::DIV) {
       // if (a || b)
            flags["div16"] = true;
        //if (!a && !b)
            flags["div8"] = true;
    }
    m_left->ApplyFlags();
    m_right->ApplyFlags();
//    qDebug() << flags.keys();
}

bool NodeBinOP::isAddress() {
    return m_left->isAddress() && m_right->isAddress();
}




bool NodeBinOP::isWord(Assembler *as) {
    return ((m_left->isWord(as) || m_right->isWord(as)) || (m_forceType==TokenType::INTEGER));
}

void NodeBinOP::setForceType(TokenType::Type t) {
//    qDebug() << "Binop set force type " << TokenType::getType(t);
    m_forceType  =t;
    m_left->setForceType(t);
    m_right->setForceType(t);
}

bool NodeBinOP::containsPointer(Assembler *as)
{
    return m_left->containsPointer(as) || m_right->containsPointer(as);
}

void NodeBinOP::parseConstants(SymbolTable *symTab) {

 //   qDebug() << "NodeBinOp :: parse HERE1" << m_left->isPureNumeric() <<m_right->isPureNumeric();
//    int a = m_left->getValueAsInt(nullptr);
//    int b = m_right->getValueAsInt(nullptr);
 //   qDebug() << a << b;

    if (!isPureNumeric()) {
        if (m_left!=nullptr)
            m_left->parseConstants(symTab);
        if (m_right!=nullptr)
            m_right->parseConstants(symTab);
        return;
    }
//    qDebug() << "NodeBinOp :: parse HERE2";
//    m_value = numValue();
  //  m_isCollapsed = true;
/*    int a = m_left->getValueAsInt(nullptr);
    int b = m_right->getValueAsInt(nullptr);
*/
}



int NodeBinOP::numValue() {
    if (!isPureNumeric())
        return 0;
    int a = m_left->numValue();
    int b = m_right->numValue();
    int res = 0;
    if (m_op.m_type==TokenType::PLUS)
        res=a+b;
    if (m_op.m_type==TokenType::MINUS)
        res=a-b;
    if (m_op.m_type==TokenType::MUL)
        res=a*b;
    if (m_op.m_type==TokenType::DIV)
        res=a/b;

    return res;
}

QString NodeBinOP::HexValue() {
    if (!isPureNumeric())
        return "";
    int res = numValue();
    //qDebug() << QString::number(res, 16);
    return "$" + QString::number(res, 16);
}

int NodeBinOP::BothPureNumbersBinOp(Assembler *as) {


    //NodeNumber *a = (NodeNumber*)dynamic_cast<const NodeNumber*>(m_left);
    //NodeNumber *b = (NodeNumber*)dynamic_cast<const NodeNumber*>(m_right);
    //BothConstants(as);
    if (dynamic_cast<NodeUnaryOp*>(m_left)!=nullptr) {
        NodeNumber *b = (NodeNumber*)dynamic_cast<const NodeNumber*>(m_right);
        if (m_left->m_op.m_type==TokenType::MINUS) {
            return b->m_val*-1;
        }
        else
            if (m_left->m_op.m_type==TokenType::PLUS) {
                return b->m_val;
            }
        ErrorHandler::e.Error("Unknown binary operation!", m_op.m_lineNumber);

    }

    int na=0;
    int nb=0;
    if (dynamic_cast<NodeBinOP*>(m_left)!=nullptr)
        na = dynamic_cast<NodeBinOP*>(m_left)->BothPureNumbersBinOp(as);
    else
        na = dynamic_cast<NodeNumber*>(m_left)->m_val;
    if (dynamic_cast<NodeBinOP*>(m_right)!=nullptr)
        nb = dynamic_cast<NodeBinOP*>(m_right)->BothPureNumbersBinOp(as);
    else
        nb = dynamic_cast<NodeNumber*>(m_right)->m_val;


    if (m_op.m_type==TokenType::PLUS)
        na+=nb;
    if (m_op.m_type==TokenType::MINUS)
        na-=nb;
    if (m_op.m_type==TokenType::DIV) {
        if (nb==0) {
            ErrorHandler::e.Error("Binary operation: Division by zero!", m_op.m_lineNumber);
        }
        else
            na/=nb;
    }
    if (m_op.m_type==TokenType::MUL)
        na*=nb;
    if (m_op.m_type==TokenType::BITAND)
        na&=nb;
    if (m_op.m_type==TokenType::BITOR)
        na|=nb;

    return na;

}


