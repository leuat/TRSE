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
}

PVar NodeBinOP::Execute(SymbolTable *symTab, uint lvl) {
    level = lvl+1;
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
    ErrorHandler::e.DebugLow("Calling BinOP",level);

    if (m_op.m_type==TokenType::PLUS)
        return m_left->Execute(symTab, level) + m_right->Execute(symTab, level);
    if (m_op.m_type==TokenType::MINUS)
        return m_left->Execute(symTab, level) - m_right->Execute(symTab, level);

    if (m_op.m_type==TokenType::DIV)
        return m_left->Execute(symTab, level) / m_right->Execute(symTab, level);
    if (m_op.m_type==TokenType::MUL)
        return m_left->Execute(symTab, level) * m_right->Execute(symTab, level);
    return PVar();

}

void NodeBinOP::ExecuteSym(SymbolTable *symTab) {
    m_left->ExecuteSym(symTab);
    m_right->ExecuteSym(symTab);
}

QString NodeBinOP::BothConstants(Assembler *as) {
    NodeNumber *a = (NodeNumber*)dynamic_cast<const Node*>(m_left);
    NodeNumber *b = (NodeNumber*)dynamic_cast<const Node*>(m_right);
    int val = 0;
    if (m_op.m_type==TokenType::PLUS)
        val = a->m_val + b->m_val;
    if (m_op.m_type==TokenType::MINUS)
        val = a->m_val - b->m_val;

    if (m_op.m_type==TokenType::DIV)
        val = a->m_val / b->m_val;
    if (m_op.m_type==TokenType::MUL)
        val = a->m_val * b->m_val;

    QString str = "";
    if (a->m_op.m_type==TokenType::ADDRESS && b->m_op.m_type==TokenType::ADDRESS) {
        str = "$" + QString::number(val,16);
    }
    else
        if (a->m_op.m_type==TokenType::INTEGER && b->m_op.m_type==TokenType::INTEGER) {
            str = "#"+ QString::number(val);
        }
    if (a->m_op.m_type==TokenType::BYTE && b->m_op.m_type==TokenType::BYTE) {
        str = "#"+ QString::number(val);
    }
    else {
        ErrorHandler::e.Error("BinOp for constants must be numbers or addresses");
    }
    as->Term(str);

    return "";
}

bool NodeBinOP::isPureNumeric() {
    if (m_left==nullptr || m_right==nullptr)
        return false;

    return (m_left->isPureNumeric() && m_right->isPureNumeric());
}

int NodeBinOP::getShiftCount(int i) {
    if (i==1) return 0;
    if (i==2) return 1;
    if (i==4) return 2;
    if (i==8) return 3;
    if (i==16) return 4;
    if (i==32) return 5;
    if (i==64) return 6;
    if (i==128) return 7;
    if (i==256) return 8;
    return -1;
}

void NodeBinOP::EightBitMul(Assembler *as) {

    QString mulVar = as->NewLabel("mulRightVar");
    QString mulVarJmp = as->NewLabel("mulRightVarJmp");

    as->Comment("8 bit mul");
    as->ClearTerm();
    m_left->Build(as);
    as->Term(); // lda z
    as->Asm("pha");
    as->Comment("Load right hand side");
    m_right->Build(as);
    as->Asm("tax");
    as->Asm("pla");

    as->Term();

    as->Asm("jsr multiply_eightbit");
    as->Asm("txa"); // result in a

}

void NodeBinOP::EightBitDiv(Assembler *as) {

    QString mulVar = as->NewLabel("mulRightVar");
    QString mulVarJmp = as->NewLabel("mulRightVarJmp");

    as->Comment("8 bit div");
    as->ClearTerm();
    m_left->Build(as);
    as->Term();
    as->Asm("sta div8x8_d");
    as->Term(); // lda z
    as->Comment("Load right hand side");
    m_right->Build(as);
    as->Term();
    as->Asm("sta div8x8_c");

    as->Term();

    as->Asm("jsr div8x8_procedure");

}

void NodeBinOP::LoadVariable(Assembler *as) {
    Build(as);
}

