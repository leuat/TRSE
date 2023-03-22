#include "codegen_z80.h"


CodeGenZ80::CodeGenZ80()
{
    // Override with Z80 values!
    m_jmp = "jp ";
    m_mov = "ld ";
    m_cmp = "cp ";
    m_jne = "jr nz,";
}

/*
 * Methods than handle all 16 bit SHR and SHL operations
 *
 */
void CodeGenZ80::Handle16bitShift(QSharedPointer<NodeBinOP> node)
{   node->m_left->setLoadType(TokenType::INTEGER);
    node->m_left->Accept(this);
    //    as->Asm("ld l,a");
    //    as->Asm("ld h,0");
    // sorry!
    if (!node->m_right->isPureNumeric()){
        node->m_right->setStoreType(TokenType::BYTE);
        node->m_right->setLoadType(TokenType::BYTE);
        auto skip = as->NewLabel("16_bit_shift_skip");
        if (!node->m_right->isPure())
            as->Asm("push hl");
        node->m_right->Accept(this);
        as->Asm("ld b,a");
        if (!node->m_right->isPure())
            as->Asm("pop hl");
        as->Asm("cp 0");
        as->Asm("jr z,"+skip);

        auto lbl = as->NewLabel("16_bit_shift");
        as->Label(lbl);
        if (node->m_op.m_type == TokenType::SHL) {
            as->Asm("add hl,hl");
        }
        if (node->m_op.m_type == TokenType::SHR) {
            as->Asm("srl h");
            as->Asm("rr l");
        }

        as->Asm("dec b");
        as->Asm("jr nz,"+lbl);
        as->PopLabel("16_bit_shift");
        as->Label(skip);
        as->PopLabel("16_bit_shift_skip");
    }
    //    ErrorHandler::e.Error("Only constant 16-bit shifts are supported", node->m_op.m_lineNumber);


    int val = node->m_right->getValueAsInt(as);
    // Shl: simply add hl N times
    if (node->m_op.m_type == TokenType::SHL) {
        for (int i=0;i<val;i++)
            as->Asm("add hl,hl");
    }
    if (node->m_op.m_type == TokenType::SHR) {
        for (int i=0;i<val;i++) {
            as->Asm("srl h");
            as->Asm("rr l");
        }
    }
}

void CodeGenZ80::AssignString(QSharedPointer<NodeAssign> node) {

    bool isPointer = node->m_left->isPointer(as);
    QSharedPointer<NodeString> right = qSharedPointerDynamicCast<NodeString>(node->m_right);
    QSharedPointer<NodeVar> left = qSharedPointerDynamicCast<NodeVar>(node->m_left);
//    QString str = as->NewLabel("stringassignstr");
    if (as->offPageStack!=0)
        return;
    QString lblCpy=as->NewLabel("stringassigncpy");

    QString str = DefineTempString(right);
    if (isPointer || left->isStringList(as)) {
        as->Asm("ld hl,"+str);
        StoreVariable(left);
        //        LoadPointer(left);
    }
    else {
        // If not a pointer, copy everything
        /*        as->Asm("ld de,"+getValue(left));
        if (left->hasArrayIndex()){

        }
  */

        as->Asm("ld de,"+getValue(left));

        as->Asm("ld hl,"+str);
        as->Label(lblCpy);
        if (isGB())
            as->Asm("ld a,[hl+]");
        else {
            as->Asm("ld a,[hl]");
            as->Asm("inc hl");

        }
        as->Asm("ld [de],a");
        as->Asm("inc de");
        //      as->Asm("dec c");
        as->Asm("cp 0");
        as->Asm("jr nz, "+lblCpy);
    }
    //  as->PopLabel("stringassign");
    as->PopLabel("stringassignstr");
    as->PopLabel("stringassigncpy");

}

QString CodeGenZ80::getJmp(bool isOffPage) {
    if (!isOffPage)
        return "jr";
    return "jp";
}

bool CodeGenZ80::UseBlocks() {
    return (Syntax::s.m_currentSystem->m_system == AbstractSystem::GAMEBOY ||
            Syntax::s.m_currentSystem->m_system == AbstractSystem::SPECTRUM);
}

void CodeGenZ80::Load16bitToHl()
{
    as->Asm("ld a,[hl]");
    as->Asm("ld e,a");
    as->Asm("inc hl");
    as->Asm("ld a,[hl]");
    as->Asm("ld d,a");
    ExDeHl();
}
/*
 *
 * Main method used in for loops. Will increase a counter in nodeA->m_left (from for a:=0 ...)
 * and then compare with the end value, jump if not equal
 */
void CodeGenZ80::CompareAndJumpIfNotEqualAndIncrementCounter(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive)
{

    QString var = nodeA->m_left->getValue(as);
    //  PopX();

    if (nodeA->m_left->isWord(as)) {
        if (!nodeB->isPure())
            ErrorHandler::e.Error("16 bit index counter target must be pure numeric or variable!",nodeB->m_op.m_lineNumber);

        QString ax = "hl";
        if (step!=nullptr) {
            as->Comment("; 16 bit counter with STEP");
            step->setLoadType(TokenType::INTEGER);
            step->Accept(this);
            //            as->Asm("ex de,hl");
            ExDeHl();
            nodeA->m_left->Accept(this);
            as->Asm("add hl,de");

        }
        else {
            as->Comment("; 16 bit counter without STEP");
            nodeA->m_left->Accept(this);
            as->Term();
            as->Asm("inc "+ax);

        }
        as->Asm(m_mov+"["+var+"],"+ax);

        as->Asm("ld c,l");
        as->Asm("ld a,"+nodeB->getValue8bit(as,false));
        if (isInclusive)
            as->Asm("inc a");
        as->Asm(m_cmp+"c");

        if (!isOffPage)
            as->Asm("jr nz,"+lblJump);
        else
            as->Asm("jp nz,"+lblJump);

        as->Asm("ld c,h");
        as->Asm("ld a,"+nodeB->getValue8bit(as,true));
        as->Asm(m_cmp+"c");

        if (!isOffPage)
            as->Asm("jr nz,"+lblJump);
        else
            as->Asm("jp nz,"+lblJump);

        return;
    }

    //    QString var = nodeA->m_left->getValue(as);

    if (!nodeB->isPureNumeric()) {
        as->ClearTerm();
        nodeB->Accept(this);
        as->Term();
        if (isInclusive)
            as->Asm("inc a");
        as->Asm("ld c,a");
    }

    // If step is a number, load it here
    // Post optimizer should take care of stuff
    if (step!=nullptr) {
        step->Accept(this);
        as->Asm("ld d,a");
    }
    //    PushX();
    QString inc = "";
    if (isInclusive) {
        inc = "+1";
        if (step!=nullptr)
            inc="+"+step->getValue(as);
    }
    QString ax = getAx(nodeA->m_left);
    //  PopX();
    as->Asm(m_mov+ax+",["+var+"]");

    if (step==nullptr)
        as->Asm("add "+ax+",1");
    else
        as->Asm("add "+ax+",d");

    as->Asm(m_mov+"["+var+"],"+ax);

    if (!nodeB->isPureNumeric())
        as->Asm(m_cmp+"c");
    else
        as->Asm(m_cmp+nodeB->getValue(as)+inc);


    if (!isOffPage)
        as->Asm("jr nz,"+lblJump);
    else
        as->Asm("jp nz,"+lblJump);
}

void CodeGenZ80::CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QString lblJump, bool isOffPage)
{
    QString var = nodeA->getValue(as);

    if (!nodeB->isPureNumeric()) {
        as->ClearTerm();
        nodeB->Accept(this);
        as->Term();
        as->Asm("ld c,a");
    }


    nodeA->Accept(this);
    as->Term();

    if (!nodeB->isPureNumeric())
        as->Asm(m_cmp+"c");
    else
        as->Asm(m_cmp+nodeB->getValue(as));


    if (!isOffPage)
        as->Asm("jr nz,"+lblJump);
    else
        as->Asm("jp nz,"+lblJump);
}

/*
 * Handles a:=a op b  for 16 bit values
 *
 *
 */
