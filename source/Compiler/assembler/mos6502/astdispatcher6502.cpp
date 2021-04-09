#include "astdispatcher6502.h"

ASTDispatcher6502::ASTDispatcher6502()
{

}

/*
 *
 *  NODE BINOP dispatcher
 *
 *  */



void ASTDispatcher6502::EightBitDiv(QSharedPointer<Node> node) {

    QString mulVar = as->NewLabel("mulRightVar");
    QString mulVarJmp = as->NewLabel("mulRightVarJmp");

    as->Comment("8 bit div");
    as->ClearTerm();
    node->m_left->Accept(this);
    as->Term();
    as->Asm("sta div8x8_d");
    as->Term(); // lda z
    as->Comment("Load right hand side");
    node->m_right->Accept(this);
    as->Term();
    as->Asm("sta div8x8_c");

    as->Term();

    as->Asm("jsr div8x8_procedure");

}

void ASTDispatcher6502::EightBitMul(QSharedPointer<Node> node) {

    QString mulVar = as->NewLabel("mulRightVar");
    QString mulVarJmp = as->NewLabel("mulRightVarJmp");

    as->Comment("8 bit mul");
    as->ClearTerm();
    node->m_left->Accept(this);
    as->Term(); // lda z
    as->Asm("pha");
    as->Comment("Load right hand side");
    node->m_right->Accept(this);
    as->Asm("tax");
    as->Asm("pla");

    as->Term();

    as->Asm("jsr multiply_eightbit");
    as->Asm("txa"); // result in a
    as->Asm("ldy #0 ; ::EightbitMul");

}

void ASTDispatcher6502::HandleGenericBinop16bit(QSharedPointer<Node> node) {


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
//    qDebug() <<node->m_left->m_op.m_value;
  //  exit(1);
 //   node->m_right->forceWord();
    node->m_right->Accept(this);

    // 255 + k - j doesn't work
    as->Term();
    QString lbl = as->StoreInTempVar("rightvarInteger", "word");

    //    as->Asm("sta " +lbl);
//    as->Asm("sty " +lbl+"+1"); // J is stored
    as->Term();

    //as->Variable(getValue(v), false);
    //as->Asm("lda " + getValue(v) + "+1");
    node->m_left->m_isWord = true;
    node->m_left->Accept(this);
    as->Term();
    as->Comment("Low bit binop:");
    as->BinOP(node->m_op.m_type);
    as->Term(lbl, true); // high bit added to a

  /*  if (node->m_op.m_type==TokenType::PLUS) {
        as->Asm("bcc "+lblword);
        as->Asm("inc " +lbl+"+1");
    }
    else {
        as->Asm("bcs "+lblword);
        as->Asm("inc  " +lbl+"+1");
    }
*/
    as->Label(lblword);
    as->Asm("sta "+lbl);
    as->Comment("High-bit binop");
    as->Asm("tya");

    //    as->BinOP(m_op.m_type);
//    if (node->m_op.m_type==TokenType::PLUS)
    as->BinOP(node->m_op.m_type,false);

    as->Term(lbl+"+1",true);
    //    as->Asm("lda #0");

    as->Asm("tay");
    as->Asm("lda "+lbl);


    as->PopLabel("wordAdd");

    //as->PopLabel("rightvarInteger");
    as->PopLabel("jmprightvarInteger");
    as->PopTempVar();
}
/*
void ASTDispatcher6502::HandleGenericBinop16bit(QSharedPointer<Node> node) {


    as->m_labelStack["wordAdd"].push();
    QString lblword = as->getLabel("wordAdd");

    //QString lbl = as->NewLabel("rightvarInteger");
    QString lblJmp = as->NewLabel("jmprightvarInteger");


    as->Comment("Generic 16 bit op");


    as->ClearTerm();
    as->Asm("ldy #0");
//    qDebug() <<node->m_left->m_op.m_value;
  //  exit(1);
//    node->m_right->forceWord();
    node->m_right->Accept(this);

    // 255 + k - j doesn't work
    as->Term();
    QString lbl = as->StoreInTempVar("rightvarInteger", "word");

    //    as->Asm("sta " +lbl);
//    as->Asm("sty " +lbl+"+1"); // J is stored
    as->Term();

    //as->Variable(getValue(v), false);
    //as->Asm("lda " + getValue(v) + "+1");
    node->m_left->m_isWord = true;
    node->m_left->Accept(this);
    as->Term();
    as->Comment("Low bit binop:");
    as->BinOP(node->m_op.m_type);
    as->Term(lbl, true); // high bit added to a

    if (node->m_op.m_type==TokenType::PLUS) {
        as->Asm("bcc "+lblword);
        as->Asm("inc " +lbl+"+1");
    }
    else {
        as->Asm("bcs "+lblword);
        as->Asm("inc  " +lbl+"+1");
    }

    as->Label(lblword);
    as->Asm("sta "+lbl);
    as->Comment("High-bit binop");
    as->Asm("tya");

    //    as->BinOP(m_op.m_type);
//    if (node->m_op.m_type==TokenType::PLUS)
    as->BinOP(node->m_op.m_type);

    as->Term(lbl+"+1",true);
    //    as->Asm("lda #0");

    as->Asm("tay");
    as->Asm("lda "+lbl);


    as->PopLabel("wordAdd");

    //as->PopLabel("rightvarInteger");
    as->PopLabel("jmprightvarInteger");
    as->PopTempVar();
}
*/
void ASTDispatcher6502::HandleVarBinopB16bit(QSharedPointer<Node> node) {


    as->m_labelStack["wordAdd"].push();
    QString lblword = as->getLabel("wordAdd");

    //QString lbl = as->NewLabel("rightvarInteger");
    //        QString lblJmp = as->NewLabel("jmprightvarInteger");

    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_left);

    //as->Asm("jmp " + lblJmp);
    //as->Write(lbl +"\t.word\t0");
    //as->Label(lblJmp);

    as->Comment("HandleVarBinopB16bit");



    as->ClearTerm();
    as->Asm("ldy #0 ; ::HandleVarBinopB16bit 0");

    if (node->m_right->isPure())  {
        as->Comment("RHS is pure, optimization");
        node->m_left->Accept(this);
        as->Term();
        as->BinOP(node->m_op.m_type);
        as->Term(node->m_right->getValue8bit(as,false),true);
        as->Comment("Testing for byte:  " + node->m_right->getValue8bit(as,true));
        if (node->m_right->getValue8bit(as,true)=="#0") {
            as->Comment("RHS is byte, optimization");
            QString lbl = as->NewLabel("skip");
            as->Asm("bcc "+lbl);
            as->Asm("iny");
            as->Label(lbl);

        }
        else {
            as->Comment("RHS is word, no optimization");
            as->Asm("pha ");
            as->Asm("tya ");
            as->BinOP(node->m_op.m_type,false);
            as->Term(node->m_right->getValue8bit(as,true),true);
            as->Asm("tay ");
            as->Asm("pla ");
        }
        return;
        //        as->Asm("sta " + varName);



/*        if (node->m_op.m_type==TokenType::PLUS) {

            as->Asm("bcc "+lblword);
            as->Asm("iny");

        }
        else {
  //          as->Asm("bcs "+lblword);
            as->Asm("dey");
        }
        as->Label(lblword);
*/
    }


//    qDebug() << node->m_right
    //qDebug() << "NodeBinop : " << TokenType::getType(node->m_right->getType(as)) <<TokenType::getType(node->m_left->getType(as)) ;

    if (node->m_right->getType(as)==TokenType::ADDRESS && node->m_left->getType(as)==TokenType::ADDRESS )
        ErrorHandler::e.Warning("You are adding together two addresses. Is this really what you indend?", node->m_op.m_lineNumber);

    node->m_right->Accept(this);

    as->Term();
    //    as->Asm("sta " +lbl);
    QString lbl = as->StoreInTempVar("rightvarInteger", "word");
//    as->Asm("sty " +lbl+"+1");
//        qDebug() << as->m_term;
    as->Term();
  //  as->Asm("clc");
    //as->Variable(getValue(v), false);
  //  as->Comment(";HEREHERE");
//    exit(1);
    if (!v->isArrayIndex()) {
        if (v->getType(as)==TokenType::POINTER || (v->m_op.m_type!=TokenType::ADDRESS && !v->m_fake16bit) )
            as->Asm("lda " + getValue(v) + "+1");
        else
            as->Asm("lda #>" + getValue(v) + "");

        as->BinOP(node->m_op.m_type);
        as->Term(lbl+"+1", true);
        as->Asm("tay");
        //    qDebug() << getValue(v) << v->m_op.getType();
        //  exit(1);
        if (v->getType(as)==TokenType::POINTER || (v->m_op.m_type!=TokenType::ADDRESS && !v->m_fake16bit) )
            as->Asm("lda "+ getValue(v));
        else
            as->Asm("lda #<"+ getValue(v));
    }
    else{
        as->Comment("Contains expression");
        v->m_expr->Accept(this);
        as->Term();
        if (v->getType(as)==TokenType::POINTER) {
            if (v->getArrayType(as)==TokenType::INTEGER) {
                // Special case : Load data from integer pointer
                as->Comment("Integer pointer, special case");
                as->Asm("asl");
                as->Asm("tay");
                as->Asm("lda (" + getValue(v) + "),y");
//                as->Asm("ldx "+lbl+"+1");

                as->BinOP(node->m_op.m_type);
                as->Term(lbl,true);
                as->Asm("pha");

                as->Asm("iny");
                as->Asm("lda (" + getValue(v) + "),y");
                as->BinOP(node->m_op.m_type,false);
                as->Term(lbl+"+1",true);

                as->Asm("tay");
                as->Asm("pla");

//                as->Asm("lda "+lbl);


                as->PopLabel("wordAdd");
                as->PopTempVar();
                return;
            }
            else {
                as->Asm("tay");
                as->Asm("lda (" + getValue(v) + "),y");
                as->Asm("ldy "+lbl+"+1");
            }
//            as->Asm("ldy #0");
        }
        else {
            as->Asm("asl");

            as->Asm("tax");
            //        as->Asm("tax");
            as->Asm("lda " + getValue(v) + "+1,x");

            as->BinOP(node->m_op.m_type);
            as->Term(lbl+"+1", true);
            as->Asm("tay");

            as->Asm("lda "+ getValue(v)+",x");
        }

//        v->Accept(this);
    }
    //as->Asm("clc");

    //            as->ClearTerm();
    as->BinOP(node->m_op.m_type);
    as->Term(lbl,true);
    //        as->Asm("sta " + varName);

    if (node->m_op.m_type==TokenType::PLUS) {
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
    as->PopTempVar();
    //        as->PopLabel("rightvarInteger");
    //      as->PopLabel("jmprightvarInteger");

}

bool ASTDispatcher6502::HandleSingleAddSub(QSharedPointer<Node> node) {

    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(node->m_right);
    QSharedPointer<NodeVar> vnum = qSharedPointerDynamicCast<NodeVar>(node->m_right);
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    if (num!=nullptr || vnum!=nullptr) {
/*        if (num!=nullptr)
            qDebug() << "Number:"  <<num->m_val;
        if (vnum!=nullptr)
            qDebug() << "Var:"  <<vnum->getValue(as);*/
        as->Comment("Add/sub where right value is constant number");
        if (num!=nullptr && num->m_op.m_type==TokenType::ADDRESS && var!=nullptr) {
            //qDebug() << "ADDRESS: " << num->StringValue();
            //exit(1);
            //as->
            as->ClearTerm();
            as->Term("lda " + getValue(var) + " + " + num->StringValue());
            return true;
        }
        node->m_left->Accept(this);
        as->Term();
        m_flag1=true;
        as->BinOP(node->m_op.m_type);
        node->m_right->Accept(this);
        as->Term();
        m_flag1=false;
        as->Term(" ; end add / sub var with constant", true);
        return true;
    }


    return false;
}

void ASTDispatcher6502::HandleMulDiv(QSharedPointer<Node> node) {

    if (node->m_left->isPureNumeric() && !node->m_right->isPureNumeric() && node->m_op.m_type==TokenType::MUL ) {
        QSharedPointer<Node> tmp = node->m_left;
        node->m_left = node->m_right;
        node->m_right = tmp;
        as->Comment("Swapping nodes :  num * expr -> exp*num (mul only)");
    }


    if (node->m_right->isPureNumeric())  {
        as->Comment("Right is PURE NUMERIC : Is word ="+ QString::number(node->isWord(as)) );
        //qDebug() << "IS PURE NUMERIC";
        if (node->isWord(as))
            RightIsPureNumericMulDiv16bit(node);
        else
            RightIsPureNumericMulDiv8bit(node);
        return;
    }
    if (node->m_op.m_type==TokenType::MUL) {
        if (node->isWord(as))
            Mul16x8(node);
        else
            EightBitMul(node);
        return;
    }
    if (node->m_op.m_type==TokenType::DIV) {
        if (node->isWord(as))
            Div16x8(node);
        //                ErrorHandler::e.Error("16 bit div not implemented",m_op.m_lineNumber);
        else
            RightIsPureNumericMulDiv8bit(node);
        return;
    }
    ErrorHandler::e.Error("Binary operation / not implemented for this type yet...");
}

void ASTDispatcher6502::RightIsPureNumericMulDiv16bit(QSharedPointer<Node> node) {
    int val = qSharedPointerDynamicCast<NodeNumber>(node->m_right)->m_val;

    int cnt = Util::getShiftCount(val);
    if (cnt == -1 && node->m_op.m_type == TokenType::DIV ) {
        //if (m_op.m_type == TokenType::MUL)
        //    EightBitMul(as);
        //else
        //ErrorHandler::e.Error("16 bit Binary operation / not implemented for this value yet ( " + QString::number(val) + ")");
        Div16x8(node);
        return;
        //return;
    }
    as->Comment("16 bit mul or div");

    QString command = "";
    QString varName;
    if (node->m_op.m_type == TokenType::DIV)
        command = "lsr";
    if (node->m_op.m_type == TokenType::MUL) {
        Mul16x8(node);
        return;
    }


    as->Asm("");
    LoadVariable(node->m_left);
    as->Term();
    varName = as->StoreInTempVar("int_shift", "word");
//    as->ClearTerm();
  //  as->Asm("sty "+varName);
   // as->Asm("sta "+varName+"+1");
    command = "\tlsr " + varName +"+1"+ "\n";
    command += "\tror " + varName+"+0" + "\n";

    for (int i=0;i<cnt;i++)
        as->Asm(command);

    as->Asm("lda " + varName);
    as->Asm("ldy " + varName+"+1");

    as->PopTempVar();
}

