
#include "codegen_6809.h"

CodeGen6809::CodeGen6809()
{

}

/*
 *
 *  NODE BINOP dispatcher
 *
 *  */



void CodeGen6809::EightBitMul(QSharedPointer<Node> node) {




    as->Comment("8 bit mul");
    as->ClearTerm();
    node->m_left->Accept(this);
    as->Term();
    as->Asm("tfr a,b");
    as->Comment("Load right hand side");
    node->m_right->Accept(this);

    as->Term();

    as->Asm("mul");
    as->Asm("tfr b,a");

}

void CodeGen6809::HandleGenericBinop16bit(QSharedPointer<Node> node) {



    //QString lbl = as->NewLabel("rightvarInteger");
    QString lblJmp = as->NewLabel("jmprightvarInteger");


    as->Comment("Generic 16 bit op");

    /*    as->Asm("jmp " + lblJmp);
        as->Write(lbl +"\t.word\t0");
        as->Label(lblJmp);*/

    as->ClearTerm();
//    as->Asm("ldy #0");
    //    qDebug() <<node->m_left->m_op.m_value;
    //  exit(1);
    //   node->m_right->forceWord();
    node->m_right->Accept(this);

    // 255 + k - j doesn't work
    as->Term();
    QString lbl = as->StoreInTempVar("rightvarInteger", as->word);

    //    as->Asm("sta " +lbl);
    //    as->Asm("sty " +lbl+"+1"); // J is stored
    as->Term();

    //as->Variable(getValue(v), false);
    //as->Asm("lda " + getValue(v) + "+1");
    node->m_left->m_isWord = true;
    node->m_left->Accept(this);
    as->Term();
    as->Asm("tfr x,d");
    as->Comment("binop:");
    as->BinOP16(node->m_op.m_type);
    as->Term(lbl, true); // high bit added to a
    as->Asm("tfr d,x");
//    as->Asm("stx "+lbl);
    as->PopLabel("wordAdd");

    //as->PopLabel("rightvarInteger");
    as->PopLabel("jmprightvarInteger");
    as->PopTempVar();
}
void CodeGen6809::HandleVarBinopB16bit(QSharedPointer<Node> node) {
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
//    as->Asm("ldy #0 ; ::HandleVarBinopB16bit 0");

    if (node->m_right->isPure())  {
        BinOp16(node);
        //as->Asm("tfr d,y ");
        return;

    }


    //    qDebug() << node->m_right
    //qDebug() << "NodeBinop : " << TokenType::getType(node->m_right->getType(as)) <<TokenType::getType(node->m_left->getType(as)) ;

    if (node->m_right->getType(as)==TokenType::ADDRESS && node->m_left->getType(as)==TokenType::ADDRESS )
        ErrorHandler::e.Warning("You are adding together two addresses. Is this really what you indend?", node->m_op.m_lineNumber);

    node->m_right->Accept(this);

    as->Term();
    //    as->Asm("sta " +lbl);
    QString lbl = as->StoreInTempVar("rightvarInteger", as->word);
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

bool CodeGen6809::HandleSingleAddSub(QSharedPointer<Node> node) {

    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(node->m_right);
    QSharedPointer<NodeVar> vnum = qSharedPointerDynamicCast<NodeVar>(node->m_right);
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    if (vnum!=nullptr && vnum->hasArrayIndex())
        return false;
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
        as->Term(node->m_right->getValue(as));
 //       node->m_right->Accept(this);
   //     as->Term();

        m_flag1=false;
        as->Term(" ; end add / sub var with constant", true);
        if (node->m_left->isBool(as))
            as->Asm("anda #1 ; boolean");
        return true;
    }


    return false;
}

void CodeGen6809::HandleMulDiv(QSharedPointer<Node> node) {

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

        if (node->isWord(as)) {
            Mul16x8(node);
            Cast(TokenType::INTEGER, node->m_left->m_castType);
        }
        else
            EightBitMul(node);

        // since mul / div is always 16-bit, perform a cast if necessary
        return;
    }
    if (node->m_op.m_type==TokenType::DIV) {

        Div16x8(node);
        // Since div always 16 bit
//        as->Asm(";Casting? "+TokenType::getType(node->m_left->m_castType));


        return;
    }
    ErrorHandler::e.Error("Binary operation / not implemented for this type yet...");
}

void CodeGen6809::RightIsPureNumericMulDiv16bit(QSharedPointer<Node> node) {
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
    as->Comment("16 bit mul or div power of two");
    QString command = "";
    QString varName;
    if (node->m_op.m_type == TokenType::DIV)
        command = "lsr";
    if (node->m_op.m_type == TokenType::MUL) {
        Mul16x8(node);
        return;
    }
    as->Asm("tfr x,d");


    as->Term();
    LoadVariable(node->m_left);
    as->Term();
    varName = as->StoreInTempVar("int_shift", as->word);
    if (node->m_op.m_type == TokenType::DIV) {
        command = "\tlsr " + varName +"+0"+ "\n";
        command += "\tror " + varName+"+1" + "\n";
    }
    else {
        command = "\tasl " + varName +"+0"+ "\n";
        command += "\trol " + varName+"+1" + "\n";
    }

    for (int i=0;i<cnt;i++)
        as->Asm(command);

    as->Asm("ldx " + varName);
 //   Cast(TokenType::INTEGER, node->m_left->t)

    as->PopTempVar();
}

