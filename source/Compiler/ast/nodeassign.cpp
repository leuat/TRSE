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

#include "nodeassign.h"


PVar NodeAssign::Execute(SymbolTable *symTab, uint lvl) {
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
    ErrorHandler::e.DebugLow("Calling Assign",level);
    level = lvl+1;

    QString varName = ((NodeVar*)m_left)->value;
    ErrorHandler::e.DebugHigh("Defining new variable : " + varName,level);
    Symbol* s = symTab->Lookup(varName, m_op.m_lineNumber);
    s->m_value = new PVar(m_right->Execute(symTab, level));

    //Syntax::s.globals[varName] = new PVar(m_right->Execute(symTab, level));
    //symTab->Define(new Symbol new PVar(m_right->Execute(symTab, level));
    return *s->m_value;

}

void NodeAssign::AssignString(Assembler *as) {

    NodeString* right = (NodeString*)dynamic_cast<const NodeString*>(m_right);
    NodeVar* left = (NodeVar*)dynamic_cast<const NodeVar*>(m_left);
    QString lbl = as->NewLabel("stringassign");
    QString str = as->NewLabel("stringassignstr");
    QString lblCpy=as->NewLabel("stringassigncpy");
    as->Asm("jmp " + lbl);
    as->Label(str + "\t.dc \"" + right->m_op.m_value + "\",0");
    as->Label(lbl);
    as->Asm("ldx #0");
    as->Label(lblCpy);
    as->Asm("lda " + str+",x");
    as->Asm("sta "+left->value +",x");
    as->Asm("inx");
    as->Asm("cmp #0");
    as->Asm("bne " + lblCpy);

    as->PopLabel("stringassign");
    as->PopLabel("stringassignstr");
    as->PopLabel("stringassigncpy");

}

void NodeAssign::AssignPointer(Assembler *as) {
    NodeVar* bVar = dynamic_cast<NodeVar*>(m_right);
    NodeNumber* bNum = dynamic_cast<NodeNumber*>(m_right);
    NodeVar* aVar = dynamic_cast<NodeVar*>(m_left);

    if (bVar==nullptr && !m_right->isPureNumeric())
        ErrorHandler::e.Error("Error assigning pointer: right-hand must be variable or number", m_op.m_lineNumber);




    if (bVar!=nullptr) {

        if (bVar->getType(as)!=TokenType::POINTER) {
            as->Asm("lda #<" + bVar->value);
            as->Asm("ldx #>" + bVar->value);
            as->Asm("sta " + aVar->value);
            as->Asm("stx "+ aVar->value+"+1");
        }
        else
        {
            as->Asm("lda " + bVar->value);
            as->Asm("ldx " + bVar->value + "+1");
            as->Asm("sta " + aVar->value);
            as->Asm("stx "+ aVar->value+"+1");
        }
    }
    if (m_right->isPureNumeric()) {
        as->Asm("lda #" + QString::number(((int)m_right->numValue()) & 255));
        as->Asm("ldx #" + QString::number(((int)m_right->numValue()>>8) & 255) );
        as->Asm("sta " + aVar->value);
        as->Asm("stx "+ aVar->value+"+1");
    }
}

bool NodeAssign::isSimpleAeqAOpB(NodeVar *var, Assembler *as) {
    NodeBinOP* rterm = dynamic_cast<NodeBinOP*>(m_right);
    if (rterm==nullptr)
        return false;

    // right first is var
    NodeVar* rvar = dynamic_cast<NodeVar*>(rterm->m_left);

    NodeVar* rrvar = dynamic_cast<NodeVar*>(rterm->m_right);
    NodeNumber* rrnum = dynamic_cast<NodeNumber*>(rterm->m_right);

    if (rrvar==nullptr && rrnum==nullptr)
        return false;

    //        if (var->m_expr!=nullptr) {
    //        return false;
    //      }

    if (!(rterm->m_op.m_type==TokenType::PLUS || rterm->m_op.m_type==TokenType::MINUS))
        return false;

    if (var->getType(as) == TokenType::INTEGER)
        return false;
    as->Comment("Optimizer: a = a +/- b");
    var->LoadVariable(as);
    as->BinOP(rterm->m_op.m_type);
    rterm->m_right->Build(as);
    as->Term();
    var->StoreVariable(as);

    /*   qDebug() << "is simple a=a+b : " << var->value << " = " << rvar->value << " op " ;
        if (rrnum!=nullptr)
            qDebug() << " " << rrnum->m_val;
        if (rrvar!=nullptr)
            qDebug() << " " << rrvar->value;*/

    return true;
}