void ASTDispatcher6502::HandleShiftLeftRight(QSharedPointer<NodeBinOP>node)
{
    QString cmd = node->m_op.m_type==TokenType::SHR?"lsr":"asl";

    if (node->m_right->isPureNumeric()) {
        node->m_left->Accept(this);
        as->Term();

        int val = node->m_right->getValueAsInt(as);
        for (int i=0;i<val;i++)
            as->Asm(cmd);
        return;
    }

    node->m_right->Accept(this);
    as->Term();
    as->Asm("tax");
    node->m_left->Accept(this);
    as->Term();
    QString lbl = as->NewLabel("lblShift");
    as->Label(lbl);
    as->Asm(cmd);
    as->Asm("dex");
    as->Asm("cpx #0");
    as->Asm("bne "+lbl);

    as->PopLabel("lblShift");
}

void ASTDispatcher6502::HandleShiftLeftRightInteger(QSharedPointer<NodeBinOP>node, bool isSimpleAeqAopB)
{
    // Not yet implemented

    QString varName = "";
    if (!isSimpleAeqAopB) {
        node->m_left->Accept(this);
        as->Term();
        varName = as->StoreInTempVar("tempVarShift","word");
    }
    else
        varName = getValue(node);

//    QString cmd = node->m_op.m_type==TokenType::SHR?"lsr":"asl";
    QString command = "";
    if (node->m_op.m_type==TokenType::SHR) {
        command = "\tlsr " + varName +"+1 ;keep"+ "\n";
        command += "\tror " + varName+"+0 ;keep" + "\n";
    }
    else {
        command = "\tasl " + varName +"+0 ;keep"+ "\n";
        command += "\trol " + varName+"+1 ;keep" + "\n";
    }
    if (node->m_right->isPureNumeric()) {

        int val = node->m_right->getValueAsInt(as);
        as->Comment("COUNT : "+QString::number(val));
        for (int i=0;i<val;i++)
            as->Asm(command);


    }
    else {
        node->m_right->Accept(this);
        as->Term();
        as->Asm("tax");
        QString lbl = as->NewLabel("lblShift");
        as->Label(lbl);
        as->Asm(command);
        as->Asm("dex");
        as->Asm("cpx #0");
        as->Asm("bne "+lbl);

        as->PopLabel("lblShift");
    }
    if (!isSimpleAeqAopB) {
        as->Asm("lda "+varName);
        as->Asm("ldy "+varName +"+1");
        as->PopTempVar();
    }
}

void ASTDispatcher6502::Mul16x8(QSharedPointer<Node> node) {
    as->Comment("Mul 16x8 setup");
    as->Asm("");
    if (node->m_left->isWord(as)) {

        LoadVariable(node->m_left);
//        if (!node->m_left->isWord(as))
        as->Term();
//        if (!node->m_left->getO(as))
  //          as->Asm("ldy #0");

        as->Asm("sta mul16x8_num1");
        as->Asm("sty mul16x8_num1Hi");
    }
    else {
        // 8x8 bit
        LoadVariable(node->m_left);
        as->Term();
        as->Asm("sta mul16x8_num1");
        as->Asm("lda #0");
        as->Asm("sta mul16x8_num1Hi");
    }

    as->Asm("");
    LoadVariable(node->m_right);
    as->Term();
    as->Asm("sta mul16x8_num2");
    as->Asm("jsr mul16x8_procedure");

}

void ASTDispatcher6502::Div16x8(QSharedPointer<Node> node) {
    as->Comment("16x8 div");
    as->Asm("ldy #0");
    node->m_left->Accept(this);
    as->Term();
    as->Asm("sta initdiv16x8_dividend");
    as->Asm("sty initdiv16x8_dividend+1");
    as->Asm("ldy #0");
    node->m_right->Accept(this);
    as->Term();
    as->Asm("sta initdiv16x8_divisor");
    as->Asm("sty initdiv16x8_divisor+1");
    as->Asm("jsr divide16x8");
    as->Asm("lda initdiv16x8_dividend");
    as->Asm("ldy initdiv16x8_dividend+1");

}




void ASTDispatcher6502::HandleRestBinOp(QSharedPointer<Node> node) {
    bool isWord16 = false;
    QString varName="";

    if ( qSharedPointerDynamicCast<NodeVar>(node->m_left) != nullptr) {
        QSharedPointer<NodeVar> v= qSharedPointerDynamicCast<NodeVar>(node->m_left);
        varName = getValue(v);
        QSharedPointer<Symbol> s = as->m_symTab->Lookup(varName, node->m_op.m_lineNumber);
    }
    isWord16 = node->isWord(as);
    if (node->m_isWord)
        isWord16 = true;

//    if (node->)


//    qDebug() << node->m_op.m_value;

    // check if both are constant values:
    if (!isWord16) {
        as->Comment("8 bit binop");
        // Optimizing check: if right var is number, then cut losses
        if (HandleSingleAddSub(node)) {
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
            node->m_right->Accept(this);
            as->Term();
            QString lbl = as->StoreInTempVar("rightvarAddSub");
            //as->Asm("sta " +lbl);
            //as->Term();

            node->m_left->Accept(this);
            as->Term();

            as->BinOP(node->m_op.m_type);
            as->Term(lbl,true);
            as->PopTempVar();
            //as->PopLabel("rightvar");
            //as->PopLabel("jmprightvar");
        }
    }
    else {
        // Word handling
        QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_left);
        if (v!=nullptr) {
            HandleVarBinopB16bit(node);
            return;
        }
        HandleGenericBinop16bit(node);
    }

}

void ASTDispatcher6502::RightIsPureNumericMulDiv8bit(QSharedPointer<Node> node) {
    int val = node->m_right->getValueAsInt(as); //qSharedPointerDynamicCast<NodeNumber>(node->m_right)->m_val;
    int cnt = Util::getShiftCount(val);
    if (cnt == -1 ) {
        if (node->m_op.m_type == TokenType::MUL)
            EightBitMul(node);
        else
            //ErrorHandler::e.Error("Binary operation / not implemented for this value yet ( " + QString::number(val) + ")");
            EightBitDiv(node);
        return;
    }
    as->Comment("8 bit mul of power 2");

    QString command = "";
    QString varName;
    if (node->m_op.m_type == TokenType::DIV)
        command = "lsr";
    if (node->m_op.m_type == TokenType::MUL)
        command = "asl";


    as->Asm("");
    LoadVariable(node->m_left);
    as->Term();

    for (int i=0;i<cnt;i++)
        as->Asm(command);

}

void ASTDispatcher6502::Load16bitVariable(QSharedPointer<Node> node, QString reg)
{
    as->ClearTerm();
//    as->Comment("Load 16 bit var IS WORD "+QString::number(node->isWord(as)));


    if (node->isWord(as))
        as->Asm("ld"+reg+" "+getValue8bit(node,true));
    as->Asm("lda "+getValue8bit(node,false));
}


void ASTDispatcher6502::dispatch(QSharedPointer<NodeBinOP>node)
{

    node->DispatchConstructor(as,this);


    // First, flip such that anything numeric / pure var is to the right
    if (node->m_op.m_type == TokenType::MUL || node->m_op.m_type == TokenType::PLUS)
    if (!(node->m_right->isPureNumeric() || node->m_right->isPureVariable())) {
        if (node->m_left->isPureNumeric() || node->m_left->isPureVariable()) {
            QSharedPointer<Node> tmp = node->m_right;
            node->m_right = node->m_left;
            node->m_left = tmp;
        }
    }


    if (node->isPureNumeric()) {

        int val = node->BothPureNumbersBinOp(as);
        QString s = "#";
        if (node->m_left->isAddress() || node->m_right->isAddress())
            s = "";

        if (as->m_term=="")
            if (!node->isWord(as))
                as->Asm("lda " + s + QString::number(val));
            else {
                as->Asm("lda " + s+QString::number(val&255));
                as->Asm("ldy " + s+QString::number((val>>8)&255));
            }

        else
            as->Term(s+QString::number(val));
        return;
    }

    if (node->m_op.m_type==TokenType::MUL || node->m_op.m_type==TokenType::DIV) {
        HandleMulDiv(node);
        return;
    }
    if (node->m_op.m_type==TokenType::SHR || node->m_op.m_type==TokenType::SHL) {
//        bool isSimpleAeqAopB = false;
  //      if (node->)


        if (node->isWord(as))
            HandleShiftLeftRightInteger(node,false);
        else
            HandleShiftLeftRight(node);
        return;
    }


    HandleRestBinOp(node);
    // The rest is only plus and minus, and etc

}



/*
 *
 *  NODE NUMBER dispatcher
 *
 *  */

void ASTDispatcher6502::dispatch(QSharedPointer<NodeNumber>node)
{
    node->DispatchConstructor(as,this);

    QString val = getValue(node);

    if (node->m_forceType==TokenType::INTEGER && node->m_val<=255) {
        as->Asm("ldy #0   ; Force integer assignment, set y = 0 for values lower than 255");
    }

//    as->Comment("Value assignment : " + Util::numToHex(node->m_val) + " "+ val + " " +QString::number(node->getValueAsInt(as)));
    if ((node->m_op.m_type==TokenType::INTEGER_CONST && node->m_val>255) || node->isReference()) {
        as->Comment("Integer constant assigning");
        Load16bitVariable(node,"y");
        return;

    }
    if (as->m_term=="")
        as->Term("lda " + val);
    else
        as->Term(val);
}
/*
 *
 *  SMALL NODE dispatcherS
 *
 *  */



QString ASTDispatcher6502::getValue(QSharedPointer<Node> n) {
    if (m_inlineParameters.contains(n->getValue(as)))
        return m_inlineParameters[n->getValue(as)]->getValue(as);

    return n->getValue(as);
}

QString ASTDispatcher6502::getValue8bit(QSharedPointer<Node> n, bool isHi) {

    if (m_inlineParameters.contains(n->getValue(as)))
        return m_inlineParameters[n->getValue(as)]->getValue8bit(as,isHi);

    return n->getValue8bit(as,isHi);
}


void ASTDispatcher6502::dispatch(QSharedPointer<Node> node)
{
    node->DispatchConstructor(as,this);
    node->m_currentLineNumber = node->m_op.m_lineNumber;


//    qDebug() << "ASTDispatcher6502 UNKNOWN TYPE! Should never be called.";
//    exit(1);
}





/*
void ASTDispatcher6502::dispatch(QSharedPointer<NodeProcedureDecl> node)
{

    node->DispatchConstructor(as,this);
    // Don't declare inline procedures

    if (node->m_isInline) {
        // Only declare variables in SYMTAB
        for (QSharedPointer<Node> n: qSharedPointerDynamicCast<NodeBlock>(node->m_block)->m_decl) {
            // Print label at end of vardecl
            auto vd = qSharedPointerDynamicCast<NodeVarDecl>(n);
            if (vd!=nullptr)
                vd->ExecuteSym(as->m_symTab);

        }
        return;
    }

    as->m_symTab->SetCurrentProcedure(node->m_procName+"_");
    int ln = node->m_currentLineNumber;
//    as->PushCounter();
//    if (node->m_curMemoryBlock!=nullptr)
  //      qDebug() << node->m_procName << "IS IN BLOCK " << node->m_curMemoryBlock->m_name << " STARTING AT " << Util::numToHex(node->m_curMemoryBlock->m_start);

    // In case memory block is acive


//    qDebug() << node->m_procName << node->m_curMemoryBlock << as->m_currentBlock;

    int ret = node->MaintainBlocks(as);
    if (ret==3) node->m_curMemoryBlock=nullptr;
    if (as->m_currentBlock!=nullptr) {
        if (node->m_curMemoryBlock==nullptr) {
            bool ok;
//            qDebug() << "Creating new block procedure for " << m_procName;
            QString p = as->m_currentBlock->m_pos;
            int pos = p.remove("$").toInt(&ok, 16);
            node->m_curMemoryBlock = QSharedPointer<MemoryBlock>(new MemoryBlock(pos,pos,MemoryBlock::ARRAY, node->m_blockInfo.m_blockName));
            as->blocks.append(node->m_curMemoryBlock);
        }
    }
    else {
        //node->m_curMemoryBlock=nullptr;
    }


    //MaintainBlocks(as);
    if (node->m_block==nullptr) {  // Is builtin procedure
        node->m_block = QSharedPointer<NodeBuiltinMethod>(new NodeBuiltinMethod(node->m_procName, QVector<QSharedPointer<Node>>(), nullptr));
//        Node::s_uniqueSymbols[node->m_block] = node->m_block; // Mark for deletion

    }

    bool isInitFunction=false;
    bool isBuiltinFunction=false;
    if (Syntax::s.builtInFunctions.contains(node->m_procName)) {
        isBuiltinFunction = true;
        isInitFunction = Syntax::s.builtInFunctions[node->m_procName].m_initFunction;
//        as->EndMemoryBlock();

    }

    as->Asm("");
    as->Asm("");
    as->Comment("***********  Defining procedure : " + node->m_procName);
    QString type = (isBuiltinFunction) ? "Built-in function" : "User-defined procedure";
    as->Comment("   Procedure type : " + type);
    if (isBuiltinFunction) {
        type = (isInitFunction) ? "yes" : "no";
        as->Comment("   Requires initialization : " + type);
    }
    as->Asm("");
    as->m_currentBlockName = node->m_procName;

    if (!isInitFunction) {
        //as->Asm("jmp afterProc_" + m_procName);


        //as->Label(m_procName);
    }

    if (node->m_block!=nullptr) {
        QSharedPointer<NodeBlock> b = qSharedPointerDynamicCast<NodeBlock>(node->m_block);
        if (b!=nullptr) {
            b->forceLabel=node->m_procName;
            b->m_isProcedure = true;
        }
        node->m_block->Accept(this);
//        node->Delete();
//        node->s_uniqueSymbols[node->m_block] = node->m_block;


//        delete node;
//        node->m_block->Build(as);
    }
    if (!isInitFunction) {
        if (node->m_type==0) {
            as->Asm("rts");
        }
        else as->Asm("rti");
    }

    if (node->m_curMemoryBlock!=nullptr) {
        node->m_curMemoryBlock->m_end+=10;
    }

    as->m_symTab->ExitProcedureScope(false);
  //  as->PopCounter(ln);
}

*/