void CodeGen6809::HandleShiftLeftRight(QSharedPointer<NodeBinOP>node)
{
    QString cmd = node->m_op.m_type==TokenType::SHR?"lsra":"asla";

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
    as->Asm("ldb #0");
    as->Asm("exg a,b");
    as->Asm("tfr d,x");
    node->m_left->Accept(this);
    as->Term();
    as->Asm("cmpx #0");
    as->Asm("beq "+lblCancel);
    as->Term();
    as->Label(lbl);
    as->Asm(cmd);
    as->Asm("leax -1,x");
    as->Asm("cmpx #0");
    as->Asm("bne "+lbl);
    as->Label(lblCancel);

    as->PopLabel("lblShift");
    as->PopLabel("lblShiftDone");
}

void CodeGen6809::HandleShiftLeftRightInteger(QSharedPointer<NodeBinOP>node, bool isSimpleAeqAopB)
{
    // Not yet implemented

    QString varName = "";
    if (!isSimpleAeqAopB) {
        node->m_left->Accept(this);
        as->Term();
        varName = as->StoreInTempVar("tempVarShift",as->word);
    }
    else
        varName = getValue(node);

    //    QString cmd = node->m_op.m_type==TokenType::SHR?"lsr":"asl";
    QString command = "";
/*    if (node->m_op.m_type == TokenType::SHL) {
        as->Asm("add hl,hl");
    }
    if (node->m_op.m_type == TokenType::SHR) {
        as->Asm("srl h");
        as->Asm("rr l");
    }
*/


    if (node->m_op.m_type==TokenType::SHR) {
        command = "\tlsr " + varName +"+0 \n";
        command += "\tror " + varName+"+1 \n";
    }
    else {
        command = "\tasl " + varName +"+1 \n";
        command += "\trol " + varName+"+0 \n";
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
        as->Asm("tfr x,y");
        as->Asm("cmpy #0");
        as->Asm("beq "+lblCancel);
        QString lbl = as->NewLabel("lblShift");
        as->Label(lbl);
        as->Asm(command);
        as->Asm("leay -1,y");
        as->Asm("cmpy #0");
        as->Asm("bne "+lbl);
        as->Label(lblCancel);

        as->PopLabel("lblShift");
        as->PopLabel("lblShiftCancel");
    }
    if (!isSimpleAeqAopB) {
        as->Asm("ldx "+varName);
//        as->Asm("ldy "+varName +"+1");
        as->PopTempVar();
    }
}

void CodeGen6809::Mul16x8(QSharedPointer<Node> node) {
    as->Comment("Mul 16x8 setup");
    as->Asm("");
    if (node->m_right->isPureNumeric() && node->m_left->getOrgType(as)==TokenType::BYTE)
        node->SwapNodes();
    else
    if (!node->m_left->isWord(as) && node->m_right->isWord(as)) {
        node->SwapNodes();
    }
    //    Disable16bit();
    LoadVariable(node->m_left);

    as->Asm("tfr x,y");
    if (!node->m_right->isPure())
        as->Asm("pshs y");
    LoadVariable(node->m_right);
    if (!node->m_right->isPure())
        as->Asm("puls y");
    as->Term();
    as->Asm("exg x,y");
    as->Asm("pshs x,y");
    as->Asm("jsr mul16_internal");
    as->Asm("puls y,x");
    as->Asm("puls d");
    as->Asm("tfr y,x");
/*    ldx i1
    ldy i2
    pshs x,y
    jsr mul16_internal
    puls x,y
    puls d
    sty i3
  */
}

void CodeGen6809::Div16x8(QSharedPointer<Node> node) {
    as->Comment("16x16 div");

    node->m_left->setForceType(TokenType::INTEGER);
    node->m_right->setForceType(TokenType::INTEGER);
    LoadVariable(node->m_right);

    as->Asm("tfr x,y");
    if (!node->m_left->isPure())
        as->Asm("pshs y");
    LoadVariable(node->m_left);
    as->Term();
    if (!node->m_left->isPure())
        as->Asm("puls y");
    as->Asm("exg x,y");
    as->Asm("pshs x,y");
    as->Asm("jsr UDIV16");
    as->Asm("puls x");
    as->Comment("; CAST TYPE "+TokenType::getType(node->m_left->m_castType));
    Cast(TokenType::INTEGER, node->m_left->m_castType);


}