bool NodeBinOP::isAddress() {
    return m_left->isAddress() && m_right->isAddress();
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

/*void NodeBinOP::BothPureNumbersBinOpOld(Assembler *as) {


    NodeNumber *a = (NodeNumber*)dynamic_cast<const NodeNumber*>(m_left);
    NodeNumber *b = (NodeNumber*)dynamic_cast<const NodeNumber*>(m_right);

    m_left->Build(as);
    if (m_op.m_type==TokenType::PLUS)
        as->Term("+");
    if (m_op.m_type==TokenType::MINUS)
        as->Term("-");
    if (m_op.m_type==TokenType::DIV)
        as->Term("/");
    if (m_op.m_type==TokenType::MUL)
        as->Term("*");
    if (m_op.m_type==TokenType::BITAND)
        as->Term("&");
    if (m_op.m_type==TokenType::BITOR)
        as->Term("|");

    m_right->Build(as);
}
*/

void NodeBinOP::RightIsPureNumericMulDiv8bit(Assembler *as) {
    int val = ((NodeNumber*)m_right)->m_val;
    int cnt = getShiftCount(val);
    if (cnt == -1 ) {
        if (m_op.m_type == TokenType::MUL)
            EightBitMul(as);
        else
            //ErrorHandler::e.Error("Binary operation / not implemented for this value yet ( " + QString::number(val) + ")");
            EightBitDiv(as);
        return;
    }
    as->Comment("8 bit mul of power 2");

    QString command = "";
    QString varName;
    if (m_op.m_type == TokenType::DIV)
        command = "lsr";
    if (m_op.m_type == TokenType::MUL)
        command = "asl";


    as->Asm("");
    m_left->LoadVariable(as);
    as->Term();

    for (int i=0;i<cnt;i++)
        as->Asm(command);



}

void NodeBinOP::Mul16x8(Assembler *as) {
    as->Comment("Mul 16x8 setup");
    as->Asm("");
    if (m_left->getType(as)==TokenType::INTEGER) {
        m_left->LoadVariable(as);
        as->Term();
        as->Asm("sty mul16x8_num1");
        as->Asm("sta mul16x8_num1Hi");
    }
    else {
        // 8x8 bit
        m_left->LoadVariable(as);
        as->Term();
        as->Asm("sta mul16x8_num1");
        as->Asm("lda #0");
        as->Asm("sta mul16x8_num1Hi");
    }

    as->Asm("");
    m_right->LoadVariable(as);
    as->Term();
    as->Asm("sta mul16x8_num2");
    as->Asm("jsr mul16x8_procedure");

}

void NodeBinOP::Div16x8(Assembler *as) {
    as->Comment("16x8 div");
    as->Asm("ldy #0");
    m_left->Build(as);
    as->Term();
    as->Asm("sta dividend");
    as->Asm("sty dividend+1");
    as->Asm("ldy #0");
    m_right->Build(as);
    as->Term();
    as->Asm("sta divisor");
    as->Asm("sty divisor+1");
    as->Asm("jsr divide16x8");
    as->Asm("lda dividend");
    as->Asm("ldy dividend+1");

}

void NodeBinOP::RightIsPureNumericMulDiv16bit(Assembler *as) {
    int val = ((NodeNumber*)m_right)->m_val;

    int cnt = getShiftCount(val);
    if (cnt == -1 && m_op.m_type == TokenType::DIV ) {
        //if (m_op.m_type == TokenType::MUL)
        //    EightBitMul(as);
        //else
        //ErrorHandler::e.Error("16 bit Binary operation / not implemented for this value yet ( " + QString::number(val) + ")");
        Div16x8(as);
        return;
        //return;
    }
    as->Comment("16 bit mul or div");

    QString command = "";
    QString varName;
    if (m_op.m_type == TokenType::DIV)
        command = "lsr";
    if (m_op.m_type == TokenType::MUL) {
        Mul16x8(as);
        return;
    }


    as->Asm("");
    m_left->LoadVariable(as);
    as->Term();

    varName = as->StoreInTempVar("int_shift", "word");
    as->Asm("sty "+varName);
    as->Asm("sta "+varName+"+1");
    command = "\t lsr " + varName +"+1"+ "\n";
    command += "\t ror " + varName+"+0" + "\n";


    for (int i=0;i<cnt;i++)
        as->Asm(command);

    as->Asm("lda " + varName);


}

void NodeBinOP::HandleMulDiv(Assembler *as) {

    if (m_right->isPureNumeric())  {
        as->Comment("Right is PURE NUMERIC");
        //qDebug() << "IS PURE NUMERIC";
        if (isWord(as))
            RightIsPureNumericMulDiv16bit(as);
        else
            RightIsPureNumericMulDiv8bit(as);
        return;
    }
    if (m_op.m_type==TokenType::MUL) {
        if (isWord(as))
            Mul16x8(as);
        else
            EightBitMul(as);
        return;
    }
    if (m_op.m_type==TokenType::DIV) {
        if (isWord(as))
            Div16x8(as);
        //                ErrorHandler::e.Error("16 bit div not implemented",m_op.m_lineNumber);
        else
            RightIsPureNumericMulDiv8bit(as);
        return;
    }
    ErrorHandler::e.Error("Binary operation / not implemented for this type yet...");
}

bool NodeBinOP::HandleSingleAddSub(Assembler *as) {

    NodeNumber* num = dynamic_cast<NodeNumber*>(m_right);
    NodeVar* vnum = dynamic_cast<NodeVar*>(m_right);
    NodeVar* var = dynamic_cast<NodeVar*>(m_left);



    if (num!=nullptr || vnum!=nullptr) {
        as->Comment("Add/sub where right value is constant number");
        if (num!=nullptr && num->m_op.m_type==TokenType::ADDRESS && var!=nullptr) {
            //qDebug() << "ADDRESS: " << num->StringValue();
            //exit(1);
            //as->
            as->ClearTerm();
            as->Term("lda " + var->value + " + " + num->StringValue());
            return true;
        }
        m_left->Build(as);
        as->Term();
        as->BinOP(m_op.m_type);
        m_right->Build(as);
        as->Term();
        as->Term(" ; end add / sub var with constant", true);
        return true;
    }


    return false;
}

bool NodeBinOP::isWord(Assembler *as) {
    return ((m_left->isWord(as) || m_right->isWord(as)) || (m_forceType==TokenType::INTEGER));
}

void NodeBinOP::HandleVarBinopB16bit(Assembler *as) {
    as->m_labelStack["wordAdd"].push();
    QString lblword = as->getLabel("wordAdd");

    //QString lbl = as->NewLabel("rightvarInteger");
    //        QString lblJmp = as->NewLabel("jmprightvarInteger");

    NodeVar* v = dynamic_cast<NodeVar*>(m_left);

    //as->Asm("jmp " + lblJmp);
    //as->Write(lbl +"\t.word\t0");
    //as->Label(lblJmp);
    as->Comment(";here");
    as->ClearTerm();
    as->Asm("ldy #0");
    m_right->Build(as);

    as->Term();
//    as->Asm("sta " +lbl);
    QString lbl = as->StoreInTempVar("rightvarInteger", "word");
    as->Asm("sty " +lbl+"+1");
    as->Term();

    as->Asm("clc");
    //as->Variable(v->value, false);
    as->Asm("lda " + v->value + "+1");
    as->BinOP(m_op.m_type);
    as->Term(lbl+"+1", true);
    as->Asm("tay");
    as->Asm("lda "+ v->value);
    as->Asm("clc");

    //            as->ClearTerm();
    as->BinOP(m_op.m_type);
    as->Term(lbl,true);
    //        as->Asm("sta " + varName);

    if (m_op.m_type==TokenType::PLUS) {
        as->Asm("bcc "+lblword);
        as->Asm("iny");
    }
    else {
        as->Asm("bcs "+lblword);
        as->Asm("dey");
    }
    as->Label(lblword);

    // Finally, add high bit of temp var


    as->PopLabel("wordAdd");

    //        as->PopLabel("rightvarInteger");
    //      as->PopLabel("jmprightvarInteger");

}


void NodeBinOP::HandleGenericBinop16bit(Assembler *as) {


    as->m_labelStack["wordAdd"].push();
    QString lblword = as->getLabel("wordAdd");

    //QString lbl = as->NewLabel("rightvarInteger");
    QString lblJmp = as->NewLabel("jmprightvarInteger");


    as->Comment("Generic 16 bit op");

/*    as->Asm("jmp " + lblJmp);
    as->Write(lbl +"\t.word\t0");
    as->Label(lblJmp);*/

    as->ClearTerm();
    as->Asm("ldy #0");
    m_right->Build(as);

        // 255 + k - j doesn't work
    as->Term();
    QString lbl = as->StoreInTempVar("rightvarInteger", "word");

//    as->Asm("sta " +lbl);
    as->Asm("sty " +lbl+"+1"); // J is stored
    as->Term();

    //as->Variable(v->value, false);
    //as->Asm("lda " + v->value + "+1");
    m_left->m_isWord = true;
    m_left->Build(as);
    as->Term();
    as->Comment("Low bit binop:");
    as->BinOP(m_op.m_type);
    as->Term(lbl, true); // high bit added to a

    if (m_op.m_type==TokenType::PLUS) {
        as->Asm("bcc "+lblword);
        as->Asm("inc " +lbl+"+1");
    }
    else {
        as->Asm("bcs "+lblword);
        as->Asm("dec  " +lbl+"+1");
    }

    as->Label(lblword);
    as->Asm("sta "+lbl + " ;wtf");
    as->Comment("High-bit binop");
    as->Asm("tya");

//    as->BinOP(m_op.m_type);
    as->BinOP(TokenType::PLUS);

    as->Term(lbl+"+1",true);
//    as->Asm("lda #0");

    as->Asm("tay");
    as->Asm("lda "+lbl);


    as->PopLabel("wordAdd");

    //as->PopLabel("rightvarInteger");
    as->PopLabel("jmprightvarInteger");

}

void NodeBinOP::HandleRestBinOp(Assembler *as) {
    bool isWord16 = false;
    QString varName="";

    if ( dynamic_cast<const NodeVar*>(m_left) != nullptr) {
        NodeVar* v= (NodeVar*)m_left;
        varName = v->value;
        Symbol* s = as->m_symTab->Lookup(varName, m_op.m_lineNumber);
    }
    isWord16 = isWord(as);
    if (m_isWord)
        isWord16 = true;
    // check if both are constant values:
    if (!isWord16) {
        as->Comment("8 bit binop");
        // Optimizing check: if right var is number, then cut losses
        if (HandleSingleAddSub(as)) {
            return;
        }
        else {
            as->Comment("Add/sub right value is variable/expression");

            //       QString lbl = as->NewLabel("rightvar");
            //QString lblJmp = as->NewLabel("jmprightvar");
            //as->Asm("jmp " + lblJmp);
            //as->Write(lbl +"\t.byte\t0");
            //as->Label(lblJmp);
            as->ClearTerm();
            m_right->Build(as);
            as->Term();
            QString lbl = as->StoreInTempVar("rightvarAddSub");
            //as->Asm("sta " +lbl);
            //as->Term();

            m_left->Build(as);
            as->Term();

            as->BinOP(m_op.m_type);
            as->Term(lbl,true);
            //as->PopLabel("rightvar");
            //as->PopLabel("jmprightvar");
        }
    }
    else {
        // Word handling
        NodeVar* v = dynamic_cast<NodeVar*>(m_left);
        if (v!=nullptr) {
            HandleVarBinopB16bit(as);
            return;
        }
        HandleGenericBinop16bit(as);
    }

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
        return 0;
    int res = numValue();
    //qDebug() << QString::number(res, 16);
    return "$" + QString::number(res, 16);
}

QString NodeBinOP::Build(Assembler *as) {
    Node::Build(as);

    // First check if both are consants:


    if (isPureNumeric()) {


        int val = BothPureNumbersBinOp(as);
        QString s = "#";
        if (m_left->isAddress() || m_right->isAddress())
            s = "";
        //s=s+QString::number(val);
        //as->Term("#" + HexValue());

        if (as->m_term=="")
            if (!isWord(as))
                as->Asm("lda " + s + QString::number(val));
            else {
                as->Asm("lda " + s+QString::number(val&255));
                as->Asm("ldy " + s+QString::number((val>>8)&255));
            }

        else
            as->Term(s+QString::number(val));
        return "";
    }

    if (m_op.m_type==TokenType::MUL || m_op.m_type==TokenType::DIV) {
        HandleMulDiv(as);
        return "";
    }
    HandleRestBinOp(as);
    // The rest is only plus and minus, and etc
    return "";
}