void ASTDispatcher6502::dispatch(QSharedPointer<NodeVarType> node)
{
    node->DispatchConstructor(as,this);

}

void ASTDispatcher6502::dispatch(QSharedPointer<NodeBinaryClause> node)
{
    node->DispatchConstructor(as,this);

}

void ASTDispatcher6502::dispatch(QSharedPointer<NodeString> node)
{
    node->DispatchConstructor(as,this);
//    exit(1);
    if (node->m_val.count()>=1 && node->m_val[0].count()>=1) {
        as->ClearTerm();

//        as->Asm("lda #"+QString::number(QChar(node->m_val[0][0]).unicode()));
        CStringItem it  = ((AsmMOS6502*)as)->m_cstr[QString(node->m_val[0][0]) ];
//        as->Asm("lda #"+QString::number(it.m_char.unicode()->unicode()));
        as->Asm("lda #"+QString::number(it.m_screenCode));
        as->Term();
        return;
    }
    as->String(node->m_val,true);
}


void ASTDispatcher6502::dispatch(QSharedPointer<NodeVarDecl> node)
{
    node->DispatchConstructor(as,this);

    AbstractASTDispatcher::dispatch(node);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);
    if (t->m_op.m_type==TokenType::INCSID || t->m_op.m_type==TokenType::INCNSF) {
        IncSid(node);
    }


}


/*
void ASTDispatcher6502::dispatch(QSharedPointer<NodeBlock> node)
{
    node->DispatchConstructor(as,this);

    AbstractASTDispatcher::dispatch(node);


    // In case memory block is acive
    //as->EndMemoryBlock();
    int ln = node->m_op.m_lineNumber-1;
    if (ln==0) ln++;
    as->PushBlock(node->m_currentLineNumber);




    bool blockLabel = false;
    bool blockProcedure = false;
    bool hasLabel = false;

    QString label = as->NewLabel("block");


    if (!node->m_ignoreDeclarations) {

        if (node->m_decl.count()!=0) {
            if (node->m_isMainBlock && !as->m_ignoreInitialJump)
                as->Asm("jmp " + label);
            hasLabel = true;
        }


        for (QSharedPointer<Node> n: node->m_decl) {
            // Print label at end of vardecl
            if (qSharedPointerDynamicCast<NodeVarDecl>(n)==nullptr) {
                if (!blockProcedure) // Print label at end of vardecl
                {
                    if (n->m_op.m_lineNumber!=0) {
                        //                      as->PopBlock(n->m_op.m_lineNumber);
                        blockProcedure = true;
                        //   qDebug() << "pop" << n->m_op.m_lineNumber << " " << TokenType::getType(n->getType(as));
                    }

                }

            }
            n->Accept(this);

        }
        as->VarDeclEnds();
    }
    as->PushCounter();

    if (node->m_isMainBlock) {
        int ret = node->MaintainBlocks(as);
//        if (ret==2)
  //          as->m_currentBlock = nullptr;

        as->m_currentBlockName="MainProgram";
//        as->EndMemoryBlock();
    }


 //   as->EndMemoryBlock();
    if (!blockLabel && hasLabel)
        as->Label(label);
    if (node->forceLabel!="")
        as->Label(node->forceLabel);

    if (node->m_isMainBlock && Syntax::s.m_currentSystem->m_system == AbstractSystem::NES)
        as->IncludeFile(":resources/code/nes_init.asm");


    if (node->m_compoundStatement!=nullptr)
        node->m_compoundStatement->Accept(this);

    as->PopBlock(node->m_currentLineNumber);
    if (node->m_isMainBlock && Syntax::s.m_currentSystem->m_system == AbstractSystem::NES) {
        as->StartMemoryBlock("$FFFA");
        as->IncludeFile(":resources/code/nes_end.asm");
        as->EndMemoryBlock();
    }
    if (node->m_isMainBlock)
        as->Label("EndSymbol");


    node->PopZeroPointers(as);
    as->PopCounter(ln);
}

*/

/*
 *
 *
 *  NODE VARDECL
 *
 *
 *
 * */




void ASTDispatcher6502::IncSid(QSharedPointer<NodeVarDecl> node) {

    // Init address or load address? hmmm

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::NES) {

        if (node->sid.m_loadAddress!=0x8000 && Syntax::s.m_currentSystem->m_programStartAddress!=0x8000) {
          QSharedPointer<Appendix> app = QSharedPointer<Appendix>(new Appendix("$8000"));
          app->Append("org $8000",1);
          app->Append("NSFfiller dc.b 0",0);
          as->m_appendix.append(app);
        }
    }

    QString pos = QString::number(node->sid.m_loadAddress,16);
    QSharedPointer<Appendix> app = QSharedPointer<Appendix>(new Appendix("$"+pos));
//    qDebug() << "INCSID dispatcher"<< pos;
    app->Append("org $" +pos,1);
    //        as->Appendix(getValue(v),0);
    app->Append("incbin \"" + as->m_projectDir + node->sid.m_outFile + "\"",1);

    as->m_appendix.append(app);

    int size=0;
    QFile f(as->m_projectDir + node->sid.m_outFile);
    if (f.open(QIODevice::ReadOnly)){
        size = f.size();  //when file does open.
        f.close();
    }
    node->m_fileSize = size;

//    qDebug() << "LOAD ADDRESS **** " << Util::numToHex(node->sid.m_loadAddress);
    as->blocks.append(QSharedPointer<MemoryBlock>(new MemoryBlock(node->sid.m_loadAddress,node->sid.m_loadAddress+size, MemoryBlock::MUSIC, node->sid.m_fileName)));
}



void ASTDispatcher6502::DeclarePointer(QSharedPointer<NodeVarDecl> node) {

    if (!as->CheckZPAvailability())
        ErrorHandler::e.Error("Could not allocate more free pointers! Please free some up, or declare more in the settings page. ", node->m_op.m_lineNumber);

    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);
    QString initVal = t->initVal;

//    qDebug() << "POINTER INIT VAL " <<initVal;
    if (initVal=="") {
        initVal = as->PushZeroPointer();
        node->m_pushedPointers++;
    }

    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    as->Label(v->value + "\t= " + initVal);

}

void ASTDispatcher6502::PrintCompare(QSharedPointer<Node> node, QString lblSuccess, QString lblFailed)
{

    QString bcs ="bcs ";
    QString bcc ="bcc ";
    if (node->isSigned(as)) {
       as->Comment("Signed compare");
       bcs = "bpl ";
       bcc = "bmi ";
    }

    if (node->m_op.m_type==TokenType::EQUALS)
        as->Asm("bne " + lblFailed);
    if (node->m_op.m_type==TokenType::NOTEQUALS)
        as->Asm("beq " + lblFailed);
    if (node->m_op.m_type==TokenType::GREATEREQUAL) {
        as->Asm(bcc + lblFailed);
    }
    if (node->m_op.m_type==TokenType::GREATER) {
        as->Asm(bcc + lblFailed);
        as->Asm("beq " + lblFailed);
    }
    if (node->m_op.m_type==TokenType::LESSEQUAL ) {
        as->Asm("beq " + lblSuccess);
        as->Asm(bcs + lblFailed);
    }

    if (node->m_op.m_type==TokenType::LESS)
        as->Asm(bcs + lblFailed);

}
/*
void ASTDispatcher6502::BinaryClause(QSharedPointer<Node> node)
{

    as->Comment("Binary clause: " + node->m_op.getType());

    BuildToCmp(node);
    as->Comment("BC done");
    QString lblFailed = as->m_lblFailed;
    bool isNew= false;
    if (lblFailed=="") {
        lblFailed = as->NewLabel("binaryclausefailed");
        isNew=true;
    }
    QString lblFinished = as->NewLabel("binaryclausefinished");

    QString lblSuccess = as->NewLabel("binaryclausesuccess");
    PrintCompare(node, lblSuccess,lblFailed);
    as->Label(lblSuccess);
    if (!node->m_ignoreSuccess) {
        as->Asm("lda #1; success");
        as->Asm("jmp " + lblFinished);
        as->Label(lblFailed);
        as->Asm("lda #0 ; failed state");
        as->Label(lblFinished);
    }
    if (isNew)
        as->PopLabel("binaryclausefailed");

    as->PopLabel("binaryclausefinished");
    as->PopLabel("binaryclausesuccess");
    // as->PopLabel("binary_clause_temp_var");
    //  as->PopLabel("binary_clause_temp_lab");

}
*/
void ASTDispatcher6502::BuildToCmp(QSharedPointer<Node> node)
{
    QString b="";

    QSharedPointer<NodeVar> varb = qSharedPointerDynamicCast<NodeVar>(node->m_right);
    if (varb!=nullptr && varb->m_expr==nullptr)
        b = getValue(varb);

    if (node->m_right->isPureNumeric())
//    QSharedPointer<NodeNumber> numb = dynamic_cast<QSharedPointer<NodeNumber>>(node->m_right);
  //  if (numb!=nullptr)
        b = getValue(node->m_right);

//    qDebug() << "WOOT " <<TokenType::getType(node->m_right->getType(as));

    node->m_left->Accept(this);
    as->Term();
    if (b!="") {
        as->Comment("Compare with pure num / var optimization");
        as->Asm("cmp " + b+";keep");
    }
    else {
        // Perform a full compare : create a temp variable
        QString tmpVarB = as->StoreInTempVar("binary_clause_temp");
        node->m_right->Accept(this);
        as->Term();
        QString tmpVarA = as->StoreInTempVar("binary_clause_temp_2");
        as->Asm("lda " + tmpVarB);
        as->Asm("cmp " + tmpVarA +";keep");
        as->PopTempVar();
        as->PopTempVar();
    }


}

void ASTDispatcher6502::BuildSimple(QSharedPointer<Node> node, QString lblSuccess, QString lblFailed, bool page)
{

    if (node->isWord(as)) {
        BinaryClauseInteger(node,lblSuccess, lblFailed, page);
        return;
    }

    as->Comment("Binary clause Simplified: " + node->m_op.getType());
    //    as->Asm("pha"); // Push that baby

    BuildToCmp(node);

    PrintCompare(node, lblSuccess,lblFailed);



}

