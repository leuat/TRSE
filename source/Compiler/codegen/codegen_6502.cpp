
#include "codegen_6502.h"

CodeGen6502::CodeGen6502()
{

}

/*
 *
 *  NODE BINOP dispatcher
 *
 *  */



void CodeGen6502::EightBitDiv(QSharedPointer<Node> node) {

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
    as->Asm(getCallSubroutine()+" div8x8_procedure");

}

void CodeGen6502::EightBitMul(QSharedPointer<Node> node) {

    QString mulVar = as->NewLabel("mulRightVar");
    QString mulVarJmp = as->NewLabel("mulRightVarJmp");

    as->Comment("8 bit mul");
    as->ClearTerm();
    node->m_left->Accept(this);
    as->Term(); // lda z
    as->Asm("pha");
    m_stackShift+=1;
    as->Comment("Load right hand side");
    node->m_right->Accept(this);
    as->Asm("tax");
    as->Asm("pla");
    m_stackShift-=1;

    as->Term();

    as->Asm(getCallSubroutine()+" multiply_eightbit");
    as->Asm("txa"); // result in a
    as->Asm("ldy #0 ; ::EightbitMul");

}

void CodeGen6502::HandleGenericBinop16bit(QSharedPointer<Node> node) {


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
    Disable16bit();
    as->Asm("tya");

    //    as->BinOP(m_op.m_type);
    //    if (node->m_op.m_type==TokenType::PLUS)
    //      as->Asm("clc");
    as->BinOP(node->m_op.m_type,false);

    as->Term(lbl+"+1",true);
    //    as->Asm("lda #0");

    as->Asm("tay");
    as->Asm("lda "+lbl);

    Enable16bit();

    as->PopLabel("wordAdd");

    //as->PopLabel("rightvarInteger");
    as->PopLabel("jmprightvarInteger");
    as->PopTempVar();
}
void CodeGen6502::HandleVarBinopB16bit(QSharedPointer<Node> node) {
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
        if (node->m_right->getValue8bit(as,true)=="#0" ) {
            as->Comment("RHS is byte, optimization");
            QString lbl = as->NewLabel("skip");
            if (node->m_op.m_type==TokenType::PLUS) {
                as->Asm("bcc "+lbl);
                as->Asm("iny");
            }
            if (node->m_op.m_type==TokenType::MINUS) {
                as->Asm("bcs "+lbl);
                as->Asm("dey");
            }
            //            if (node->m_op.m_type==TokenType::MINUS)
            //              as->Asm("dey ; or dey?");
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
    if (!v->hasArrayIndex()) {
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

bool CodeGen6502::HandleSingleAddSub(QSharedPointer<Node> node) {

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

void CodeGen6502::HandleMulDiv(QSharedPointer<Node> node) {

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
        Disable16bit();

        if (node->isWord(as))
            Mul16x8(node);
        else
            EightBitMul(node);
        Enable16bit();
        return;
    }
    if (node->m_op.m_type==TokenType::DIV) {
        Disable16bit();

        if (node->isWord(as))
            Div16x8(node);
        //                ErrorHandler::e.Error("16 bit div not implemented",m_op.m_lineNumber);
        else
            RightIsPureNumericMulDiv8bit(node);
        Enable16bit();
        return;
    }
    ErrorHandler::e.Error("Binary operation / not implemented for this type yet...");
}

void CodeGen6502::RightIsPureNumericMulDiv16bit(QSharedPointer<Node> node) {
    int val = node->m_right->getValueAsInt(as);

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

void CodeGen6502::HandleShiftLeftRight(QSharedPointer<NodeBinOP>node)
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
    QString lbl = as->NewLabel("lblShift");
    QString lblCancel = as->NewLabel("lblShiftDone");

    node->m_right->Accept(this);
    as->Term();
    as->Asm("tax");
    node->m_left->Accept(this);
    as->Term();
    as->Asm("cpx #0");
    as->Asm("beq "+lblCancel);
    as->Term();
    as->Label(lbl);
    as->Asm(cmd);
    as->Asm("dex");
    as->Asm("cpx #0");
    as->Asm("bne "+lbl);
    as->Label(lblCancel);

    as->PopLabel("lblShift");
    as->PopLabel("lblShiftDone");
}

void CodeGen6502::HandleShiftLeftRightInteger(QSharedPointer<NodeBinOP>node, bool isSimpleAeqAopB)
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
        QString lblCancel = as->NewLabel("lblShiftCancel");
        as->Asm("tax");
        as->Asm("cpx #0");
        as->Asm("beq "+lblCancel);
        QString lbl = as->NewLabel("lblShift");
        as->Label(lbl);
        as->Asm(command);
        as->Asm("dex");
        as->Asm("cpx #0");
        as->Asm("bne "+lbl);
        as->Label(lblCancel);

        as->PopLabel("lblShift");
        as->PopLabel("lblShiftCancel");
    }
    if (!isSimpleAeqAopB) {
        as->Asm("lda "+varName);
        as->Asm("ldy "+varName +"+1");
        as->PopTempVar();
    }
}

void CodeGen6502::Mul16x8(QSharedPointer<Node> node) {
    as->Comment("Mul 16x8 setup");
    as->Asm("");
    if (!node->m_left->isWord(as) && node->m_right->isWord(as)) {
        node->SwapNodes();
    }
    //    Disable16bit();
    if (node->m_left->isWord(as)) {

        LoadVariable(node->m_left);
        //        if (!node->m_left->isWord(as))
        as->Term();
        //        if (!node->m_left->isWord(as))
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
    as->Asm(getCallSubroutine()+" mul16x8_procedure");
    //  Enable16bit();

}

void CodeGen6502::Div16x8(QSharedPointer<Node> node) {
    as->Comment("16x8 div");
    Disable16bit();
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
    as->Asm(getCallSubroutine()+" divide16x8");
    as->Asm("lda initdiv16x8_dividend");
    as->Asm("ldy initdiv16x8_dividend+1");

    Enable16bit();
}




void CodeGen6502::HandleRestBinOp(QSharedPointer<Node> node) {
    bool isWord16 = false;
    QString varName="";

    if ( qSharedPointerDynamicCast<NodeVar>(node->m_left) != nullptr) {
        QSharedPointer<NodeVar> v= qSharedPointerDynamicCast<NodeVar>(node->m_left);
        varName = getValue(v);
        QSharedPointer<Symbol> s = as->m_symTab->Lookup(varName, node->m_op.m_lineNumber);
    }
    isWord16 = node->isWord(as);

    //    as->Comment("IS WORD: " + QString::number(isWord16));
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

void CodeGen6502::RightIsPureNumericMulDiv8bit(QSharedPointer<Node> node) {
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

void CodeGen6502::Load16bitVariable(QSharedPointer<Node> node, QString reg)
{
    as->ClearTerm();
    //    as->Comment("Load 16 bit var IS WORD "+QString::number(node->isWord(as)));


    if (node->isWord(as))
        as->Asm("ld"+reg+" "+getValue8bit(node,true));
    as->Asm("lda "+getValue8bit(node,false));
}


void CodeGen6502::dispatch(QSharedPointer<NodeBinOP>node)
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

void CodeGen6502::dispatch(QSharedPointer<NodeNumber>node)
{
    node->DispatchConstructor(as,this);

    QString val = getValue(node);
    if (node->m_forceType==TokenType::INTEGER && node->m_val<=255) {
        as->Asm("ldy #0   ; Force integer assignment, set y = 0 for values lower than 255");
    }

    //    as->Comment("Value assignment : " + Util::numToHex(node->m_val) + " "+ val + " " +QString::number(node->getValueAsInt(as)));
    if (((node->m_op.m_type==TokenType::INTEGER_CONST && node->m_val>255) || node->isReference()) && as->m_term=="") {
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






void CodeGen6502::dispatch(QSharedPointer<Node> node)
{
    node->DispatchConstructor(as,this);
    node->m_currentLineNumber = node->m_op.m_lineNumber;


    //    qDebug() << "CodeGen6502 UNKNOWN TYPE! Should never be called.";
    //    exit(1);
}






void CodeGen6502::dispatch(QSharedPointer<NodeVarType> node)
{
    node->DispatchConstructor(as,this);

}

void CodeGen6502::dispatch(QSharedPointer<NodeBinaryClause> node)
{
    node->DispatchConstructor(as,this);

}

void CodeGen6502::dispatch(QSharedPointer<NodeString> node)
{
    node->DispatchConstructor(as,this);
    //    exit(1);
    if (node->m_val.length()>=1 && node->m_val[0].length()>=1) {
        as->ClearTerm();

        //        as->Asm("lda #"+QString::number(QChar(node->m_val[0][0]).unicode()));
        CStringItem it  = ((Asm6502*)as)->m_cstr[QString(node->m_val[0][0]) ];
        //        as->Asm("lda #"+QString::number(it.m_char.unicode()->unicode()));
        as->Asm("lda #"+QString::number(it.m_screenCode));
        as->Term();
        return;
    }
    as->String(node->m_val,true);
}


void CodeGen6502::dispatch(QSharedPointer<NodeVarDecl> node)
{
    node->DispatchConstructor(as,this);


    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);

    QSharedPointer<Appendix> old = as->m_currentBlock;

    if (t->m_flags.contains("wram")) {
        as->m_currentBlock = as->m_wram;
    }

    if (t->m_flags.contains("bank")) {
        QString bnk = getBank(t);
        as->m_currentBlock = as->m_banks[bnk];
    }
    if (v->m_isGlobal) {
        as->m_currentBlock = nullptr;
        return;
    }


    //    qDebug() << "" <<as->m_currentBlock;
    AbstractCodeGen::dispatch(node);
    if (t->m_op.m_type==TokenType::INCSID || t->m_op.m_type==TokenType::INCNSF) {
        IncSid(node);
        return;
    }
    //  qDebug() << as->m_currentBlock;
    as->m_currentBlock = old;



}




void CodeGen6502::IncSid(QSharedPointer<NodeVarDecl> node) {

    // Init address or load address? hmmm

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::NES) {

        if (node->sid.m_loadAddress!=0x8000 && Syntax::s.m_currentSystem->m_programStartAddress!=0x8000) {
            QSharedPointer<Appendix> app = QSharedPointer<Appendix>(new Appendix("$8000"));
            app->Append("org $8000",1);
            app->Append("NSFfiller dc.b 0",0);
            as->m_appendix.append(app);
        }
    }
    //    qDebug() << Util::numToHex(node->sid.m_loadAddress)<<Util::numToHex(node->sid.m_initAddress)<<Util::numToHex(node->sid.m_playAddress);
    QString pos = QString::number(node->sid.m_loadAddress,16);
    QSharedPointer<Appendix> app = QSharedPointer<Appendix>(new Appendix("$"+pos));
    //    qDebug() << "INCSID dispatcher"<< pos;

    app->Append(as->GetOrg(node->sid.m_loadAddress),1);
    //        as->Appendix(getValue(v),0);
    app->Append(getIncbin()+"\t\"" + as->m_projectDir + node->sid.m_outFile + "\"",1);

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



void CodeGen6502::DeclarePointer(QSharedPointer<NodeVarDecl> node) {

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

QString CodeGen6502::getIncbin() {
    if (Syntax::s.m_currentSystem->CL65Syntax())
        return ".incbin";
    return "incbin";
}

void CodeGen6502::PrintCompare(QSharedPointer<Node> node, QString lblSuccess, QString lblFailed)
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
void CodeGen6502::BinaryClause(QSharedPointer<Node> node)
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
void CodeGen6502::BuildToCmp(QSharedPointer<Node> node)
{
    QString b="";

    QSharedPointer<NodeVar> varb = qSharedPointerDynamicCast<NodeVar>(node->m_right);
    if (varb!=nullptr && !varb->hasArrayIndex())
        b = getValue(varb);

    if (node->m_right->isPureNumeric())
        //    QSharedPointer<NodeNumber> numb = dynamic_cast<QSharedPointer<NodeNumber>>(node->m_right);
        //  if (numb!=nullptr)
        b = getValue(node->m_right);

    //    qDebug() << "WOOT " <<TokenType::getType(node->m_right->getType(as));
    if (b=="#$0") {
        as->Asm("clc");
    }

    node->m_left->Accept(this);
    as->Term();
    if (b!="") {
        if (b!="#$0") {
            as->Comment("Compare with pure num / var optimization");
            as->Asm("cmp " + b+";keep");
        }
        else {
            as->Comment("cmp #$00 ignored");
            //            as->Asm("cmp " + b+";keep");
        }
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

void CodeGen6502::Disable16bit()
{
    if (Syntax::s.m_currentSystem->isWDC65()) {
        if (block16bit==0)
            as->Asm("sep #$10        ; disable X/Y 16-bit");
        block16bit=1;
    }

}

void CodeGen6502::Enable16bit()
{
    if (Syntax::s.m_currentSystem->isWDC65()) {
        if (block16bit!=0)
            as->Asm("rep #$10        ; enable X/Y 16-bit");
        block16bit=0;
    }

}

void CodeGen6502::BuildSimple(QSharedPointer<Node> node, QString lblSuccess, QString lblFailed, bool page)
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

void CodeGen6502::BinaryClauseInteger(QSharedPointer<Node> node,QString lblSuccess, QString lblFailed, bool page)
{

    as->Comment("Binary clause INTEGER: " + node->m_op.getType());
    QString lbl2 = lblFailed;
    QString lbl1 = lblSuccess;

    QString lo1,lo2,hi1,hi2;
    Evaluate16bitExpr(node->m_left,lo1,hi1);
    Evaluate16bitExpr(node->m_right,lo2,hi2);



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


bool CodeGen6502::IsSimpleAndOr(QSharedPointer<NodeBinaryClause> node, QString labelSuccess, QString labelFail)
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

QString CodeGen6502::getReturn() {
    if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::WDC65C816) {
        return "plb\n"
               "\trtl";
    }
    return "rts";
}

QString CodeGen6502::getCallSubroutine() {
    if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::WDC65C816) {
        return "jsl";
    }
    return "jsr";
}

QString CodeGen6502::ProcedureEndWithoutReturn() {
    if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::WDC65C816) {
        return "plb\n";
    }
    return "";

}


QString CodeGen6502::getInitProcedure() {
    if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::WDC65C816) {
        return "phb\n"
               "\tphk\n"
               "\tplb\n";
    }
    return "";
}

bool CodeGen6502::IsSimpleAssignInteger(QSharedPointer<NodeAssign> node)
{

    //    qDebug() << "HERE" <<node->m_right->isPointer(as);
    //  qDebug() << TokenType::getType(as->m_symTab->Lookup(node->m_left->getValue(as), node->m_op.m_lineNumber)->getTokenType());
    // Only assign pure variables ( z := ... and not z[i] := ....)
    if (node->m_left->hasArrayIndex())
        return false;




    if (node->m_right->isPointer(as) && node->m_right->hasArrayIndex() && node->m_right->getArrayType(as)==TokenType::INTEGER) {
        // We made sure that we only have stuff like z := p[ expr() ]
        as->Comment("Assigning pure variable, rhs is pure integer pointer optimization");
        auto ptr = qSharedPointerDynamicCast<NodeVar>(node->m_right);
        auto var = qSharedPointerDynamicCast<NodeVar>(node->m_left);
        if (!ptr) return false;
        if (!var) return false;
        // First, evaluate the expression
        if (ptr->m_expr->isPureNumeric()) {
            as->Asm("ldy #"+Util::numToHex(ptr->m_expr->getValueAsInt(as)*2));
        }
        else {
            ptr->m_expr->Accept(this);
            as->Term();
            as->Asm("asl ; integer shift");
            as->Asm("tay");
        }
        as->Asm("lda ("+ptr->getValue(as)+"),y");
        as->Asm("sta "+var->getValue(as)+"");
        if (var->isWord(as)) {
            as->Asm("iny");
            as->Asm("lda ("+ptr->getValue(as)+"),y");
            as->Asm("sta "+var->getValue(as)+"+1");

        }

        return true;
    }


    return false;
}


bool CodeGen6502::IsSimpleAssignPointer(QSharedPointer<NodeAssign> node)
{
    auto var = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    if (var==nullptr)
        return false;

    if (var->getType(as)!=TokenType::POINTER)
        return false;

    if (var->m_expr==nullptr)
        return false;


    if (var->m_writeType==TokenType::INTEGER) {
        as->Comment("Writing an integer class pointer");
        //        node->m_right->setForceType(TokenType::INTEGER);
        as->ClearTerm();
        node->m_right->Accept(this);
        as->Term();
        as->Asm("pha");
        //        as->Asm("tax");
        as->Asm("tya");
        as->Asm("pha");
        //      as->Asm("txa");
        var->m_expr->Accept(this);
        as->Term();
        as->Asm("tay");
        as->Asm("pla");
        as->Asm("iny");
        as->Asm("sta ("+var->getValue(as)+"),y");
        as->Asm("dey");
        as->Asm("pla");
        as->Asm("sta ("+var->getValue(as)+"),y");

        return true;
    }


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







void CodeGen6502::Compare(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, bool isLarge, QString loopDone, QString loopNotDone, bool inclusive) {


    if (nodeA->m_left->isWord(as)) {
        Token t = nodeA->m_op;

        auto nasm = NodeFactory::CreateAsm(t,"  jmp " + loopNotDone + "\n");
        auto block = NodeFactory::CreateBlockFromStatements(t,QVector<QSharedPointer<Node>>() <<nasm);

        if (inclusive)
            nodeB = NodeFactory::CreateBinop(t,TokenType::PLUS,nodeB,NodeFactory::CreateNumber(t,1));

        auto cond = NodeFactory::CreateSingleConditional(t,TokenType::NOTEQUALS,isLarge,nodeA->m_left, nodeB,block);


        as->Comment("Executing integer comparison " + nodeB->getValue(as));
        cond->Accept(this);
        return;

    }
    QString cmp ="cmp ";
    if (Syntax::s.m_currentSystem->is6809())
        cmp = "cmpa ";
    if (nodeB->isPure() && (1==2)) { // DISABLE this optimization - for now
        as->Comment("Optimization: switch A and B, allow for optimizer");
        as->ClearTerm();
        nodeA->m_left->Accept(this);
        as->Term();

        as->Asm(cmp + nodeB->getValue(as) +" ;keep");

    }
    else {
        as->ClearTerm();

        if (!nodeA->m_left->isPureVariable() || nodeA->m_left->hasArrayIndex()) {
            as->Comment("Compare variable is complex, storing in temp variable : "+nodeA->getValue(as));
            nodeA->m_left->Accept(this);
            as->Term();
            QString temp = as->StoreInTempVar("compare_temp");
            as->ClearTerm();
            nodeB->Accept(this);
            as->Term();
            as->Asm(cmp+temp+" ;keep");
        }
        else {
            nodeB->Accept(this);
            as->Term();
            as->Asm(cmp + nodeA->m_left->getValue(as) +" ;keep");
        }
    }
    int stepValue = 1; // do we have a step value?
    if (step != nullptr) {
        stepValue = step->getValueAsInt(as); //node->m_step->getValue(as);
        if (stepValue>127)
            stepValue=-(256-stepValue);

        if (stepValue==0)
            stepValue =1;
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

}




//       if (node->m_loopCounter!=0)
//         ErrorHandler::e.Error("Error: Loop with step other than 1,-1 cannot have loopy/loopx flag");
// Is word




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
void CodeGen6502::dispatch(QSharedPointer<NodeConditional> node)
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
void CodeGen6502::dispatch(QSharedPointer<NodeForLoop> node)
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



void CodeGen6502::LoadPointer(QSharedPointer<NodeVar> node) {
    //    qDebug() << "Dispatcher 6502 loadpointer " <<node->value<<node->isReference();
    as->Comment("Load pointer array");
    QSharedPointer<NodeNumber> number = qSharedPointerDynamicCast<NodeNumber>(node->m_expr);
    QString m = as->m_term;



    as->ClearTerm();
    QString p1 = "(";
    QString p2 = ")";
    if (node->hasFlag(as,"lpointer")) {
        p1="[";
        p2="]";
    }
    if (node->m_expr==nullptr) {
        as->Asm("lda "+getValue(node));
        Disable16bit();
        as->Asm("ldy "+getValue(node) + " +1");
        Enable16bit();
        return;
    }
    bool disable = true;
    if (node->m_expr && node->m_expr->is8bitValue(as))
        Disable16bit();

    if (!LoadXYVarOrNum(node, node->m_expr,false,true))
    {
        if (!(m=="" || m.startsWith("lda")))
            as->Asm("pha");
        node->m_expr->Accept(this);
        as->Term();
        as->Asm("tay");
        if (!(m=="" || m.startsWith("lda")))
            as->Asm("pla");
    }
    if (node->getArrayType(as)==TokenType::INTEGER || node->m_writeType==TokenType::INTEGER) {
        as->Asm("lda "+p1+ getValue(node)+""+p2+",y");
        as->Asm("pha");
        as->Asm("iny");
        as->Asm("lda "+p1+"" + getValue(node)+""+p2+",y");
        as->Asm("tay");
        as->Asm("pla");
        if (disable)
            Enable16bit();
        return;
    }

    if (m=="")
        m="lda ";
    as->Asm(m+  ""+p1+"" + getValue(node)+""+p2+",y");
    if (node->m_forceType == TokenType::INTEGER)
        as->Asm("ldy #0 ; Loading 8-bit pointer, but return type should be integer");

    if (disable)
        Enable16bit();

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

void CodeGen6502::dispatch(QSharedPointer<NodeVar> node)
{
    // Override inline parameters
    //    qDebug() << "INLINE cnt :" <<m_inlineParameters.length() << m_inlineParameters.keys();
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
    if (s->m_isStackVariable) {
        LoadStackVariable(node);
        return;
    }

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

        if (((s->getTokenType()==TokenType::ADDRESS || s->getTokenType()==TokenType::INCBIN || node->isReference())  && as->m_term=="")) {
            if (!node->isPointer(as)) {
                if (!node->isReference())
                    ErrorHandler::e.Error("'"+val+"' is an array. Did you mean to reference it with '#'?",node->m_op.m_lineNumber);
                as->Asm("lda #<" + val);
                Disable16bit();
                as->Asm("ldy #>" + val);
                Enable16bit();
            }
            else {
                as->Asm("lda " + val);
                Disable16bit();
                as->Asm("ldy " + val + "+1");
                Enable16bit();

            }
            return;
        }
        if (node->m_fake16bit && s->getTokenType()==TokenType::BYTE )
            as->Asm("ldy #0 ; Fake 16 bit");

        as->Variable(val, isOK);
    }

}



bool CodeGen6502::LoadXYVarOrNum(QSharedPointer<NodeVar> node, QSharedPointer<Node> other, bool isx, bool scale) {
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
        if (s->m_arrayType==TokenType::INTEGER && scale) // integer array index is *2 (two bytes per array slot)
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
            int s = 1;
            if (scale) s=2;
            as->Asm(operand + "#" + QString::number(other->getValueAsInt(as) * s) + " ; watch for bug, Integer array has max index of 128");
        }
        else
            as->Asm(operand  + getValue(other));
        return true;
    }
    return false;

}

void CodeGen6502::LoadByteArray(QSharedPointer<NodeVar> node) {
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
    bool unknownType = false;
    bool scale = true;
    //    Disable16bit();
    if (node->m_writeType!=TokenType::NADA) {
        s->m_arrayType = node->m_writeType;
        s->m_arrayTypeText = TokenType::getType(node->m_writeType);
        scale = false;
    }

    if (s->m_arrayType==TokenType::INTEGER)
        as->Comment("Load Integer array");

    else if (s->m_arrayType==TokenType::BYTE)
        as->Comment("Load Byte array");
    else {
        as->Comment("Load Unknown type array, assuming BYTE");
        unknownType = true;
    }
    //    bool disable16bit =false;

    if (node->getOrgType(as)!=TokenType::INTEGER && node->m_forceType == TokenType::INTEGER) {
        as->Asm("ldy #0 ; lhs is byte, but integer required");
    }
    // Optimization : ldx #3, lda a,x   FIX
    if ((s->m_arrayType==TokenType::BYTE||unknownType) && node->m_expr!=nullptr) {
        if (node->m_expr->isPureNumeric()) {
            //            as->Comment("Optimising loading byte array with constant");
            QString op = "lda ";
            //  disable16bit = true;
            if (as->m_term!="") {
                //          Disable16bit();
                //        disable16bit = true;
                op = as->m_term + " ";
            }
            as->ClearTerm();
            as->Asm(op+getValue(node) + " +"+node->m_expr->getValue(as) + " ; array with const index optimization");
            //          if (disable16bit)
            //            Enable16bit();
            return;
        }
    }
    QString m = as->m_term;
    if (node->m_expr==nullptr) {
        ErrorHandler::e.Error("Unknown operation with address!",node->m_op.m_lineNumber);
    }
    as->ClearTerm();
    Disable16bit();


    if (!LoadXYVarOrNum(node, node->m_expr,true,scale))
    {
        // calculation version, eg: index+2  or 3+2

        as->ClearTerm();
        if (m_flag1)
            as->Asm("pha");

        //as->Asm("pha");
        node->m_expr->Accept(this);
        as->Term();
        if (s->m_arrayType==TokenType::INTEGER && scale) // integer array index is *2 (two bytes per array slot)
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
    //    if (disable16bit)
    Enable16bit();
}

void CodeGen6502::LoadVariable(QSharedPointer<Node> node)
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


void CodeGen6502::LoadVariable(QSharedPointer<NodeVar> node) {

    /*        if (as->m_symTab->Lookup(value)==nullptr)
                ErrorHandler::e.Error("Could not find variable '" +value +"' for storing.",m_op.m_lineNumber);
    */
    TokenType::Type t = as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber)->getTokenType();
    QString type = as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber)->m_type;
    if (as->m_symTab->m_records.contains(type))
        t = TokenType::ADDRESS; // Working with a CLASS directly (not pointer)

    if (node->isStackVariable()) {
        LoadStackVariable(node);
        return;
    }


    //    qDebug() << "LoadVariable: "<<pp;
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


void CodeGen6502::LoadVariable(QSharedPointer<NodeNumber>node)
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

void CodeGen6502::LoadVariable(QSharedPointer<NodeProcedure> node)
{
    as->Asm("lda #<"+node->m_procedure->m_procName);
    Disable16bit();
    as->Asm("ldy #>"+node->m_procedure->m_procName);
    Enable16bit();
}


void CodeGen6502::StoreVariable(QSharedPointer<NodeVar> node) {
    //        as->Comment("VarNode StoreVariable");
    //          ErrorHandler::e.Error("Could not find variable '" +value +"' for storing.", m_op.m_lineNumber);
    if (!node->m_ignoreLookup)
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
                Disable16bit();
                as->Asm("sty "  + getValue(node) +"+"+ QString::number(i+1));
                Enable16bit();

            }
            else {
                as->Asm("sta " + getValue(node) + "+"+ getValue(node->m_expr));
                if (node->m_writeType==TokenType::INTEGER) {
                    Disable16bit();
                    as->Asm("sty " + getValue(node) + "+"+ getValue(node->m_expr)+"+1");
                    Enable16bit();
                }
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
                if (node->hasFlag(as,"lpointer")) {
                    pa="[";
                    pb="]";
                }
                as->Comment("Storing to a pointer");
            }
            //            as->Comment("Writetype: " +TokenType::getType(node->m_writeType));


            // Optimize for number or pure var
            if (node->m_expr->getType(as)==TokenType::INTEGER_CONST && node->getArrayType(as)!=TokenType::INTEGER && node->m_writeType!=TokenType::INTEGER) {
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
            if (node->getArrayType(as)==TokenType::INTEGER || node->m_writeType==TokenType::INTEGER) {
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
                if (as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber)->getTokenType() == TokenType::INTEGER || node->m_writeType==TokenType::INTEGER) {
                    as->Asm("sta " + getValue(node));
                    Disable16bit();

                    as->Asm("sty " + getValue(node) + "+1");
                    Enable16bit();

                    return;
                }
                else {
                    ErrorHandler::e.Error("Unable to assign variable : "+getValue(node)+ " of type "+node->getTypeText(as),node->m_op.m_lineNumber);
                }

    }




}

void CodeGen6502::LoadStackVariable(QSharedPointer<NodeVar> node) {
    as->Asm("tsx");
    //    as->Asm("txa");
    as->Asm("lda $103+"+QString::number(node->getStackShift()+m_stackShift)+",x");
    //    qDebug() << node->getStackShift()+m_stackShift;

}


void CodeGen6502::AssignString(QSharedPointer<NodeAssign> node) {

    bool isPointer = node->m_left->isPointer(as);
    QSharedPointer<NodeString> right = qSharedPointerDynamicCast<NodeString>(node->m_right);
    QSharedPointer<NodeVar> left = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    //    QString lbl = as->NewLabel("stringassign");

    /*    if (isPointer && node->m_left->hasArrayIndex()) {
        right->Accept(this);

        as->Asm("sta ("+ getValue(left)+"),y");
        return;

    }
*/

    QString str = as->NewLabel("stringassignstr");
    QString lblCpy=as->NewLabel("stringassigncpy");


    //    as->Asm("jmp " + lbl);
    as->StartExistingBlock(as->m_tempVarsBlock);
    if (right->m_op.m_type==TokenType::CSTRING) {
        as->DeclareCString(str,QStringList() <<right->m_op.m_value,right->flags.keys());
    }
    else {
        //        QString strAssign = str + "\t.dc \"" + right->m_op.m_value + "\",0";
        as->DeclareString(str,QStringList() <<right->m_op.m_value,right->flags.keys());
        //      as->m_tempVars<<strAssign;
    }

    as->EndCurrentBlock();

    //as->Label(str + "\t.dc \"" + right->m_op.m_value + "\",0");
    //  as->Label(lbl);

    //    qDebug() << "IS POINTER " << isPointer;
    if (isPointer && node->m_left->hasArrayIndex()==false) {
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

bool CodeGen6502::AssignPointer(QSharedPointer<NodeAssign> node) {
    QSharedPointer<NodeVar> bVar = qSharedPointerDynamicCast<NodeVar>(node->m_right);
    //    QSharedPointer<NodeNumber> bNum = dynamic_cast<QSharedPointer<NodeNumber>>(node->m_right);
    QSharedPointer<NodeVar> aVar = qSharedPointerDynamicCast<NodeVar>(node->m_left);

    if (IsSimpleIncDec(node))
        return true;

    if (aVar==nullptr)
        return false;


    if (!aVar->isPointer(as))
        return false;
    // Only for assigning PURE pointers!
    if (aVar->hasArrayIndex())
        return false;

    node->VerifyReferences(as);


    if (node->m_right->isPure()) {

        Disable16bit();
        as->Asm("lda " + getValue8bit(node->m_right,false));
        as->Asm("ldx " + getValue8bit(node->m_right,true));
        as->Asm("sta " + getValue(aVar));
        as->Asm("stx "+ getValue(aVar)+"+1");
        if (aVar->hasFlag(as,"lpointer")) {
            as->Asm("lda " + getValue8bit(node->m_right,2));
            as->Asm("sta "+ getValue8bit(aVar,2));
        }

        Enable16bit();
        //if (!node->m_right->isWord(as))
        //    ErrorHandler::e.Warning("Assigning an 8-bit value to pointer. Is this intentional?", node->m_op.m_lineNumber);

        return true;
    }
    // Generic expression

    node->m_right->forceWord();
    node->m_right->setForceType(TokenType::INTEGER);
    as->Term();
    node->m_right->Accept(this);
    as->Term();
    as->Asm("sta " + getValue(aVar));
    Disable16bit();
    as->Asm("sty "+ getValue(aVar)+"+1");
    Enable16bit();

    return true;
    //     ErrorHandler::e.Error("Right-hand side must be constant or address", node->m_op.m_lineNumber);

}

bool CodeGen6502::isSimpleAeqAOpB(QSharedPointer<NodeVar> var, QSharedPointer<NodeAssign> node) {
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
//    Cast(var->getType(as), rterm->m_castType);
    StoreVariable(var);
    return true;
}

bool CodeGen6502::isSimpleAeqAOpB16Bit(QSharedPointer<NodeVar> var, QSharedPointer<NodeAssign> node)
{
    QSharedPointer<NodeBinOP> rterm = qSharedPointerDynamicCast<NodeBinOP>(node->m_right);
    if (rterm==nullptr)
        return false;

    // right first is var
    QSharedPointer<NodeVar> rvar = qSharedPointerDynamicCast<NodeVar>(rterm->m_right);

    QSharedPointer<NodeVar> bvar = qSharedPointerDynamicCast<NodeVar>(rterm->m_left);
    if (bvar==nullptr)
        return false;

    if (bvar->hasArrayIndex())
        return false;

    if (var->hasArrayIndex())
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

        //       qDebug() << "CodeGen6502::isSimpleAeqAOpB16Bit HERE";
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
            if (var->hasFlag(as,"lpointer")) {
                as->Comment("Oops! 24 bit value");
                as->Asm("lda "+getValue8bit(bvar,2)); //hi-hi byte
                as->ClearTerm();
                as->BinOP(rterm->m_op.m_type,false);
                as->Term(rterm->m_right->getValue8bit(as,2),true);
                as->Asm("sta " + getValue(var) + "+2");

            }

                // HEREE
        }
        //        as->Asm("sty " + getValue(var) +"+1");

        as->PopLabel("WordAdd");
        return true;

    }


    return false;
}




bool CodeGen6502::IsSimpleIncDec(QSharedPointer<NodeAssign> node) {
    // Right is binop
    //    return false;
    auto var = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    if (var==nullptr)
        return false;
    QSharedPointer<NodeBinOP> rterm = qSharedPointerDynamicCast<NodeBinOP>(node->m_right);
    if (rterm==nullptr)
        return false;
    // right first is var

    //   qDebug() << rterm->m_op.getType();


    /*    QSharedPointer<NodeVar> rvar = qSharedPointerDynamicCast<NodeVar>(rterm->m_left);
    as->Comment("Test Inc dec "+QString::number(rvar==nullptr));
    if (rvar==nullptr)
        return false;
*/
    auto rvar = rterm->m_left;
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

    // #array[i]
    //    if (rterm->isReference() && rterm->hasArrayIndex())
    //      return false;


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

    as->Comment("Test Inc dec D");


    if (!var->hasArrayIndex() && !rvar->hasArrayIndex()) {
        as->Asm(operand +getValue(var));

        return true;
    }
    else {
        if (!rvar->hasArrayIndex())
            return false;
        if (var->m_expr==nullptr)
            return false;
        // BOTH must have expressions
        auto rv = qSharedPointerDynamicCast<NodeVar>(rvar);
        if (rv==nullptr)
            return false;
        if (!rv->m_expr->DataEquals(var->m_expr))
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

        if (LoadXYVarOrNum(var, var->m_expr,true,true)) {

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



void CodeGen6502::dispatch(QSharedPointer<NodeRepeatUntil> node)
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

void CodeGen6502::dispatch(QSharedPointer<NodeComment> node)
{
    node->DispatchConstructor(as,this);

    if (node->m_comment!="") {
        //        as->Comment("A COMMENT");

        as->Comment(node->m_comment.replace("\n","\n; "));
    }

}

QString CodeGen6502::resolveTemporaryClassPointer(QString name, int mul, int& res)
{
    // pm_POINTER_Monster_x
    QStringList lst = name.split("_POINTER_");
    QString orgPointer = lst[0];
    QString recordName = lst[1].split("_").first();
    QString var = name;
    var = var.remove(orgPointer+"_POINTER_");



    auto record = as->m_symTab->m_records[recordName];
    res = record->getShiftedPositionOfVariable(var,1);
    return orgPointer;

    /*    as->ClearTerm();
    as->Comment("Resolving temporary class pointer");
    QString zp = as->m_internalZP.Get();
    if (sizeShift==0) {
        as->Asm("lda "+orgPointer);
        as->Asm("sta "+zp);
        as->Asm("lda "+orgPointer+"+1");
        as->Asm("sta "+zp+"+1");

        return zp;

    }
    else {

        as->Asm("lda "+orgPointer);
        as->Asm("clc");
        as->Asm("adc #"+Util::numToHex(sizeShift&0xFF));
        as->Asm("sta "+zp);
        as->Asm("lda "+orgPointer+"+1");
        as->Asm("adc #"+Util::numToHex((sizeShift>>8)&0xFF));
        as->Asm("sta "+zp+"+1");
    }
    as->m_internalZP.Pop(zp);
*/

    //return zp;
}




void CodeGen6502::HackPointer(Assembler *as, QSharedPointer<Node> n)
{
    if (n==nullptr)
        return;

    auto v = qSharedPointerDynamicCast<NodeVar>(n);

    if (v==nullptr)
        return;

    if (v->getValue(as).contains("_POINTER_")) {

        as->Comment("Assigning pointer of record/class for " +v->getValue(as));
        int val = 0;
        QString zp = resolveTemporaryClassPointer(v->getValue(as),1,val); // For now, only allow to point to single objects

        //return;
        // simply hack value
        v->value = zp;
        v->setForceType(TokenType::POINTER);
        v->m_op.m_type=TokenType::POINTER;
        Token t = v->m_op;
        t.m_type = TokenType::INTEGER;
        t.m_value = Util::numToHex(val);
        t.m_intVal = val;
        v->m_ignoreLookup = true;
        v->m_expr = QSharedPointer<NodeNumber>(new NodeNumber(t,val));

    }

}

void CodeGen6502::PopLostStack(int num)
{
    if (num==0) return;
    as->Asm("tax");
    for (int i=0;i<num;i++)
        as->Asm("pla");
    as->Asm("txa");
}

void CodeGen6502::Cast(TokenType::Type from, TokenType::Type to)
{
    //    qDebug() <<"Cast " <<TokenType::getType(from) << " " << TokenType::getType(to);
    if (from==to)
        return;

    if (from==TokenType::BYTE && (to == TokenType::INTEGER || to ==TokenType::INTEGER_CONST)) {
        as->Comment("Casting from byte to integer");
        as->Asm("ldy #0");
    }
 /*   if (from==TokenType::INTEGER && to == TokenType::BYTE) {
        as->Comment("Casting from integer to byte");
        as->Asm("ld a,l");
    }
*/
}

void CodeGen6502::AssignFromRegister(QSharedPointer<NodeAssign> node)
{
    QString vname = node->m_left->getValue(as);

    QString reg = node->m_right->getValue(as).toLower();
    //if (vname=="_a" || vname=="_x" || vname=="_y")
    //{
    if (!node->m_right->isPure())
        ErrorHandler::e.Error("Using _A, _X and _Y register values must be pure.", node->m_op.m_lineNumber);

    if (reg=="_ax") {
        as->Asm("sta "+vname);
        as->Asm("stx "+vname+"+1");
        return;
    }
    if (reg=="_ay") {
        as->Asm("sta "+vname);
        as->Asm("sty "+vname+"+1");
        return;
    }

    QString cmd = "st"+QString(reg[1]) + " "+vname;
    as->Asm(cmd);
}

void CodeGen6502::AssignToRegister(QSharedPointer<NodeAssign> node)
{
    QString vname = node->m_left->getValue(as).toLower();
    //if (vname=="_a" || vname=="_x" || vname=="_y")
    //{
    QString reg = QString(vname[1]);
    as->Comment("Assigning register : " + vname);
    if (vname.length()==2) {
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
    if (vname.length()==3) {
        if (!node->m_right->isPure())
            ErrorHandler::e.Error("Setting _AX and _AX, and _XY register values must be pure number or variable.", node->m_op.m_lineNumber);


        QString cmdA = "ld"+QString(vname[1]) + " "+node->m_right->getValue8bit(as,false);
        QString cmdB = "ld"+QString(vname[2]) + " "+node->m_right->getValue8bit(as,true);
        as->Asm(cmdA);
        as->Asm(cmdB);
        return;
    }
    return;
}

void CodeGen6502::OptimizeBinaryClause(QSharedPointer<Node> node, Assembler* as)
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

void CodeGen6502::dispatch(QSharedPointer<NodeUnaryOp> node)
{
    node->DispatchConstructor(as,this);
    AbstractCodeGen::dispatch(node);
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


void CodeGen6502::CompareAndJumpIfNotEqualAndIncrementCounter(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive)
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


void CodeGen6502::CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QString lblJump, bool isOffPage)
{
    if (!nodeA->isPure())
        ErrorHandler::e.Error("Compare must be pure variable",nodeA->m_op.m_lineNumber);
    //        if (nodeA->isWord(as))
    //          ErrorHandler::e.Error("Integer compares not supported yet on the 6502",nodeA->m_op.m_lineNumber);
    if (nodeA->isWord(as))
        nodeB->setForceType(TokenType::INTEGER);
    as->ClearTerm();
    nodeB->Accept(this);
    as->Term();
    as->Asm("cmp " + nodeA->getValue(as) +" ;keep");
    as->Asm("bne " +lblJump);
    if (nodeA->isWord(as)) {
        as->Asm("cpy " + nodeA->getValue(as) +"+1 ;keep");
        as->Asm("bne " +lblJump);

    }
    return;
}




bool CodeGen6502::StoreVariableSimplified(QSharedPointer<NodeAssign> assignNode)
{
    //QSharedPointer<NodeNumber> num = dynamic_cast<QSharedPointer<NodeNumber>>(node->m_expr);
    QSharedPointer<NodeVar> node = qSharedPointerDynamicCast<NodeVar>(assignNode->m_left);
    auto expr = assignNode->m_right;
    if (node==nullptr)
        return false;


    // IGNORE zp:=#zp when THIS implied


    if (!(!node->isWord(as) && expr->isPure() && node->m_expr!=nullptr))
        return false;
    if (node->m_writeType != TokenType::BYTE)
        return false;

    //    as->Comment("Simplified storevariable");




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
            return true;
        }

    QString secondReg="x";
    QString pa = "";
    QString pb= "";
    if (node->getType(as)==TokenType::POINTER) {
        secondReg="y";
        pa="(";
        pb=")";

        if (node->hasFlag(as,"lpointer")) {
            pa="[";
            pb="]";
        }


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
        as->Asm("sta "+pa+getValue(node)+pb+",y");
        as->Asm("iny");
        as->Asm("txa");
        as->Asm("sta "+pa+getValue(node)+pb+",y");
        return true;

    }
    as->Asm("ta" + secondReg);
    as->ClearTerm();
    expr->Accept(this);
    as->Term();
    as->Asm("sta " +pa + getValue(node)+pb+","+ secondReg);
    return true;

}

bool CodeGen6502::StoreStackParameter(QSharedPointer<NodeAssign> n)
{
    auto var = qSharedPointerDynamicCast<NodeVar>(n->m_left);
    if (var==nullptr)
        return false;

    if (!var->isStackVariable())
        return false;

    LoadVariable(n->m_right);

    if (n->m_isProcedureParameterAssign) {
        as->Asm("pha");
        if (var->isWord(as)) {
            as->Asm("tya");
            as->Asm("pha");
        }
        return true;
    }

    as->Asm("tsx");
    //    as->Asm("txa");
    as->Asm("sta $103+"+QString::number(var->getStackShift())+",x");



    return true;

}
