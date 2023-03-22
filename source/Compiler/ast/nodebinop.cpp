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


NodeBinOP::NodeBinOP(QSharedPointer<Node> left, Token op, QSharedPointer<Node> right):Node() {
    m_right = right;
    m_left = left;
    m_op = op;

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::GAMEBOY)
    if (m_right->isPureNumericOrAddress()) {
        int val = m_right->getValueAsInt(nullptr);
        if (power2.contains(val)) {
            int idx = power2.indexOf(val);
            if (m_op.m_type == TokenType::MUL) {
                m_op.m_type = TokenType::SHL;
                m_right = QSharedPointer<NodeNumber>(new NodeNumber(right->m_op,idx));
//                qDebug() << "Replacing a MUL " << val << " with SHL "<<idx;
            }else
            if (m_op.m_type == TokenType::DIV) {
                m_op.m_type = TokenType::SHR;
                m_right = QSharedPointer<NodeNumber>(new NodeNumber(right->m_op,idx));
  //              qDebug() << "Replacing a DIV " << val << " with SHR "<<idx;
            }
        }
    }
    ApplyFlags();
}


void NodeBinOP::ExecuteSym(QSharedPointer<SymbolTable>  symTab) {
    m_left->ExecuteSym(symTab);
    m_right->ExecuteSym(symTab);
}


bool NodeBinOP::isPureNumeric() {
    if (m_left==nullptr || m_right==nullptr)
        return false;

    return (m_left->isPureNumeric() && m_right->isPureNumeric() &&
            !m_left->containsVariables() && !m_right->containsVariables()

            );
}

bool NodeBinOP::containsVariables() {
    if (m_left==nullptr || m_right==nullptr)
        return false;

    return ((m_left->containsVariables() || m_right->containsVariables()));
}

bool NodeBinOP::isPureNumericOrAddress()
{
    if (m_left==nullptr || m_right==nullptr)
        return false;

    return (m_left->isPureNumericOrAddress() && m_right->isPureNumericOrAddress());
}

bool NodeBinOP::is8bitValue(Assembler *as)
{
    return (m_right->is8bitValue(as) && m_left->is8bitValue(as));
}