void ASTDispatcher6502::BinaryClauseInteger(QSharedPointer<Node> node,QString lblSuccess, QString lblFailed, bool page)
{

    as->Comment("Binary clause INTEGER: " + node->m_op.getType());
    //    as->Asm("pha"); // Push that baby
/*
    QSharedPointer<NodeVar> varb = qSharedPointerDynamicCast<NodeVar>(node->m_right);

    QSharedPointer<NodeNumber> numb = qSharedPointerDynamicCast<NodeNumber>(node->m_right);


    QSharedPointer<NodeVar> vara = qSharedPointerDynamicCast<NodeVar>(node->m_left);
*/
    /*
    if (!node->m_left->isPure())
        ErrorHandler::e.Error("Integer comparison: only pure integer number / variable is supported", node->m_op.m_lineNumber);

    if (!node->m_right->isPure())
        ErrorHandler::e.Error("Integer comparison: only pure integer number / variable is supported", node->m_op.m_lineNumber);
*/
    QString lbl2 = lblFailed;
    QString lbl1 = lblSuccess;

    QString lo1,lo2,hi1,hi2;
    Evaluate16bitExpr(node->m_left,lo1,hi1);
    Evaluate16bitExpr(node->m_right,lo2,hi2);
/*    lo1 = getValue8bit(node->m_left,false);
    hi1 = getValue8bit(node->m_left,true);

    lo2 = getValue8bit(node->m_right,false);
    hi2 = getValue8bit(node->m_right,true);
*/
    //m_left->Build(as);
    //as->Term();




    QString bcs ="bcs ";
    QString bcc ="bcc ";
    if (node->isSigned(as)) {
       as->Comment("Signed compare");
       bcs = "bpl ";
       bcc = "bmi ";
       QString label1 = as->NewLabel("label1");
       QString label2 = as->NewLabel("label1");

       if (node->m_op.m_type==TokenType::LESS || node->m_op.m_type==TokenType::LESSEQUAL) {
           as->Asm("sec");
           as->Asm("lda " + hi1 + "   ; compare high bytes");
           as->Asm("sbc " + hi2 + " ");
           as->Asm("bvc " + label1);
           as->Asm("eor #$80");
           as->Label(label1);
           as->Asm("bmi "+lblSuccess);
           as->Asm("bvc "+label2);
           as->Asm("eor #$80");
           as->Label(label2);
           as->Asm("bne "+lblFailed);
           as->Asm("lda " + lo1 + "   ; compare high bytes");
           as->Asm("sbc " + lo2 + " ");
           as->Asm("bcs "+lblFailed);
            return;
/*
           SEC
                LDA NUM1H  ; compare high bytes
                SBC NUM2H
                BVC LABEL1 ; the equality comparison is in the Z flag here
                EOR #$80   ; the Z flag is affected here
         LABEL1 BMI LABEL4 ; if NUM1H < NUM2H then NUM1 < NUM2
                BVC LABEL2 ; the Z flag was affected only if V is 1
                EOR #$80   ; restore the Z flag to the value it had after SBC NUM2H
         LABEL2 BNE LABEL3 ; if NUM1H <> NUM2H then NUM1 > NUM2 (so NUM1 >= NUM2)
                LDA NUM1L  ; compare low bytes
                SBC NUM2L
                BCC LABEL4 ; if NUM1L < NUM2L then NUM1 < NUM2
         LABEL3
  */


       }
       ErrorHandler::e.Error("Signed integer comparison: only 'less' (&le;) is currently implemented.", node->m_op.m_lineNumber);


    }




        as->Comment("Compare INTEGER with pure num / var optimization. GREATER. ");
        if (node->m_op.m_type==TokenType::GREATER) {
            as->Asm("lda " + hi1 + "   ; compare high bytes");
            as->Asm("cmp " + hi2 + " ;keep");
            as->Asm(bcc + lbl2);
        //    as->Asm("beq " + lbl2);
            as->Asm("bne " + lbl1);
            as->Asm("lda " + lo1);
            as->Asm("cmp " + lo2 +" ;keep");
            as->Asm(bcc + lbl2);
            as->Asm("beq " + lbl2);
        }
        if (node->m_op.m_type==TokenType::GREATEREQUAL) {
            as->Asm("lda " + hi1 + "   ; compare high bytes");
            as->Asm("cmp " + hi2 + " ;keep");
            as->Asm(bcc + lbl2);
            as->Asm("bne " + lbl1);
            as->Asm("lda " + lo1);
            as->Asm("cmp " + lo2 +" ;keep");
            as->Asm(bcc + lbl2);
        }
        if (node->m_op.m_type==TokenType::LESS || node->m_op.m_type==TokenType::LESSEQUAL) {
            as->Asm("lda " + hi1 + "   ; compare high bytes");
            as->Asm("cmp " + hi2 + " ;keep");
            as->Asm(bcc + lbl1);
            as->Asm("bne " + lbl2);
            as->Asm("lda " + lo1);
            as->Asm("cmp " + lo2+" ;keep");
            if (node->m_op.m_type==TokenType::LESSEQUAL)
                as->Asm("beq "+lbl1);
            as->Asm(bcs + lbl2);



        }
        if (node->m_op.m_type==TokenType::EQUALS) {
            as->Asm("lda " + hi1 + "   ; compare high bytes");
            as->Asm("cmp " + hi2 + " ;keep");
            as->Asm("bne " + lbl2);
            as->Asm("lda " + lo1);
            as->Asm("cmp " + lo2+" ;keep");
            as->Asm("bne " + lbl2);
            as->Asm("jmp " + lbl1);
        }
        if (node->m_op.m_type==TokenType::NOTEQUALS){
//            ErrorHandler::e.Error("Comparison of integer NOTEQUALS<> not implemented!", node->m_op.m_lineNumber);
            QString lblPass1  = as->NewLabel("pass1");
            as->Asm("lda " + hi1 + "   ; compare high bytes");
            as->Asm("cmp " + hi2 + " ;keep");
            as->Asm("beq " + lblPass1);
            as->Asm("jmp " + lbl1);
            as->Label(lblPass1);
            as->Asm("lda " + lo1);
            as->Asm("cmp " + lo2+" ;keep");
            as->Asm("beq " + lbl2);
            as->Asm("jmp " + lbl1);
            as->PopLabel("pass1");

        }
/*        as->Label(lbl1); // ok
        as->Asm("lda #1");
        as->Asm("jmp " + lbl3);
        as->Label(lbl2); // failed
        as->Asm("lda #2");

        as->Label(lbl3); // DONE

*/
        // Now all is ok
  //  }
  /*  else {
        ErrorHandler::e.Error("Comparison of integer only implemented for pure number or variable",node->m_op.m_lineNumber);
    }
*/



    /*
        if (m_op.m_type==TokenType::EQUALS)
            as->Asm("bne " + lblFailed);
        if (m_op.m_type==TokenType::NOTEQUALS)
            as->Asm("beq " + lblFailed);
        if (m_op.m_type==TokenType::GREATER)
            as->Asm("bcc " + lblFailed);
        if (m_op.m_type==TokenType::LESS)
            as->Asm("bcs " + lblFailed);

        as->Asm("lda #1; success");
        as->Asm("jmp " + lblFinished);
        as->Label(lblFailed);
        as->Asm("lda #0 ; failed state");
        as->Label(lblFinished);
    */
 //   as->PopLabel("binaryclauseinteger_success");
 //   as->PopLabel("binaryclauseinteger_fail");
 //   as->PopLabel("binaryclauseintegerfinished");
    // as->PopLabel("binary_clause_temp_var");
    //  as->PopLabel("binary_clause_temp_lab");
}


bool ASTDispatcher6502::IsSimpleAndOr(QSharedPointer<NodeBinaryClause> node, QString labelSuccess, QString labelFail)
{
    if(node==nullptr)
        return false;
//    if (dynamic_cast<NodeBinaryClausenode->m_left)
    QSharedPointer<NodeBinaryClause> a = qSharedPointerDynamicCast<NodeBinaryClause>(node->m_left);
    QSharedPointer<NodeBinaryClause> b = qSharedPointerDynamicCast<NodeBinaryClause>(node->m_right);
//    return false;
    if (a==nullptr || b==nullptr)
        return false;
    if (a->m_op.m_type==TokenType::AND || a->m_op.m_type==TokenType::OR)
        return false;
    if (b->m_op.m_type==TokenType::AND || b->m_op.m_type==TokenType::OR)
        return false;


//    return false;
    if (node->m_op.m_type==TokenType::AND) {
        a->m_ignoreSuccess = true;
        b->m_ignoreSuccess = true;

        as->m_lblFailed = labelFail;
//        as->m_lblSuccess = labelSuccess;
        a->Accept(this);
        b->Accept(this);
        as->m_lblFailed="";
        as->m_lblSuccess="";
        return true;
    }

    if (node->m_op.m_type==TokenType::OR) {
        a->m_ignoreSuccess = true;
        b->m_ignoreSuccess = true;
        QString tempFailLabel = as->NewLabel("tempfail");
       // as->m_lblSuccess = labelSuccess;
        as->m_lblFailed = tempFailLabel;
        a->Accept(this);
        as->Asm("jmp "+labelSuccess);
        as->m_lblFailed = labelFail;
        as->Label(tempFailLabel);
        b->Accept(this);
        as->PopLabel("tempfail");

        as->m_lblFailed="";
        as->m_lblSuccess="";
        return true;

    }
    return false;

//    as->m_lblFailed="";
  //  as->m_lblSuccess="";
    //  return true;
}

bool ASTDispatcher6502::IsSimpleAssignPointerExpression(QSharedPointer<NodeVar> var, QSharedPointer<NodeAssign> node)
{
    if (var->getType(as)!=TokenType::POINTER)
        return false;
    if (var->m_expr==nullptr)
        return false;

    if (var->getArrayType(as)==TokenType::INTEGER) {
        if (!node->m_right->isPure())
            return false; // for now, only pure RHS expr
        as->Comment("Is simple pointer assigning : p[n] := expr");
        // First, set up y:
        if (!var->m_expr->isPureNumeric()) {
            as->ClearTerm();
            var->m_expr->Accept(this);
            as->Term();
            as->Asm("asl");
            as->Asm("tay");
        }
        else as->Asm("ldy #"+QString::number(var->m_expr->getValueAsInt(as)*2));
        as->Asm("lda "+node->m_right->getValue8bit(as,false));
        as->Asm("sta ("+var->getValue(as)+"),y");
        as->Asm("iny");
        as->Asm("lda "+node->m_right->getValue8bit(as,true));
        as->Asm("sta ("+var->getValue(as)+"),y");
        return true;
    }



    return false;
}





/*
 *
 *
 *  NODE BINARY CLAUSE
 *
 */

/*
void ASTDispatcher6502::dispatch(QSharedPointer<NodeBinaryClause> node)
{
    node->DispatchConstructor(as,this);

    //    node->accept(this);
    //    Node::Build(as);

    // First, check the byte
    if (node->m_op.m_type==TokenType::AND || node->m_op.m_type == TokenType::OR) {
        LogicalClause(node);
        //qDebug() << "NodeBinaryClause dispatch ";
    }
    else
        if (node->m_op.m_type==TokenType::LESS || node->m_op.m_type == TokenType::GREATER ||
                node->m_op.m_type==TokenType::EQUALS || node->m_op.m_type == TokenType::NOTEQUALS
                || node->m_op.m_type==TokenType::LESSEQUAL || node->m_op.m_type == TokenType::GREATEREQUAL )
        {

            if (node->m_left->getType(as)==TokenType::INTEGER || (node->m_left->getType(as)==TokenType::POINTER && !node->m_left->isArrayIndex())) {
                BinaryClauseInteger(node);
            }
            else
                BinaryClause(node);
        }
        else
            ErrorHandler::e.Error("Unknown compare type : '" + node->m_op.m_value+"'. Did you mean '=' or '>' etc?",node->m_op.m_lineNumber);

}



void ASTDispatcher6502::LogicalClause(QSharedPointer<Node> node)
{
    if (qSharedPointerDynamicCast<NodeBinaryClause>(node->m_left)==nullptr)
        ErrorHandler::e.Error("Logical clause: left hand term must be binary clause");

    if (qSharedPointerDynamicCast<NodeBinaryClause>(node->m_right)==nullptr)
        ErrorHandler::e.Error("Logical clause: right hand term must be binary clause");


    // Test for optimization : if left and right are pure


    node->m_left->Accept(this);
    QString tmpVar = as->StoreInTempVar("logical_class_temp");
    node->m_right->Accept(this);
    if (node->m_op.m_type==TokenType::AND)
        as->Asm("and " + tmpVar);

    if (node->m_op.m_type==TokenType::OR)
        as->Asm("ora " + tmpVar);


    as->PopTempVar();

    //as->Asm("lda " + tmpVar);

    // Done comparing!
}

*/
void ASTDispatcher6502::Compare(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, bool isLarge, QString loopDone, QString loopNotDone, bool inclusive) {


    if (nodeA->m_left->isWord(as)) {
        Token t = nodeA->m_op;
        Token t_asm = nodeA->m_op;
        Token t_clause = nodeA->m_op;
        t_clause.m_type = TokenType::NOTEQUALS;
        t_asm.m_value = "  jmp " + loopNotDone + "\n";
        auto nasm = QSharedPointer<NodeAsm>(new NodeAsm(t_asm));
        QSharedPointer<NodeCompound> comp = QSharedPointer<NodeCompound>(new NodeCompound(t));
        comp->children.append(nasm);

        QSharedPointer<NodeBlock> block = QSharedPointer<NodeBlock>(new NodeBlock(t,QVector<QSharedPointer<Node>>(),
                                                                                        comp, false));



        QSharedPointer<NodeBinaryClause> clause = QSharedPointer<NodeBinaryClause>(
                    new NodeBinaryClause(t_clause,nodeA->m_left, nodeB));

        QSharedPointer<NodeConditional> cond = QSharedPointer<NodeConditional>(
                    new NodeConditional(t,isLarge,clause,block,false));

        as->Comment("Executing integer comparison " + nodeB->getValue(as));
        cond->Accept(this);
        return;



    }
    if (nodeB->isPure() && (1==2)) { // DISABLE this optimization - for now
        as->Comment("Optimization: switch A and B, allow for optimizer");
        as->ClearTerm();
        nodeA->m_left->Accept(this);
        as->Term();

        as->Asm("cmp " + nodeB->getValue(as) +" ;keep");

    }
    else {
        as->ClearTerm();
        nodeB->Accept(this);
        as->Term();

        as->Asm("cmp " + nodeA->m_left->getValue(as) +" ;keep");
    }
    int stepValue = 1; // do we have a step value?
    if (step != nullptr) {
        stepValue = step->getValueAsInt(as); //node->m_step->getValue(as);
    }
    if (inclusive) {    // inclusive version will END after the TO value

        if (!isLarge) {

            if (stepValue > 0) {
                as->Asm("bcs "+loopNotDone); // or FOR index > TO value
            } else { //if (stepValue < -1) {
                as->Asm("bcc "+loopNotDone); // FOR index < TO value
                as->Asm("beq "+loopNotDone); // BEQ then the BCC below
            }

        }
        else {

            // LargeLoops needs checking
            if (stepValue > 0) {
                as->Asm("bcc "+loopDone); // or FOR index > TO value
            } else { //if (stepValue < -1) {
                as->Asm("beq "+loopNotDone); // BEQ then the BCC below
                as->Asm("bcs "+loopDone); // FOR index < TO value
            }

        }

    }
    else {            // TRSE version will END on the TO value

        if (!isLarge) {

            if (stepValue == 1 || stepValue == -1) {
                // increments / decrements of 1 are safe for BNE
                as->Asm("bne "+loopNotDone);
            } else if (stepValue > 1) {
                as->Asm("beq "+loopDone); // FOR index == TO value
                as->Asm("bcs "+loopNotDone); // or FOR index > TO value
            } else { //if (stepValue < -1) {
                as->Asm("bcc "+loopNotDone); // FOR index < TO value
            }

        }
        else {

            // LargeLoops needs checking
            //as->Asm("beq "+loopDone);
            if (stepValue == 1 || stepValue == -1) {
                // increments / decrements of 1 are safe for BNE
                as->Asm("beq "+loopDone);
            } else if (stepValue > 1) {
                as->Asm("beq "+loopDone); // FOR index == TO value
                as->Asm("bcc "+loopDone); // or FOR index > TO value
            } else { //if (stepValue < -1) {
                as->Asm("bcs "+loopDone); // FOR index < TO value
            }

        }

        return;
    }

/* // FOR loops with word values not currently supported
    ErrorHandler::e.Error("Compare word not implemented in for loops");

    // Is word:
// Branch to   LABEL2 if NUM1 < NUM2

    QString label1 = as->NewLabel("forWordLabel1");
    QString label2 =as->getLabel("for");
//    QString tvar = as->NewLabel("forWordVar");
    as->ClearTerm();

    as->Comment("Compare integers");
    node->m_b->Accept(this);
    as->Term();
    QString tvar = as->StoreInTempVar("wordVar","word");

  //  as->Asm("lda "+as->m_stack["for"].current());

    QString counter = as->m_stack["for"].current();

    as->Asm("lda " + counter+"+1");
    as->Asm("cmp " + tvar+" +1 ");
    as->Asm("bcc " + label2);
    as->Asm("bne " + label1);
    as->Asm("lda " + counter);
    as->Asm("cmp " + tvar);
    as->Asm("bcc "+  label2);
    as->Label(label1);
// Branches to LABEL2 if NUM1 < NUM2

*/

/*    LDA NUM1H  ; compare high bytes
             CMP NUM2H
             BCC LABEL2 ; if NUM1H < NUM2H then NUM1 < NUM2
             BNE LABEL1 ; if NUM1H <> NUM2H then NUM1 > NUM2 (so NUM1 >= NUM2)
             LDA NUM1L  ; compare low bytes
             CMP NUM2L
             BCC LABEL2 ; if NUM1L < NUM2L then NUM1 < NUM2


    as->PopLabel("forWordLabel1");*/
//    as->PopLabel("forWordVar");
}