void CodeGenZ80::HandleAeqAopB16bit(QSharedPointer<NodeBinOP> bop, QSharedPointer<NodeVar> var)
{
    as->Comment("16 bit BINOP");
    /*
     * First case : is the RHS of the binary operation pure?
     */
    if (bop->m_right->isPure()) {
        as->Comment("RHS is pure ");
        // a := a + $200 etc
        if (bop->m_right->isPureNumeric() && (bop->m_right->getValueAsInt(as)&0xFF)==0) {
            as->Comment("RHS is pure constant of $100");
            as->Asm("ld b,"+Util::numToHex(bop->m_right->getValueAsInt(as)>>8));
            as->Asm("ld a,[ "+var->value+"+1]");
            as->Asm("add a,b");
            as->Asm("ld [ "+var->value+"+1],a");
            return;

        }
        m_useNext="de";
        bop->m_right->Accept(this);
    }
    else {
        bop->m_right->Accept(this);
        ExDeHl();
    }
    LoadVariable(var); // Load address into hl
    //    as->Comment(";Comment : "+node->m_left->isWord(as)+TokenType::getType(node->m_left->getLoadType()));
    if (!var->isWord(as)) {
        as->Comment(";var is 8-bit, compensate");
        as->Asm("ld l,a");
        as->Asm("ld h,0");
    }
    // Plus is easy!
    if (bop->m_op.m_type==TokenType::PLUS) {
        as->Asm(getPlusMinus(bop->m_op)+" hl,de");
        StoreAddress(var);
        return;
    }
    if (!isGB()) {
        SbcHlDe();
        StoreAddress(var);
        return;

    }
    as->Asm("ld a,l");
    as->Asm("or a");
    as->Asm("sub e");

    as->Asm("ld l,a");
    as->Asm("ld a,h");

    as->Asm("sbc a,d");
    //    as->Asm("ld h,a");


    as->Asm("ld ["+var->value+"],a");
    as->Asm("ld a,l");
    as->Asm("ld ["+var->value+"+1],a");
    //    StoreVariable(var);


}

void CodeGenZ80::LoadVariable(QSharedPointer<NodeProcedure> node)
{
    as->Asm("ld hl,"+node->m_procedure->m_procName);
}

void CodeGenZ80::LoadVariable(QSharedPointer<NodeVar> n)
{
    n->Accept(this);
}

void CodeGenZ80::LoadVariable(QSharedPointer<Node> n)
{
    n->Accept(this);

}

void CodeGenZ80::BinaryClauseInteger(QSharedPointer<Node> node, QString lblSuccess, QString lblFailed, bool offPage)
{

    as->Comment("Binary clause INTEGER: " + node->m_op.getType());
    QString lbl2 = lblFailed;
    QString lbl1 = lblSuccess;

    /*    QString lo1,lo2,hi1,hi2;
    Evaluate16bitExpr(node->m_left,lo1,hi1);
    Evaluate16bitExpr(node->m_right,lo2,hi2);
*/

    // First, check for the special case <>0
    if (node->m_right->isPureNumeric())
        if (node->m_right->getValueAsInt(as)==0)
            if (node->m_op.m_type==TokenType::NOTEQUALS)
            {
                // First check for "while true" stuff
                if (node->m_left->isPureNumeric()) {
                    if (node->m_left->getValueAsInt(as)!=node->m_right->getValueAsInt(as) ){
                        as->Asm("Comparing two non-equal numerical values");
                        as->Asm("jr "+lblFailed);
                        return;
                    }

                }


                as->Comment("Special case for integer<>0");
                node->m_left->setLoadType(TokenType::INTEGER);
                node->m_left->Accept(this);
                as->Term();
                as->Asm("ld a,h");
                as->Asm("or l");
                as->Asm("jr z, "+lblFailed);
                return;

            }

    QString bcs ="bcs ";
    QString bcc ="bcc ";
    if (node->isSigned(as)) {
        ErrorHandler::e.Error("Signed integer comparison on the z80 not implemented. Please bug Leuat!", node->m_op.m_lineNumber);
    }

    QString p = "r";
    if (offPage)
        p="p";
    as->Term();
    if (node->m_right->isPureNumeric())
        node->m_right->setLoadType(TokenType::INTEGER);
    if (node->m_left->isPureNumeric())
        node->m_left->setLoadType(TokenType::INTEGER);

    if (node->m_op.m_type!=TokenType::LESSEQUAL) {
        node->m_right->setLoadType(TokenType::INTEGER);
        node->m_right->Accept(this);
        as->Asm("push hl");
        //        as->Asm("ex de,hl");
        node->m_left->setLoadType(TokenType::INTEGER);
        node->m_left->Accept(this);
        as->Asm("pop de");
        //        as->Asm("sbc hl,de");
        SbcHlDe();
    }

    if (node->m_op.m_type==TokenType::EQUALS)
        as->Asm("j"+p+" nz," + lblFailed);
    if (node->m_op.m_type==TokenType::NOTEQUALS)
        as->Asm("j"+p+" z, " + lblFailed);
    if (node->m_op.m_type==TokenType::LESS) {
        as->Asm("j"+p+" nc," + lblFailed);
        //        as->Asm("j"+p+" z, " + lblFailed);
    }
    if (node->m_op.m_type==TokenType::GREATER) {
        as->Asm("j"+p+" c, " + lblFailed);
        as->Asm("j"+p+" z, " + lblFailed);
    }
    if (node->m_op.m_type==TokenType::GREATEREQUAL) {
        as->Asm("j"+p+" c, " + lblFailed);

    }
    if (node->m_op.m_type==TokenType::LESSEQUAL) {
        node->m_left->Accept(this);
        //        as->Asm("ex de,hl");
        //        ExDeHl();
        as->Asm("push hl");
        node->m_right->Accept(this);
        as->Asm("pop de");
        //        as->Asm("sbc hl,de");
        SbcHlDe();

        as->Asm("j"+p+" c," + lblFailed);
        //      as->Asm("j"+p+" z, " + lblFailed);
        //        as->Asm("j"+p+" z," + lblFailed);

    }


}