void NodeBinOP::ApplyFlags()
{
    if (m_op.m_type==TokenType::MUL) {
     //   if (a || b)
        // Crash here.. wtf??
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
    return m_left->isAddress() || m_right->isAddress();
}




bool NodeBinOP::isWord(Assembler *as) {
    return ((m_left->isWord(as) || m_right->isWord(as)) || (m_loadType==TokenType::INTEGER));
}

bool NodeBinOP::isLong(Assembler *as) {
    return ((m_left->isLong(as) || m_right->isLong(as)) || (m_loadType==TokenType::LONG));
}

void NodeBinOP::setLoadType(TokenType::Type t) {
//    qDebug() << "Binop set force type " << TokenType::getType(t);
    m_loadType  =t;
    m_left->setLoadType(t);
    m_right->setLoadType(t);
}

/*void NodeBinOP::setLoadTypeFunctions(TokenType::Type t) {
    m_left->setLoadTypeFunctions(t);
    m_right->setLoadTypeFunctions(t);
}*/
void NodeBinOP::setStoreType(TokenType::Type t) {
    m_left->setStoreType(t);
    m_right->setStoreType(t);
}

TokenType::Type NodeBinOP::getClassvariableType() {
    if (m_left->getClassvariableType()==TokenType::LONG || m_right->getClassvariableType()==TokenType::LONG)
        return TokenType::LONG;
    if (m_left->getClassvariableType()==TokenType::INTEGER || m_right->getClassvariableType()==TokenType::INTEGER)
        return TokenType::INTEGER;
    if (m_left->getClassvariableType()==TokenType::BYTE || m_right->getClassvariableType()==TokenType::BYTE)
        return TokenType::BYTE;
    return TokenType::NADA;
}

bool NodeBinOP::containsPointer(Assembler *as)
{
    return m_left->containsPointer(as) || m_right->containsPointer(as);
}

void NodeBinOP::SwapVariableFirst()
{
    if (m_op.m_type==TokenType::PLUS || m_op.m_type==TokenType::MUL) {
        QSharedPointer<NodeVar> isRight = qSharedPointerDynamicCast<NodeVar>(m_right);
        if (isRight) {
            QSharedPointer<Node> tmp = m_right;
            m_right = m_left;
            m_left = tmp;
        }
    }
}

bool NodeBinOP::isReference() {
    return m_left->isReference() || m_right->isReference() || m_op.m_isReference; }

bool NodeBinOP::ContainsVariable(Assembler *as, QString var)
{
    if (m_right->isPureVariable() && m_right->getValue(as)==var) {
        // Only switch if PLUS
        if (m_op.m_type == TokenType::PLUS) {
            QSharedPointer<Node> n = m_left;
            m_left = m_right;
            m_right = n;
            return true;
        }
    }

    if (m_left->isPureVariable() && m_left->getValue(as)==var) {
        return true;
    }
    return false;
}

void NodeBinOP::parseConstants(QSharedPointer<SymbolTable>  symTab) {

    if (!isPureNumericOrAddress()) {
        if (m_left!=nullptr)
            m_left->parseConstants(symTab);
        if (m_right!=nullptr)
            m_right->parseConstants(symTab);
        return;
    }
}

QString NodeBinOP::getValue(Assembler *as) {
    QString hash = "";
    if (!isAddress() && as!=nullptr)
        hash=as->m_hash;
    if (m_isCollapsed)
        return hash + "$" + QString::number(m_value, 16);

    if (isAddress()) return HexValue();
    return hash + HexValue();
}

QString NodeBinOP::getValue8bit(Assembler *as, int isHi)
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

TokenType::Type NodeBinOP::getType(Assembler *as) {
    TokenType::Type a =m_right->getType(as);
    TokenType::Type b =m_left->getType(as);


    if (a==TokenType::LONG || b==TokenType::LONG)
        return TokenType::LONG;
    if (a==TokenType::INTEGER || b==TokenType::INTEGER)
        return TokenType::INTEGER;
    return TokenType::BYTE;

}

QString NodeBinOP::getTypeText(Assembler *as) {
    return m_left->getTypeText(as);
}

bool NodeBinOP::isPure() {
    if (isPureNumeric())
        return true;
    return false;
}

TokenType::Type NodeBinOP::VerifyAndGetNumericType() {
    TokenType::Type a =m_right->VerifyAndGetNumericType();
    TokenType::Type b =m_left->VerifyAndGetNumericType();
    if (a!=b) {
/*        if (a==TokenType::ADDRESS || b==TokenType::ADDRESS)
            return TokenType::ADDRESS;*/
        if (a==TokenType::ADDRESS)
            return a;
        if (b==TokenType::ADDRESS)
            return b;

        ErrorHandler::e.Error("Binary operations must occur between same token types ("+TokenType::getType(a)+" vs "+TokenType::getType(b)+")", m_op.m_lineNumber);

    }
    return a;
}

QString NodeBinOP::getStringOperation() {
    if (m_op.m_type == TokenType::PLUS)
        return "+";
    if (m_op.m_type == TokenType::MINUS)
        return "-";
    if (m_op.m_type == TokenType::MUL)
        return "*";
    if (m_op.m_type == TokenType::DIV)
        return "/";
    if (m_op.m_type == TokenType::AND)
        return "&";
    if (m_op.m_type == TokenType::OR)
        return "|";

    return "";
}

QString NodeBinOP::getLiteral(Assembler *as) {
    return m_left->getLiteral(as) + getStringOperation() + m_right->getLiteral(as);
}



int NodeBinOP::numValue() {
    if (!isPureNumericOrAddress())
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
    if (m_op.m_type==TokenType::BITAND)
        res=a&b;
    if (m_op.m_type==TokenType::BITOR)
        res=a|b;
    if (m_op.m_type==TokenType::XOR)
        res=a^b;
    if (m_op.m_type==TokenType::SHR)
        res=a>>b;
    if (m_op.m_type==TokenType::SHL)
        res=a<<b;

    return res;
}

QString NodeBinOP::HexValue() {
    if (!isPureNumericOrAddress())
        return "";
    int res = numValue();
    //qDebug() << QString::number(res, 16);
    return "$" + QString::number(res, 16);
}

int NodeBinOP::BothPureNumbersBinOp(Assembler *as) {


    if (qSharedPointerDynamicCast<NodeUnaryOp>(m_left)!=nullptr) {
        QSharedPointer<NodeNumber>b = qSharedPointerDynamicCast<NodeNumber>(m_right);
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
    if (qSharedPointerDynamicCast<NodeBinOP>(m_left)!=nullptr)
        na = qSharedPointerDynamicCast<NodeBinOP>(m_left)->BothPureNumbersBinOp(as);
    else
        na = qSharedPointerDynamicCast<NodeNumber>(m_left)->m_val;

    if (qSharedPointerDynamicCast<NodeBinOP>(m_right)!=nullptr)
        nb = qSharedPointerDynamicCast<NodeBinOP>(m_right)->BothPureNumbersBinOp(as);
    else
        nb = qSharedPointerDynamicCast<NodeNumber>(m_right)->m_val;


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
    if (m_op.m_type==TokenType::SHR)
        na>>=nb;
    if (m_op.m_type==TokenType::SHL)
        na<<=nb;


    return na;

}