void CodeGen6809::HandleRestBinOp(QSharedPointer<Node> node) {
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
            if (node->m_left->isBool(as))
                as->Asm("anda #1");
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

void CodeGen6809::RightIsPureNumericMulDiv8bit(QSharedPointer<Node> node) {
    int val = node->m_right->getValueAsInt(as); //qSharedPointerDynamicCast<NodeNumber>(node->m_right)->m_val;
    int cnt = Util::getShiftCount(val);
    if (cnt == -1 ) {
        if (node->m_op.m_type == TokenType::MUL)
            EightBitMul(node);
        else
            //ErrorHandler::e.Error("Binary operation / not implemented for this value yet ( " + QString::number(val) + ")");
        {
            Div16x8(node);
        }
        return;
    }
    as->Comment("8 bit mul of power 2");

    QString command = "";
    QString varName;
    if (node->m_op.m_type == TokenType::DIV)
        command = "lsra";
    if (node->m_op.m_type == TokenType::MUL)
        command = "asla";


    as->Asm("");
    LoadVariable(node->m_left);
    as->Term();

    for (int i=0;i<cnt;i++)
        as->Asm(command);

}

void CodeGen6809::Load16bitVariable(QSharedPointer<Node> node, QString reg)
{
    as->ClearTerm();
    //    as->Comment("Load 16 bit var IS WORD "+QString::number(node->isWord(as)));
    node->Accept(this);
    as->Term();

/*    if (node->isWord(as)) {

        as->Asm("ld"+reg+" "+getValue(node));
    }
    else
        as->Asm("lda "+getValue(node));*/
}


void CodeGen6809::dispatch(QSharedPointer<NodeBinOP>node)
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
                as->Asm("ldx " + s+QString::number(val));
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

void CodeGen6809::dispatch(QSharedPointer<NodeNumber>node)
{
    node->DispatchConstructor(as,this);
    LoadVariable(node);
}
/*
 *
 *  SMALL NODE dispatcherS
 *
 *  */






void CodeGen6809::dispatch(QSharedPointer<Node> node)
{
    node->DispatchConstructor(as,this);
    node->m_currentLineNumber = node->m_op.m_lineNumber;


    //    qDebug() << "CodeGen6809 UNKNOWN TYPE! Should never be called.";
    //    exit(1);
}






void CodeGen6809::dispatch(QSharedPointer<NodeVarType> node)
{
    node->DispatchConstructor(as,this);

}

void CodeGen6809::dispatch(QSharedPointer<NodeBinaryClause> node)
{
    node->DispatchConstructor(as,this);

}

void CodeGen6809::dispatch(QSharedPointer<NodeString> node)
{
    node->DispatchConstructor(as,this);
    //    exit(1);
/*    if (node->m_val.length()>=1 && node->m_val[0].length()>=1) {
        as->ClearTerm();

        //        as->Asm("lda #"+QString::number(QChar(node->m_val[0][0]).unicode()));
        CStringItem it  = ((Asm6502*)as)->m_cstr[QString(node->m_val[0][0]) ];
        //        as->Asm("lda #"+QString::number(it.m_char.unicode()->unicode()));
        as->Asm("lda #"+QString::number(it.m_screenCode));
        as->Term();
        return;
    }*/
    as->String(node->m_val,true);
}


void CodeGen6809::dispatch(QSharedPointer<NodeVarDecl> node)
{
    node->DispatchConstructor(as,this);


    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);

    QSharedPointer<Appendix> old = as->m_currentBlock;

    if (t->m_flags.contains("ram")) {
        as->m_currentBlock = as->m_wram;
    }

    if (t->m_flags.contains("bank")) {
        QString bnk = getBank(t);
        as->m_currentBlock = as->m_banks[bnk];
    }
/*    if (v->m_isGlobal) {
        as->m_currentBlock = nullptr;
        return;
    }
*/

    //    qDebug() << "" <<as->m_currentBlock;
    AbstractCodeGen::dispatch(node);
    if (t->m_op.m_type==TokenType::INCSID || t->m_op.m_type==TokenType::INCNSF) {
        IncSid(node);
        return;
    }
    //  qDebug() << as->m_currentBlock;
    as->m_currentBlock = old;



}




void CodeGen6809::IncSid(QSharedPointer<NodeVarDecl> node) {

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



void CodeGen6809::DeclarePointer(QSharedPointer<NodeVarDecl> node) {

//    if (!as->CheckZPAvailability())
  //      ErrorHandler::e.Error("Could not allocate more free pointers! Please free some up, or declare more in the settings page. ", node->m_op.m_lineNumber);

    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);
    QString initVal = t->initVal;

    //    qDebug() << "POINTER INIT VAL " <<initVal;
/*    if (initVal=="") {
        initVal = as->PushZeroPointer();
        node->m_pushedPointers++;
    }
*/  if (initVal.trimmed()=="") initVal="$00";
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    as->Write(v->value + ":\tfdb\t" + initVal,0);

}

void CodeGen6809::BinOp16(QSharedPointer<Node> node)
{
//    if (node->m_op.m_type!=TokenType::PLUS && node->m_op.m_type!=TokenType::MINUS)
  //      ErrorHandler::e.Error("Only + and - are currently implemented",node->m_op.m_lineNumber);
   // node->m_right->setForceType(TokenType::INTEGER);
    node->m_left->setForceType(TokenType::INTEGER);
    if (node->m_op.m_type==TokenType::PLUS) {
        LoadIndex(node->m_right,TokenType::BYTE);
        if (!node->m_left->isPure())
            PushD();
        LoadVariable(node->m_left);
        if (!node->m_left->isPure())
            PopD();
        as->Asm("leax d,x");
    }
    if (node->m_op.m_type==TokenType::MINUS) {
        LoadIndex(node->m_right,TokenType::BYTE);
      //  as->Asm("pshs y");
        //as->Asm("tfr y,d");
        as->Asm("negb");
        as->Asm("sex");
        if (!node->m_left->isPure())
            PushD();
        LoadVariable(node->m_left);
        as->Term();
        if (!node->m_left->isPure())
            PopD();

//        as->Asm("nega");
//        as->Asm("puls y");
        as->Asm("leax d,x");
    }
//    StoreVariable(node->m_left);

}

void CodeGen6809::AssignVariable(QSharedPointer<NodeAssign> node)
{
    if (!node->m_left->isWord(as)) {
        node->m_right->setForceType(TokenType::BYTE);
    }
    AbstractCodeGen::AssignVariable(node);
}

QString CodeGen6809::getIncbin() {
    return "includebin";
}