void CodeGenZ80::dispatch(QSharedPointer<NodeBinOP>node)
{

    if (node->m_left->isWord(as))
        node->m_right->setLoadType(TokenType::INTEGER);

    if (node->m_right->isWord(as))
        node->m_left->setLoadType(TokenType::INTEGER);

    //    as->Comment("Cast type " + TokenType::getType(node->getStoreType()));
    if (node->getStoreType()==TokenType::INTEGER)
        node->setLoadType(TokenType::INTEGER);


    if (node->isPureNumeric()) {
        as->Asm("ld "+getAx(node)+", " + node->getValue(as) + "; binop is pure numeric");
        return;
    }
    if (node->isPureVariable()) {
        as->Asm("ld "+getAx(node)+", [" + node->getValue(as)+"] ; binop is pure variable");
        return;
    }
    /*    if (!node->m_left->isPure() && node->m_right->isPure()) {
        QSharedPointer<Node> t = node->m_right;
        node->m_right = node->m_left;
        node->m_left = t;
        qDebug() << "SWITCH";
    }
*/
    if (node->m_op.m_type == TokenType::MUL || node->m_op.m_type == TokenType::DIV) {

        if (node->m_op.m_type == TokenType::DIV) {
            //            node->m_right->setLoadType(TokenType::BYTE);
            //            ErrorHandler::e.Error("Generic division is not implemented yet!",  node->m_op.m_lineNumber);
            //            as->Asm("cwd");
            if (node->m_left->isWord(as) && node->m_right->isWord(as)) {

                node->m_right->Accept(this);
                //                as->Asm("ex de,hl");
                ExDeHl();
                if (!node->m_left->isPure())
                    as->Asm("push de");
                node->m_left->Accept(this);
                //                as->Asm("ld ac,hl");
                as->Asm("ld a,h");
                as->Asm("ld c,l");
                if (!node->m_left->isPure())
                    as->Asm("pop de");
                //               as->Asm("clc");
                as->Asm("call div_16x16");
                as->Asm("ld h,a");
                as->Asm("ld l,c");

                //               as->Asm("ld hl,ac");

                return;
            }

            node->m_right->Accept(this);
            as->Asm("ld c,a");
            node->m_left->setLoadType(TokenType::INTEGER);
            node->m_left->Accept(this);
            //               as->Asm("clc");
            as->Asm("call div_16x8");
            as->Asm("ld a,l");
//            node->m_returnType = TokenType::BYTE;
            if (node->getLoadType()==TokenType::NADA)
                 node->m_left->setLoadType(TokenType::NADA);


            return;



        }
        if (node->m_op.m_type == TokenType::MUL) {
            QString skip1 = as->NewLabel("skip1");

            if (node->m_right->getOrgType(as)==TokenType::BYTE ||
                    (node->m_right->getOrgType(as)==TokenType::INTEGER_CONST && node->m_right->getValueAsInt(as)<0x100)) {
                as->Comment("Swapping nodes");
                node->SwapNodes();
            }
            if (node->m_right->getClassvariableType()==TokenType::BYTE && node->m_left->getClassvariableType()==TokenType::INTEGER) {
                as->Comment("Swapping nodes");
                node->SwapNodes();
            }
            node->m_left->setStoreType(TokenType::NADA);
            node->m_left->setLoadType(TokenType::BYTE);
            //node->m_left->setLoadType(TokenType::INTEGER);

            as->Comment("Generic mul");
            // make sure 16 bit number is loaded first
            if (!node->isWord(as) && node->getClassvariableType()!=TokenType::INTEGER) {
                node->m_right->Accept(this);
                as->Asm("ld e,a");
                as->Asm("ld d,0");
                bool isAdvanced = !node->m_left->isPure();
                if (isAdvanced) as->Asm("push de");
                node->m_left->Accept(this);
                as->Asm("ld h,a");
                as->Asm("ld l,0");
                if (isAdvanced) as->Asm("pop de");
                //               as->Asm("clc");
                as->Asm("call mul_8x8");
                as->Asm("ld a,l");

                return;
            }

            //                ErrorHandler::e.Error("NOT IMPLEMENTED YET", node->m_op.m_lineNumber);
            as->Comment("Node is pure : "+QString::number(node->m_right->isPure()));

            node->m_left->Accept(this);
            if (!node->m_right->isPureNumeric())
                as->Asm("push af");
            //            node->m_right->setLoadType(TokenType::BYTE);
            // Push pop de since not pure

            node->m_right->Accept(this);
            ExDeHl();
            //                as->Asm("ld a,l");
            //    if (node->m_left->isWord(as)) {
            //      ExDeHl();
            //   }
            as->Asm("ld hl,0");
            as->Asm("ld c,0");
            if (!node->m_right->isPureNumeric()) {
                as->Asm("pop af");
            }
            as->Asm("call mul_16x8");
            return;
        }
        node->m_left->Accept(this);
        QString bx = getAx(node->m_left);
        PushX();
        node->m_right->Accept(this);

        QString ax = getAx(node->m_right);
        PopX();
        as->BinOP(node->m_op.m_type,node->isSigned(as));

        as->Asm(as->m_term + " " +  ax);
        as->m_term = "";
        return;
    }

    if (node->m_op.m_type == TokenType::SHR || node->m_op.m_type == TokenType::SHL) {
        as->Comment("Node is integer : ");
        if (node->isWord(as)) {
            Handle16bitShift(node);
            return;
        }


        if (!node->m_right->isPureNumeric()) {
            node->m_right->Accept(this);
            as->Asm("ld c,a");
            node->m_left->Accept(this);

            QString lbl = as->NewLabel("bitshift");
            as->Label(lbl);
            if (node->m_op.m_type == TokenType::SHR)
                as->Asm("rrca");
            if (node->m_op.m_type == TokenType::SHL)
                as->Asm("rlca");
            as->Asm("dec c");
            as->Asm("jr nz,"+lbl);
            as->PopLabel("bitshift");
            return;
        }
        node->m_left->Accept(this);

        //            ErrorHandler::e.Error("bit shifting on the Z80 must have constant numeric values!", node->m_op.m_lineNumber);
        int num = node->m_right->getValueAsInt(as);
        if (node->m_op.m_type == TokenType::SHR)
            as->Asm("and "+Util::numToHex(0xFF^(int)(pow(2,num)-1)));
        for (int i=0;i<num;i++) {
            if (node->m_op.m_type == TokenType::SHR)
                as->Asm("rrca");
            if (node->m_op.m_type == TokenType::SHL)
                as->Asm("rlca");
        }
        return;
    }

    // Special case if B-node is simple:
    if (node->m_right->isPure() && node->m_left->isPure() && !node->m_right->isWord(as)) {
        if (node->m_right->isPureVariable())  {
            node->m_right->Accept(this);
            as->Asm("ld b,a");
        }
        else {
            if (node->m_right->isPureNumeric()) {
                // PURE numeric
                as->Comment("RHS is pure numeric optimisation");
                node->m_left->Accept(this);
//                qDebug() << node->m_left->isBool(as) <<  node->m_left->getValue(as);

                as->Asm(getBinaryOperation(node)+" "+node->m_right->getValue(as));
                if (node->m_left->isBool(as)) {
                    as->Asm("and 1");
                }

                return;
            }
            as->Asm("ld b,"+node->m_right->getValue(as));
        }
        node->m_left->Accept(this);
        as->Asm(getBinaryOperation(node)+" b");
        return;
    }

    if (node->m_right->isWord(as) || node->m_right->getStoreType()==TokenType::INTEGER || node->m_left->getClassvariableType()==TokenType::INTEGER || node->m_right->getClassvariableType()==TokenType::INTEGER) {
        node->m_right->setStoreType(TokenType::INTEGER);
        node->m_left->setStoreType(TokenType::INTEGER);
  //      node->m_left->setLoadType(TokenType::INTEGER);
        as->Comment("Generic 16-bit binop");
        //        if (node->m_right->isWord(as))
        //          as->Asm("ld d,0");
        node->m_right->Accept(this);
        ExDeHl();

        bool advanced = !node->m_left->isPure();

        if (advanced) as->Asm("push de");
        node->m_left->Accept(this);
        as->Term();
        if (advanced) as->Asm("pop de");


        if (node->m_op.m_type == TokenType::PLUS) {
            as->Asm("add hl,de");
            return;
        }
        if (node->m_op.m_type == TokenType::MINUS) {
            //            as->Asm("sbc hl,de");
            SbcHlDe();
            return;

        }
        ErrorHandler::e.Error("TRSE currently does not support the following 16-bit binary opreation : "+node->m_op.getType(),node->m_op.m_lineNumber);

        //        as->Asm(getBinaryOperation(node)+" hl,de");

        return;
    }

    node->m_left->Accept(this);
    as->Asm("push af");
    node->m_right->Accept(this);
    as->Asm("ld b,a");
    as->Asm("pop af");

//        as->Comment("RHS is type "+node->m_right->getTypeText(as) + " " + TokenType::getType(node->m_right->m_op.m_type));
    as->Asm(getBinaryOperation(node)+" b");

    if (node->m_left->isBool(as)) {
        as->Asm("and 1");
    }


}



void CodeGenZ80::dispatch(QSharedPointer<NodeBinaryClause> node)
{

}


/*
void CodeGenZ80::dispatch(QSharedPointer<NodeConditional> node)
{

}
*/
/*
void CodeGenZ80::dispatch(QSharedPointer<NodeForLoop> node)
{

}
*/
void CodeGenZ80::dispatch(QSharedPointer<NodeVarDecl> node)
{
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);

    QSharedPointer<Appendix> old = as->m_currentBlock;

    if (t->m_flags.contains("hram")) {
        as->m_currentBlock = as->m_hram;
    }
    if (t->m_flags.contains("wram")) {
        as->m_currentBlock = as->m_wram;
    }

    if (t->m_flags.contains("sprram"))
        as->m_currentBlock = as->m_sprram;

    if (t->m_flags.contains("ram"))
        as->m_currentBlock = as->m_ram;

    if (t->m_flags.contains("bank")) {
        QString bnk = t->m_flags[t->m_flags.indexOf("bank")+1];//Banks always placed +1
        if (!as->m_banks.contains(bnk)) {
            as->m_banks[bnk] = QSharedPointer<Appendix>(new Appendix());
            as->m_banks[bnk]->m_pos = "$4000";
            as->m_banks[bnk]->m_isMainBlock = true;
        }
        as->m_currentBlock = as->m_banks[bnk];
    }
    if (v->m_isGlobal) {
        as->m_currentBlock = nullptr;
        return;
    }


    //    qDebug() << "" <<as->m_currentBlock;
    AbstractCodeGen::dispatch(node);
    //  qDebug() << as->m_currentBlock;
    as->m_currentBlock = old;
}