void ASTDispatcher6502::IncreaseCounter(QSharedPointer<Node> step, QSharedPointer<NodeVar> var) {

    // no STEP included in FOR TO DO, we assume STEP 1
    if (step==nullptr) {

        if (!var->isWord(as)) {
            as->Asm("inc " + var->getValue(as));
        }
        else {
            as->Asm("inc " + var->getValue(as));
            QString lbl = as->NewLabel("lblCounterWord");
            as->Asm("bne "+lbl);
            as->Asm("inc " + var->getValue(as) +"+1");

            as->Label(lbl);

            as->PopLabel("lblCounterWord");

        }

    } else {




        // STEP included in FOR TO DO statement
        int stepValue = step->getValueAsInt(as);
        //stepValue = node->m_step->getInteger();
        //qDebug() << node->m_step->numValue();
        //qDebug() << node->m_step->getInteger();

        if (var->isWord(as)) {
            if (stepValue == 1) {
                as->Asm("inc " + var->getValue(as));
                QString lbl = as->NewLabel("lblCounterWord");
                as->Asm("bne "+lbl);
                as->Asm("inc " + var->getValue(as) +"+1");

                as->Label(lbl);

                as->PopLabel("lblCounterWord");
                return;

            }
            if (stepValue == 255) {
                as->Comment("Decrement word counter");
                as->Asm("lda " + var->getValue(as));
                QString lbl = as->NewLabel("lblCounterWord");
                as->Asm("bne "+lbl);
                as->Asm("dec " + var->getValue(as) +"+1");

                as->Label(lbl);
                as->Asm("dec " + var->getValue(as));

                as->PopLabel("lblCounterWord");
                return;

            }
            if (!step->isPure())
                ErrorHandler::e.Error("16 bit counters in for loops can only have integer steps", var->m_op.m_lineNumber);

            QString lbl = as->NewLabel("lblCounterWord");
            as->Asm("clc");
            as->Asm("lda " + var->getValue(as));
            as->Asm("adc #" + Util::numToHex(stepValue));
            as->Asm("sta " + var->getValue(as));

            as->Asm("lda " + var->getValue(as)+"+1");
            as->Asm("adc #0");
            as->Asm("sta " + var->getValue(as)+"+1");

            as->PopLabel("lblCounterWord");
            return;


        }

        // if 1 or -1 we can optimise!
        if (stepValue == 1) {

            as->Asm("inc " + var->getValue(as));

        } else if (stepValue == -1) {

            as->Asm("dec " + var->getValue(as));

        } else {
            // Larger +ve/-ve STEP

            // Handles a -ve and +ve step as a byte, eg: -1 == 255
            as->Asm("clc");
            as->Asm("lda " + var->getValue(as));
            as->ClearTerm();
            as->Term("adc ");
            step->Accept(this);
            //            m_step->Build(as);
            as->Term();

            if (var->isWord(as)) {
                    QString lbl = as->NewLabel("lblCounterWord");
                    as->Asm("bcc "+lbl);
                    as->Asm("inc " + var->getValue(as) +"+1");
                    as->Label(lbl);
                    as->PopLabel("lblCounterWord");
            }
            as->Asm("sta "+var->getValue(as));

        }

    }



 //       if (node->m_loopCounter!=0)
   //         ErrorHandler::e.Error("Error: Loop with step other than 1,-1 cannot have loopy/loopx flag");
        // Is word


}

// handle a small loop
void ASTDispatcher6502::SmallLoop(QSharedPointer<NodeForLoop> node, QSharedPointer<NodeVar> var, bool inclusive)
{
    QString loopDone = as->NewLabel("forLoopDone");
    //  Compare(as);
    //  as->Asm("beq "+loopDone);

    node->m_block->Accept(this);
    as->m_stack["for"].pop();
    IncreaseCounter(node->m_step,var);
  //  Compare(node, var, false, loopDone, nullptr, inclusive);

//    as->Asm("jmp " + as->getLabel("for"));

    as->Label(loopDone);

    as->m_labelStack["for"].pop();
    as->m_labelStack["forLoopDone"].pop();

}

// handle a large loop
void ASTDispatcher6502::LargeLoop(QSharedPointer<NodeForLoop> node, QSharedPointer<NodeVar> var, bool inclusive)
{
    QString loopForFix = as->NewLabel("forLoopFix");
    QString loopDone = as->NewLabel("forLoopDone");
    QString loopNotDone = as->NewLabel("forLoopNotDone");

    as->Label(loopForFix);
    node->m_block->Accept(this);
    as->m_stack["for"].pop();

    IncreaseCounter(node->m_step,var);
//    Compare(node, var, true, loopDone, loopNotDone, inclusive);

    as->Label(loopNotDone);
    as->Asm("jmp " + as->getLabel("for"));

    as->Label(loopDone);

    as->m_labelStack["for"].pop();
    as->m_labelStack["forLoopFix"].pop();
    as->m_labelStack["forLoopDone"].pop();
    as->m_labelStack["forLoopNotDone"].pop();

}



/*
 *
 *
 *
 *
 * NodeConditional
 *
 *
 *
*/



/*
void ASTDispatcher6502::dispatch(QSharedPointer<NodeConditional> node)
{
    node->DispatchConstructor(as,this);


//    as->PushCounter();
    QString labelStartOverAgain = as->NewLabel("while");
    QString lblstartTrueBlock = as->NewLabel("ConditionalTrueBlock");

    QString labelElse = as->NewLabel("elseblock");
    QString labelElseDone = as->NewLabel("elsedoneblock");
    // QString labelFailed = as->NewLabel("conditionalfailed");

    if (node->m_isWhileLoop)
        as->Label(labelStartOverAgain);

    // Test all binary clauses:
    bool isSimplified = false;
    bool isOKBranchSize = true;
    QSharedPointer<NodeBinaryClause> bn = qSharedPointerDynamicCast<NodeBinaryClause>(node->m_binaryClause);

//    qDebug() << "TESTVERIFY BRANCH SIZE ";
    if (node->verifyBlockBranchSize(as, node->m_block)) {
        isSimplified = !bn->cannotBeSimplified(as);
    }
    else isOKBranchSize = false;

    // Then, check m_forcepage
    if (node->m_forcePage==1) // force OFFPAGE
        isSimplified = false;

    if (node->m_forcePage==2) {
        if (!bn->cannotBeSimplified(as)) // force ONPAGE
            isSimplified = true;
        else ErrorHandler::e.Error("keyword onpage can only be used with 1 compare clause (no and, or etc)", node->m_op.m_lineNumber);
    }

    if (!isSimplified) {
//        node->m_binaryClause->Build(as);
        if (isOKBranchSize && IsSimpleAndOr(qSharedPointerDynamicCast<NodeBinaryClause>(node->m_binaryClause), lblstartTrueBlock,labelElse)){
        }
        else {
            as->Comment("Full binary clause");
//            as->Comment(" Here : " + QString::number(isOKBranchSize) +" "  +
  //                      QString::number(IsSimpleAndOr(dynamic_cast<QSharedPointer<NodeBinaryClause>>(node->m_binaryClause), lblstartTrueBlock,labelElse)));
            node->m_binaryClause->Accept(this);
            // Now, a should be either true or false

            as->Asm("cmp #1");
            as->Asm("beq " + lblstartTrueBlock); // All conditionals checked out!
            // Do we have an else block?
            if (node->m_elseBlock!=nullptr)
                as->Asm("jmp " + labelElse); // All conditionals false: skip to end (or else block)
            as->Asm("jmp " + labelElseDone);
        }
    }
    else {
        // Simplified version <80 instructions & just one clause

        QString failedLabel = labelElseDone;
        if (node->m_elseBlock!=nullptr)
            failedLabel = labelElse;
        BuildSimple(bn,  lblstartTrueBlock,failedLabel,node->m_forcePage==1 );
    }
    // Start main block
    as->Label(lblstartTrueBlock); // This means skip inside

    node->m_block->Accept(this);

    if (node->m_elseBlock!=nullptr)
        as->Asm("jmp " + labelElseDone);

    // If while loop, return to beginning of conditionals
    if (node->m_isWhileLoop)
        as->Asm("jmp " + labelStartOverAgain);

    // An else block?
    as->Label(labelElse);
    if (node->m_elseBlock!=nullptr) {
//        as->Label(labelElse);
        node->m_elseBlock->Accept(this);
//        m_elseBlock->Build(as);

    }
    as->Label(labelElseDone); // Jump here if not

    as->PopLabel("while");
    as->PopLabel("ConditionalTrueBlock");
    as->PopLabel("elseblock");
    as->PopLabel("elsedoneblock");
//    as->PopLabel("conditionalfailed");

  //  as->PopCounter(node->m_op.m_lineNumber);

}

*/
/*
 *
 *
 *
 *
 * NodeForLoop
 *
 *
 *
*/


/*
void ASTDispatcher6502::dispatch(QSharedPointer<NodeForLoop> node)
{
    node->DispatchConstructor(as,this);


    QSharedPointer<NodeAssign> nVar = qSharedPointerDynamicCast<NodeAssign>(node->m_a);

    // get the inclusive flag for the method used for the coparison, ie: false is < and true is <=
    bool inclusive =(node->m_inclusive);

    // left QSharedPointer<Node> must* be an assign statement (e.g a:=10)
    if (nVar==nullptr)
        ErrorHandler::e.Error("Index must be variable", node->m_op.m_lineNumber);


    // Get the variable name
    QString var = getValue(nVar->m_left);
    // accept statement (assign variable)
    node->m_a->Accept(this);


    as->m_stack["for"].push(var);

    as->Label(as->NewLabel("for"));

    bool isSmall = node->verifyBlockBranchSize(as, node->m_block,nullptr,this);

    if (node->m_forcePage == 1)
        isSmall = false;

    if (node->m_forcePage == 2)
        isSmall = true;

    if (isSmall)
        SmallLoop(node,qSharedPointerDynamicCast<NodeVar>(nVar->m_left), inclusive);
    else
        LargeLoop(node,qSharedPointerDynamicCast<NodeVar>(nVar->m_left), inclusive);

}
*/



void ASTDispatcher6502::LoadPointer(QSharedPointer<NodeVar> node) {
    as->Comment("Load pointer array");
    QSharedPointer<NodeNumber> number = qSharedPointerDynamicCast<NodeNumber>(node->m_expr);
    QString m = as->m_term;

    as->ClearTerm();

    if (node->m_expr==nullptr) {
        as->Asm("lda "+getValue(node));
        as->Asm("ldy "+getValue(node) + " +1");
        return;
    }

    if (!LoadXYVarOrNum(node, node->m_expr,false))
    {
        if (!(m=="" || m.startsWith("lda")))
            as->Asm("pha");
        node->m_expr->Accept(this);
        as->Term();
        as->Asm("tay");
        if (!(m=="" || m.startsWith("lda")))
            as->Asm("pla");
    }
    if (node->getArrayType(as)==TokenType::INTEGER) {
        as->Asm("lda (" + getValue(node)+"),y");
        as->Asm("pha");
        as->Asm("iny");
        as->Asm("lda (" + getValue(node)+"),y");
        as->Asm("tay");
        as->Asm("pla");
        return;
    }

    if (m=="")
        m="lda ";
    as->Asm(m+  "(" + getValue(node)+"),y");
    if (node->m_forceType == TokenType::INTEGER)
        as->Asm("ldy #0 ; Loading 8-bit pointer, but return type should be integer");
}


/*
 *
 *
 * NodeVar
 *
 *
 *
 *
*/

void ASTDispatcher6502::dispatch(QSharedPointer<NodeVar> node)
{
    // Override inline parameters
//    qDebug() << "INLINE cnt :" <<m_inlineParameters.count() << m_inlineParameters.keys();
    if (m_inlineParameters.contains(node->value)) {
  //      qDebug()<< "INLINE node override : "<< node->value;
        m_inlineParameters[node->value]->Accept(this);
        return;
    }

    node->DispatchConstructor(as,this);

    QString  val = getValue(node);
    Pmm::Data::d.lineNumber = node->m_op.m_lineNumber;
    QSharedPointer<Symbol> s = as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber);
    //        if (s==nullptr) {
    //          ErrorHandler::e.Error("Could not find variable '" + value +"'.\nDid you mispell?", m_op.m_lineNumber);
    //    }
    if (node->m_expr!=nullptr) {
        LoadVariable(node);

        //LoadByteArray(as);
    }
    else {
        bool isOK = true;
//        qDebug() << val << " is " << s->getTokenType();

        if (s->getTokenType()==TokenType::INTEGER)
            isOK = false;
        if (s->getTokenType()==TokenType::POINTER && as->m_term=="") {
            isOK = false;
        }
//        qDebug() << "HERE " <<as->m_term << node->isReference() << val;
        if (((s->getTokenType()==TokenType::ADDRESS || s->getTokenType()==TokenType::INCBIN || node->isReference())  && as->m_term=="")) {
            as->Asm("lda #<" + val);
            as->Asm("ldy #>" + val);
            return;
        }
        if (node->m_fake16bit && s->getTokenType()==TokenType::BYTE )
            as->Asm("ldy #0 ; Fake 16 bit");
        as->Variable(val, isOK);
    }

}