void CodeGen6809::PrintCompare(QSharedPointer<Node> node, QString lblSuccess, QString lblFailed)
{

    QString bcs ="lbcc ";
    QString bcc ="lbcs ";
    if (node->isSigned(as)) {
        as->Comment("Signed compare");
        bcs = "lbpl ";
        bcc = "lbmi ";
    }
    if (node->m_op.m_type==TokenType::EQUALS)
        as->Asm("lbne " + lblFailed);
    if (node->m_op.m_type==TokenType::NOTEQUALS)
        as->Asm("lbeq " + lblFailed);
    if (node->m_op.m_type==TokenType::GREATEREQUAL) {
        as->Asm(bcc + lblFailed);
    }
    if (node->m_op.m_type==TokenType::GREATER) {
        as->Asm(bcc + lblFailed);
        as->Asm("lbeq " + lblFailed);
    }
    if (node->m_op.m_type==TokenType::LESSEQUAL ) {
        as->Asm("lbeq " + lblSuccess);
        as->Asm(bcs + lblFailed);
    }

    if (node->m_op.m_type==TokenType::LESS)
        as->Asm(bcs + lblFailed);

}
void CodeGen6809::BuildToCmp(QSharedPointer<Node> node)
{
    QString b="";

    QSharedPointer<NodeVar> varb = qSharedPointerDynamicCast<NodeVar>(node->m_right);
    if (varb!=nullptr && !varb->hasArrayIndex())
        b = getValue(varb);

    if (node->m_right->isPureNumeric())
        b = getValue(node->m_right);

    node->m_left->Accept(this);
    as->Term();
    if (b!="") {
        if (b!="#$0") {
            as->Comment("Compare with pure num / var optimization");
            as->Asm("cmpa " + b);
        }
        else {
            as->Comment("cmpa #$00 ignored");
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
        as->Asm("cmpa " + tmpVarA);
        as->PopTempVar();
        as->PopTempVar();
    }


}
void CodeGen6809::BuildSimple(QSharedPointer<Node> node, QString lblSuccess, QString lblFailed, bool page)
{

    if (node->isWord(as)) {
        as->Comment("Binary clause Simplified integer: " + node->m_op.getType());
        BinaryClauseInteger(node,lblSuccess, lblFailed, page);
        return;
    }

    as->Comment("Binary clause Simplified: " + node->m_op.getType());
    BuildToCmp(node);
    PrintCompare(node, lblSuccess,lblFailed);



}

void CodeGen6809::BinaryClauseInteger(QSharedPointer<Node> node,QString lblSuccess, QString lblFailed, bool page)
{

    as->Comment("Binary clause INTEGER: " + node->m_op.getType());
    QString lbl2 = lblFailed;
    QString lbl1 = lblSuccess;

    QString lo1,lo2,hi1,hi2;
    Evaluate16bitExpr(node->m_left,lo1,hi1);
    Evaluate16bitExpr(node->m_right,lo2,hi2);



    if (node->isSigned(as)) {
        ErrorHandler::e.Error("Signed integer comparison not implemented yet", node->m_op.m_lineNumber);
    }


    QString bcc = "lbcs";
    QString bcs = "lbcc";

    as->Comment("Compare INTEGER with pure num / var optimization. GREATER. ");
    as->Asm("ldx "+lo1);
    if (node->m_op.m_type==TokenType::LESSEQUAL) {
        as->Asm("cmpx "+node->m_right->getValue(as));
        as->Asm("lbgt "+ lbl2);
    }
    if (node->m_op.m_type==TokenType::LESS) {
        as->Asm("cmpx "+node->m_right->getValue(as));
        as->Asm("lbge "+lbl2);
    }
    if (node->m_op.m_type==TokenType::GREATEREQUAL) {
        as->Asm("cmpx "+node->m_right->getValue(as));
        as->Asm("lbcs "+lbl2);
    }
    if (node->m_op.m_type==TokenType::GREATER) {
        as->Asm("cmpx "+node->m_right->getValue(as));
        as->Asm("lble "+lbl2);
    }
    if (node->m_op.m_type==TokenType::EQUALS) {
        as->Asm("cmpx "+node->m_right->getValue(as));
        as->Asm("lbne "+lbl2);
    }
    if (node->m_op.m_type==TokenType::NOTEQUALS){
        //            ErrorHandler::e.Error("Comparison of integer NOTEQUALS<> not implemented!", node->m_op.m_lineNumber);
        as->Asm("cmpx "+node->m_right->getValue(as));
        as->Asm("lbeq "+lbl2);
    }
}


bool CodeGen6809::IsSimpleAndOr(QSharedPointer<NodeBinaryClause> node, QString labelSuccess, QString labelFail)
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

QString CodeGen6809::getReturn() {
    if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::WDC65C816) {
        return "plb\n"
               "\trtl";
    }
    return "rts";
}

QString CodeGen6809::getCallSubroutine() {
    if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::WDC65C816) {
        return "jsl";
    }
    return "jsr";
}

QString CodeGen6809::ProcedureEndWithoutReturn() {
    if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::WDC65C816) {
        return "plb\n";
    }
    return "";

}


QString CodeGen6809::getInitProcedure() {
    if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::WDC65C816) {
        return "phb\n"
               "\tphk\n"
               "\tplb\n";
    }
    return "";
}