void CodeGenZ80::dispatch(QSharedPointer<NodeVar> node)
{

    if (m_inlineParameters.contains(node->value)) {
        //      qDebug()<< "INLINE node override : "<< node->value;
        m_inlineParameters[node->value]->Accept(this);
        return;
    }

    TokenType::Type wt = node->getClassvariableType();
    if (node->m_expr!=nullptr) {
        if (node->m_expr->isPureNumeric() && node->m_expr->getValueAsInt(as)==0) {
            as->Asm("; Optimization : zp[0]");
            //            LoadVariable(as,)
            //            as->Asm("ld hl,"+node->getValue(as));
            LoadAddress(node);
            if (node->isReference())
                return;
            as->Asm("ld a,[hl]");
            if ((node->isWord(as) && node->getArrayType(as)==TokenType::INTEGER) && node->m_classvariableType==TokenType::NADA ) {//|| node->getClassvariableType()==TokenType::INTEGER*/) {
                wt = TokenType::INTEGER;
/*                as->Asm("ld e,a");
                as->Asm("inc hl");
                as->Asm("ld a,[hl]");
                as->Asm("ld d,a");
                ExDeHl();*/
                as->Asm("inc hl");
                as->Asm("ld h,[hl]");
                as->Asm("ld l,a");
            }
            if (node->m_classvariableType==TokenType::INTEGER) {//|| node->getClassvariableType()==TokenType::INTEGER*/) {
//                as->Asm("ld e,a");
                as->Asm("inc hl");
                as->Asm("ld h,[hl]");
                as->Asm("ld l,a");
                wt = TokenType::INTEGER;
//                ExDeHl();
            }
            if (node->m_classvariableType==TokenType::NADA) // not in a class
                Cast(node->getArrayType(as), node->getStoreType());
            else
                Cast(wt, node->getStoreType());

            return;
        }
        if (node->m_expr->isPureNumeric()) {
            as->Asm("ld de,"+node->m_expr->getValue(as));
        }
        else {
            node->m_expr->Accept(this);
            /*        if (node->isWord(as))
                as->Asm("rlca"); // *2 for integer arrays
      */
            if (node->m_expr->isWord(as)) {
                as->Term();
                ExDeHl();
            }
            else {
                as->Term();
                as->Asm("ld e,a ; variable is 8-bit");
                as->Asm("ld d,0 ;keep");
            }
        }
        //      as->Asm("ld hl,"+node->getValue(as));
        LoadAddress(node);
        as->Asm("add hl,de");

        as->Comment("TYPETEST : "+TokenType::getType(node->getArrayType(as)) + "  WriteType : "+TokenType::getType(node->m_classvariableType));
        //        if ((node->getArrayType(as)==TokenType::INTEGER || node->getArrayType(as)==TokenType::POINTER ) && node->m_classvariableType==TokenType::NADA)
        if ((node->getArrayType(as)==TokenType::INTEGER || node->getArrayType(as)==TokenType::POINTER ) && node->m_classvariableType==TokenType::NADA)
            as->Asm("add hl,de");

  //      as->Comment(" IS REF " +QString::number(node->isReference())+ "  "+node->getValue(as));
        if (node->isReference())
            return;

        as->Asm("ld a,[hl]");

        //        as->Comment("LoadVar Testing if '"+node->getValue(as)+"' is word : "+QString::number(node->isWord(as)));
        if ((node->getArrayType(as)==TokenType::INTEGER || node->getArrayType(as)==TokenType::POINTER) && node->m_classvariableType==TokenType::NADA) // More complicated: Load integer byte array into de
        {
//            as->Asm("ld e,a");
            as->Asm("inc hl");
            as->Asm("ld h,[hl]");
            as->Asm("ld l,a");
            wt = TokenType::INTEGER;
//            ExDeHl();
        }
        if ( node->m_classvariableType==TokenType::INTEGER) // More complicated: Load integer byte array into de
        {
//            as->Asm("ld e,a");
            as->Asm("inc hl");
            as->Asm("ld h,[hl]");
            as->Asm("ld l,a");
            wt = TokenType::INTEGER;
//            ExDeHl();
        }
        if (node->m_classvariableType==TokenType::NADA) // not in a class
            Cast(node->getArrayType(as), node->getStoreType());
        else
        {
//            if (node->getStoreType()!=TokenType::NADA)
//           as->Comment("WTF going on here: "+TokenType::getType(node->m_classvariableType) + " " +TokenType::getType(node->getStoreType()));

            Cast(wt, node->getStoreType());
        }
        return;



        /*        as->Asm("mov ah,0");
        node->m_expr->Accept(this);
        as->Asm("mov di,ax");
        if (node->getArrayType(as)==TokenType::INTEGER)
            as->Asm("shl di,1 ; Accomodate for word");
        ending = "+di]";*/

    }

    //    if (node->getOrgType(as)!=TokenType::INTEGER) {
    if (!node->isWord(as)) {
        if (node->isPureVariable())
            as->Asm("ld a,["+node->getValue(as)+"]");
        else
            as->Asm("ld a,"+node->getValue(as));

        /*      if (node->getLoadType()==TokenType::INTEGER) {
            as->Comment("; Byte, but forced to be integer");
        }
*/
        Cast(node->getType(as), node->getStoreType());
        return;
    }
    // 16 bit all
    // Word
    as->Comment("Variable is 16-bit");

    if (node->isPureNumeric()) {
        QString hl =getHL();

        as->Asm("ld "+hl+", "+node->getValue(as));
    }
    else
    {

        if (node->getOrgType(as) == TokenType::POINTER) {
            LoadAddress(node);
        }
        else if (node->getOrgType(as)==TokenType::INTEGER) {
            LoadInteger(node);

        }
        else {
            QString hl =getHL();

            if (node->isReference()) {
                //                as->Comment("")
                as->Asm("ld hl,"+node->getValue(as));
                return;

            }

            as->Asm("ld a,["+node->getValue(as)+"]");
            as->Asm("ld "+QString(hl[1])+",a");
            as->Asm("ld "+QString(hl[0])+",0");

            /*            if (Syntax::s.m_currentSystem->m_system==AbstractSystem::GAMEBOY) {
                //                as->Asm("xor a,a");
                as->Asm("ld "+QString(hl[0])+",0");
            }
            else
            {
                as->Asm("ld "+QString(hl[0])+",0");
            }*/
        }
        Cast(node->getType(as), node->getStoreType());


    }
}

void CodeGenZ80::dispatch(QSharedPointer<NodeNumber> node)
{
    QString hl = getHL();

    // fix for  val := someAddress which should be the same as val := someAddress[0];
    if (node->isAddress() && !node->isReference()) {
        as->Asm("ld hl, "+node->getValue(as));
        as->Asm("ld a,[hl]");
        return;
    }


/*    as->Comment("Casttype : "+TokenType::getType(node->getStoreType())) ;
    as->Comment("Forcetype : "+TokenType::getType(node->getLoadType())) ;
    as->Comment("Writetype : "+TokenType::getType(node->getClassvariableType())) ;*/
    if (!node->isWord(as) )
        as->Asm("ld a,"+node->getValue(as) );
    else
        as->Asm("ld "+hl+","+node->getValue(as));


}

void CodeGenZ80::Cast(TokenType::Type from, TokenType::Type to)
{
    //    qDebug() <<"Cast " <<TokenType::getType(from) << " " << TokenType::getType(to);
    if (from==to)
        return;


    if (from==TokenType::BYTE && to == TokenType::INTEGER) {
        as->Comment("Casting from byte to integer");
        as->Asm("ld l,a");
        as->Asm("ld h,0");
    }
    if (from==TokenType::INTEGER && to == TokenType::BYTE) {
        as->Comment("Casting from integer to byte");
        as->Asm("ld a,l");
    }

}
void CodeGenZ80::Cast(TokenType::Type from, TokenType::Type to, TokenType::Type writeType)
{
    if (from==to && to==writeType)
        return;
    if (from==TokenType::BYTE && to == TokenType::INTEGER) {
        if (writeType==TokenType::INTEGER) {
            as->Comment("Casting from byte to integer to integer");
            as->Asm("ld l,a");
            as->Asm("ld h,0");
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
            as->Asm("ld a,l");
        }
        if (writeType==TokenType::INTEGER) {
            as->Comment("Casting from integer to byte to integer");
            as->Asm("ld h,0");
        }
    }

}

void CodeGenZ80::ExDeHl()
{
    if (isGB()) {
        as->Asm("push hl");
        as->Asm("push de");
        as->Asm("pop hl");
        as->Asm("pop de");
    }
    else
        as->Asm("ex de,hl");
}

void CodeGenZ80::SbcHlDe()
{
    if (isGB()) {
        as->Asm("ld a,l");
        as->Asm("or a");
        as->Asm("sub e");

        as->Asm("ld l,a");
        as->Asm("ld a,h");

        as->Asm("sbc a,d");
    }
    else {
        as->Asm("xor a ; clear carry");
        as->Asm("sbc hl,de");
    }

}



