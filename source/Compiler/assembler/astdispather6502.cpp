#include "astdispather6502.h"

ASTDispather6502::ASTDispather6502()
{

}

/*
 *
 *  NODE BINOP DISPATCHER
 *
 *  */



void ASTDispather6502::EightBitDiv(Node *node) {

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

void ASTDispather6502::EightBitMul(Node *node) {

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

void ASTDispather6502::HandleGenericBinop16bit(Node *node) {


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
//    node->m_right->forceWord();
    node->m_right->Accept(this);

    // 255 + k - j doesn't work
    as->Term();
    QString lbl = as->StoreInTempVar("rightvarInteger", "word");

    //    as->Asm("sta " +lbl);
//    as->Asm("sty " +lbl+"+1"); // J is stored
    as->Term();

    //as->Variable(v->getValue(as), false);
    //as->Asm("lda " + v->getValue(as) + "+1");
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
        as->Asm("dec  " +lbl+"+1");
    }

    as->Label(lblword);
    as->Asm("sta "+lbl);
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
    as->PopTempVar();
}

void ASTDispather6502::HandleVarBinopB16bit(Node *node) {

/*    if ((node->m_left->isPureNumeric() || node->m_left->isPureVariable()) &&
       (node->m_right->isPureNumeric() || node->m_right->isPureVariable())) {
        as->Comment("16 bit operation rvar & lvar are pure number or variables");

        node->m_right->Accept(this);
        as->Term();
        as->Asm("clc");
        as->Asm("add ");
        lda #40
        clc
        adc zp
        bcc incscreenx52298
        inc zp +1
    incscreenx52298
        sta zp


    }
  */

    as->m_labelStack["wordAdd"].push();
    QString lblword = as->getLabel("wordAdd");

    //QString lbl = as->NewLabel("rightvarInteger");
    //        QString lblJmp = as->NewLabel("jmprightvarInteger");

    NodeVar* v = dynamic_cast<NodeVar*>(node->m_left);

    //as->Asm("jmp " + lblJmp);
    //as->Write(lbl +"\t.word\t0");
    //as->Label(lblJmp);
    as->Comment("HandleVarBinopB16bit");
    as->ClearTerm();
    as->Asm("ldy #0 ; ::HandleVarBinopB16bit 0");

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
    //as->Variable(v->getValue(as), false);
  //  as->Comment(";HEREHERE");
//    exit(1);
    if (v->getType(as)==TokenType::POINTER || (v->m_op.m_type!=TokenType::ADDRESS && !v->m_fake16bit) )
        as->Asm("lda " + v->getValue(as) + "+1");
    else
        as->Asm("lda #>" + v->getValue(as) + "");

    as->BinOP(node->m_op.m_type);
    as->Term(lbl+"+1", true);
    as->Asm("tay");
//    qDebug() << v->getValue(as) << v->m_op.getType();
  //  exit(1);
    if (v->getType(as)==TokenType::POINTER || (v->m_op.m_type!=TokenType::ADDRESS && !v->m_fake16bit) )
        as->Asm("lda "+ v->getValue(as));
    else
        as->Asm("lda #<"+ v->getValue(as));
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

bool ASTDispather6502::HandleSingleAddSub(Node *node) {

    NodeNumber* num = dynamic_cast<NodeNumber*>(node->m_right);
    NodeVar* vnum = dynamic_cast<NodeVar*>(node->m_right);
    NodeVar* var = dynamic_cast<NodeVar*>(node->m_left);
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
            as->Term("lda " + var->getValue(as) + " + " + num->StringValue());
            return true;
        }
        node->m_left->Accept(this);
        as->Term();
        as->BinOP(node->m_op.m_type);
        node->m_right->Accept(this);
        as->Term();
        as->Term(" ; end add / sub var with constant", true);
        return true;
    }


    return false;
}