bool CodeGen6809::IsSimpleAssignInteger(QSharedPointer<NodeAssign> node)
{
    return false;
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
            as->Asm("ldx #"+Util::numToHex(ptr->m_expr->getValueAsInt(as)*2));
        }
        else {
/*            ptr->m_expr->Accept(this);
            as->Term();
            as->Asm("asl ; integer shift");
            as->Asm("tay");*/
            as->Asm(";c22");
            LoadIndex(ptr->m_expr,ptr->getArrayType(as));

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






void CodeGen6809::Compare(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, bool isLarge, QString loopDone, QString loopNotDone, bool inclusive) {


//    Cast(nodeA->m_left->getOrgType(as), nodeB->getOrgType(as));
    // Force casting
    nodeB->setCastType(nodeA->m_left->getOrgType(as));

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
    QString cmp ="cmpa ";
    as->ClearTerm();
//    Cast(nodeA->m_left->getOrgType(as), nodeB->getOrgType(as));
//    as->Comment("NODE B cast type: " + TokenType::getType(nodeA->m_castType));
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
                as->Comment("fori include step >0");
                as->Asm("bcc "+loopNotDone); // or FOR index > TO value
            } else { //if (stepValue < -1) {
                as->Asm("bcs "+loopNotDone); // FOR index < TO value
                as->Asm("beq "+loopNotDone); // BEQ then the BCC below
            }

        }
        else {
            // LargeLoops needs checking
            if (stepValue > 0) {
                as->Asm("bcs "+loopDone); // or FOR index > TO value
            } else { //if (stepValue < -1) {
                as->Asm("beq "+loopNotDone); // BEQ then the BCC below
                as->Asm("bcc "+loopDone); // FOR index < TO value
            }

        }

    }
    else {            // TRSE version will END on the TO value


            if (stepValue == 1 || stepValue == -1) {
                // increments / decrements of 1 are safe for BNE
                as->Asm("lbne "+loopNotDone);
            } else if (stepValue > 1) {
                as->Asm("lbeq "+loopDone); // FOR index == TO value
                as->Asm("lbcc "+loopNotDone); // or FOR index > TO value
            } else { //if (stepValue < -1) {
                as->Asm("bcs "+loopNotDone); // FOR index < TO value
            }

        return;
    }

}



void CodeGen6809::LoadPointer(QSharedPointer<NodeVar> node) {
    //    qDebug() << "Dispatcher 6809 loadpointer " <<node->value<<node->isReference();
    as->Comment("Load pointer array");
    QSharedPointer<NodeNumber> number = qSharedPointerDynamicCast<NodeNumber>(node->m_expr);
    QString m = as->m_term;


    QString addr = "#";
    if (node->isPointer(as))
        addr="";

    if (node->m_expr==nullptr) {
        as->Comment("No index load");
        as->ClearTerm();
        as->Asm("ldx "+addr+getValue(node));
        as->Term();
        return;
    }
    QString shift="d";
    if (node->m_expr->isPureNumeric()) {
        as->Comment("Special case when expression is const");
        as->Asm("ldx "+addr+node->getValue(as));
        shift = QString::number(node->m_expr->getValueAsInt(as)*node->getArrayDataSize(as));
    }
    else {
        LoadIndex(node->m_expr, node->getArrayType(as));
        as->Asm("ldx "+addr+node->getValue(as));
    }
//    as->Asm("leax d,x");
    if (node->m_forceType==TokenType::INTEGER && node->getArrayType(as)==TokenType::BYTE) {
        as->Comment("Forcing data data from byte array to be integer ");
        as->Asm("ldb "+shift+",x");
        as->Asm("lda #0");
        as->Asm("tfr d,x");
    }
    else
    {
        if (node->getArrayType(as)==TokenType::INTEGER)
        {
            as->Asm("ldx "+shift+",x");
            if (node->m_forceType==TokenType::BYTE) {
                as->Comment("load from integer array, force result to be byte");
                as->Asm("tfr x,d");
                as->Asm("exg a,b");

            }
        }
        else
        {
            if (node->m_forceType==TokenType::INTEGER) {
                    as->Comment("Forcing data data from byte array to be integer ");
                    as->Asm("ldb "+shift+",x");
                    as->Asm("lda #0");
                    as->Asm("tfr d,x");
            }
            else
              as->Asm("lda "+shift+",x");
        }

    }


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

void CodeGen6809::dispatch(QSharedPointer<NodeVar> node)
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
    /*    if (s->m_isStackVariable) {
        LoadStackVariable(node);
        return;
    }
*/
    if (node->m_expr!=nullptr) {
        LoadVariable(node);

        //LoadByteArray(as);
    }
    else {

        if (((s->getTokenType()==TokenType::ADDRESS || s->getTokenType()==TokenType::INCBIN || node->isReference())  && as->m_term=="")) {
            if (!node->isPointer(as)) {
                if (!node->isReference())
                    ErrorHandler::e.Error("'"+val+"' is an array. Did you mean to reference it with '#'?",node->m_op.m_lineNumber);

                as->Asm("ldx #" + val);
            }
            else {
                if (node->m_castType==TokenType::BYTE)
                    as->Asm("lda " + val+"+1");
                else
                as->Asm("ldx " + val);

            }
            return;
        }
        /*        if (node->m_fake16bit && s->getTokenType()==TokenType::BYTE )
            as->Asm("ldy #0 ; Fake 16 bit");

        as->Variable(val, isOK);*/
        if (node->getOrgType(as)==TokenType::BYTE || node->getOrgType(as)==TokenType::BOOLEAN) {

            if (node->m_forceType == TokenType::INTEGER) {
                // OOps byte but must be integer
                as->Comment("Forcing byte to be integer ");
 //               if (dstack==0)
   //                 as->Asm("pshs d");
                as->Asm("ldb "+val);
                as->Asm("lda #0");
                as->Asm("tfr d,x");
     //           if (dstack==0)
       //             as->Asm("puls d");
                if (node->m_castType==TokenType::INTEGER)
                    node->m_castType = TokenType::NADA;
                //                Cast(node->getOrgType(as),node->m_castType);

            }
            else {
                if (as->m_term=="")
                    as->m_term = "lda ";
                as->m_term+=val;
            }
        }
        else {
            as->Comment("integer assignment NodeVar");
            if (node->m_forceType == TokenType::BYTE) {
                as->Comment("Force integer to be byte");
                as->Asm("lda "+val+"+1");
                return;
            }

            as->Term("ldx "+val);
            as->Term();
           // Cast(node->getOrgType(as),node->m_castType);
            as->Comment("HERE cast type: "+TokenType::getType(node->m_castType));

        }

    }

}