void CodeGenZ80::AssignFromRegister(QSharedPointer<NodeAssign> node)
{

}
void CodeGenZ80::AssignToRegister(QSharedPointer<NodeAssign> node)
{

}

void CodeGenZ80::LoadIndex(QSharedPointer<Node> n, TokenType::Type arrayType)
{
    auto node = qSharedPointerDynamicCast<NodeVar>(n);
    node->m_expr->setLoadType(TokenType::INTEGER);

    node->m_expr->Accept(this);
    ExDeHl();
    if (node->isPointer(as))
        LoadPointer(node);
    else
        as->Asm("ld hl,"+node->getValue(as));

    as->Asm("add hl,de");
    if (arrayType==TokenType::INTEGER)
        as->Asm("add hl,de ; integer array");

}

void CodeGenZ80::StoreVariable(QSharedPointer<NodeVar> node)
{
    if (node->hasArrayIndex()) {
        if (node->getArrayType(as)==TokenType::INTEGER) {
            as->Comment("Store to integer array");
            as->Asm("push hl");
            LoadIndex(node,TokenType::INTEGER);
            as->Asm("pop bc");
            as->Asm("ld a,c");
            as->Asm("ld [hl],a");
            as->Asm("ld a,b");
            as->Asm("inc hl");
            as->Asm("ld [hl],a");
        }
        else {
            as->Comment("Store to byte array");
            if (!node->isPure())
                as->Asm("push af");
            LoadIndex(node,TokenType::BYTE);
            if (!node->isPure())
                as->Asm("pop af");
            as->Asm("ld [hl],a");

        }
        return;
    }
    if (node->isWord(as))
        LoadPointer(node);

    else as->Asm(" ld a,["+node->getValue(as)+"]");
}

bool CodeGenZ80::StoreVariableSimplified(QSharedPointer<NodeAssign> node) {

    return false;
}


bool CodeGenZ80::IsSimpleIncDec(QSharedPointer<NodeAssign> node)
{
    // Simple a:=b;
    auto var = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    // Check for a:=a+2; etc
    QSharedPointer<NodeBinOP> bop =  qSharedPointerDynamicCast<NodeBinOP>(node->m_right);

    if (bop!=nullptr && (bop->m_op.m_type==TokenType::PLUS || bop->m_op.m_type==TokenType::MINUS)) {
        //      as->Comment("PREBOP searching for "+var->getValue(as));
        if (bop->ContainsVariable(as,var->getValue(as))) {
            // We are sure that a:=a ....
            // first, check if a:=a + number

            // Handle 16 bit op
            if (bop->isWord(as)) {
                HandleAeqAopB16bit(bop,var);
                return true;
            }


            if (bop->m_right->isPureNumeric()) {
                as->Comment("'a:=a + const'  optimization ");
                //as->Asm(getBinaryOperation(bop) + " ["+var->getValue(as)+"], "+type + " "+bop->m_right->getValue(as));
                as->Asm("ld a,["+var->getValue(as)+"]");
                as->Asm(getBinaryOperation(bop)  +bop->m_right->getValue(as));
                as->Asm("ld ["+var->getValue(as)+"], a");
                return true;
            }
            as->Comment("'a:=a + expression'  optimization ");
            bop->m_right->Accept(this);
            as->Asm("ld b,a");
            as->Asm("ld a,["+var->getValue(as)+"]");
            as->Asm(getBinaryOperation(bop) + " b");
            as->Asm("ld ["+var->getValue(as)+"], a");

            return true;
        }

    }
    return false;
}

bool CodeGenZ80::IsSimpleAssignPointer(QSharedPointer<NodeAssign> node)
{
    auto var = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    if (var==nullptr)
        return false;


    if (var->hasArrayIndex() && var->m_classvariableType==TokenType::INTEGER && !var->isPointer(as)) {
/*        as->Comment("Casttype : "+TokenType::getType(var->getStoreType())) ;
        as->Comment("Forcetype : "+TokenType::getType(var->getLoadType())) ;
        as->Comment("Writetype : "+TokenType::getType(var->getClassvariableType())) ;*/

        if (var->m_expr->isPureNumeric()) {
            node->m_right->setLoadType(TokenType::INTEGER);
            node->m_right->Accept(this);
            as->Asm("ld a,l");
            as->Asm("ld ["+var->getValue(as)+"+"+var->m_expr->getValue(as)+"],a");
            as->Asm("ld a,h");
            as->Asm("ld ["+var->getValue(as)+"+"+var->m_expr->getValue(as)+"+1],a");
            return true;
        }
        else ErrorHandler::e.Error("Storing writetype index not supported yet ",node->m_op.m_lineNumber);
    }


    if ((var->hasArrayIndex() && !var->isWord(as))) {
        // Is an array
        as->Comment("Storing to array  ");
        // Optimization : array[ constant ] := expr

        if (var->m_expr->isPureNumeric()) {
            node->m_right->Accept(this);
            as->Term();

            as->Asm("ld ["+var->getValue(as)+"+"+var->m_expr->getValue(as)+"],a");
            return true;
        }
        // GENERIC expression
        bool rightIsPure = node->m_right->isPure();
        bool isAligned = as->m_symTab->Lookup(var->getValue(as),var->m_op.m_lineNumber)->m_flags.contains("aligned");
        //        qDebug() << "IS ALIGNED "<< isAligned;

        if (!rightIsPure) {
            node->m_right->Accept(this);
            as->Asm("push af");
        }
        var->m_expr->Accept(this);
        if (isAligned) {
            as->Asm("ld hl," + var->getValue(as));
            as->Asm("add a,l");
            as->Asm("ld l,a");
        }
        else {
            //            as->Asm("; shift in de");
            if (var->m_expr->isWord(as)){
                ExDeHl();
            }
            else {
                as->Asm("ld e,a");
                as->Asm("ld d,0");
            }
            as->Asm("ld hl," + var->getValue(as));
            as->Asm("add hl,de");
        }
        if (!rightIsPure)
            as->Asm("pop af");
        else
            node->m_right->Accept(this);

        as->Asm("ld [hl],a ");
        return true;
    }


    if (node->m_right->isPureNumeric() && !var->isWord(as)) {
        //        if (node->isWord())
        as->Asm("ld "+getAx(node) + ", "+node->m_right->getValue(as));
        as->Asm("ld ["+var->getValue(as)+ "], "+getAx(node));
        return true;
    }

    return false;

}

void CodeGenZ80::GenericAssign(QSharedPointer<NodeAssign> node)
{
    auto var = qSharedPointerDynamicCast<NodeVar>(node->m_left);

    //    if (qSharedPointerDynamicCast<NodeNumber>(node->m_left)!=nullptr) {
    if (var->value.startsWith("$")) {
        node->m_right->Accept(this);
        as->Term();
        as->Asm("ld ["+node->m_left->getValue(as)+"],a");
        return;

    }




    as->Comment("generic assign ");
    QString name = var->getValue(as);
    if (var->isWord(as)  ||node->m_right->getClassvariableType()==TokenType::INTEGER) {
        node->m_right->setLoadType(TokenType::INTEGER);

        node->m_right->Accept(this);
        as->Term();

        if (var->hasArrayIndex())  { // Storing in 16 bit array index

            as->Comment("Storing in 16-bit array index");
            as->Asm("push hl");
            LoadAddress(var);
            if (var->m_expr->isPureNumeric()) {
                if (var->m_expr->getValueAsInt(as)!=0) {
                    as->Asm("ld de,"+Util::numToHex(var->m_expr->getValueAsInt(as)*2));
                    as->Asm("add hl,de");
                }
            }
            else {
                var->m_expr->Accept(this);
                as->Asm("ld d,0");
                as->Asm("ld e,a");
                as->Asm("add hl,de");
                as->Asm("add hl,de");
            }
            as->Asm("pop de");
            as->Asm("ld a,e");
            as->Asm("ld [hl],a");
            as->Asm("ld a,d");
            as->Asm("inc hl");
            as->Asm("ld [hl],a");
            return;
        }


//        as->Comment("Integer assignment ");
        LoadPointer(var);
        return;
    }
    // special case : byte := integer
    if (node->m_right->isPureVariable())
//    if (node->m_right->isWord(as))
        if (node->m_left->getOrgType(as)==TokenType::BYTE)
            if (!node->m_right->hasArrayIndex())
                if (!node->m_right->hasArrayIndex()) {
                    // simple byte := integer;
                    node->m_right->setStoreType(TokenType::NONE);
                    as->Asm("ld a,["+node->m_right->getValue(as)+"]");
                    as->Asm("ld ["+name + "], "+getAx(node->m_left));
                    return;

                }
//    as->Comment(";gen");
    node->m_right->Accept(this);


    if (node->m_left->hasArrayIndex()) {
        as->Asm("ld [hl], a");
        return;
    }
    if (node->m_right->isWord(as) && !node->m_left->isWord(as)) {
        as->Asm("ld a,l ");
    }
    //    as->Comment(TokenType::getType(node->m_left->getType(as)));
    Cast(node->m_left->getType(as), node->m_right->getStoreType());
    as->Asm("ld ["+name + "], "+getAx(node->m_left));
}