bool ASTDispatcher6502::LoadXYVarOrNum(QSharedPointer<NodeVar> node, QSharedPointer<Node> other, bool isx) {
    QSharedPointer<Symbol> s = as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber);
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(other);
    //QSharedPointer<NodeNumber> num = dynamic_cast<QSharedPointer<NodeNumber>>(other);
    if (other==nullptr)
        return false;
    bool isNumber = other->isPureNumeric();
    QString operand = "ldx ";
    QString x = "x";
    if (!isx) {
        operand="ldy ";
        x = "y";
    }



    if (var!=nullptr && var->m_expr == nullptr) {
            if (s->m_arrayType==TokenType::INTEGER) // integer array index is *2 (two bytes per array slot)
            {
                //as->Asm("txa   ; watch for bug, Integer array has index range of 0 to 127");
                as->Asm("lda "+ getValue(var));
                as->Asm("asl");
                as->Asm("ta"+x);
            }
            else {
                as->Asm(operand + getValue(var));

            }
        return true;
    }
    if (isNumber) {
//        qDebug() << "LoadXYVarorNum HERE ";
        if (s->m_arrayType==TokenType::INTEGER) {
            as->Asm(operand + "#" + QString::number(other->getValueAsInt(as) * 2) + " ; watch for bug, Integer array has max index of 128");
        }
        else
            as->Asm(operand  + getValue(other));
        return true;
    }
    return false;

}

void ASTDispatcher6502::LoadByteArray(QSharedPointer<NodeVar> node) {
    // Optimizer: if expression is number, just return direct

    QSharedPointer<Symbol> s = as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber);
    if (node->isReference()) {
        // This should now be handled by the parser
        ErrorHandler::e.Error("Unknown syntax: referenced address with index. ", node->m_op.m_lineNumber);
        // Handle reference! 16-bit
        //Load16bitVariable(node);
/*        if (node->m_expr->isPureNumeric()) {
            as->Asm("ldy "+getValue8bit(node,true));
            int size = 1;
            if (node->getArrayType(as)==TokenType::INTEGER)
                size=2;
            as->Asm("lda "+getValue8bit(node,false) +" + "+ Util::numToHex(node->m_expr->getValueAsInt(as)*size));

        }
        return;
*/
    }

    if (s->m_arrayType==TokenType::INTEGER)
        as->Comment("Load Integer array");

    else if (s->m_arrayType==TokenType::BYTE)
        as->Comment("Load Byte array");
    else
        as->Comment("Load Unknown type array");

    // Optimization : ldx #3, lda a,x   FIX
    if (s->m_arrayType==TokenType::BYTE && node->m_expr!=nullptr) {
        if (node->m_expr->isPureNumeric()) {
            as->ClearTerm();
            as->Asm("lda "+getValue(node) + " +"+node->m_expr->getValue(as) + " ; array with const index optimization");
            return;
        }
    }

    QString m = as->m_term;
    if (node->m_expr==nullptr) {
        ErrorHandler::e.Error("Unknown operation with address!",node->m_op.m_lineNumber);
    }
    as->ClearTerm();
    if (!LoadXYVarOrNum(node, node->m_expr,true))
    {
        // calculation version, eg: index+2  or 3+2

        as->ClearTerm();
        if (m_flag1)
            as->Asm("pha");

        //as->Asm("pha");
        node->m_expr->Accept(this);
        as->Term();
        if (s->m_arrayType==TokenType::INTEGER) // integer array index is *2 (two bytes per array slot)
            as->Asm("asl");
        as->Asm("tax");
        if (m_flag1)
            as->Asm("pla");
    }
    if (m=="")
        m="lda ";
    as->Asm(m+  getValue(node)+",x");

    if (s->m_arrayType==TokenType::INTEGER) { // integer array need to load the high byte also
//        as->Asm("ldy "+  getValue(node)+",x");
  //      as->Asm("inx");
        as->Asm("ldy "+  getValue(node)+"+1,x");
    }
}

void ASTDispatcher6502::LoadVariable(QSharedPointer<Node> node)
{
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node);
    if (v!=nullptr) {
        LoadVariable(v);
        return;
    }

    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(node);
    if (num!=nullptr) {
        LoadVariable(num);
        return;
    }

    node->Accept(this);
//    ErrorHandler::e.Error("Uknown variable type!", node->m_op.m_lineNumber);

}


void ASTDispatcher6502::LoadVariable(QSharedPointer<NodeVar> node) {

    /*        if (as->m_symTab->Lookup(value)==nullptr)
                ErrorHandler::e.Error("Could not find variable '" +value +"' for storing.",m_op.m_lineNumber);
    */
    TokenType::Type t = as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber)->getTokenType();
    if (t==TokenType::ADDRESS || t==TokenType::STRING || t==TokenType::CSTRING || t==TokenType::INCBIN) {
        LoadByteArray(node);
        return;
    }
    if (t==TokenType::POINTER) {
        LoadPointer(node);
        return;
    }

    if (t==TokenType::BYTE) {
        if (node->m_expr!=nullptr)
            LoadByteArray(node);
        else {
            Load16bitVariable(node);
        }
        return;
    }
    if (t == TokenType::INTEGER) {
        node->m_isWord = true;
        if (node->m_expr!=nullptr)
            LoadByteArray(node);
        else
            Load16bitVariable(node);
        return;
    }
    ErrorHandler::e.Error(TokenType::getType(t) + " assignment not supported yet for exp: " + getValue(node));
    return;
}

void ASTDispatcher6502::LoadAddress(QSharedPointer<Node> node)
{

}

void ASTDispatcher6502::LoadVariable(QSharedPointer<NodeNumber>node)
{
   as->ClearTerm();
//   qDebug() << "OAD NUMBER";
   if (node->isReference()) {
       as->ClearTerm();
       as->Asm("lda "+node->getValue8bit(as,false));
       as->Asm("ldy "+node->getValue8bit(as,true));
        return;
   }
   as->Term("lda ");
   node->Accept(this);
   as->Term();
}

void ASTDispatcher6502::LoadVariable(QSharedPointer<NodeProcedure> node)
{
    as->Asm("lda #<"+node->m_procedure->m_procName);
    as->Asm("ldy #>"+node->m_procedure->m_procName);
}


void ASTDispatcher6502::StoreVariable(QSharedPointer<NodeVar> node) {
    //        as->Comment("VarNode StoreVariable");
    //          ErrorHandler::e.Error("Could not find variable '" +value +"' for storing.", m_op.m_lineNumber);
    as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber);
    if (node->m_subNode!=nullptr){
        QSharedPointer<NodeVar> nv = qSharedPointerDynamicCast<NodeVar>(node->m_subNode);
        if (nv->m_expr!=nullptr) {
            node->m_expr = nv->m_expr;
            nv->m_expr = nullptr;
            node->m_ignoreRecordExpr = true;
            nv->m_ignoreRecordExpr = true;
        }
    }

    if (node->m_expr != nullptr) {
//        QSharedPointer<NodeNumber> number = dynamic_cast<QSharedPointer<NodeNumber>>(node->m_expr);
        bool isNumber = node->m_expr->isPureNumeric();
        if (isNumber && node->getType(as)!=TokenType::POINTER)
        { // IS NUMBER optimize}
            if (node->getArrayType(as)==TokenType::INTEGER) {
                // Store integer array
                int i = node->m_expr->getValueAsInt(as)*2;
                as->Asm("sta " + getValue(node) + "+"+ QString::number(i));
                as->Asm("sty "  + getValue(node) +"+"+ QString::number(i+1));

            }
            else {
                as->Asm("sta " + getValue(node) + "+"+ getValue(node->m_expr));
            }
            //                as->Asm("tya");
            return;
        }
        else {
            //if regular array
            QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(node->m_expr);
            //QSharedPointer<NodeNumber> num = dynamic_cast<QSharedPointer<NodeNumber>>(node->m_expr);
            QString secondReg="x";
            QString pa = "";
            QString pb= "";
            if (node->getType(as)==TokenType::POINTER) {
                secondReg="y";
                pa="(";
                pb=")";
                as->Comment("Storing to a pointer");
            }



            // Optimize for number or pure var
            if (node->m_expr->getType(as)==TokenType::INTEGER_CONST && node->getArrayType(as)!=TokenType::INTEGER) {
                //qDebug() << "StoreVariable:: HER";
                as->ClearTerm();

                // Special case: if POINTER



                as->Term("ld"+secondReg +" ");
                node->m_expr->Accept(this);
                as->Term();
//                as->Asm("COMMENT BUT WHY DOES IT DISAPPEAR");
                as->Asm("sta " +pa + getValue(node)+ pb + "," + secondReg);
                if (node->getArrayType(as)==TokenType::INTEGER) {
                    as->Asm("in"+secondReg);
                    as->Asm("tya");
//                    as->Asm("sta " +pa + getValue(node)+ pb + "," + secondReg);

                    as->Asm("sta "  + getValue(node) + "," + secondReg);
                }
                return;
            }
            // Just regular var optimize
            // Regular expression

            // Special case: p[5] := something;
/*            if (node->isPointer(as) && node->m_expr->getType(as)==TokenType::INTEGER_CONST && node->getArrayType(as)==TokenType::INTEGER)
            {
                as->Comment("Integer const pointer set to expression");
                QString val = QString::number(node->m_expr->getValueAsInt(as)*2);
                as->ReplacePrevLdyWithLdx();
//                if (node->)
                return;
            }
*/

            QString tya ="tya";
            if (node->isPointer(as) && node->getArrayType(as)==TokenType::INTEGER) {
                as->Comment("Storing integer to a pointer of integer, need to move data in y to x");
                as->Asm("pha");
                as->Asm("tya");
                as->Asm("tax");
                as->Asm("pla");
                tya = "txa";
            }

            bool usePush = true;

//            if (node->m_expr->isPure())
  //              usePush = false;

            if (usePush)
                as->Asm("pha");
            as->ClearTerm();
            if (node->m_expr->isPureVariable() && node->m_expr->isWord(as)) {
                ErrorHandler::e.Error("You cannot use integers variables as array indices!", node->m_op.m_lineNumber);
            }
            node->m_expr->Accept(this);
            //                node->m_expr->Build(as);
            as->Term();
            if (node->getArrayType(as)==TokenType::INTEGER)
                as->Asm("asl");
            as->Asm("ta" + secondReg);
            if (usePush)
                as->Asm("pla");
            as->Asm("sta " +pa + getValue(node)+pb+","+ secondReg);
            if (node->getArrayType(as)==TokenType::INTEGER) {
                if (pa=="") {
                    as->Asm(tya);
                    as->Asm("sta " + getValue(node)+"+1,"+ secondReg);
                    return;
                }
                as->Asm("in"+secondReg);

                as->Asm(tya);

                as->Asm("sta " +pa + getValue(node)+pb+","+ secondReg);
            }
        }
        return;
    }
    else {
        // Not array
        if (as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber)->getTokenType() == TokenType::BYTE) {

            as->Asm("sta " + getValue(node));
            return;
        }
        else
        if (as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber)->getTokenType() == TokenType::ADDRESS) {

            as->Asm("sta " + getValue(node));
            return;
        }
        else
        if (as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber)->getTokenType() == TokenType::INTEGER) {
            as->Asm("sta " + getValue(node));
            as->Asm("sty " + getValue(node) + "+1");
            return;
        }
        else {
            ErrorHandler::e.Error("Unable to assign variable : "+getValue(node)+ " of type "+node->getTypeText(as),node->m_op.m_lineNumber);
        }

    }




}


void ASTDispatcher6502::AssignString(QSharedPointer<NodeAssign> node, bool isPointer) {

    QSharedPointer<NodeString> right = qSharedPointerDynamicCast<NodeString>(node->m_right);
    QSharedPointer<NodeVar> left = qSharedPointerDynamicCast<NodeVar>(node->m_left);
//    QString lbl = as->NewLabel("stringassign");
    QString str = as->NewLabel("stringassignstr");
    QString lblCpy=as->NewLabel("stringassigncpy");

//    as->Asm("jmp " + lbl);
    QString strAssign = str + "\t.dc \"" + right->m_op.m_value + "\",0";
    as->m_tempVars<<strAssign;
    //as->Label(str + "\t.dc \"" + right->m_op.m_value + "\",0");
  //  as->Label(lbl);

//    qDebug() << "IS POINTER " << isPointer;
    if (isPointer) {
  //      qDebug() << "HERE";
        as->Asm("lda #<"+str);
        as->Asm("sta "+getValue(left));
        as->Asm("lda #>"+str);
        as->Asm("sta "+getValue(left)+"+1");
    }
    else {

        as->Asm("ldx #0");
        as->Label(lblCpy);
        as->Asm("lda " + str+",x");
        as->Asm("sta "+getValue(left) +",x");
        as->Asm("inx");
        as->Asm("cmp #0 ;keep");  // ask post optimiser to not remove this
        as->Asm("bne " + lblCpy);
    }
  //  as->PopLabel("stringassign");
    as->PopLabel("stringassignstr");
    as->PopLabel("stringassigncpy");

}