bool CodeGen6809::LoadXYVarOrNum(QSharedPointer<NodeVar> node, QSharedPointer<Node> other, bool isx, bool scale) {
    QSharedPointer<Symbol> s = as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber);
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(other);
    //QSharedPointer<NodeNumber> num = dynamic_cast<QSharedPointer<NodeNumber>>(other);
    if (other==nullptr)
        return false;
    bool isNumber = other->isPureNumeric();
    QString operand = "ldx ";



    if (var!=nullptr && var->m_expr == nullptr) {
        if (s->m_arrayType==TokenType::INTEGER && scale) // integer array index is *2 (two bytes per array slot)
        {
            //as->Asm("txa   ; watch for bug, Integer array has index range of 0 to 127");
            as->Asm("lda "+ getValue(var));
            as->Asm("asl");
            as->Asm("tra a,x");
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

void CodeGen6809::LoadByteArray(QSharedPointer<NodeVar> node) {
    // Optimizer: if expression is number, just return direct

    QSharedPointer<Symbol> s = as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber);
    if (node->isReference()) {
        // This should now be handled by the parser
        ErrorHandler::e.Error("Unknown syntax: referenced address with index. ", node->m_op.m_lineNumber);
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
//        as->Asm("ldy #0 ; lhs is byte, but integer required");
    }
    // Optimization : ldx #3, lda a,x   FIX
    if ((s->m_arrayType==TokenType::BYTE||unknownType) && node->m_expr!=nullptr) {
        if (node->m_expr->isPureNumeric()) {
            //            as->Comment("Optimising loading byte array with constant");
            QString op = "lda ";
            if (as->m_term!="") {
                op = as->m_term + " ";
            }
            as->ClearTerm();
            as->Asm(op+getValue(node) + " +"+node->m_expr->getValue(as) + " ; array with const index optimization");
            return;
        }
    }
    QString m = as->m_term;
    if (node->m_expr==nullptr) {
        ErrorHandler::e.Error("Unknown operation with address!",node->m_op.m_lineNumber);
    }
    as->ClearTerm();


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
        as->Asm("ldx "+  getValue(node)+"+1,x");
    }
}

void CodeGen6809::LoadVariable(QSharedPointer<Node> node)
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


void CodeGen6809::LoadVariable(QSharedPointer<NodeVar> node) {

    /*        if (as->m_symTab->Lookup(value)==nullptr)
                ErrorHandler::e.Error("Could not find variable '" +value +"' for storing.",m_op.m_lineNumber);
    */
    TokenType::Type t = as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber)->getTokenType();
    QString type = as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber)->m_type;
    if (as->m_symTab->m_records.contains(type))
        t = TokenType::ADDRESS; // Working with a CLASS directly (not pointer)

    as->Comment("LoadVariable");


    if (node->isStackVariable()) {
        LoadStackVariable(node);
        return;
    }


    //    qDebug() << "LoadVariable: "<<pp;
/*    if (t==TokenType::ADDRESS || t==TokenType::STRING || t==TokenType::CSTRING || t==TokenType::INCBIN) {
        ErrorHandler::e.Error("Not implemented yet", node->m_op.m_lineNumber);
//        LoadByteArray(node);
        return;
    }*/
    if (t==TokenType::POINTER || t==TokenType::ADDRESS || t==TokenType::STRING || t==TokenType::CSTRING || t==TokenType::INCBIN) {
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


void CodeGen6809::LoadVariable(QSharedPointer<NodeNumber>node)
{
    as->ClearTerm();
    //   qDebug() << "OAD NUMBER";
    if (node->isReference()) {
        as->ClearTerm();
        if (node->getValue(as).startsWith("#"))
        as->Asm("ldx "+node->getValue(as));
        else
        as->Asm("ldx #"+node->getValue(as));
        return;
    }
    if (node->isWord(as)) {
        as->Asm("ldx "+node->getValue(as));
        return;
    }

    as->Asm("lda "+node->getValue(as));
    as->Term();
}

void CodeGen6809::LoadVariable(QSharedPointer<NodeProcedure> node)
{
    as->Asm("ldx #"+node->m_procedure->m_procName);
}


void CodeGen6809::StoreVariable(QSharedPointer<NodeVar> node) {
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
                as->Asm("stx " + getValue(node) + "+"+ QString::number(i));

            }
            else {
//                if (node->m_writeType==TokenType::BYTE)
                as->Asm("sta " + getValue(node) + "+"+ getValue(node->m_expr));
            }
            //                as->Asm("tya");
            return;
        }
        else {
            //if regular array
            QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(node->m_expr);
            as->Comment("Storing to a pointer");
            //            as->Comment("Writetype: " +TokenType::getType(node->m_writeType));


            as->ClearTerm();
            // get index
            if (node->m_expr->isPureNumeric()) {
                as->Comment("Index is constant number optimisation");
                if (!node->isPointer(as) || (node->isAddress() && node->getValue(as).startsWith("$")))
                    as->Asm("ldx #"+node->getValue(as));
                else
                    as->Asm("ldx "+node->getValue(as));

                as->Asm("sta "+QString::number(node->m_expr->getValueAsInt(as)*node->getArrayDataSize(as))+",x");
                return;
            }
            as->Asm("pshs a");

            LoadIndex(node->m_expr, node->getArrayType(as));
//            as->Asm(";is address ???");
            if (!node->isPointer(as) || (node->isAddress() && node->getValue(as).startsWith("$")))
                as->Asm("ldx #"+node->getValue(as));
            else
                as->Asm("ldx "+node->getValue(as));
            as->Asm("leax d,x");
            as->Asm("puls a");
            as->Asm("sta ,x");


            /*            if (node->getArrayType(as)==TokenType::INTEGER || node->m_writeType==TokenType::INTEGER) {
                if (pa=="") {
                    as->Asm(tya);
                    as->Asm("sta " + getValue(node)+"+1,"+ secondReg);
                    return;
                }
                as->Asm("in"+secondReg);

                as->Asm(tya);

                as->Asm("sta " +pa + getValue(node)+pb+","+ secondReg);

            }
            */
        }
        return;
    }
    else {
        TokenType::Type t = as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber)->getTokenType();
        if (t == TokenType::BYTE || t == TokenType::ADDRESS || t == TokenType::BOOLEAN) {
            as->Asm("sta " + getValue(node));
            return;
        }
        if (t == TokenType::ADDRESS) {

            as->Asm("sta " + getValue(node));
            return;
        }
        if (t == TokenType::INTEGER || node->m_writeType==TokenType::INTEGER) {

            as->Asm("stx " + getValue(node));
            return;
        }
        if (t == TokenType::POINTER || node->m_writeType==TokenType::INTEGER) {

            as->Asm("stx " + getValue(node));
            return;
        }
        ErrorHandler::e.Error("Unable to assign variable : "+getValue(node)+ " of type "+node->getTypeText(as),node->m_op.m_lineNumber);

    }




}