void CodeGenZ80::AssignVariable(QSharedPointer<NodeAssign> node)
{

    AbstractCodeGen::AssignVariable(node);
}

void CodeGenZ80::LoadPointer(QSharedPointer<Node> node)
{
    QString name = getValue(node);
    as->Comment("Loading pointer");
    if (!isGB())
        as->Asm("ld ["+name+"],hl");
    else {
        as->Asm("ld a,h");
        as->Asm("ld ["+name+"],a");
        as->Asm("ld a,l");
        as->Asm("ld ["+name+"+1],a");
    }

}

void CodeGenZ80::LoadPointerToHl(QSharedPointer<Node> node)
{
    QString name = getValue(node);
    as->Comment("Loading pointer");
    if (!isGB()) {
        as->Asm("ld hl,["+name+"]");
        if (node->getOrgType(as)==TokenType::BYTE || node->getOrgType(as)==TokenType::BOOLEAN)
            as->Asm("ld h,0 ; variable is byte");
    }
    else {
        as->Asm("ld a,["+name+"]");
        as->Asm("ld h,a");
        as->Asm("ld a,["+name+"+1]");
        as->Asm("ld l,a");
    }

}



/*
void CodeGenZ80::AssignVariable(QSharedPointer<NodeAssign> node)
{

    if (node->m_left->isWord(as)) {
        node->m_right->setLoadType(TokenType::INTEGER);
    }
    as->ClearTerm();

    if (qSharedPointerDynamicCast<NodeString>(node->m_right))
    {
        AssignString(node);
        return;
    }


    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    as->ClearTerm();

    // Think about this one... numbers
    if (qSharedPointerDynamicCast<NodeNumber>(node->m_left)!=nullptr) {
        node->m_right->Accept(this);
        as->Term();
        as->Asm("ld ["+node->m_left->getValue(as)+"],a");
        return;

    }


    if (var->isPointer(as)) {
        AssignPointer(node);
        return;
    }

    if (var->hasArrayIndex() && !var->isWord(as)) {
        // Is an array

        // Optimization : array[ constant ] := expr
        if (var->m_expr->isPureNumeric()) {
            node->m_right->Accept(this);
            as->Term();

            as->Asm("ld [+"+var->getValue(as)+"+"+var->m_expr->getValue(as)+"],a");
            return;
        }
        // GENERIC expression

        bool rightIsPure = node->m_right->isPure();
        bool isAligned = as->m_symTab->Lookup(var->getValue(as),var->m_op.m_lineNumber)->m_flags.contains("aligned");
//        qDebug() << "IS ALIGNED "<< isAligned;

        if (!rightIsPure) {
            node->m_right->Accept(this);
            as->Asm("push af");
        }
        var->m_expr->Accept(this);
        if (isAligned) {
            as->Asm("ld hl," + var->getValue(as));
            as->Asm("add a,l");
            as->Asm("ld l,a");
        }
        else {
            as->Asm("ld e,a");
            as->Asm("ld d,0");
            as->Asm("ld hl," + var->getValue(as));
            as->Asm("add hl,de");
        }
        if (!rightIsPure)
            as->Asm("pop af");
        else
            node->m_right->Accept(this);

        as->Asm("ld [hl],a");
        return;
    }

    //    if (var->getValue())
    // Simple a:=b;
    QString type ="byte";
    if (var->isWord(as))
        type = "word";

    if (node->m_right->isPureNumeric() && !var->isWord(as)) {
        as->Asm("ld "+getAx(node) + ", "+node->m_right->getValue(as));
        as->Asm("ld ["+var->getValue(as)+ "], "+getAx(node));
        return;
    }
    // Check for a:=a+2;
    QSharedPointer<NodeBinOP> bop =  qSharedPointerDynamicCast<NodeBinOP>(node->m_right);
    // as->Comment("Testing for : a:=a+ expr " + QString::number(bop!=nullptr));
    // if (bop!=nullptr)
    //  as->Comment(TokenType::getType(bop->getType(as)));
    if (bop!=nullptr && (bop->m_op.m_type==TokenType::PLUS || bop->m_op.m_type==TokenType::MINUS)) {
        //      as->Comment("PREBOP searching for "+var->getValue(as));
        if (bop->ContainsVariable(as,var->getValue(as))) {
            // We are sure that a:=a ....
            // first, check if a:=a + number
            //            as->Comment("In BOP");

            // Handle 16 bit op
            if (bop->isWord(as)) {
                HandleAeqAopB16bit(bop,var);
                return;
            }


            if (bop->m_right->isPureNumeric()) {
                as->Comment("'a:=a + const'  optimization ");
                //as->Asm(getBinaryOperation(bop) + " ["+var->getValue(as)+"], "+type + " "+bop->m_right->getValue(as));
                as->Asm("ld a,["+var->getValue(as)+"]");
                as->Asm(getBinaryOperation(bop)  +bop->m_right->getValue(as));
                as->Asm("ld ["+var->getValue(as)+"], a");
                return;
            }
            as->Comment("'a:=a + expression'  optimization ");
            bop->m_right->Accept(this);
            as->Asm("ld b,a");
            as->Asm("ld a,["+var->getValue(as)+"]");
            as->Asm(getBinaryOperation(bop) + " b");
            as->Asm("ld ["+var->getValue(as)+"], a");

            return;
        }
        // Check for a:=a+

    }

    as->Comment("generic assign ");
    QString name = var->getValue(as);
    if (var->isWord(as)) {
        node->m_right->setLoadType(TokenType::INTEGER);

        node->m_right->Accept(this);
        if (var->hasArrayIndex())  { // Storing in 16 bit array index

            as->Comment("Storing in 16-bit array index");
            as->Asm("push hl");
            LoadAddress(var);
            var->m_expr->Accept(this);
            as->Asm("ld d,0");
            as->Asm("ld e,a");
            as->Asm("add hl,de");
            as->Asm("add hl,de");
            as->Asm("pop de");
            as->Asm("ld a,e");
            as->Asm("ld [hl],a");
            as->Asm("ld a,d");
            as->Asm("inc hl");
            as->Asm("ld [hl],a");
            return;
        }


        as->Comment("Integer assignment ");
        if (!isGB())
            as->Asm("ld ["+name+"],hl");
        else {
            as->Asm("ld a,h");
            as->Asm("ld ["+name+"+1],a");
            as->Asm("ld a,l");
            as->Asm("ld ["+name+"],a");
        }
        return;
    }
    node->m_right->Accept(this);


    if (node->m_left->hasArrayIndex()) {
        as->Asm("ld [hl], a");
        return;
    }
    if (node->m_right->isWord(as) && !node->m_left->isWord(as)) {
        as->Asm("ld a,l ; word assigned to byte");
    }

    as->Asm("ld ["+name + "], "+getAx(node->m_left));
    return;
}
*/
QString CodeGenZ80::getAx(QSharedPointer<Node> n) {
    QString a = m_regs[m_lvl];
    if (n->getLoadType()==TokenType::INTEGER)
        return "hl";
    if (n->getType(as)==TokenType::INTEGER)
        return "hl";
    if (n->getType(as)==TokenType::ADDRESS)
        return "hl";
    if (n->getType(as)==TokenType::INTEGER_CONST)
        if (n->isWord(as))
            return "hl";
    //        if (n->isPureNumeric())
    //          if (n->getValue()
    return a;

}

QString CodeGenZ80::getA(QSharedPointer<Node> n) {
    QString a = m_regs[m_lvl];
    return a;

}