void ASTDispatcher6502::AssignPointer(QSharedPointer<NodeAssign> node) {
    QSharedPointer<NodeVar> bVar = qSharedPointerDynamicCast<NodeVar>(node->m_right);
//    QSharedPointer<NodeNumber> bNum = dynamic_cast<QSharedPointer<NodeNumber>>(node->m_right);
    QSharedPointer<NodeVar> aVar = qSharedPointerDynamicCast<NodeVar>(node->m_left);

    node->VerifyReferences(as);

//    if (!node->m_right->isAddress())
  //      ErrorHandler::e.Error("Must be address", node->m_op.m_lineNumber);
    if (IsSimpleIncDec(aVar,  node))
        return;

/*

    if (bVar==nullptr && !node->m_right->isPureNumeric()) {

        //ErrorHandler::e.Error("Error assigning pointer: right-hand must be variable or number", node->m_op.m_lineNumber);
//        if (!node->m_right->isAddress())
  //          ErrorHandler::e.Error("Error assigning pointer: right-hand must be variable or number", node->m_op.m_lineNumber);

        node->m_right->forceWord();
        node->m_right->setForceType(TokenType::INTEGER);
        as->Term();
        node->m_right->Accept(this);
        as->Term();
     //  as->Comment(";end");
        as->Asm("sta " + getValue(aVar));
        as->Asm("sty "+ getValue(aVar)+"+1");
        return;
    }
*/



//    if (bVar!=nullptr) {

/*        if (bVar->getType(as)!=TokenType::POINTER) {
//            if (bVar->m_op.m_type==TokenType::ADDRESS) {
                as->Asm("lda #<" + getValue(bVar));
                as->Asm("ldx #>" + getValue(bVar));
                as->Asm("sta " + getValue(aVar));
                as->Asm("stx "+ getValue(aVar)+"+1");
                return;
  //          }
        }
        else
        {
            as->Asm("lda " + getValue(bVar));
            as->Asm("ldx " + getValue(bVar) + "+1");
            as->Asm("sta " + getValue(aVar));
            as->Asm("stx "+ getValue(aVar)+"+1");
            return;
        }*/

/*    if (node->m_right->isPureVariable()) {
        if (!node->m_right->isPointer(as))
        if (!node->m_right->isReference())
            if (!node->m_right->isArrayIndex())
                ErrorHandler::e.Error("Unknown usage of data or array. <font color=\"orange\">Did you mean to reference it? (#"+node->m_right->getValue(as)+")</font>",node->m_op.m_lineNumber);
    }
*/
    // Make sure that everything is a reference
    if (node->m_right->isPure()) {
//        as->Comment("EPIC FAIL " + QString::number(qSharedPointerDynamicCast<NodeNumber>(node->m_right)!=nullptr));
        as->Asm("lda " + getValue8bit(node->m_right,false));
        as->Asm("ldx " + getValue8bit(node->m_right,true));
        as->Asm("sta " + getValue(aVar));
        as->Asm("stx "+ getValue(aVar)+"+1");
        //if (!node->m_right->isWord(as))
        //    ErrorHandler::e.Warning("Assigning an 8-bit value to pointer. Is this intentional?", node->m_op.m_lineNumber);

        return;
    }
    // Generic expression

  /*  if (node->m_right->isPureNumeric()) {
        as->Asm("lda #" + QString::number(((int)node->m_right->numValue()) & 255));
        as->Asm("ldx #" + QString::number(((int)node->m_right->numValue()>>8) & 255) );
        as->Asm("sta " + getValue(aVar));
        as->Asm("stx "+ getValue(aVar)+"+1");
        return;

    }*/
    node->m_right->forceWord();
    node->m_right->setForceType(TokenType::INTEGER);
//    qDebug() << "ARGH HERE";
    as->Term();
    node->m_right->Accept(this);
    as->Term();
 //  as->Comment(";end");
    as->Asm("sta " + getValue(aVar));
//    if (node->m_right->isWord(as))
        as->Asm("sty "+ getValue(aVar)+"+1");
  //  else
    //    ErrorHandler::e.Warning("Assigning an 8-bit value to pointer. Is this intentional?", node->m_op.m_lineNumber);

    return;
     ErrorHandler::e.Error("Right-hand side must be constant or address", node->m_op.m_lineNumber);

}

bool ASTDispatcher6502::isSimpleAeqAOpB(QSharedPointer<NodeVar> var, QSharedPointer<NodeAssign> node) {
    QSharedPointer<NodeBinOP> rterm = qSharedPointerDynamicCast<NodeBinOP>(node->m_right);
    if (rterm==nullptr)
        return false;

    // right first is var
    QSharedPointer<NodeVar> rvar = qSharedPointerDynamicCast<NodeVar>(rterm->m_left);
    QSharedPointer<NodeVar> bvar = qSharedPointerDynamicCast<NodeVar>(rterm->m_left);

    if (bvar == nullptr)
        return false;


    QSharedPointer<NodeVar> rrvar = qSharedPointerDynamicCast<NodeVar>(rterm->m_right);
    if (!rterm->m_right->isPureNumeric())
        return false;
  //  int num = rterm->m_right->getValueAsInt(as);

//    QSharedPointer<NodeNumber> rrnum = dynamic_cast<QSharedPointer<NodeNumber>>(rterm->m_right);


    if (rrvar==nullptr && rvar==nullptr)
        return false;

//    qDebug() << "HERE1";

    if (!(rterm->m_op.m_type==TokenType::PLUS || rterm->m_op.m_type==TokenType::MINUS))
        return false;
  //  qDebug() << "HERE2";


    if (var->isWord(as))
        return false;



//    return false;

    as->Comment("Optimizer: a = a +/- b");
    LoadVariable(bvar);
    as->BinOP(rterm->m_op.m_type);
    rterm->m_right->Accept(this);
    as->Term();
    StoreVariable(var);
    return true;
}

bool ASTDispatcher6502::isSimpleAeqAOpB16Bit(QSharedPointer<NodeVar> var, QSharedPointer<NodeAssign> node)
{
    QSharedPointer<NodeBinOP> rterm = qSharedPointerDynamicCast<NodeBinOP>(node->m_right);
    if (rterm==nullptr)
        return false;

    // right first is var
     QSharedPointer<NodeVar> rvar = qSharedPointerDynamicCast<NodeVar>(rterm->m_right);

     QSharedPointer<NodeVar> bvar = qSharedPointerDynamicCast<NodeVar>(rterm->m_left);
     if (bvar==nullptr)
         return false;

     if (bvar->isArrayIndex())
         return false;

     if (var->isArrayIndex())
         return false;

//    QSharedPointer<NodeVar> rrvar = dynamic_cast<QSharedPointer<NodeVar>>(rterm->m_right);
//    QSharedPointer<NodeNumber> rrnum = dynamic_cast<QSharedPointer<NodeNumber>>(rterm->m_right);

//    qDebug() << "IsPure "<<rterm->m_right->isPure();

    bool variable = rvar!=nullptr;

    if (!variable)
        if (!(rterm->m_right->isPure()))
        return false;



    if (!(rterm->m_op.m_type==TokenType::PLUS || rterm->m_op.m_type==TokenType::MINUS))
        return false;
//    qDebug()<< rterm->m_right->getValue(as);
/*    qDebug() << "isSimpleAeqAOpB16Bit" << var->isWord(as) << rterm->m_right->is8bitValue(as) << TokenType::getType(node->m_forceType) ;

    qDebug() << "Is pure " <<rterm->m_right->isPure() << " is variable " << variable;
    qDebug() << "Is word " <<var->isWord(as) << " is 8bit " << rterm->m_right->is8bitValue(as);
    qDebug() << "type " << TokenType::getType(rterm->m_right->getType(as));
*/



/*    if (var->isWord(as) &&  rterm->m_right->is8bitValue(as) && !(node->m_forceType==TokenType::INTEGER)) {
  //      qDebug() << "WHOO";
        //          qDebug() << "Cont";

        //       qDebug() << "ASTDispatcher6502::isSimpleAeqAOpB16Bit HERE";
        QString lbl = as->NewLabel("WordAdd");
        as->Comment("WORD optimization: a=a+b");
        //var->Accept(this);
        as->Asm("lda " + getValue(var));
        as->Term();
        //        as->Asm("clc");
        as->BinOP(rterm->m_op.m_type);

        rterm->m_right->Accept(this);
        as->Term();
        if (rterm->m_op.m_type==TokenType::PLUS) {
            as->Asm("bcc "+lbl);
            as->Asm("inc " + getValue(var) + "+1");
        }
        else {
            as->Asm("bcs "+lbl);
            as->Asm("dec " + getValue(var) + "+1");
        }

        as->Label(lbl);
        as->Asm("sta " + getValue(var) + "+0");
        //        as->Asm("sty " + getValue(var) +"+1");

        as->PopLabel("WordAdd");
        return true;

    }*/
    if (var->isWord(as) && rterm->m_right->isPure()) {
        QString lbl = as->NewLabel("WordAdd");
        if (bvar->getValue(as)!=var->getValue(as))
//        qDebug() << "OPTIMIZATION " <<bvar->getValue(as) << var->getValue(as);
//
        as->Comment("INTEGER optimization: a=b+c ");
        QString Blo = getValue8bit(bvar,false);
        QString Bhi = getValue8bit(bvar,true);
/*        if (var->isPointer(as)) {
            as->Comment("Resulting var is POINTER: Assuming : a=address+c ");
            if (bvar->isPureVariable() && !bvar->isPointer(as)) {
                Blo ="#<"+getValue(bvar);
                Bhi="#>"+getValue(bvar);
            }
        }*/
        //var->Accept(this);
        as->Asm("lda " +Blo);
        as->Term();
        //        as->Asm("clc");
        as->BinOP(rterm->m_op.m_type);

        as->Term(rterm->m_right->getValue8bit(as,false),true);

        //rterm->m_right->Accept(this);
        as->Term();
/*        if (rterm->m_op.m_type==TokenType::PLUS) {
            as->Asm("bcc "+lbl);
            as->Asm("inc " + getValue(var) + "+1");
        }
        else {
            as->Asm("bcs "+lbl);
            as->Asm("dec " + getValue(var) + "+1");
        }
*/
//        as->Label(lbl);
        as->Asm("sta " + getValue(var) + "+0");
        // Optimization
//        qDebug() << rterm->m_right->getValue8bit(as,true);
        if ((rterm->m_right->getValue8bit(as,true)=="#$00" || rterm->m_right->getValue8bit(as,true)=="#0") && bvar->getValue(as) == var->getValue(as)) {
            as->Comment("Optimization : A := A op 8 bit - var and bvar are the same - perform inc");
            if (rterm->m_op.m_type==TokenType::PLUS) {
                as->Asm("bcc "+lbl);
                as->Asm("inc " + getValue(var) + "+1");
            }
            else {
                as->Asm("bcs "+lbl);
                as->Asm("dec " + getValue(var) + "+1");
            }
            as->Label(lbl);

        }
        else {

            as->Asm("lda "+Bhi);
            as->ClearTerm();
            as->BinOP(rterm->m_op.m_type,false);
            as->Term(rterm->m_right->getValue8bit(as,true),true);
            as->Asm("sta " + getValue(var) + "+1");
        }
        //        as->Asm("sty " + getValue(var) +"+1");

        as->PopLabel("WordAdd");
        return true;

    }


    return false;
}




bool ASTDispatcher6502::IsSimpleIncDec(QSharedPointer<NodeVar> var, QSharedPointer<NodeAssign> node) {
    // Right is binop
//    return false;
    QSharedPointer<NodeBinOP> rterm = qSharedPointerDynamicCast<NodeBinOP>(node->m_right);
    if (rterm==nullptr)
        return false;

    // right first is var

    QSharedPointer<NodeVar> rvar = qSharedPointerDynamicCast<NodeVar>(rterm->m_left);
    if (rvar==nullptr)
        return false;

//    qDebug() << "IsSimpleIncDec test";

    // DANGEROUS

/*    if (getValue(rvar)!=getValue(var)) {
  //      qDebug() << "NOT SAME " <<rvar->getValue(as) << var->getValue(as);
        return false;
    }
*/


//    QSharedPointer<NodeNumber> num = dynamic_cast<QSharedPointer<NodeNumber>>(rterm->m_right);
    bool isPureNumber = rterm->m_right->isPureNumeric();

    int num = -1;

    if (isPureNumber)
        num = rterm->m_right->getValueAsInt(as);


//    qDebug() << "HERE;" << num;

    if (!var->isWord(as)) {
        if ((num!=1)) {
            return isSimpleAeqAOpB(var, node);
        }
    }
    else {
//        as->Asm("IS simple 16 bit");
        return isSimpleAeqAOpB16Bit(var, node);
    }

//    if (var->isWord(as))
  //      return false;

    if (!isPureNumber)
        return false;

    if (num!=1)
        return false;

    if (getValue(rvar)!=getValue(var)) {
  //      qDebug() << "NOT SAME " <<rvar->getValue(as) << var->getValue(as);
        return false;
    }


    // OK: it is i:=i+1;
    QString operand ="";
    if (rterm->m_op.m_type==TokenType::PLUS)
        operand="inc ";
    if (rterm->m_op.m_type==TokenType::MINUS)
        operand="dec ";
    if (operand=="")
        return false; // other operand



    if (var->m_expr==nullptr && rvar->m_expr==nullptr) {
        as->Asm(operand +getValue(var));
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
        if (var->isPointer(as))
            return false;

        if (var->m_expr->isPureNumeric()) {
            as->ClearTerm();

            as->Comment("Optimize operand array with const index " + operand);
            as->Asm(operand + getValue(var)+" + "+var->m_expr->getValue(as));
            return true;

        }

        if (LoadXYVarOrNum(var, var->m_expr,true)) {

            as->Comment("Optimize byte array " + operand);
            as->Asm(operand + getValue(var)+",x");
            return true;
        }

        return false;
    }


    return false;

}



/*
 *
 *
 * NodeAssign
 *
 *
*/

void ASTDispatcher6502::dispatch(QSharedPointer<NodeAssign> node)
{
    node->DispatchConstructor(as,this);

//    as->PushCounter();

    AssignVariable(node);

  //  as->PopCounter(node->m_op.m_lineNumber);

}

void ASTDispatcher6502::dispatch(QSharedPointer<NodeCase> node)
{
    node->DispatchConstructor(as,this);
  //  as->PushCounter();
    bool hasElse = node->m_elseBlock!=nullptr;
    QString labelEnd = as->NewLabel("caseend");
    for (int i=0;i<node->m_conditionals.count();i++) {
        QString labelNext = as->NewLabel("casenext");
        as->PopLabel("casenext");

        if (node->m_conditionals[i]->isPure()) {
            node->m_variable->Accept(this);
            as->Term();
            as->Asm("cmp " + getValue(node->m_conditionals[i]) +" ;keep");

        }
        else { // General term
            node->m_conditionals[i]->Accept(this);
            as->Term();
            QString compare = as->StoreInTempVar("temp1");
            node->m_variable->Accept(this);
            as->Term();
            as->Asm("cmp " + compare +" ;keep");
        }
        as->Asm("bne "+labelNext);

        node->m_statements[i]->Accept(this); // RUN the current block

        as->Asm("jmp "+labelEnd);


        as->Label(labelNext);
    }
    if (hasElse)
        node->m_elseBlock->Accept(this);

    as->Label(labelEnd);

    as->PopLabel("caseend");

    //    as->PopCounter(node->m_op.m_lineNumber);
}