void CodeGen6809::LoadIndex(QSharedPointer<Node> node, TokenType::Type arrayType)
{
    /*    if (!node->isWord(as)) {
        as->Term();
        as->Asm("tfr a,b");
        as->Asm("lda #0");
    }*/
    as->ClearTerm();
    as->Comment("Loading index");
    if (node->isPure()) {
//        as->Comment(";fix");
        QString ref = "";
        if (node->isReference())
            ref="#";
        if (node->isByte(as)) {
            as->Asm("ldb "+ref+node->getValue(as));
            as->Asm("lda #0");
        }
        else
            as->Asm("ldd "+ref+node->getValue(as));
    }
    else {
        node->setForceType(TokenType::INTEGER);
        if (!node->isPure())
            dstack++;
        node->Accept(this);
        if (!node->isPure())
            dstack--;
        as->Term();
        as->Asm("tfr x,d");
    }
    if (arrayType==TokenType::INTEGER) {
        as->Comment("Multiply by two to lookup integer");
        as->Asm("aslb");
        as->Asm("rola");
//        as->Asm("addd y");
    }
    as->Comment("End load index");

}

QString CodeGen6809::getJmp(bool isOffPage) {
    return "lbra";
}

void CodeGen6809::PushD() {
    //        if (dstack!=0)
    as->Asm("pshs d");
    dstack++;
}

void CodeGen6809::PopD() {
    //      if (dstack!=0)
    as->Asm("puls d");
    dstack--;
}

bool CodeGen6809::UseBlocks() { return false;
}

void CodeGen6809::LoadStackVariable(QSharedPointer<NodeVar> node) {
    as->Asm("tsx");
    //    as->Asm("txa");
    as->Asm("lda $103+"+QString::number(node->getStackShift()+m_stackShift)+",x");
    //    qDebug() << node->getStackShift()+m_stackShift;

}


void CodeGen6809::AssignString(QSharedPointer<NodeAssign> node) {

    bool isPointer = node->m_left->isPointer(as);
    QSharedPointer<NodeString> right = qSharedPointerDynamicCast<NodeString>(node->m_right);
    QSharedPointer<NodeVar> left = qSharedPointerDynamicCast<NodeVar>(node->m_left);

    QString lblCpy=as->NewLabel("stringassigncpy");


    QString str = DefineTempString(right);

    if (isPointer || left->isStringList(as)) {
            as->Asm("ldx #"+str);
            StoreVariable(left);
    }
    else {
        as->Asm("ldx #"+str);
        as->Asm("ldy #"+getValue(left));
        as->Label(lblCpy);
        as->Asm("lda ,x+");
        as->Asm("sta ,y+");
        as->Asm("cmpa #0 ;keep");  // ask post optimiser to not remove this
        as->Asm("bne " + lblCpy);
    }
    //  as->PopLabel("stringassign");
    as->PopLabel("stringassignstr");
    as->PopLabel("stringassigncpy");

}

bool CodeGen6809::AssignPointer(QSharedPointer<NodeAssign> node) {
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

        QString ref="";
        if (node->m_right->isReference())
            ref="#";
        as->Asm("ldx "+ref + getValue(node->m_right));
        as->Asm("stx "+ getValue(aVar));

        return true;
    }
    // Generic expression

    node->m_right->forceWord();
    node->m_right->setForceType(TokenType::INTEGER);
    as->Term();
    node->m_right->Accept(this);
    as->Term();
    as->Asm("stx " + getValue(aVar));

    return true;
    //     ErrorHandler::e.Error("Right-hand side must be constant or address", node->m_op.m_lineNumber);

}