QString CodeGenZ80::getX86Value( QSharedPointer<Node> n) {
    if (n->isPureVariable())
        return "["+n->getValue(as)+"]";
    return n->getValue(as);

}

QString CodeGenZ80::getBinaryOperation(QSharedPointer<NodeBinOP> bop) {

    QString add = "";
    //    if (Syntax::s.m_currentSystem->m_processor==AbstractSystem::GBZ80)
    add = " a,";

    if (bop->m_op.m_type == TokenType::PLUS)
        return "add "+add;
    if (bop->m_op.m_type == TokenType::MINUS)
        return "sub ";//+add;
    if (bop->m_op.m_type == TokenType::BITAND)
        return "and ";
    if (bop->m_op.m_type == TokenType::BITOR)
        return "or ";
    if (bop->m_op.m_type == TokenType::XOR)
        return "xor ";
    /*    if (bop->m_op.m_type == TokenType::DIV)
        return "div";
    if (bop->m_op.m_type == TokenType::MUL)
        return "mul";*/
    ErrorHandler::e.Error("Unknown binary operation : " + bop->m_op.getType(),bop->m_op.m_lineNumber);
    return "UNKNOWN BINARY OPERATION";
}

QString CodeGenZ80::LoadAddress(QSharedPointer<Node> n)
{
    QString hl =getHL();
    if (n->isPointer(as)) {
        if (isGB()) {
            as->Asm("ld a,[" +n->getValue(as)+"]");
            as->Asm("ld "+QString(hl[0])+",a");
            as->Asm("ld a,[" +n->getValue(as)+"+1]");
            as->Asm("ld "+QString(hl[1])+",a");


        }
        else
            //            if (n->isReference())
            //              as->Asm("ld "+hl+","+n->getValue(as)+"");
            //    else
            as->Asm("ld "+hl+",["+n->getValue(as)+"]");

    }
    else as->Asm("ld "+hl+"," +n->getValue(as));
    return "";
}


void CodeGenZ80::LoadInteger(QSharedPointer<Node> n)
{
    QString hl =getHL();

    if (isGB()) {
        as->Asm("ld a,[" +n->getValue(as)+"]");
        as->Asm("ld "+QString(hl[0])+",a");
        as->Asm("ld a,[" +n->getValue(as)+"+1]");
        as->Asm("ld "+QString(hl[1])+",a");
    }
    else
        as->Asm("ld "+hl+",["+n->getValue(as)+"]");
}


void CodeGenZ80::StoreAddress(QSharedPointer<Node> n)
{
    QString hl =getHL();
    if (!n->isWord(as)) {
        as->Comment("Stored address is byte, not var: compensate");
        as->Asm("ld a,l");
        as->Asm("ld ["+n->getValue(as)+"],a");
        return;
    }
    if ( isGB()) {
        as->Asm("ld a,"+QString(hl[0])+"");
        as->Asm("ld ["+n->getValue(as)+"], a");
        as->Asm("ld a,"+QString(hl[1])+"");
        as->Asm("ld ["+n->getValue(as)+"+1], a");

    }
    else
        as->Asm("ld ["+n->getValue(as)+"],"+hl);
}

QString CodeGenZ80::getHL() {
    QString hl ="hl";
    if (m_useNext!="")
        hl = m_useNext;
    m_useNext="";
    return hl;
}

void CodeGenZ80::BuildConditional(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool offPage)
{

    as->Comment("Binary clause core: " + node->m_op.getType());
    //    as->Asm("pha"); // Push that baby

    if (node->isWord(as) || node->getClassvariableType()==TokenType::INTEGER) {
        BinaryClauseInteger(node,lblSuccess, lblFailed, offPage);
        return;
    }

    if (node->m_op.m_type==TokenType::LESSEQUAL) {
        //        as->Comment("LESS equal flipped perform actual flip "+node->m_right->getValue(as) + " " +node->m_left->getValue(as));
        node->SwapNodes();
        //      as->Comment("AFTER  flip "+node->m_right->getValue(as) + " " +node->m_left->getValue(as));
    }
    QString p = "r";
    if (offPage)
        p="p";

    // comparing two non-identical integers
    if (node->m_op.m_type==TokenType::NOTEQUALS)
        if (node->m_right->isPureNumeric())
            if (node->m_left->isPureNumeric()) {
                if (node->m_left->getValueAsInt(as)!=node->m_right->getValueAsInt(as) ){
                    as->Comment("Comparing two non-equal numerical values");
                    as->Asm("j"+p+" "+lblSuccess);
                    return;
                }
                else {
                    as->Comment("Comparing two equal numerical values");
                    as->Asm("j"+p+" "+lblFailed);

                }

            }


    BuildToCmp(node);


    QSharedPointer<NodeConditional> n = qSharedPointerDynamicCast<NodeConditional>(node);

    if (node->m_op.m_type==TokenType::EQUALS)
        as->Asm("j"+p+" nz," + lblFailed);
    if (node->m_op.m_type==TokenType::NOTEQUALS)
        as->Asm("j"+p+" z, " + lblFailed);
    if (node->m_op.m_type==TokenType::LESS) {
        as->Asm("j"+p+" nc," + lblFailed);
        //        as->Asm("j"+p+" z, " + lblFailed);
    }
    if (node->m_op.m_type==TokenType::GREATER) {
        as->Asm("j"+p+" c, " + lblFailed);
        as->Asm("j"+p+" z, " + lblFailed);
    }
    if (node->m_op.m_type==TokenType::GREATEREQUAL) {
        as->Asm("j"+p+" c, " + lblFailed);

    }
    if (node->m_op.m_type==TokenType::LESSEQUAL) {
        //        as->Asm("j"+p+" z, " + lblFailed);
        // Flipped!
        //       as->Comment("LESS equal flipped");
        as->Asm("j"+p+" c," + lblFailed);
    }



}

void CodeGenZ80::BuildToCmp(QSharedPointer<Node> node)
{

    if (node->m_right->isPureNumeric())
    {
        as->Comment("Compare with pure num / var optimization");
        //            TransformVariable(as,"cmp",node->m_left->getValue(as),node->m_right->getValue(as),node->m_left);
        //            TransformVariable(as,"cmp",node->m_left->getValue(as),node->m_right->getValue(as),node->m_left);
        LoadVariable(node->m_left);
        if (node->m_right->getValueAsInt(as)!=0)
            as->Asm("cp " + node->m_right->getValue(as));
        else
            as->Asm("or a"); // Special case for 0
        return;
    }

    as->Comment("Compare two vars optimization");
    if (node->m_right->isPureVariable()) {
        //QString wtf = as->m_regAcc.Get();
        LoadVariable(node->m_right);
        as->Asm("ld b,a");
        LoadVariable(node->m_left);
        //TransformVariable(as,"move",wtf,qSharedPointerDynamicCast<NodeVar>node->m_left);
        //TransformVariable(as,"cmp",wtf,as->m_varStack.pop());
        as->Asm("cp b");

        return;
    }
    /*
            node->m_right->Accept(this);

            as->Asm("cp " + getAx(node->m_right));

            //            TransformVariable(as,"cmp",qSharedPointerDynamicCast<NodeVar>node->m_left,as->m_varStack.pop());
            return;*/


    node->m_right->Accept(this);
    as->Term();
    as->Asm("ld c, a");
    node->m_left->Accept(this);
    as->Term();

    as->Asm("cp c");

    //    TransformVariable(as,"cmp",qSharedPointerDynamicCast<NodeVar>node->m_left, as->m_varStack.pop());

    // Perform a full compare : create a temp variable
    //        QString tmpVar = as->m_regAcc.Get();//as->StoreInTempVar("binary_clause_temp");
    //        node->m_right->Accept(this);
    //      as->Asm("cmp " + tmpVar);
    //      as->PopTempVar();


}