bool NodeAssign::IsSimpleIncDec(NodeVar *var, Assembler *as) {
    // Right is binop
    NodeBinOP* rterm = dynamic_cast<NodeBinOP*>(m_right);
    if (rterm==nullptr)
        return false;

    // right first is var
    NodeVar* rvar = dynamic_cast<NodeVar*>(rterm->m_left);
    if (rvar==nullptr)
        return false;

    if (rvar->value!=var->value)
        return false;

    NodeNumber* num = dynamic_cast<NodeNumber*>(rterm->m_right);

    // If a = a + b
    if (!(num!=nullptr && num->m_val==1))
        return isSimpleAeqAOpB(var, as);

    if (num==nullptr)
        return false;

    if (num->m_val!=1)
        return false;

    // OK: it is i:=i+1;
    QString operand ="";
    if (rterm->m_op.m_type==TokenType::PLUS)
        operand="inc ";
    if (rterm->m_op.m_type==TokenType::MINUS)
        operand="dec ";
    if (operand=="")
        return false; // other operand



    if (var->m_expr==nullptr && rvar->m_expr==nullptr) {
        as->Asm(operand +var->value);
        return true;
    }
    else {
        if (rvar->m_expr==nullptr)
            return false;
        if (var->m_expr==nullptr)
            return false;
        // BOTH must have expressions
        if (!rvar->m_expr->DataEquals(var->m_expr))
            return false;
        // Ok. Both are equal. OPTIMIZE!
        //            return false;
        if (var->LoadXYVarOrNum(as, var->m_expr,true)) {
            as->Comment("Optimize byte array " + operand);

            as->Asm(operand + var->value+",x");
            return true;
        }

        return false;
    }


    return false;

}

QString NodeAssign::AssignVariable(Assembler *as) {

    NodeVar* v = (NodeVar*)dynamic_cast<const NodeVar*>(m_left);
    //        qDebug() << "AssignVariable: " <<v->value << " : " << TokenType::getType( v->getType(as));

    NodeNumber* num = (NodeNumber*)dynamic_cast<const NodeNumber*>(m_left);
    if (v==nullptr && num == nullptr)
        ErrorHandler::e.Error("Left value not variable or memory address! ");
    if (num!=nullptr && num->getType(as)!=TokenType::ADDRESS)
        ErrorHandler::e.Error("Left value must be either variable or memory address");



    if (num!=nullptr) {
        as->Comment("Assigning memory location (poke replacement)");
        v = new NodeVar(num->m_op); // Create a variable copy
        v->value = num->HexValue();
        //return num->HexValue();
    }


    as->Comment("Assigning single variable : " + v->value);
    Symbol* s = as->m_symTab->Lookup(v->value, m_op.m_lineNumber, v->isAddress());
    //        if (s==nullptr)
    //          ErrorHandler::e.Error("Could not find variable :" + v->value,m_op.m_lineNumber);

    TokenType::Type t = s->getTokenType();
    //qDebug() << TokenType::getType(m_left->getType(as)) << " " << v->m_expr;

    if (m_left->getType(as)==TokenType::POINTER && v->m_expr==nullptr) {
        AssignPointer(as);
        return v->value;
    }

    if ((NodeString*)dynamic_cast<const NodeString*>(m_right))
    {
        AssignString(as);
        return v->value;
    }
    if (m_right==nullptr)
        ErrorHandler::e.Error("Node assign: right hand must be expression", m_op.m_lineNumber);
    if (m_left->getType(as)==TokenType::INTEGER) {
        as->Asm("ldy #0");
        m_right->m_forceType = TokenType::INTEGER; // FORCE integer on right-hand side
    }

    // For constant i:=i+1;
    if (IsSimpleIncDec(v,  as))
        return v->value;


    m_right->Build(as);
    as->Term();
    v->StoreVariable(as);
    return v->value;
}

QString NodeAssign::Build(Assembler *as) {

    Node::Build(as);
    as->PushCounter();
    QString s = AssignVariable(as);
    as->PopCounter(m_op.m_lineNumber);
    return s;

}

void NodeAssign::ExecuteSym(SymbolTable *symTab) {
    QString varName = ((NodeVar*)m_left)->value;
    Symbol* varSymbol = symTab->Lookup(varName, m_op.m_lineNumber);
    m_right->ExecuteSym(symTab);

}