void ASTDispather6502::HandleMulDiv(Node *node) {

    if (node->m_left->isPureNumeric() && !node->m_right->isPureNumeric() && node->m_op.m_type==TokenType::MUL ) {
        Node* tmp = node->m_left;
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

void ASTDispather6502::RightIsPureNumericMulDiv16bit(Node *node) {
    int val = ((NodeNumber*)node->m_right)->m_val;

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

void ASTDispather6502::HandleShiftLeftRight(NodeBinOP *node)
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

void ASTDispather6502::HandleShiftLeftRightInteger(NodeBinOP *node, bool isSimpleAeqAopB)
{
    // Not yet implemented

    QString varName = "";
    if (!isSimpleAeqAopB) {
        node->m_left->Accept(this);
        as->Term();
        varName = as->StoreInTempVar("tempVarShift","word");
    }
    else
        varName = node->m_left->getValue(as);

//    QString cmd = node->m_op.m_type==TokenType::SHR?"lsr":"asl";
    QString command = "";
    if (node->m_op.m_type==TokenType::SHR) {
        command = "\tlsr " + varName +"+1"+ "\n";
        command += "\tror " + varName+"+0" + "\n";
    }
    else {
        command = "\tasl " + varName +"+0"+ "\n";
        command += "\trol " + varName+"+1" + "\n";
    }
    if (node->m_right->isPureNumeric()) {

        int val = node->m_right->getValueAsInt(as);
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

void ASTDispather6502::Mul16x8(Node *node) {
    as->Comment("Mul 16x8 setup");
    as->Asm("");
    if (node->m_left->getType(as)==TokenType::INTEGER) {

        as->Asm("ldy #0");
        LoadVariable(node->m_left);
//        if (!node->m_left->isWord(as))
        as->Term();
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

void ASTDispather6502::Div16x8(Node *node) {
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




void ASTDispather6502::HandleRestBinOp(Node* node) {
    bool isWord16 = false;
    QString varName="";

    if ( dynamic_cast<const NodeVar*>(node->m_left) != nullptr) {
        NodeVar* v= (NodeVar*)node->m_left;
        varName = v->getValue(as);
        Symbol* s = as->m_symTab->Lookup(varName, node->m_op.m_lineNumber);
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
        NodeVar* v = dynamic_cast<NodeVar*>(node->m_left);
        if (v!=nullptr) {
            HandleVarBinopB16bit(node);
            return;
        }
        HandleGenericBinop16bit(node);
    }

}

void ASTDispather6502::RightIsPureNumericMulDiv8bit(Node *node) {
    int val = ((NodeNumber*)node->m_right)->m_val;
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


void ASTDispather6502::dispatch(NodeBinOP *node)
{

    node->DispatchConstructor(as);

/*    if (node->m_isCollapsed) {
        as->Asm("lda "+node->getValue(as));
        return;
    }
*/
    // First check if both are consants:



    // First, flip such that anything numeric / pure var is to the right
    if (node->m_op.m_type == TokenType::MUL || node->m_op.m_type == TokenType::PLUS)
    if (!(node->m_right->isPureNumeric() || node->m_right->isPureVariable())) {
        if (node->m_left->isPureNumeric() || node->m_left->isPureVariable()) {
            Node* tmp = node->m_right;
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
 *  NODE NUMBER DISPATCHER
 *
 *  */




void ASTDispather6502::dispatch(NodeNumber *node)
{
    node->DispatchConstructor(as);

    QString val = "";

    /*        if (as->m_symTab->m_constants.contains(m_op.m_value)) {
            m_val = as->m_symTab->m_constants[m_op.m_value]->m_value->m_fVal;
        }
*/

//    qDebug() << TokenType::types[node->getType(as)];
    if (node->m_op.m_type==TokenType::BYTE)
        val = "#"+QString::number((int)node->m_val);
    if (node->m_op.m_type==TokenType::INTEGER)
        val = "#"+QString::number((int)node->m_val);
    if (node->m_op.m_type==TokenType::INTEGER_CONST)
        val = "#"+QString::number((int)node->m_val);
    if (node->m_op.m_type==TokenType::ADDRESS) {

        val = "$" + QString::number((int)node->m_val,16);
    }

    if (node->m_forceType==TokenType::INTEGER && node->m_val<=255) {
        as->Asm("ldy #0   ; Force integer assignment, set y = 0 for values lower than 255");
    }


    if (node->m_op.m_type==TokenType::INTEGER_CONST && node->m_val>255) {
        as->Comment("Integer constant assigning");
        int hiBit = ((int)node->m_val)>>8;
        int loBit = ((int)node->m_val)&0xff;
        as->ClearTerm();
        as->Asm("ldy #" + Util::numToHex(hiBit) );
        //            as->Asm("tax");
        as->Asm("lda #" + Util::numToHex(loBit) );
        return;

        //qDebug() << m_op.m_value <<":" << m_val << " : " << hiBit << "  , " << loBit;
        //exit(1);
    }

    // qDebug() << m_op.getType() << " for " << val;;

    if (as->m_term=="")

        as->Term("lda " + val);
    else
        as->Term(val);
}
/*
 *
 *  SMALL NODE DISPATCHERS
 *
 *  */


void ASTDispather6502::dispatch(NodeBuiltinMethod *node)
{
    node->DispatchConstructor(as);
    node->VerifyParams(as);

//    as->PushCounter();
//    qDebug() <<"Dispatcher::builtin" << as->m_tempZeroPointers;

    Methods6502 methods;
    methods.m_node = node;
    methods.Assemble(as,this);
 //   as->PopCounter(node->m_op.m_lineNumber);
}


void ASTDispather6502::dispatch(Node *node)
{
    node->DispatchConstructor(as);

    node->m_currentLineNumber = node->m_op.m_lineNumber;
}



void ASTDispather6502::dispatch(NodeProcedure *node)
{
    node->DispatchConstructor(as);
//    as->PushCounter();
//    int ln = node->m_op.m_lineNumber;
    if (node->m_parameters.count()!=node->m_procedure->m_paramDecl.count())
        ErrorHandler::e.Error("Procedure '" + node->m_procedure->m_procName+"' requires "
                              + QString::number(node->m_procedure->m_paramDecl.count()) +" parameters, not "
                              + QString::number(node->m_parameters.count()) + ".", node->m_op.m_lineNumber);

    for (int i=0; i<node->m_parameters.count();i++) {
        // Assign all variables
        NodeVarDecl* vd = (NodeVarDecl*)node->m_procedure->m_paramDecl[i];
        NodeAssign* na = new NodeAssign(vd->m_varNode, node->m_parameters[i]->m_op, node->m_parameters[i]);
        na->Accept(this);
//        na->Build(as);
    }

    as->Asm("jsr " + node->m_procedure->m_procName);
  //  qDebug() << ln;
//    as->PopCounter(ln);

}

void ASTDispather6502::dispatch(NodeProcedureDecl *node)
{
    node->DispatchConstructor(as);
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
            node->m_curMemoryBlock = new MemoryBlock(pos,pos,MemoryBlock::ARRAY, node->m_blockInfo.m_blockName);
            as->blocks.append(node->m_curMemoryBlock);
        }
    }
    else {
        //node->m_curMemoryBlock=nullptr;
    }



    //MaintainBlocks(as);
    if (node->m_block==nullptr)  // Is builtin procedure
        node->m_block = new NodeBuiltinMethod(node->m_procName, QVector<Node*>(), nullptr);

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
        NodeBlock* b = dynamic_cast<NodeBlock*>(node->m_block);
        if (b!=nullptr)
            b->forceLabel=node->m_procName;
        node->m_block->Accept(this);
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



void ASTDispather6502::dispatch(NodeProgram *node)
{
    node->DispatchConstructor(as);

//    as->EndMemoryBlock();
    NodeBuiltinMethod::m_isInitialized.clear();
    as->Program(node->m_name, node->m_param);
    as->m_source << node->m_initJumps;
    node->m_NodeBlock->m_isMainBlock = true;
    node->m_NodeBlock->Accept(this);

//    qDebug() << as->m_currentBlock;
    as->EndProgram();
}

void ASTDispather6502::dispatch(NodeVarType *node)
{
    node->DispatchConstructor(as);

}
void ASTDispather6502::dispatch(NodeAsm *node)
{
    node->DispatchConstructor(as);

    QStringList txt = node->m_asm.split("\n");
    as->Comment("");
    as->Comment("****** Inline assembler section");
    for (QString t: txt) {
        as->Write(t,0);
    }
    as->Asm("");
}

void ASTDispather6502::dispatch(NodeString *node)
{
    node->DispatchConstructor(as);

    as->String(node->m_val);
}

void ASTDispather6502::dispatch(NodeUnaryOp *node)
{
    node->DispatchConstructor(as);

    NodeNumber* num = dynamic_cast<NodeNumber*>(node->m_right);

    if (num!=nullptr) {
        int s = num->m_val;
        bool isWord = node->m_forceType==TokenType::INTEGER;
        if (node->m_op.m_type==TokenType::MINUS) {
            if (!isWord)
                num->m_val=256-num->m_val;
            else
                    num->m_val=65536-num->m_val;

        }

        num->Accept(this);
        num->m_val = s;
/*        QString ss= num->Build(as);
        num->m_val = s;
*/
    }


}

void ASTDispather6502::dispatch(NodeCompound *node)
{
    node->DispatchConstructor(as);

    as->BeginBlock();
    for (Node* n: node->children)
        n->Accept(this);


    as->EndBlock();
}



void ASTDispather6502::dispatch(NodeBlock *node)
{
    node->DispatchConstructor(as);

    // In case memory block is acive
    //as->EndMemoryBlock();
    int ln = node->m_op.m_lineNumber-1;
    if (ln==0) ln++;
    as->PushBlock(node->m_currentLineNumber);




    bool blockLabel = false;
    bool blockProcedure = false;
    bool hasLabel = false;

    QString label = as->NewLabel("block");


    if (node->m_decl.count()!=0) {
        if (node->m_isMainBlock)
            as->Asm("jmp " + label);
        hasLabel = true;
        //           as->PushBlock(m_decl[0]->m_op.m_lineNumber-1);
    }

/*    if (node->m_isMainBlock) {
        int ret = node->MaintainBlocks(as);
        if (ret==2)
            as->m_currentBlock = nullptr;
    }
*/

    for (Node* n: node->m_decl) {
        // Print label at end of vardecl
        if (dynamic_cast<NodeVarDecl*>(n)==nullptr) {
/*            if (!blockLabel)
                as->Label(label);
            blockLabel = true;
*/
            if (!blockProcedure) // Print label at end of vardecl
            {
                if (n->m_op.m_lineNumber!=0) {
                    //                      as->PopBlock(n->m_op.m_lineNumber);
                    blockProcedure = true;
                    //   qDebug() << "pop" << n->m_op.m_lineNumber << " " << TokenType::getType(n->getType(as));
                }

            }

        }
        //if (dynamic_cast<NodeProcedureDecl*>(n)==nullptr)
        //qDebug() << "VarDeclBuild:" ;
        n->Accept(this);

    }
    as->VarDeclEnds();
    as->PushCounter();

    if (node->m_isMainBlock) {
        int ret = node->MaintainBlocks(as);
        if (ret==2)
            as->m_currentBlock = nullptr;

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

//    qDebug() << QString::number(ln);
    as->PopCounter(ln);
}



/*
 *
 *
 *  NODE VARDECL
 *
 *
 *
 * */


void ASTDispather6502::dispatch(NodeVarDecl *node)
{
    node->DispatchConstructor(as);

    int ret = node->MaintainBlocks(as);

    if (ret==3) node->m_curMemoryBlock = nullptr;
//    qDebug() << "NodeVarDecl new memory block "  << ret;
    if (node->m_curMemoryBlock!=nullptr)
  //      qDebug() << node->m_curMemoryBlock->m_start;
    if (as->m_currentBlock!=nullptr) {
    //    qDebug() <<as->m_currentBlock->m_pos;
        if (node->m_curMemoryBlock==nullptr) {
            bool ok;
            QString p = as->m_currentBlock->m_pos;
            int pos = p.remove("$").toInt(&ok, 16);
            node->m_curMemoryBlock = new MemoryBlock(pos,pos,MemoryBlock::ARRAY, node->m_blockInfo.m_blockName);
            as->blocks.append(node->m_curMemoryBlock);
        }
    }
    else
        node->m_curMemoryBlock=nullptr;
 /*   if (ret==2) {
        m_curMemoryBlock = nullptr;

    }*/


    NodeVar* v = (NodeVar*)node->m_varNode;
    NodeVarType* t = (NodeVarType*)node->m_typeNode;
    QString keep = v->value;
    v->value = as->m_symTab->getCurrentProcedure()+v->getValue(as);
//    qDebug() << v->value;
    node->ExecuteSym(as->m_symTab);
//    qDebug() << as->m_symTab->m_symbols.keys();
   //   v->value = keep;
    //        v->m_op.m_type =t->m_op.m_type;
    //v->m_type = t;

    if (t->m_op.m_type==TokenType::ARRAY) {
        as->DeclareArray(v->value, t->m_arrayVarType.m_value, t->m_op.m_intVal, t->m_data, t->m_position);
        //qDebug() << "IS: " << TokenType::types[as->m_symTab->Lookup(v->getValue(as))->getTokenType()];
        node->m_dataSize=t->m_op.m_intVal;
        Symbol* s = as->m_symTab->Lookup(v->value, node->m_op.m_lineNumber);
        s->isUsed=false;
        s->m_type="address";
        s->m_arrayType = t->m_arrayVarType.m_type;
    }else
    if (t->m_op.m_type==TokenType::STRING) {
        as->DeclareString(v->value, t->m_data);
        node->m_dataSize = 0;
        for (QString s: t->m_data)
            node->m_dataSize+=s.count();
        node->m_dataSize++; // 0 end
    }
    else
    if (t->m_op.m_type==TokenType::CSTRING) {
        as->DeclareCString(v->value, t->m_data);
        node->m_dataSize = 0;
        for (QString s: t->m_data)
            node->m_dataSize+=s.count();
        node->m_dataSize++; // 0 end
    }
    else
    if (t->m_op.m_type==TokenType::INCBIN) {
        if (node->m_curMemoryBlock!=nullptr && ((NodeVarType*)node->m_typeNode)->m_position!="")
             ErrorHandler::e.Error("IncBin can not be declared within a user-defined memory block with an abslute address. :",node->m_op.m_lineNumber);

        IncBin(node);
    }
    else
    if (t->m_op.m_type==TokenType::INCSID || t->m_op.m_type==TokenType::INCNSF) {
        if (node->m_curMemoryBlock!=nullptr)
            ErrorHandler::e.Error("IncSid can not be declared within a user-defined memory block :",node->m_op.m_lineNumber);
        IncSid(node);
    }
    else
    if (t->m_op.m_type==TokenType::POINTER) {
        //if (node->m_curMemoryBlock!=nullptr)
        //    ErrorHandler::e.Error("Pointers can not be declared within a user-defined memory block :",node->m_op.m_lineNumber);
        DeclarePointer(node);
    }else {
        node->m_dataSize=1;
        if (t->getValue(as).toLower()=="integer") node->m_dataSize = 2;
        QString typeVal = t->getValue(as);
        if (t->m_flag==1)
            typeVal="const";

        as->DeclareVariable(v->value, typeVal, t->initVal,t->m_position);
        // Increase by data counter IF
        if (t->m_flag==1)
            t->initVal = Util::numToHex(Util::NumberFromStringHex(t->initVal)+node->m_dataSize);
    }

    if (node->m_curMemoryBlock!=nullptr) {
        node->m_curMemoryBlock->m_end+=node->m_dataSize;
    }

}


void ASTDispather6502::IncSid(NodeVarDecl *node) {

    // Init address or load address? hmmm

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::NES) {

        if (node->sid.m_loadAddress!=0x8000 && Syntax::s.m_currentSystem->m_programStartAddress!=0x8000) {
          Appendix* app = new Appendix("$8000");
          app->Append("org $8000",1);
          app->Append("NSFfiller dc.b 0",0);
          as->m_appendix.append(app);
        }
    }

    QString pos = QString::number(node->sid.m_loadAddress,16);
    Appendix* app = new Appendix("$"+pos);
//    qDebug() << "INCSID DISPATCHER"<< pos;
    app->Append("org $" +pos,1);
    //        as->Appendix(v->getValue(as),0);
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
    as->blocks.append(new MemoryBlock(node->sid.m_loadAddress,node->sid.m_loadAddress+size, MemoryBlock::MUSIC, node->sid.m_fileName));


}

void ASTDispather6502::IncBin(NodeVarDecl *node) {
    NodeVar* v = (NodeVar*)node->m_varNode;
    NodeVarType* t = (NodeVarType*)node->m_typeNode;
    QString filename = as->m_projectDir + "/" + t->m_filename.replace("\\","/");
    if (!QFile::exists(filename))
        ErrorHandler::e.Error("Could not locate binary file for inclusion :" +filename);

    int size=0;
    QFile f(filename);
    if (f.open(QIODevice::ReadOnly)){
        size = f.size();  //when file does open.
        f.close();
    }


    if (t->m_position=="") {
        as->Label(v->value);
        as->Asm("incbin \"" + filename + "\"");
    }
    else {
        //            qDebug() << "bin: "<<v->getValue(as) << " at " << t->m_position;
        Appendix* app = new Appendix(t->m_position);

        Symbol* typeSymbol = as->m_symTab->Lookup(v->value, node->m_op.m_lineNumber);
        typeSymbol->m_org = Util::C64StringToInt(t->m_position);
        typeSymbol->m_size = size;
        //            qDebug() << "POS: " << typeSymbol->m_org;
        app->Append("org " +t->m_position,1);
        app->Append(v->value,0);
        app->Append("incbin \"" + filename + "\"",1);
        as->m_appendix.append(app);
        bool ok;
        int start=0;
        if (t->m_position.startsWith("$")) {
            start = t->m_position.remove("$").toInt(&ok, 16);
        }
        else start = t->m_position.toInt();

        as->blocks.append(new MemoryBlock(start,start+size, MemoryBlock::DATA,t->m_filename));

    }
}


void ASTDispather6502::DeclarePointer(NodeVarDecl *node) {

    if (!as->CheckZPAvailability())
        ErrorHandler::e.Error("Could not allocate more free pointers! Please free some up, or declare more in the settings page. ", node->m_op.m_lineNumber);

    NodeVarType* t = (NodeVarType*)node->m_typeNode;
    QString initVal = t->initVal;

    if (initVal=="") {
        initVal = as->PushZeroPointer();
        node->m_pushedPointers++;
    }

    NodeVar* v = (NodeVar*)node->m_varNode;
    as->Label(v->value + "\t= " + initVal);

}
void ASTDispather6502::PrintCompare(Node *node, QString lblSuccess, QString lblFailed)
{
    if (node->m_op.m_type==TokenType::EQUALS)
        as->Asm("bne " + lblFailed);
    if (node->m_op.m_type==TokenType::NOTEQUALS)
        as->Asm("beq " + lblFailed);
    if (node->m_op.m_type==TokenType::GREATEREQUAL) {
        as->Asm("bcc " + lblFailed);
    }
    if (node->m_op.m_type==TokenType::GREATER) {
        as->Asm("bcc " + lblFailed);
        as->Asm("beq " + lblFailed);
    }
    if (node->m_op.m_type==TokenType::LESSEQUAL ) {
        as->Asm("beq " + lblSuccess);
        as->Asm("bcs " + lblFailed);
    }

    if (node->m_op.m_type==TokenType::LESS)
        as->Asm("bcs " + lblFailed);

}

void ASTDispather6502::BinaryClause(Node *node)
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

void ASTDispather6502::BuildToCmp(Node *node)
{
    QString b="";

    NodeVar* varb = dynamic_cast<NodeVar*>(node->m_right);
    if (varb!=nullptr && varb->m_expr==nullptr)
        b = varb->getValue(as);

    if (node->m_right->isPureNumeric())
//    NodeNumber* numb = dynamic_cast<NodeNumber*>(node->m_right);
  //  if (numb!=nullptr)
        b = node->m_right->getValue(as);

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

void ASTDispather6502::BuildSimple(Node *node, QString lblSuccess, QString lblFailed)
{

    as->Comment("Binary clause Simplified: " + node->m_op.getType());
    //    as->Asm("pha"); // Push that baby

    BuildToCmp(node);

    PrintCompare(node, lblSuccess,lblFailed);



}

void ASTDispather6502::BinaryClauseInteger(Node *node)
{

    as->Comment("Binary clause INTEGER: " + node->m_op.getType());
    //    as->Asm("pha"); // Push that baby

    NodeVar* varb = dynamic_cast<NodeVar*>(node->m_right);

    NodeNumber* numb = dynamic_cast<NodeNumber*>(node->m_right);


    NodeVar* vara = dynamic_cast<NodeVar*>(node->m_left);

    if (vara==nullptr)
        ErrorHandler::e.Error("Integer comparison: only pure integer variable is supported");

    QString lbl1 = as->NewLabel("binaryclauseintegerlbl1");
    QString lbl2 = as->NewLabel("binaryclauseintegerlbl2"); // failed
    QString lbl3 = as->NewLabel("binaryclauseintegerfinished"); // failed

    QString lo = "";
    QString hi = "";

    if (numb!=nullptr) {
        lo = "#" + QString::number(((int)numb->m_val) & 255);
        hi = "#" + QString::number(((int)numb->m_val>>8) & 255);
    }
    if (varb!=nullptr) {
        lo = varb->getValue(as);
        hi = varb->getValue(as)+ "+1";
    }

    //m_left->Build(as);
    //as->Term();

    if (numb!=nullptr || varb!=nullptr) {
        if (node->m_op.m_type==TokenType::GREATER || node->m_op.m_type==TokenType::GREATEREQUAL) {
            as->Comment("Compare INTEGER with pure num / var optimization");
            as->Asm("lda " + vara->getValue(as) + "+1   ; compare high bytes");
            as->Asm("cmp " + hi + " ;keep");
            as->Asm("bcc " + lbl2);
            as->Asm("bne " + lbl1);
            as->Asm("lda " + vara->getValue(as));
            as->Asm("cmp " + lo +" ;keep");
            as->Asm("bcc " + lbl2);
        }
        if (node->m_op.m_type==TokenType::LESS || node->m_op.m_type==TokenType::LESSEQUAL) {
            as->Comment("Compare INTEGER with pure num / var optimization");
            as->Asm("lda " + vara->getValue(as) + "+1   ; compare high bytes");
            as->Asm("cmp " + hi + " ;keep");
            as->Asm("bcc " + lbl1);
            as->Asm("bne " + lbl2);
            as->Asm("lda " + vara->getValue(as));
            as->Asm("cmp " + lo+" ;keep");
            as->Asm("bcs " + lbl2);
        }
        if (node->m_op.m_type==TokenType::EQUALS) {
            as->Comment("Compare INTEGER with pure num / var optimization");
            as->Asm("lda " + vara->getValue(as) + "+1   ; compare high bytes");
            as->Asm("cmp " + hi + " ;keep");
            as->Asm("bne " + lbl2);
            as->Asm("lda " + vara->getValue(as));
            as->Asm("cmp " + lo+" ;keep");
            as->Asm("bne " + lbl2);
            as->Asm("jmp " + lbl1);
        }
        as->Label(lbl1); // ok
        as->Asm("lda #1");
        as->Asm("jmp " + lbl3);
        as->Label(lbl2); // failed
        as->Asm("lda #2");

        as->Label(lbl3); // DONE


        // Now all is ok
    }
    else {
        ErrorHandler::e.Error("Comparison of integer only works with number or variable");
    }




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
    as->PopLabel("binaryclauseintegerlbl1");
    as->PopLabel("binaryclauseintegerlbl2");
    as->PopLabel("binaryclauseintegerfinished");
    // as->PopLabel("binary_clause_temp_var");
    //  as->PopLabel("binary_clause_temp_lab");
}


bool ASTDispather6502::IsSimpleAndOr(NodeBinaryClause *node, QString labelSuccess, QString labelFail)
{
    if(node==nullptr)
        return false;
//    if (dynamic_cast<NodeBinaryClausenode->m_left)
    NodeBinaryClause* a = dynamic_cast<NodeBinaryClause*>(node->m_left);
    NodeBinaryClause* b = dynamic_cast<NodeBinaryClause*>(node->m_right);
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





/*
 *
 *
 *  NODE BINARY CLAUSE
 *
 */

void ASTDispather6502::dispatch(NodeBinaryClause *node)
{
    node->DispatchConstructor(as);

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



void ASTDispather6502::LogicalClause(Node *node)
{
    if (dynamic_cast<NodeBinaryClause*>(node->m_left)==nullptr)
        ErrorHandler::e.Error("Logical clause: left hand term must be binary clause");

    if (dynamic_cast<NodeBinaryClause*>(node->m_right)==nullptr)
        ErrorHandler::e.Error("Logical clause: right hand term must be binary clause");


    // Test for optimization : if left and right are pure


/*
    if (node->m_op.m_type==TokenType::OR) {
        node->m_left->Accept(this);
        QString tmpVar = as->StoreInTempVar("logical_class_temp");
        node->m_right->Accept(this);
        if (node->m_op.m_type==TokenType::AND)
            as->Asm("and " + tmpVar);
        if (node->m_op.m_type==TokenType::OR)
            as->Asm("ora " + tmpVar);

        as->PopTempVar();
        return;
    }
    if (node->m_op.m_type==TokenType::AND) {
        node->m_left->m_ignoreSuccess=true;
        node->m_right->m_ignoreSuccess=true;
        as->m_lblFailed = as->NewLabel("logicalclausefail");
        node->m_left->Accept(this);
        node->m_right->Accept(this);
        as->Label(as->m_lblFailed);
        as->PopLabel("logicalclausefail");
        return;
    }
*/


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


void ASTDispather6502::Compare(NodeForLoop *node, NodeVar* var, bool isLarge, QString loopDone, QString loopNotDone, bool inclusive) {

//    if (!var->isWord(as))
    {
        as->ClearTerm();
        node->m_b->Accept(this);
        as->Term();

        as->Asm("cmp " + as->m_stack["for"].current() +" ;keep");

        int stepValue = 1; // do we have a step value?
        if (node->m_step != nullptr) {

            stepValue = node->m_step->numValue(); //node->m_step->getValue(as);

        }
        if (inclusive) {    // inclusive version will END after the TO value

            if (!isLarge) {

                if (stepValue > 0) {
                    as->Asm("bcs "+as->getLabel("for")); // or FOR index > TO value
                } else { //if (stepValue < -1) {
                    as->Asm("bcc "+as->getLabel("for")); // FOR index < TO value
                    as->Asm("beq "+as->getLabel("for")); // BEQ then the BCC below
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

        } else {            // TRSE version will END on the TO value

            if (!isLarge) {

                if (stepValue == 1 || stepValue == -1) {
                    // increments / decrements of 1 are safe for BNE
                    as->Asm("bne "+as->getLabel("for"));
                } else if (stepValue > 1) {
                    as->Asm("beq "+loopDone); // FOR index == TO value
                    as->Asm("bcs "+as->getLabel("for")); // or FOR index > TO value
                } else { //if (stepValue < -1) {
                    as->Asm("bcc "+as->getLabel("for")); // FOR index < TO value
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

void ASTDispather6502::IncreaseCounter(NodeForLoop *node, NodeVar* var) {

    // no STEP included in FOR TO DO, we assume STEP 1
    if (node->m_step==nullptr) {

        //if (!var->isWord(as)) {
            as->Asm("inc " + as->m_stack["for"].current());
       /* }
        else {
            as->Asm("clc");
            as->Asm("inc " + as->m_stack["for"].current());
            QString lbl = as->NewLabel("lblCounterWord");
            as->Asm("bcc "+lbl);
            as->Asm("inc " + as->m_stack["for"].current() +"+1");

            as->Label(lbl);

            as->PopLabel("lblCounterWord");

        }*/

    } else {

        // STEP included in FOR TO DO statement
        int stepValue = node->m_step->numValue();
        //stepValue = node->m_step->getInteger();
        //qDebug() << node->m_step->numValue();
        //qDebug() << node->m_step->getInteger();

        // if 1 or -1 we can optimise!
        if (stepValue == 1) {

            as->Asm("inc " + as->m_stack["for"].current());

        } else if (stepValue == -1) {

            as->Asm("dec " + as->m_stack["for"].current());

        } else {
            // Larger +ve/-ve STEP

            // Handles a -ve and +ve step as a byte, eg: -1 == 255
            as->Asm("clc");
            as->Asm("lda " + as->m_stack["for"].current());
            as->ClearTerm();
            as->Term("adc ");
            node->m_step->Accept(this);
            //            m_step->Build(as);
            as->Term();

            if (var->isWord(as)) {
                    QString lbl = as->NewLabel("lblCounterWord");
                    as->Asm("bcc "+lbl);
                    as->Asm("inc " + as->m_stack["for"].current() +"+1");
                    as->Label(lbl);
                    as->PopLabel("lblCounterWord");
            }
            as->Asm("sta "+as->m_stack["for"].current());

        }

    }



 //       if (node->m_loopCounter!=0)
   //         ErrorHandler::e.Error("Error: Loop with step other than 1,-1 cannot have loopy/loopx flag");
        // Is word


}

// handle a small loop
void ASTDispather6502::SmallLoop(NodeForLoop *node, NodeVar* var, bool inclusive)
{
    QString loopDone = as->NewLabel("forLoopDone");
    //  Compare(as);
    //  as->Asm("beq "+loopDone);

    node->m_block->Accept(this);
    as->m_stack["for"].pop();
    IncreaseCounter(node,var);
    Compare(node, var, false, loopDone, nullptr, inclusive);

//    as->Asm("jmp " + as->getLabel("for"));

    as->Label(loopDone);

    as->m_labelStack["for"].pop();
    as->m_labelStack["forLoopDone"].pop();

}

// handle a large loop
void ASTDispather6502::LargeLoop(NodeForLoop *node, NodeVar* var, bool inclusive)
{
    QString loopForFix = as->NewLabel("forLoopFix");
    QString loopDone = as->NewLabel("forLoopDone");
    QString loopNotDone = as->NewLabel("forLoopNotDone");

    as->Label(loopForFix);
    node->m_block->Accept(this);
    as->m_stack["for"].pop();

    IncreaseCounter(node,var);
    Compare(node, var, true, loopDone, loopNotDone, inclusive);

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

void ASTDispather6502::dispatch(NodeConditional *node)
{
    node->DispatchConstructor(as);


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
    NodeBinaryClause* bn = dynamic_cast<NodeBinaryClause*>(node->m_binaryClause);

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
        if (isOKBranchSize && IsSimpleAndOr(dynamic_cast<NodeBinaryClause*>(node->m_binaryClause), lblstartTrueBlock,labelElse)){
        }
        else {

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
        BuildSimple(bn,  lblstartTrueBlock,failedLabel);
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

void ASTDispather6502::dispatch(NodeForLoop *node)
{
    node->DispatchConstructor(as);


    NodeAssign *nVar = dynamic_cast<NodeAssign*>(node->m_a);

    // get the inclusive flag for the method used for the coparison, ie: false is < and true is <=
    bool inclusive =(node->m_inclusive);

    // left node *must* be an assign statement (e.g a:=10)
    if (nVar==nullptr)
        ErrorHandler::e.Error("Index must be variable", node->m_op.m_lineNumber);


    // Get the variable name
    QString var = dynamic_cast<NodeVar*>(nVar->m_left)->getValue(as);
    // accept statement (assign variable)
    node->m_a->Accept(this);


    as->m_stack["for"].push(var);

    as->Label(as->NewLabel("for"));

    bool isSmall = node->verifyBlockBranchSize(as, node->m_block);

    if (node->m_forcePage == 1)
        isSmall = false;

    if (node->m_forcePage == 2)
        isSmall = true;

    if (isSmall)
        SmallLoop(node,dynamic_cast<NodeVar*>(nVar->m_left), inclusive);
    else
        LargeLoop(node,dynamic_cast<NodeVar*>(nVar->m_left), inclusive);


}




void ASTDispather6502::LoadPointer(NodeVar *node) {
    as->Comment("Load pointer array");
    NodeNumber* number = dynamic_cast<NodeNumber*>(node->m_expr);
    QString m = as->m_term;

    as->ClearTerm();
    if (!LoadXYVarOrNum(node, node->m_expr,false))
    {
        as->Comment("LDA stuff");
        if (!(m=="" || m.startsWith("lda")))
            as->Asm("pha");
        node->m_expr->Accept(this);
        as->Term();
        as->Asm("tay");
        if (!(m=="" || m.startsWith("lda")))
            as->Asm("pla");
    }
    if (m=="")
        m="lda ";
    as->Asm(m+  "(" + node->getValue(as)+"),y");

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

void ASTDispather6502::dispatch(NodeVar *node)
{

    node->DispatchConstructor(as);

    QString  val = node->getValue(as);
    Pmm::Data::d.lineNumber = node->m_op.m_lineNumber;
    Symbol* s = as->m_symTab->Lookup(node->getValue(as), node->m_op.m_lineNumber);
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
        if (((s->getTokenType()==TokenType::ADDRESS || s->getTokenType()==TokenType::INCBIN)  && as->m_term=="")) {
            as->Asm("lda #<" + val);
            as->Asm("ldy #>" + val);
            return;
        }
        if (node->m_fake16bit && s->getTokenType()==TokenType::BYTE )
            as->Asm("ldy #0 ; Fake 16 bit");
        as->Variable(val, isOK);
    }

}



bool ASTDispather6502::LoadXYVarOrNum(NodeVar *node, Node *other, bool isx) {
    Symbol* s = as->m_symTab->Lookup(node->getValue(as), node->m_op.m_lineNumber);
    NodeVar* var = dynamic_cast<NodeVar*>(other);
    //NodeNumber* num = dynamic_cast<NodeNumber*>(other);
    if (other==nullptr)
        return false;
    bool isNumber = other->isPureNumeric();
    QString operand = "ldx ";
    if (!isx) operand="ldy ";
    if (var!=nullptr && var->m_expr == nullptr) {
            as->Asm(operand + var->getValue(as));
            if (s->m_arrayType==TokenType::INTEGER) // integer array index is *2 (two bytes per array slot)
            {
                as->Asm("txa   ; watch for bug, Integer array has index range of 0 to 127");
                as->Asm("asl");
                as->Asm("tax");
            }
        return true;
    }
    if (isNumber) {
//        qDebug() << "LoadXYVarorNum HERE ";
        if (s->m_arrayType==TokenType::INTEGER) {
            as->Asm(operand + "#" + QString::number(other->getValueAsInt(as) * 2) + " ; watch for bug, Integer array has max index of 128");
        }
        else
            as->Asm(operand  + other->getValue(as));
        return true;
    }
    return false;

}

void ASTDispather6502::LoadByteArray(NodeVar *node) {
    // Optimizer: if expression is number, just return direct

    Symbol* s = as->m_symTab->Lookup(node->getValue(as), node->m_op.m_lineNumber);
    if (s->m_arrayType==TokenType::INTEGER)
        as->Comment("Load Integer array");
    else if (s->m_arrayType==TokenType::BYTE)
        as->Comment("Load Byte array");
    else
        as->Comment("Load Unknown type array");

    QString m = as->m_term;
    if (node->m_expr==nullptr) {
        ErrorHandler::e.Error("Unknown operation with address!",node->m_op.m_lineNumber);
    }
    as->ClearTerm();
    if (!LoadXYVarOrNum(node, node->m_expr,true))
    {
        // calculation version, eg: index+2  or 3+2
        //       as->Asm("pha");
        node->m_expr->Accept(this);
        as->Term();
        if (s->m_arrayType==TokenType::INTEGER) // integer array index is *2 (two bytes per array slot)
            as->Asm("asl");
        as->Asm("tax");
        //          as->Asm("pla");
    }
    if (m=="")
        m="lda ";
    as->Asm(m+  node->getValue(as)+",x");

    if (s->m_arrayType==TokenType::INTEGER) { // integer array need to load the high byte also
//        as->Asm("ldy "+  node->getValue(as)+",x");
  //      as->Asm("inx");
        as->Asm("ldy "+  node->getValue(as)+"+1,x");
    }
}

void ASTDispather6502::LoadVariable(Node *node)
{
    NodeVar* v = dynamic_cast<NodeVar*>(node);
    if (v!=nullptr) {
        LoadVariable(v);
        return;
    }

    NodeNumber* num = dynamic_cast<NodeNumber*>(node);
    if (num!=nullptr) {
        LoadVariable(num);
        return;
    }

    node->Accept(this);
//    ErrorHandler::e.Error("Uknown variable type!", node->m_op.m_lineNumber);

}


void ASTDispather6502::LoadVariable(NodeVar *node) {

    /*        if (as->m_symTab->Lookup(value)==nullptr)
                ErrorHandler::e.Error("Could not find variable '" +value +"' for storing.",m_op.m_lineNumber);
    */
    TokenType::Type t = as->m_symTab->Lookup(node->getValue(as), node->m_op.m_lineNumber)->getTokenType();
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
            if (node->m_fake16bit)
                as->Asm("ldy #0");
            as->Asm("lda " +node->getValue(as));
        }
        return;
    }
    if (t == TokenType::INTEGER) {
        node->m_isWord = true;
        as->Comment("Integer assignment in nodevar");
        as->Asm("lda " +node->getValue(as));
        as->Asm("ldy " +node->getValue(as)+"+1");
        return;
    }
    ErrorHandler::e.Error(TokenType::getType(t) + " assignment not supported yet for exp: " + node->getValue(as));
    return;
}

void ASTDispather6502::LoadAddress(Node *node)
{

}

void ASTDispather6502::LoadVariable(NodeNumber *node)
{
   as->ClearTerm();
   as->Term("lda ");
   node->Accept(this);
   as->Term();
}


void ASTDispather6502::StoreVariable(NodeVar *node) {
    //        as->Comment("VarNode StoreVariable");
    as->m_symTab->Lookup(node->getValue(as), node->m_op.m_lineNumber);
    //          ErrorHandler::e.Error("Could not find variable '" +value +"' for storing.", m_op.m_lineNumber);

    // Is array
    if (node->m_expr != nullptr) {
//        NodeNumber* number = dynamic_cast<NodeNumber*>(node->m_expr);
        bool isNumber = node->m_expr->isPureNumeric();
        if (isNumber && node->getType(as)!=TokenType::POINTER)
        { // IS NUMBER optimize}
            if (node->getArrayType(as)==TokenType::INTEGER) {
                // Store integer array
                int i = node->m_expr->getValueAsInt(as)*2;
                as->Asm("sta " + node->getValue(as) + "+"+ QString::number(i));
                as->Asm("sty "  + node->getValue(as) +"+"+ QString::number(i+1));

            }
            else {
                as->Asm("sta " + node->getValue(as) + "+"+ node->m_expr->getValue(as));
            }
            //                as->Asm("tya");
            return;
        }
        else {
            //if regular array
            NodeVar* var = dynamic_cast<NodeVar*>(node->m_expr);
            //NodeNumber* num = dynamic_cast<NodeNumber*>(node->m_expr);
            QString secondReg="x";
            QString pa = "";
            QString pb= "";
            if (node->getType(as)==TokenType::POINTER) {
                secondReg="y";
                pa="(";
                pb=")";
            }

            // Optimize for number or pure var
            if (node->m_expr->getType(as)==TokenType::INTEGER_CONST) {
                //qDebug() << "StoreVariable:: HER";
                as->ClearTerm();
                as->Term("ld"+secondReg +" ");
                node->m_expr->Accept(this);
                as->Term();
//                as->Asm("COMMENT BUT WHY DOES IT DISAPPEAR");
                as->Asm("sta " +pa + node->getValue(as)+ pb + "," + secondReg);
                if (node->getArrayType(as)==TokenType::INTEGER) {
                    as->Asm("in"+secondReg);
                    as->Asm("tya");
                    as->Asm("sta "  + node->getValue(as) + "," + secondReg);
                }
                return;
            }
            // Just regular var optimize
            // Regular expression



            bool usePush = true;

//            if (node->m_expr->isPure())
  //              usePush = false;

            if (usePush)
                as->Asm("pha");
            as->ClearTerm();
            node->m_expr->Accept(this);
            //                node->m_expr->Build(as);
            as->Term();
            if (node->getArrayType(as)==TokenType::INTEGER)
                as->Asm("asl");
            as->Asm("ta" + secondReg);
            if (usePush)
                as->Asm("pla");
            as->Asm("sta " +pa + node->getValue(as)+pb+","+ secondReg);
            if (node->getArrayType(as)==TokenType::INTEGER) {
                as->Asm("in"+secondReg);
             as->Asm("tya");
             as->Asm("sta " +pa + node->getValue(as)+pb+","+ secondReg);
            }
        }
        return;
    }
    else {
        // Not array
        if (as->m_symTab->Lookup(node->getValue(as), node->m_op.m_lineNumber)->getTokenType() == TokenType::BYTE) {

            as->Asm("sta " + node->getValue(as));
            return;
        }
        if (as->m_symTab->Lookup(node->getValue(as), node->m_op.m_lineNumber)->getTokenType() == TokenType::ADDRESS) {

            as->Asm("sta " + node->getValue(as));
            return;
        }
        if (as->m_symTab->Lookup(node->getValue(as), node->m_op.m_lineNumber)->getTokenType() == TokenType::INTEGER) {
            as->Asm("sta " + node->getValue(as));
            as->Asm("sty " + node->getValue(as) + "+1");
            return;
        }

    }




}


void ASTDispather6502::AssignString(NodeAssign *node, bool isPointer) {

    NodeString* right = (NodeString*)dynamic_cast<const NodeString*>(node->m_right);
    NodeVar* left = (NodeVar*)dynamic_cast<const NodeVar*>(node->m_left);
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
        as->Asm("sta "+left->getValue(as));
        as->Asm("lda #>"+str);
        as->Asm("sta "+left->getValue(as)+"+1");
    }
    else {

        as->Asm("ldx #0");
        as->Label(lblCpy);
        as->Asm("lda " + str+",x");
        as->Asm("sta "+left->getValue(as) +",x");
        as->Asm("inx");
        as->Asm("cmp #0 ;keep");  // ask post optimiser to not remove this
        as->Asm("bne " + lblCpy);
    }
  //  as->PopLabel("stringassign");
    as->PopLabel("stringassignstr");
    as->PopLabel("stringassigncpy");

}

void ASTDispather6502::AssignPointer(NodeAssign *node) {
    NodeVar* bVar = dynamic_cast<NodeVar*>(node->m_right);
//    NodeNumber* bNum = dynamic_cast<NodeNumber*>(node->m_right);
    NodeVar* aVar = dynamic_cast<NodeVar*>(node->m_left);


//    if (!node->m_right->isAddress())
  //      ErrorHandler::e.Error("Must be address", node->m_op.m_lineNumber);

    if (IsSimpleIncDec(aVar,  node))
        return;



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
        as->Asm("sta " + aVar->getValue(as));
        as->Asm("sty "+ aVar->getValue(as)+"+1");
        return;
    }




    if (bVar!=nullptr) {

        if (bVar->getType(as)!=TokenType::POINTER) {
//            if (bVar->m_op.m_type==TokenType::ADDRESS) {
                as->Asm("lda #<" + bVar->getValue(as));
                as->Asm("ldx #>" + bVar->getValue(as));
                as->Asm("sta " + aVar->getValue(as));
                as->Asm("stx "+ aVar->getValue(as)+"+1");
                return;
  //          }
        }
        else
        {
            as->Asm("lda " + bVar->getValue(as));
            as->Asm("ldx " + bVar->getValue(as) + "+1");
            as->Asm("sta " + aVar->getValue(as));
            as->Asm("stx "+ aVar->getValue(as)+"+1");
            return;
        }

    }
    if (node->m_right->isPureNumeric()) {
        as->Asm("lda #" + QString::number(((int)node->m_right->numValue()) & 255));
        as->Asm("ldx #" + QString::number(((int)node->m_right->numValue()>>8) & 255) );
        as->Asm("sta " + aVar->getValue(as));
        as->Asm("stx "+ aVar->getValue(as)+"+1");
        return;

    }
     ErrorHandler::e.Error("Right-hand side must be constant or address", node->m_op.m_lineNumber);

}

bool ASTDispather6502::isSimpleAeqAOpB(NodeVar *var, NodeAssign *node) {
    NodeBinOP* rterm = dynamic_cast<NodeBinOP*>(node->m_right);
    if (rterm==nullptr)
        return false;

    // right first is var
    NodeVar* rvar = dynamic_cast<NodeVar*>(rterm->m_left);

    NodeVar* rrvar = dynamic_cast<NodeVar*>(rterm->m_right);
    if (!rterm->m_right->isPureNumeric())
        return false;
  //  int num = rterm->m_right->getValueAsInt(as);

//    NodeNumber* rrnum = dynamic_cast<NodeNumber*>(rterm->m_right);


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
    LoadVariable(var);
    as->BinOP(rterm->m_op.m_type);
    rterm->m_right->Accept(this);
    as->Term();
    StoreVariable(var);
    return true;
}

bool ASTDispather6502::isSimpleAeqAOpB16Bit(NodeVar *var, NodeAssign *node)
{
    NodeBinOP* rterm = dynamic_cast<NodeBinOP*>(node->m_right);
    if (rterm==nullptr)
        return false;

    // right first is var
     NodeVar* rvar = dynamic_cast<NodeVar*>(rterm->m_right);

//    NodeVar* rrvar = dynamic_cast<NodeVar*>(rterm->m_right);
//    NodeNumber* rrnum = dynamic_cast<NodeNumber*>(rterm->m_right);

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

    if (var->isWord(as) &&  rterm->m_right->is8bitValue(as) && !(node->m_forceType==TokenType::INTEGER)) {
  //      qDebug() << "WHOO";
        //          qDebug() << "Cont";

        //       qDebug() << "ASTDispather6502::isSimpleAeqAOpB16Bit HERE";
        QString lbl = as->NewLabel("WordAdd");
        as->Comment("WORD optimization: a=a+b");
        //var->Accept(this);
        as->Asm("lda " + var->getValue(as) + "+0");
        as->Term();
        //        as->Asm("clc");
        as->BinOP(rterm->m_op.m_type);

        rterm->m_right->Accept(this);
        as->Term();
        if (rterm->m_op.m_type==TokenType::PLUS) {
            as->Asm("bcc "+lbl);
            as->Asm("inc " + var->getValue(as) + "+1");
        }
        else {
            as->Asm("bcs "+lbl);
            as->Asm("dec " + var->getValue(as) + "+1");
        }

        as->Label(lbl);
        as->Asm("sta " + var->getValue(as) + "+0");
        //        as->Asm("sty " + var->getValue(as) +"+1");

        as->PopLabel("WordAdd");
        return true;

    }
    return false;
}

bool ASTDispather6502::IsSimpleIncDec(NodeVar *var, NodeAssign *node) {
    // Right is binop
//    return false;
    NodeBinOP* rterm = dynamic_cast<NodeBinOP*>(node->m_right);
    if (rterm==nullptr)
        return false;

    // right first is var

    NodeVar* rvar = dynamic_cast<NodeVar*>(rterm->m_left);
    if (rvar==nullptr)
        return false;

//    qDebug() << "IsSimpleIncDec test";


    if (rvar->getValue(as)!=var->getValue(as))
        return false;


//    NodeNumber* num = dynamic_cast<NodeNumber*>(rterm->m_right);
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
        return isSimpleAeqAOpB16Bit(var, node);
    }

//    if (var->isWord(as))
  //      return false;

    if (!isPureNumber)
        return false;

    if (num!=1)
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
        as->Asm(operand +var->getValue(as));
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

        if (LoadXYVarOrNum(var, var->m_expr,true)) {
            as->Comment("Optimize byte array " + operand);

            as->Asm(operand + var->getValue(as)+",x");
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

void ASTDispather6502::dispatch(NodeAssign *node)
{
    node->DispatchConstructor(as);

//    as->PushCounter();

    AssignVariable(node);

  //  as->PopCounter(node->m_op.m_lineNumber);

}

void ASTDispather6502::dispatch(NodeCase *node)
{
    node->DispatchConstructor(as);
  //  as->PushCounter();
    bool hasElse = node->m_elseBlock!=nullptr;
    QString labelEnd = as->NewLabel("caseend");
    for (int i=0;i<node->m_conditionals.count();i++) {
        QString labelNext = as->NewLabel("casenext");
        as->PopLabel("casenext");
        if (node->m_conditionals[i]->isPure()) {
            node->m_variable->Accept(this);
            as->Term();
            as->Asm("cmp " + node->m_conditionals[i]->getValue(as) +" ;keep");

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

void ASTDispather6502::dispatch(NodeRepeatUntil *node)
{
    node->DispatchConstructor(as);
    QString lbl = as->NewLabel("repeatUntil");
    QString lblDone = as->NewLabel("repeatUntil");
    as->Label(lbl);
    node->m_block->Accept(this);

    bool isSimplified = false;
    bool isOKBranchSize = true;
    if (node->verifyBlockBranchSize(as, node->m_block)) {
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
        if (isOKBranchSize && IsSimpleAndOr(dynamic_cast<NodeBinaryClause*>(node->m_clause), lblDone,lbl)){
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
        BuildSimple(node->m_clause,  lblDone,lbl);
    }
    // Start main block

    as->Label(lblDone);


}

void ASTDispather6502::dispatch(NodeComment *node)
{
    if (node->m_comment!="") {
//        as->Comment("A COMMENT");

        as->Comment(node->m_comment.replace("\n","\n; "));
    }

}





QString ASTDispather6502::AssignVariable(NodeAssign *node) {


    NodeVar* v = (NodeVar*)dynamic_cast<const NodeVar*>(node->m_left);
    //        qDebug() << "AssignVariable: " <<v->getValue(as) << " : " << TokenType::getType( v->getType(as));

    NodeNumber* num = (NodeNumber*)dynamic_cast<const NodeNumber*>(node->m_left);

    if (v==nullptr && num == nullptr)
        ErrorHandler::e.Error("Left value not variable or memory address! ", node->m_op.m_lineNumber);
    if (num!=nullptr && num->getType(as)!=TokenType::ADDRESS)
        ErrorHandler::e.Error("Left value must be either variable or memory address, not a constant.", node->m_op.m_lineNumber);

//    qDebug() << TokenType::getType(num->getType(as));


    if (num!=nullptr) {
        as->Comment("Assigning memory location");
        v = new NodeVar(num->m_op); // Create a variable copy
        v->value = num->HexValue();
        //return num->HexValue();
    }


    as->Comment("Assigning single variable : " + v->getValue(as));
    Symbol* s = as->m_symTab->Lookup(v->getValue(as), node->m_op.m_lineNumber, v->isAddress());
    //        if (s==nullptr)
    //          ErrorHandler::e.Error("Could not find variable :" + v->getValue(as),m_op.m_lineNumber);



    TokenType::Type t = s->getTokenType();
    //qDebug() << TokenType::getType(m_left->getType(as)) << " " << v->m_expr;

    if (node->m_left->getType(as)==TokenType::POINTER && v->m_expr==nullptr) {

        if (dynamic_cast<const NodeString*>(node->m_right)) {
            AssignString(node,node->m_left->isPointer(as));
            return v->getValue(as);

        }

        AssignPointer(node);
        return v->getValue(as);
    }

    if (dynamic_cast<const NodeString*>(node->m_right))
    {
        AssignString(node,node->m_left->isPointer(as));
        return v->getValue(as);
    }
    if (node->m_right==nullptr)
        ErrorHandler::e.Error("Node assign: right hand must be expression", node->m_op.m_lineNumber);

    if (node->m_left->isWord(as)) {
//        qDebug() << "AssignVariable HERE " << v->getValue(as);
        as->Asm("ldy #0");    // AH:20190722: Does not appear to serve a purpose - takes up space in prg. Breaks TRSE scroll in 4K C64 demo if take this out
//        node->m_right->m_forceType = TokenType::INTEGER; // FORCE integer on right-hand side
        node->m_right->setForceType(TokenType::INTEGER);
    }
    // For constant i:=i+1;
    if (IsSimpleIncDec(v,  node))
        return v->getValue(as);

    // Special case:
/*
    // typically "a[i+1]:=b;" or "a[i+3]:=b+c*2;" but no Y set. Then
    // execute [expr] first and THEN do a lda # and save! No more pha pla
    if (!node->m_right->containsPointer(as)) {
        as->ClearTerm();
        v->Accept(this);
        as->Term();
        QString secondReg="x";
        QString pa = "";
        QString pb= "";
        if (v->getType(as)==TokenType::POINTER) {
            secondReg="y";
            pa="(";
            pb=")";
        }

        as->Asm("ta" + secondReg);
        as->Asm("sta " +pa + v->getValue(as)+pb+","+ secondReg);
        return v->getValue(as);

    }
*/

    // Optimization : a[2*b]:=2; -> no need for pha/pla

    if (!v->isWord(as) && node->m_right->isPure() && v->m_expr!=nullptr) {
        StoreVariableSimplified(v, node->m_right);
        return v->getValue(as);
    }


    node->m_right->Accept(this);
    as->Term();
//    as->Comment("Calling STOREVARIABLE");
    StoreVariable(v);

     return v->getValue(as);
}

void ASTDispather6502::StoreVariableSimplified(NodeVar *node, Node* expr)
{
    //NodeNumber* num = dynamic_cast<NodeNumber*>(node->m_expr);
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

    as->Asm("ta" + secondReg);
    as->ClearTerm();
    expr->Accept(this);
    as->Term();
    as->Asm("sta " +pa + node->getValue(as)+pb+","+ secondReg);

}