bool CodeGen6809::isSimpleAeqAOpB(QSharedPointer<NodeVar> var, QSharedPointer<NodeAssign> node) {
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
//    as->Comment("binop");
    as->BinOP(rterm->m_op.m_type);
  //  as->Comment("accept");
    as->Term(rterm->m_right->getValue(as));
    //rterm->m_right->Accept(this);
    as->Term();
    as->Comment("store");
    StoreVariable(var);
    return true;
}

bool CodeGen6809::isSimpleAeqAOpB16Bit(QSharedPointer<NodeVar> var, QSharedPointer<NodeAssign> node)
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

//    if (var->isWord(as) && rterm->m_right->isPure()) {

        as->Comment(" integer a:=a+b");
        BinOp16(node->m_right);
/*        LoadIndex(rterm->m_right,TokenType::BYTE);
        LoadVariable(var);
        as->Asm("leay d,y");*/
        StoreVariable(var);
        return true;

    return false;

}





bool CodeGen6809::IsSimpleIncDec(QSharedPointer<NodeAssign> node) {
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

//    as->Comment("Test Inc dec D");


    if (!var->hasArrayIndex() && !rvar->hasArrayIndex()) {
//        as->Comment(" inc dec here");
        if (var->isWord(as)){
            as->Comment("Inc/dec integer");
            LoadVariable(var);
            as->Asm("inc d");
            StoreVariable(var);
        }
        else
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



void CodeGen6809::dispatch(QSharedPointer<NodeRepeatUntil> node)
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
            as->Asm("lbeq " + lblDone);
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

void CodeGen6809::dispatch(QSharedPointer<NodeComment> node)
{
    node->DispatchConstructor(as,this);

    if (node->m_comment!="") {
        //        as->Comment("A COMMENT");

        as->Comment(node->m_comment.replace("\n","\n; "));
    }

}

QString CodeGen6809::resolveTemporaryClassPointer(QString name, int mul, int& res)
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





void CodeGen6809::HackPointer(Assembler *as, QSharedPointer<Node> n)
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

void CodeGen6809::PopLostStack(int num)
{
    if (num==0) return;
    as->Asm("tax");
    for (int i=0;i<num;i++)
        as->Asm("pla");
    as->Asm("txa");
}

void CodeGen6809::AssignFromRegister(QSharedPointer<NodeAssign> node)
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

void CodeGen6809::AssignToRegister(QSharedPointer<NodeAssign> node)
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

void CodeGen6809::OptimizeBinaryClause(QSharedPointer<Node> node, Assembler* as)
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

void CodeGen6809::dispatch(QSharedPointer<NodeUnaryOp> node)
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


void CodeGen6809::CompareAndJumpIfNotEqualAndIncrementCounter(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive)
{
    /*    if (!isOffPage)
        SmallLoop(node,qSharedPointerDynamicCast<NodeVar>(nVar->m_left), inclusive);
    else
        LargeLoop(node,qSharedPointerDynamicCast<NodeVar>(nVar->m_left), inclusive);

*/

    //if (!isOffPage)
    {
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


void CodeGen6809::CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QString lblJump, bool isOffPage)
{
    if (!nodeA->isPure())
        ErrorHandler::e.Error("Compare must be pure variable",nodeA->m_op.m_lineNumber);
    //        if (nodeA->isWord(as))
    //          ErrorHandler::e.Error("Integer compares not supported yet on the 6809",nodeA->m_op.m_lineNumber);
    if (nodeA->isWord(as))
        nodeB->setForceType(TokenType::INTEGER);

    as->ClearTerm();
    nodeB->Accept(this);
    as->Term();
    if (!nodeA->isWord(as)) {
        as->Asm("cmpa " + nodeA->getValue(as) );
        as->Asm("lbne " +lblJump);
       }
    else
     {
        as->Asm("cmpx " + nodeA->getValue(as) );
        as->Asm("lbne " +lblJump);

    }
    return;
}



/*
bool CodeGen6809::StoreVariableSimplified(QSharedPointer<NodeAssign> assignNode)
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
*/
void CodeGen6809::Cast(TokenType::Type from, TokenType::Type to)
{
    //    qDebug() <<"Cast " <<TokenType::getType(from) << " " << TokenType::getType(to);
    if (from==to)
        return;


    if (from==TokenType::BYTE && to == TokenType::INTEGER) {
        as->Comment("Casting from byte to integer");
        as->Asm("ldb #0");
        as->Asm("exg b,a");
        as->Asm("tfr d,x");
    }
    if (from==TokenType::INTEGER && to == TokenType::BYTE) {
        as->Comment("Casting from integer to byte");
        as->Asm("tfr x,d");
        as->Asm("exg a,b");
    }

}

void CodeGen6809::Cast(TokenType::Type from, TokenType::Type to, TokenType::Type writeType)
{
    if (from==to && to==writeType)
        return;
    if (from==TokenType::BYTE && to == TokenType::INTEGER) {
        if (writeType==TokenType::INTEGER) {
            as->Comment("Casting from byte to integer to integer");
        }
        if (writeType==TokenType::BYTE) {
            as->Comment("Casting from byte to integer to byte");
            //            as->Asm("ld l,a");
            //          as->Asm("ld h,0");
        }
    }
    if (from==TokenType::INTEGER && to == TokenType::BYTE) {
        if (writeType==TokenType::BYTE) {
            as->Comment("Casting from integer to byte");
        }
        if (writeType==TokenType::INTEGER) {
            as->Comment("Casting from integer to byte to integer");
            as->Asm("tfr x,d");
            as->Asm("lda #0");
            as->Asm("tfr d,x");
        }
    }

}

bool CodeGen6809::StoreStackParameter(QSharedPointer<NodeAssign> n)
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