void ASTDispatcher6502::dispatch(QSharedPointer<NodeRepeatUntil> node)
{
    node->DispatchConstructor(as,this);
    QString lbl = as->NewLabel("repeatUntil");
    QString lblDone = as->NewLabel("repeatUntil");
    as->Label(lbl);
    node->m_block->Accept(this);

    bool isSimplified = false;
    bool isOKBranchSize = true;

    if (node->verifyBlockBranchSize(as, node->m_block,nullptr,this)) {
        isSimplified = !node->m_clause->cannotBeSimplified(as);
    }
    else isOKBranchSize = false;

    // Then, check m_forcepage
    if (node->m_forcePage==1) // force OFFPAGE
        isSimplified = false;

    if (node->m_forcePage==2) {
        if (!node->m_clause->cannotBeSimplified(as)) // force ONPAGE
            isSimplified = true;
        else ErrorHandler::e.Error("keyword onpage can only be used with 1 compare clause (no and, or etc)", node->m_op.m_lineNumber);
    }
    if (!isSimplified) {
//        node->m_binaryClause->Build(as);
        if (isOKBranchSize && IsSimpleAndOr(qSharedPointerDynamicCast<NodeBinaryClause>(node->m_clause), lblDone,lbl)){
        }
        else {

            node->m_clause->Accept(this);

            as->Asm("cmp #1");
            as->Asm("beq " + lblDone);
            // Do we have an else block?
            as->Asm("jmp " + lbl);
        }
    }
    else {
        // Simplified version <80 instructions & just one clause
        BuildSimple(node->m_clause,  lblDone,lbl, node->m_forcePage==1);
    }
    // Start main block

    as->Label(lblDone);


}

void ASTDispatcher6502::dispatch(QSharedPointer<NodeComment> node)
{
    node->DispatchConstructor(as,this);

    if (node->m_comment!="") {
//        as->Comment("A COMMENT");

        as->Comment(node->m_comment.replace("\n","\n; "));
    }

}

bool ASTDispatcher6502::Evaluate16bitExpr(QSharedPointer<Node> node, QString &lo, QString &hi)
{
    if (node->isPure()) {
        lo = getValue8bit(node,false);
        hi = getValue8bit(node,true);
        return true;
    }
    node->Accept(this);
    QString lbl = as->StoreInTempVar("rightvarInteger", "word");
    lo = lbl;
    hi = lbl+"+1";
    return false;
}





void ASTDispatcher6502::AssignVariable(QSharedPointer<NodeAssign> node) {


    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    //        qDebug() << "AssignVariable: " <<getValue(v) << " : " << TokenType::getType( v->getType(as));

    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(node->m_left);

    if (v==nullptr && num == nullptr)
        ErrorHandler::e.Error("Left value not variable or memory address! ", node->m_op.m_lineNumber);
    if (num!=nullptr && num->getType(as)!=TokenType::ADDRESS)
        ErrorHandler::e.Error("Left value must be either variable or memory address, not a constant.", node->m_op.m_lineNumber);

//    qDebug() << TokenType::getType(num->getType(as));


    if (num!=nullptr) {
        as->Comment("Assigning memory location");
        v = QSharedPointer<NodeVar>(new NodeVar(num->m_op)); // Create a variable copy
        v->value = num->HexValue();
//        Node::s_uniqueSymbols[v] = v; // Mark for deletion

        //return num->HexValue();
    }


    QString vname = getValue(v);
//    as->Comm nt("IS REGISTER : "+Util::numToHex(v->m_isRegister) + " "+vname);
    if (v->m_isRegister) {
        vname = vname.toLower();
        //if (vname=="_a" || vname=="_x" || vname=="_y")
        //{
        QString reg = QString(vname[1]);
        as->Comment("Assigning register : " + vname);
        if (vname.count()==2) {
            if (reg=="x" || reg=="y") {
                if (!node->m_right->isPure())
                    ErrorHandler::e.Error("Setting _X and _Y register values must be pure number or variable.", node->m_op.m_lineNumber);

                QString cmd = "ld"+QString(reg) + " "+node->m_right->getValue(as);
                as->Asm(cmd);
                return;
            }
            node->m_right->Accept(this);
            as->Term();

            return;
        }
        if (vname.count()==3) {
            if (!node->m_right->isPure())
                ErrorHandler::e.Error("Setting _AX and _AX, and _XY register values must be pure number or variable.", node->m_op.m_lineNumber);


            QString cmdA = "ld"+QString(vname[1]) + " "+node->m_right->getValue8bit(as,false);
            QString cmdB = "ld"+QString(vname[2]) + " "+node->m_right->getValue8bit(as,true);
            as->Asm(cmdA);
            as->Asm(cmdB);
            return;
        }
        return;
        //}
    }

    if (node->m_right->m_isRegister) {
        QString reg = node->m_right->getValue(as).toLower();
        as->Comment("Saving register "+ reg);
        //if (vname=="_a" || vname=="_x" || vname=="_y")
        //{
            if (!node->m_right->isPure())
                ErrorHandler::e.Error("Using _A, _X and _Y register values must be pure.", node->m_op.m_lineNumber);

            QString cmd = "st"+QString(reg[1]) + " "+vname;
            as->Asm(cmd);

            return;
        //}
    }

    as->Comment("Assigning single variable : " + getValue(v));
    as->ClearTerm();
//    as->Comment("Is word : " + QString::number(v->isWord(as)));
    QSharedPointer<Symbol> s = as->m_symTab->Lookup(getValue(v), node->m_op.m_lineNumber, v->isAddress());

    // Trying to assign a PURE record
    if (v->isRecord(as) && !v->isRecordData(as)) {
        if (!node->m_right->isRecord(as))
            ErrorHandler::e.Error("Right-hand side of assignment must also be record of type '"+v->getTypeText(as)+"'", v->m_op.m_lineNumber);
        if (v->getTypeText(as)!=node->m_right->getTypeText(as))
            ErrorHandler::e.Error("Right-hand side of assignment must also be of type '"+v->getTypeText(as)+"'", v->m_op.m_lineNumber);
        if (node->m_right->isRecordData(as))
            ErrorHandler::e.Error("Right-hand side of assignment must also be of type '"+v->getTypeText(as)+"'", v->m_op.m_lineNumber);

        // Copy record:
        HandleNodeAssignCopyRecord(node);

        return;
    }

    // POINTER = RECORD errors
    if (node->m_left->getType(as)==TokenType::POINTER && node->m_right->isRecord(as)) {
//        if (!node->m_left->isArrayIndex())
  //          ErrorHandler::e.Error("Cannot assign a pointer to a record.", node->m_op.m_lineNumber);
        if (!node->m_right->isRecordData(as))
            ErrorHandler::e.Error("Cannot assign a pointer data to a record.", node->m_op.m_lineNumber);
 //       if (node->)
    }
    // Variable = POINTER
    if (node->m_right->isRecord(as) && (!node->m_right->isRecordData(as))) {
        ErrorHandler::e.Error("Cannot assign a record of type '"+node->m_right->getTypeText(as)+"' to a single variable. ",node->m_op.m_lineNumber);
    }


    if (node->m_left->getType(as)==TokenType::POINTER && v->m_expr==nullptr) {

        if (qSharedPointerDynamicCast<NodeString>(node->m_right)) {
            AssignString(node,node->m_left->isPointer(as));
            return;

        }

        AssignPointer(node);
        return;
    }

    if (qSharedPointerDynamicCast<NodeString>(node->m_right) && v->m_expr==nullptr)
    {
        AssignString(node,node->m_left->isPointer(as));
        return;
    }
    if (node->m_right==nullptr)
        ErrorHandler::e.Error("Node assign: right hand must be expression", node->m_op.m_lineNumber);

//    qDebug() << "HER" << v->getValue(as) << v->getTypeText(as);
    if (v->getTypeText(as).toLower()=="array" && v->m_expr==nullptr) {
//        ErrorHandler::e.Error("Cannot assign an address to an array. Did you forget to use an [] index?", node->m_op.m_lineNumber);

    }

//    if (node->m_left->isWord(as))

    if (node->m_left->isWord(as)) {
        as->Asm("ldy #0");    // AH:20190722: Does not appear to serve a purpose - takes up space in prg. Breaks TRSE scroll in 4K C64 demo if take this out
        node->m_right->setForceType(TokenType::INTEGER);
    }
    // For constant i:=i+1;
    if (IsSimpleIncDec(v,  node))
        return;

    if (IsSimpleAssignPointerExpression(v,node))
        return;

//    if (!v->isWord(as) && node->m_right->isPure() && v->m_expr!=nullptr && (!(v->isPointer(as) && v->getArrayType(as)==TokenType::INTEGER))) {
    if (!v->isWord(as) && node->m_right->isPure() && v->m_expr!=nullptr) {
        StoreVariableSimplified(v, node->m_right);
        return;
    }


    node->m_right->Accept(this);
    as->Term();
    as->Comment("Calling storevariable");
    StoreVariable(v);

    return;
}

void ASTDispatcher6502::OptimizeBinaryClause(QSharedPointer<Node> node, Assembler* as)
{
    if (node->m_left->isWord(as)) // no word optimizations.. yet
        return;

//    return;
    if (node->m_right->isPureNumeric()) {
        // is numeric
        //   a >= N+1  is better than a > N
        int val = node->m_right->getValueAsInt(as);
        if (node->m_op.m_type == TokenType::GREATER && val!=255) {
            node->m_op.m_type = TokenType::GREATEREQUAL;
            Token t = node->m_right->m_op;
            t.m_intVal = val+1;
            // Replace with N+1
            node->m_right = QSharedPointer<NodeNumber>(new NodeNumber(t,t.m_intVal));
            as->Comment("Optimization: replacing a > N with a >= N+1");
            return;
       }
        if (node->m_op.m_type == TokenType::LESSEQUAL && val!=0 && val!=255) {
            as->Comment("Optimization: replacing a <= N with a <= N-1");
            node->m_op.m_type = TokenType::LESS;
            Token t = node->m_right->m_op;
            t.m_intVal = val+1;
            // Replace with N+1
            node->m_right = QSharedPointer<NodeNumber>(new NodeNumber(t,t.m_intVal));
            return;
       }

    }
}

void ASTDispatcher6502::dispatch(QSharedPointer<NodeUnaryOp> node)
{
    node->DispatchConstructor(as,this);
    AbstractASTDispatcher::dispatch(node);
//    as->Comment("Unary op beware!");
    if (node->m_right->isPureNumeric())
        return;
    node->m_right->Accept(this);
    as->Term();
    if (node->m_op.m_type==TokenType::MINUS) {
        if (node->m_right->isWord(as)) {

            ErrorHandler::e.Error("Unary operator (-) for integer not implemented yet. Please bug the developer!",node->m_op.m_lineNumber);
            /*Token t = node->m_op;
            QSharedPointer<NodeBinOP> bop = new QSharedPointer<NodeBinOP>(new NodeBinOP())
            */
        }
        as->Comment("Unary operator: Negate 8-bit number");
        as->Asm("eor #$FF");
        as->Asm("clc");
        as->Asm("adc #1");
    }


}


void ASTDispatcher6502::CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive)
{
    /*    if (!isOffPage)
        SmallLoop(node,qSharedPointerDynamicCast<NodeVar>(nVar->m_left), inclusive);
    else
        LargeLoop(node,qSharedPointerDynamicCast<NodeVar>(nVar->m_left), inclusive);

*/

    if (!isOffPage) {
        QString loopDone = as->NewLabel("loopdone");
        as->Comment("Compare is onpage");

        IncreaseCounter(step,qSharedPointerDynamicCast<NodeVar>(nodeA->m_left));
        Compare(nodeA, nodeB, step, false, loopDone, lblJump, isInclusive);
        as->PopLabel("loopdone");
        as->Label(loopDone+": ;keep");
        return;
    }


    QString loopDone = as->NewLabel("loopdone");
    QString loopNotDone = as->NewLabel("loopnotdone");

    IncreaseCounter(step,qSharedPointerDynamicCast<NodeVar>(nodeA->m_left));
    Compare(nodeA, nodeB, step, true, loopDone, loopNotDone, isInclusive);

    as->Label(loopNotDone);
    as->Asm("jmp " + lblJump);

    as->Label(loopDone);
    //  Compare(node, var, false, loopDone, nullptr, inclusive);

    as->PopLabel("loopdone");
    as->PopLabel("loopnotdone");



}

void ASTDispatcher6502::StoreVariableSimplified(QSharedPointer<NodeVar> node, QSharedPointer<Node> expr)
{
    //QSharedPointer<NodeNumber> num = dynamic_cast<QSharedPointer<NodeNumber>>(node->m_expr);

    if(node->getType(as)!=TokenType::POINTER)
    if (node->m_expr->isPureNumeric()) {
        int pos = node->m_expr->getValueAsInt(as);
        if (node->getArrayType(as)==TokenType::INTEGER)
            pos*=2;

        as->Comment("Store Variable is pure numeric and not pointer - store directly!");
        as->ClearTerm();
        expr->Accept(this);
        as->Term();
        as->Asm("sta " + getValue(node)+" + " +Util::numToHex(pos));
        return;
    }

    QString secondReg="x";
    QString pa = "";
    QString pb= "";
    if (node->getType(as)==TokenType::POINTER) {
        secondReg="y";
        pa="(";
        pb=")";
    }
    as->Comment("Store Variable simplified optimization : right-hand term is pure");
    as->ClearTerm();
    node->m_expr->Accept(this);
    //                node->m_expr->Build(as);
    as->Term();
    if (node->getArrayType(as)==TokenType::INTEGER)
        as->Asm("asl");


    if (node->getType(as)==TokenType::POINTER && node->getArrayType(as)==TokenType::INTEGER) {
        // Integer array. Special case.
        as->Comment("Store Variable simplified optimization : right-hand term is pure, integer pointer assignment");
        as->Asm("tay");
        as->Asm("ldx "+getValue8bit(expr,true));
        as->Asm("lda "+getValue8bit(expr,false));
        as->Asm("sta ("+getValue(node)+"),y");
        as->Asm("iny");
        as->Asm("txa");
        as->Asm("sta ("+getValue(node)+"),y");
        return;

    }
    as->Asm("ta" + secondReg);
    as->ClearTerm();
    expr->Accept(this);
    as->Term();
    as->Asm("sta " +pa + getValue(node)+pb+","+ secondReg);

}