bool CodeGenZ80::AssignPointer(QSharedPointer<NodeAssign> node)
{
    //        qDebug() << "IS POINTER "<<node->m_right->isPure();
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    if (var==nullptr)
        return false;

    if (!var->isPointer(as))
        return false;
    node->VerifyReferences(as);



    /*    if (var->isPointer(as) && var->m_classvariableType==TokenType::INTEGER && var->hasArrayIndex()) {
        as->Comment("Writing an integer class pointer");
        as->ClearTerm();
        node->m_right->setLoadType(TokenType::INTEGER);
        node->m_right->Accept(this);
        as->Term();
        as->Asm("push hl");
        LoadAddress(var);
        as->Asm("push hl");
        var->m_expr->setLoadType(TokenType::INTEGER);
        var->m_expr->Accept(this);
        as->Term();
        as->Asm("pop de");
        as->Asm("add hl,de");
        as->Term();

        as->Asm("pop de");
        as->Asm("ld [hl],e");
        as->Asm("inc hl");
        as->Asm("ld [hl],d");

        return true;
    }
*/

    if (!var->hasArrayIndex()) {

        // P := Address / variable
        if (node->m_right->isPure()) {
            if (node->m_right->isWord(as) && !node->m_right->isReference() && node->m_right->isVariable()) {
                LoadPointerToHl(node->m_right);
                /*                if (isGB()) {
                    as->Asm("ld a, ["+node->m_right->getValue(as)+"]");
                    as->Asm("ld l, a");
                    as->Asm("ld a, ["+node->m_right->getValue(as)+"+1]");
                    as->Asm("ld h, l");

                }
                else
               as->Asm("ld hl, ["+node->m_right->getValue(as)+"]");
               */

            }
            else
                LoadAddress(node->m_right);
            StoreAddress(var);
            return true;
        }
        // First test for the simplest case: p := p  + const
        QSharedPointer<NodeBinOP> bop = qSharedPointerDynamicCast<NodeBinOP>(node->m_right);
        // P:= (expr);
        if (bop!=nullptr) {
            // Pointer := Pointer BOP expr
            if (bop->m_left->getValue(as)==var->getValue(as)) {
                as->Comment(";generic pointer/integer P:=P+(expr) add expression");

                bop->setLoadType(TokenType::INTEGER);
                if (bop->m_right->isPure()) {
                    as->Comment("RHS is pure ");
                    if (bop->m_right->isPureNumeric() && (bop->m_right->getValueAsInt(as)&0xFF)==0) {
                        as->Comment("RHS is pure constant of $100");
                        if (isGB()) {
                            as->Asm("ld b,"+Util::numToHex(bop->m_right->getValueAsInt(as)>>8));
                            as->Asm("ld a,[ "+var->value+"]");
                            as->Asm("add a,b");
                            as->Asm("ld [ "+var->value+"],a");
                        }
                        else {
                            as->Asm("ld b,"+Util::numToHex(bop->m_right->getValueAsInt(as)>>8));
                            as->Asm("ld a,[ "+var->value+"+1]");
                            as->Asm("add a,b");
                            as->Asm("ld [ "+var->value+"+1],a");

                        }
                        return true;

                    }
                    m_useNext="de";
                    bop->m_right->Accept(this);
                }
                else {
                    as->Comment("Executing 16 bit binop here");
                    bop->Accept(this);
                    /*                    as->Asm("ld d,h");
                    as->Asm("ld e,l");*/

                    /*                    as->Asm("ld e,a");
                    as->Asm("inc hl");
                    as->Asm("ld a,[hl]");
                    as->Asm("ld d,a");
                    as->Asm("ex de,hl");*/
                    StoreAddress(var);
                    return true;

                }
                LoadAddress(var); // Load address into hl
                if (bop->m_op.m_type==TokenType::PLUS) {
                    as->Asm(getPlusMinus(bop->m_op)+" hl,de");
                    StoreAddress(var);
                    return true;
                }

                as->Asm("ld a,l");
                as->Asm("or a");
                as->Asm("sub e");

                as->Asm("ld l,a");
                as->Asm("ld a,h");

                as->Asm("sbc a,d");
                as->Asm("ld h,a");


                StoreAddress(var);


                // DAMN. Is minus.
                /*
                ld a,


                  */
                //                as->Asm(getPlusMinus(bop->m_op)+" hl,de");
                // Store address

                return true;
            }


            //            else ErrorHandler::e.Error("Pointers in GBZ80 TRSE do not support this expression",bop->m_op.m_lineNumber);
        }
        // Generic : Doesn't really work
        as->Comment("Generic assign 16-bit pointer");
        node->m_right->setLoadType(TokenType::INTEGER);
        node->m_right->Accept(this);
        as->Comment("Store 16-bit address");
        StoreAddress(var);
        return true;

        // Generic case ))
    }
    else
    {
        // HAS array index, so P[ index ] := ..
        // Much simpler. set value.
        if (!node->m_right->isPure()) {
            node->m_right->Accept(this);
            if (node->m_right->isWord(as) || node->m_right->getClassvariableType()==TokenType::INTEGER)
                as->Asm("push hl");
            else
                as->Asm("push af");
        }
        LoadAddress(var);
        /*        as->Asm("ld a,[p1]");
        as->Asm("ld h,a");
        as->Asm("ld a,[p1+1]");
        as->Asm("ld l,a");*/
        if (var->m_expr->isPureNumeric() && var->m_expr->getValueAsInt(as)==0) {

        }
        else
        {
            if (var->m_expr->isPureNumeric()) {
                as->Comment("; index is pure number optimization");
                int scale = var->getArrayDataSize(as);
                if (var->m_classvariableType!=TokenType::NADA)
                    scale = 1;
                as->Asm("ld de,"+Util::numToHex(var->m_expr->getValueAsInt(as)*scale));
                as->Asm("add hl,de");
            }
            else {
                if (var->m_expr->isWord(as)) {
                    as->Comment(";general 16-bit expression for the index");
                    //                  as->Asm("ex de,hl ;keep");
                    as->Asm("push hl");
                    var->m_expr->Accept(this);
                    as->Asm("pop de");
                    //                as->Asm("ex de,hl");
                    as->Asm("add hl,de");
                }
                else {
                    as->Comment(";general 8-bit expression for the index");
                    //                as->Asm("xor a");
                    as->Asm("ld d,0");
                    var->m_expr->Accept(this);
                    as->Asm("ld e,a");
                    as->Asm("add hl,de");
                    if (var->getArrayType(as)==TokenType::INTEGER)
                        as->Asm("add hl,de ; integer array");
                }
            }
        }
        as->Comment("Writetype: "+TokenType::getType(node->m_left->getClassvariableType()));
        if (!node->m_right->isPure()) {
            if (node->m_right->isWord(as) || node->m_right->getClassvariableType()==TokenType::INTEGER)
                as->Asm("pop de");
            else
                as->Asm("pop af");
        }
        else

            if (node->m_left->isWord(as) || node->m_left->getClassvariableType()==TokenType::INTEGER) {
                if (node->m_right->isPure()) {
                    as->Comment("Optimization: rhs is integer, but pure");
                    if (node->m_right->isPureVariable()) {
                        as->Asm("push hl");
                        LoadPointerToHl(node->m_right);
                        /*                        if (isGB()) {
                            as->Asm("ld a, ["+node->m_right->getValue(as)+"]");
                            as->Asm("ld l, a");
                            as->Asm("ld a, ["+node->m_right->getValue(as)+"+1]");
                            as->Asm("ld h, l");

                        }
                        else
                        as->Asm("ld hl,["+node->m_right->getValue(as)+"]");*/
                        as->Asm("pop de");
                        ExDeHl();
                    }
                    else // is number
                        as->Asm("ld de,"+Util::numToHex(node->m_right->getValueAsInt(as)));

                    as->Asm("ld a,e");
                    as->Asm("ld [hl],a"); // Store in pointer
                    as->Asm("inc hl");
                    as->Asm("ld a,d");
                    as->Asm("ld [hl],a"); // Store in pointer
                    return true;

                }
                else {
                    as->Asm("push hl");
                    node->m_right->Accept(this);
                    as->Term();
                    as->Asm("ld a,l");
                    as->Asm("pop hl");
                }
            }
            else
                node->m_right->Accept(this);

        if (node->m_right->isWord(as) || node->m_right->getClassvariableType()==TokenType::INTEGER) {
            as->Asm("ld a,e");
            as->Asm("ld [hl],a"); // Store in pointer
            as->Asm("inc hl");
            as->Asm("ld a,d");
            as->Asm("ld [hl],a"); // Store in pointer
        }
        else
            as->Asm("ld [hl],a"); // Store in pointer


        return true;
    }
    ErrorHandler::e.Error("Pointers must be assigned to variables or addresses.",node->m_op.m_lineNumber);
    return false;
}


QString CodeGenZ80::getPlusMinus(Token t) {
    if (t.m_type == TokenType::PLUS) {
        return "add ";
    }
    if (t.m_type == TokenType::MINUS) {
        return "sub ";
    }
    ErrorHandler::e.Error("Only plus / minus are supported for this type of operation.",t.m_lineNumber);
    return "";
}

