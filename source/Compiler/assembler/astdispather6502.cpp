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
    as->Asm("ldy #0");

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

    //as->Variable(v->value, false);
    //as->Asm("lda " + v->value + "+1");
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
    as->Comment("HandleVarBinopB16bit here");
    as->ClearTerm();
    as->Asm("ldy #0");
    node->m_right->Accept(this);

    as->Term();
    //    as->Asm("sta " +lbl);
    QString lbl = as->StoreInTempVar("rightvarInteger", "word");
//    as->Asm("sty " +lbl+"+1");
//        qDebug() << as->m_term;
    as->Term();
  //  as->Asm("clc");
    //as->Variable(v->value, false);
  //  as->Comment(";HEREHERE");
//    exit(1);
    if (v->m_op.m_type!=TokenType::ADDRESS)
        as->Asm("lda " + v->value + "+1");
    else
        as->Asm("lda #>" + v->value + "");

    as->BinOP(node->m_op.m_type);
    as->Term(lbl+"+1", true);
    as->Asm("tay");
//    qDebug() << v->value << v->m_op.getType();
  //  exit(1);
    if (v->m_op.m_type!=TokenType::ADDRESS)
        as->Asm("lda "+ v->value);
    else
        as->Asm("lda #<"+ v->value);
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
            qDebug() << "Var:"  <<vnum->value;*/
        as->Comment("Add/sub where right value is constant number");
        if (num!=nullptr && num->m_op.m_type==TokenType::ADDRESS && var!=nullptr) {
            //qDebug() << "ADDRESS: " << num->StringValue();
            //exit(1);
            //as->
            as->ClearTerm();
            as->Term("lda " + var->value + " + " + num->StringValue());
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

    if (node->m_right->isPureNumeric())  {
        as->Comment("Right is PURE NUMERIC");
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

    as->Asm("sty "+varName);
    as->Asm("sta "+varName+"+1");
    command = "\t lsr " + varName +"+1"+ "\n";
    command += "\t ror " + varName+"+0" + "\n";


    for (int i=0;i<cnt;i++)
        as->Asm(command);

    as->Asm("lda " + varName);

    as->PopTempVar();
}

void ASTDispather6502::Mul16x8(Node *node) {
    as->Comment("Mul 16x8 setup");
    as->Asm("");
    if (node->m_left->getType(as)==TokenType::INTEGER) {
        LoadVariable(node->m_left);
        as->Term();
        as->Asm("sty mul16x8_num1");
        as->Asm("sta mul16x8_num1Hi");
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
    as->Asm("sta dividend");
    as->Asm("sty dividend+1");
    as->Asm("ldy #0");
    node->m_right->Accept(this);
    as->Term();
    as->Asm("sta divisor");
    as->Asm("sty divisor+1");
    as->Asm("jsr divide16x8");
    as->Asm("lda dividend");
    as->Asm("ldy dividend+1");

}




void ASTDispather6502::HandleRestBinOp(Node* node) {
    bool isWord16 = false;
    QString varName="";

    if ( dynamic_cast<const NodeVar*>(node->m_left) != nullptr) {
        NodeVar* v= (NodeVar*)node->m_left;
        varName = v->value;
        Symbol* s = as->m_symTab->Lookup(varName, node->m_op.m_lineNumber);
    }
    isWord16 = node->isWord(as);
    if (node->m_isWord)
        isWord16 = true;

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

    node->DispatchConstructor();
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
    node->DispatchConstructor();

    QString val = "";

    /*        if (as->m_symTab->m_constants.contains(m_op.m_value)) {
            m_val = as->m_symTab->m_constants[m_op.m_value]->m_value->m_fVal;
        }
*/


    if (node->m_op.m_type==TokenType::BYTE)
        val = "#"+QString::number((int)node->m_val);
    if (node->m_op.m_type==TokenType::INTEGER)
        val = "#"+QString::number((int)node->m_val);
    if (node->m_op.m_type==TokenType::INTEGER_CONST)
        val = "#"+QString::number((int)node->m_val);
    if (node->m_op.m_type==TokenType::ADDRESS) {

        val = "$" + QString::number((int)node->m_val,16);
    }

    if (node->m_op.m_type==TokenType::INTEGER_CONST && node->m_val>255) {
        as->Comment("Integer constant assigning");
        int hiBit = ((int)node->m_val)>>8;
        int loBit = ((int)node->m_val)&0xff;
        as->ClearTerm();
        as->Asm("ldy #" + QString::number(hiBit) );
        //            as->Asm("tax");
        as->Asm("lda #" + QString::number(loBit) );
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
    node->DispatchConstructor();

    node->VerifyParams(as);

    as->PushCounter();

    Methods6502 methods;
    methods.m_node = node;
    methods.Assemble(as,this);

    as->PopCounter(node->m_op.m_lineNumber-1);

}


void ASTDispather6502::dispatch(Node *node)
{
    node->DispatchConstructor();

    node->m_currentLineNumber = node->m_op.m_lineNumber;
}



void ASTDispather6502::dispatch(NodeProcedure *node)
{
    node->DispatchConstructor();


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

}

void ASTDispather6502::dispatch(NodeProcedureDecl *node)
{
    node->DispatchConstructor();

//    if (node->m_curMemoryBlock!=nullptr)
  //      qDebug() << node->m_procName << "IS IN BLOCK " << node->m_curMemoryBlock->m_name << " STARTING AT " << Util::numToHex(node->m_curMemoryBlock->m_start);

    // In case memory block is acive
//    as->EndMemoryBlock();

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
    else
        node->m_curMemoryBlock=nullptr;



    //MaintainBlocks(as);
    if (node->m_block==nullptr)  // Is builtin procedure
        node->m_block = new NodeBuiltinMethod(node->m_procName, QVector<Node*>(), nullptr);

    bool isInitFunction=false;
    bool isBuiltinFunction=false;
    if (Syntax::s.builtInFunctions.contains(node->m_procName)) {
        isBuiltinFunction = true;
        isInitFunction = Syntax::s.builtInFunctions[node->m_procName].m_initFunction;
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


    if (!isInitFunction) {
        //as->Asm("jmp afterProc_" + m_procName);


        //as->Label(m_procName);
    }
//    if (m_isInterrupt)
  //      as->Asm("dec $d019        ; acknowledge IRQ");
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
      //as->Label("afterProc_" + m_procName);
    }

    if (node->m_curMemoryBlock!=nullptr) {
        node->m_curMemoryBlock->m_end+=10;
    }

}



void ASTDispather6502::dispatch(NodeProgram *node)
{
    node->DispatchConstructor();

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
    node->DispatchConstructor();

}
void ASTDispather6502::dispatch(NodeAsm *node)
{
    node->DispatchConstructor();

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
    node->DispatchConstructor();

    as->String(node->m_val);
}

void ASTDispather6502::dispatch(NodeUnaryOp *node)
{
    node->DispatchConstructor();

    NodeNumber* num = dynamic_cast<NodeNumber*>(node->m_right);

    if (num!=nullptr) {
        int s = num->m_val;

        if (node->m_op.m_type==TokenType::MINUS) {
            if (num->m_op.m_type<256)
                num->m_val=256-num->m_val;
            else
                if (num->m_op.m_type<65536)
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
    node->DispatchConstructor();

    as->BeginBlock();
    for (Node* n: node->children)
        n->Accept(this);


    as->EndBlock();
}



void ASTDispather6502::dispatch(NodeBlock *node)
{
    node->DispatchConstructor();

    // In case memory block is acive
    //as->EndMemoryBlock();
    as->PushBlock(node->m_currentLineNumber);



    bool blockLabel = false;
    bool blockProcedure = false;
    bool hasLabel = false;

    QString label = as->NewLabel("block");


    if (node->m_decl.count()!=0) {
        as->Asm("jmp " + label);
        hasLabel = true;
        //           as->PushBlock(m_decl[0]->m_op.m_lineNumber-1);
    }
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
 //   as->EndMemoryBlock();
    if (!blockLabel && hasLabel)
        as->Label(label);
    if (node->forceLabel!="")
        as->Label(node->forceLabel);

    if (node->m_isMainBlock && Syntax::s.m_currentSystem == Syntax::NES)
        as->IncludeFile(":resources/code/nes_init.asm");

    if (node->m_compoundStatement!=nullptr)
        node->m_compoundStatement->Accept(this);


    as->PopCounter(node->m_op.m_lineNumber-1);
    as->PopBlock(node->m_currentLineNumber);
    if (node->m_isMainBlock && Syntax::s.m_currentSystem == Syntax::NES)
        as->IncludeFile(":resources/code/nes_end.asm");

    node->PopZeroPointers(as);

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
    node->DispatchConstructor();

    int ret = node->MaintainBlocks(as);

    if (ret==3) node->m_curMemoryBlock=nullptr;
    if (as->m_currentBlock!=nullptr) {
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
    node->ExecuteSym(as->m_symTab);

    NodeVar* v = (NodeVar*)node->m_varNode;
    NodeVarType* t = (NodeVarType*)node->m_typeNode;

    //        v->m_op.m_type =t->m_op.m_type;
    //v->m_type = t;

    if (t->m_op.m_type==TokenType::ARRAY) {
        as->DeclareArray(v->value, t->m_arrayVarType.m_value, t->m_op.m_intVal, t->m_data, t->m_position);
        //qDebug() << "IS: " << TokenType::types[as->m_symTab->Lookup(v->value)->getTokenType()];
        node->m_dataSize=t->m_op.m_intVal;
        as->m_symTab->Lookup(v->value, node->m_op.m_lineNumber)->m_type="address";
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
        if (node->m_curMemoryBlock!=nullptr)
            ErrorHandler::e.Error("IncBin can not be declared within a user-defined memory block :",node->m_op.m_lineNumber);

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
        if (node->m_curMemoryBlock!=nullptr)
            ErrorHandler::e.Error("Pointers can not be declared within a user-defined memory block :",node->m_op.m_lineNumber);
        DeclarePointer(node);
    }else {
        node->m_dataSize=1;
        if (t->value.toLower()=="integer") node->m_dataSize = 2;
        as->DeclareVariable(v->value, t->value, t->initVal);
    }

    if (node->m_curMemoryBlock!=nullptr) {
        node->m_curMemoryBlock->m_end+=node->m_dataSize;
    }

}


void ASTDispather6502::IncSid(NodeVarDecl *node) {

    // Init address or load address? hmmm


    QString pos = QString::number(node->sid.m_loadAddress,16);
    Appendix app("$"+pos);

    app.Append("org $" +pos,1);
    //        as->Appendix(v->value,0);
    app.Append("incbin \"" + as->m_projectDir + node->sid.m_outFile + "\"",1);

    as->m_appendix.append(app);

    int size=0;
    QFile f(as->m_projectDir + node->sid.m_outFile);
    if (f.open(QIODevice::ReadOnly)){
        size = f.size();  //when file does open.
        f.close();
    }
    node->m_fileSize = size;

    as->blocks.append(new MemoryBlock(node->sid.m_loadAddress,node->sid.m_loadAddress+size, MemoryBlock::MUSIC, node->sid.m_fileName));


}

void ASTDispather6502::IncBin(NodeVarDecl *node) {
    NodeVar* v = (NodeVar*)node->m_varNode;
    NodeVarType* t = (NodeVarType*)node->m_typeNode;
    QString filename = as->m_projectDir + "/" + t->m_filename;
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
        //            qDebug() << "bin: "<<v->value << " at " << t->m_position;
        Appendix app(t->m_position);

        Symbol* typeSymbol = as->m_symTab->Lookup(v->value, node->m_op.m_lineNumber);
        typeSymbol->m_org = Util::C64StringToInt(t->m_position);
        typeSymbol->m_size = size;
        //            qDebug() << "POS: " << typeSymbol->m_org;
        app.Append("org " +t->m_position,1);
        app.Append(v->value,0);
        app.Append("incbin \"" + filename + "\"",1);
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

void ASTDispather6502::BinaryClause(Node *node)
{

    as->Comment("Binary clause: " + node->m_op.getType());

    BuildToCmp(node);

    QString lblFailed = as->NewLabel("binaryclausefailed");
    QString lblFinished = as->NewLabel("binaryclausefinished");


    if (node->m_op.m_type==TokenType::EQUALS)
        as->Asm("bne " + lblFailed);
    if (node->m_op.m_type==TokenType::NOTEQUALS)
        as->Asm("beq " + lblFailed);
    if (node->m_op.m_type==TokenType::GREATER)
        as->Asm("bcc " + lblFailed);
    if (node->m_op.m_type==TokenType::LESS)
        as->Asm("bcs " + lblFailed);

    as->Asm("lda #1; success");
    as->Asm("jmp " + lblFinished);
    as->Label(lblFailed);
    as->Asm("lda #0 ; failed state");
    as->Label(lblFinished);

    as->PopLabel("binaryclausefailed");
    as->PopLabel("binaryclausefinished");
    // as->PopLabel("binary_clause_temp_var");
    //  as->PopLabel("binary_clause_temp_lab");

}

void ASTDispather6502::BuildToCmp(Node *node)
{
    QString b="";

    NodeVar* varb = dynamic_cast<NodeVar*>(node->m_right);
    if (varb!=nullptr && varb->m_expr==nullptr)
        b = varb->value;

    NodeNumber* numb = dynamic_cast<NodeNumber*>(node->m_right);
    if (numb!=nullptr)
        b = numb->StringValue();

    node->m_left->Accept(this);
    as->Term();
    if (b!="") {
        as->Comment("Compare with pure num / var optimization");
        as->Asm("cmp " + b);
    }
    else {
        // Perform a full compare : create a temp variable
        QString tmpVar = as->StoreInTempVar("binary_clause_temp");
        node->m_right->Accept(this);
        as->Term();
        as->Asm("cmp " + tmpVar);
        as->PopTempVar();
    }


}

void ASTDispather6502::BuildSimple(Node *node, QString lblFailed)
{

    as->Comment("Binary clause Simplified: " + node->m_op.getType());
    //    as->Asm("pha"); // Push that baby

    BuildToCmp(node);

    if (node->m_op.m_type==TokenType::EQUALS)
        as->Asm("bne " + lblFailed);
    if (node->m_op.m_type==TokenType::NOTEQUALS)
        as->Asm("beq " + lblFailed);
    if (node->m_op.m_type==TokenType::GREATER)
        as->Asm("bcc " + lblFailed);
    if (node->m_op.m_type==TokenType::LESS)
        as->Asm("bcs " + lblFailed);



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
        lo = varb->value;
        hi = varb->value+ "+1";
    }

    //m_left->Build(as);
    //as->Term();

    if (numb!=nullptr || varb!=nullptr) {
        if (node->m_op.m_type==TokenType::GREATER) {
            as->Comment("Compare INTEGER with pure num / var optimization");
            as->Asm("lda " + vara->value + "+1   ; compare high bytes");
            as->Asm("cmp " + hi + " ;keep");
            as->Asm("bcc " + lbl2);
            as->Asm("bne " + lbl1);
            as->Asm("lda " + vara->value);
            as->Asm("cmp " + lo);
            as->Asm("bcc " + lbl2);
        }
        if (node->m_op.m_type==TokenType::LESS) {
            as->Comment("Compare INTEGER with pure num / var optimization");
            as->Asm("lda " + vara->value + "+1   ; compare high bytes");
            as->Asm("cmp " + hi + " ;keep");
            as->Asm("bcc " + lbl1);
            as->Asm("bne " + lbl2);
            as->Asm("lda " + vara->value);
            as->Asm("cmp " + lo);
            as->Asm("bcs " + lbl2);
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


/*
 *
 *
 *  NODE BINARY CLAUSE
 *
 */

void ASTDispather6502::dispatch(NodeBinaryClause *node)
{
    node->DispatchConstructor();

    //    node->accept(this);
    //    Node::Build(as);

    // First, check the byte
    if (node->m_op.m_type==TokenType::AND || node->m_op.m_type == TokenType::OR)
        LogicalClause(node);
    else
        if (node->m_op.m_type==TokenType::LESS || node->m_op.m_type == TokenType::GREATER ||
                node->m_op.m_type==TokenType::EQUALS || node->m_op.m_type == TokenType::NOTEQUALS) {
            if (node->m_left->getType(as)==TokenType::INTEGER) {
                BinaryClauseInteger(node);
            }
            else
                BinaryClause(node);
        }
        else
            ErrorHandler::e.Error("Binary clause: Unknown compare token : " + node->m_op.getType());

}



void ASTDispather6502::LogicalClause(Node *node)
{
    if (dynamic_cast<NodeBinaryClause*>(node->m_left)==nullptr)
        ErrorHandler::e.Error("Logical clause: left hand term must be binary clause");

    if (dynamic_cast<NodeBinaryClause*>(node->m_right)==nullptr)
        ErrorHandler::e.Error("Logical clause: right hand term must be binary clause");




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

void ASTDispather6502::Compare(NodeForLoop *node, NodeVar* var, bool isLarge, QString loopDone) {


//    if (!var->isWord(as))
    {
        as->ClearTerm();
        node->m_b->Accept(this);
        as->Term();
//        as->Asm("clc");
        as->Asm("cmp " + as->m_stack["for"].current());

        if (!isLarge) {
            as->Asm("bne "+as->getLabel("for"));
            //as->Asm("bcs "+as->getLabel("for"));
        }
        else {
            as->Asm("beq "+loopDone);

        }
        return;
    }


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



/*    LDA NUM1H  ; compare high bytes
             CMP NUM2H
             BCC LABEL2 ; if NUM1H < NUM2H then NUM1 < NUM2
             BNE LABEL1 ; if NUM1H <> NUM2H then NUM1 > NUM2 (so NUM1 >= NUM2)
             LDA NUM1L  ; compare low bytes
             CMP NUM2L
             BCC LABEL2 ; if NUM1L < NUM2L then NUM1 < NUM2
*/

    as->PopLabel("forWordLabel1");
//    as->PopLabel("forWordVar");
}

void ASTDispather6502::IncreaseCounter(NodeForLoop *node, NodeVar* var) {
    if (node->m_step==nullptr) {
            if (!var->isWord(as)) {
                as->Asm("inc " + as->m_stack["for"].current());
            }
            else {
                as->Asm("clc");
                as->Asm("inc " + as->m_stack["for"].current());
                QString lbl = as->NewLabel("lblCounterWord");
                as->Asm("bcc "+lbl);
                as->Asm("inc " + as->m_stack["for"].current() +"+1");

                as->Label(lbl);

                as->PopLabel("lblCounterWord");

            }
            return;
    }
        if (node->m_step->isMinusOne()) {
//                as->Asm("dec " + as->m_stack["for"].current());
                if (!var->isWord(as)) {
                    as->Asm("dec " + as->m_stack["for"].current());
                }
                else {
                    as->Asm("clc");
                    as->Asm("dec " + as->m_stack["for"].current());
                    QString lbl = as->NewLabel("lblCounterWord");
                    as->Asm("bcc "+lbl);
                    as->Asm("dec " + as->m_stack["for"].current() +"+1");

                    as->Label(lbl);

                    as->PopLabel("lblCounterWord");

                }
                return;


        }
 //       if (node->m_loopCounter!=0)
   //         ErrorHandler::e.Error("Error: Loop with step other than 1,-1 cannot have loopy/loopx flag");
        // Is word
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

void ASTDispather6502::SmallLoop(NodeForLoop *node, NodeVar* var)
{
    QString loopDone = as->NewLabel("forLoopDone");
    //  Compare(as);
    //  as->Asm("beq "+loopDone);

    node->m_block->Accept(this);
    as->m_stack["for"].pop();
    IncreaseCounter(node,var);
    Compare(node,var,false, loopDone);

//    as->Asm("jmp " + as->getLabel("for"));

    as->Label(loopDone);

    as->m_labelStack["for"].pop();
    as->m_labelStack["forLoopDone"].pop();

}


void ASTDispather6502::LargeLoop(NodeForLoop *node, NodeVar* var)
{
    QString loopForFix = as->NewLabel("forLoopFix");
    QString loopDone = as->NewLabel("forLoopDone");

    /*    Compare(as);
        as->Asm("bne "+loopForFix);
        as->Asm("jmp "+loopDone);*/
    as->Label(loopForFix);
    node->m_block->Accept(this);
    //        m_block->Build(as);
    //    as->EndForLoop(m_b);
    as->m_stack["for"].pop();

    IncreaseCounter(node,var);
    Compare(node,var,true,loopDone);
    //as->Asm("beq "+loopDone);

    as->Asm("jmp " + as->getLabel("for"));

    as->Label(loopDone);

    as->m_labelStack["for"].pop();
    as->m_labelStack["forLoopFix"].pop();
    as->m_labelStack["forLoopDone"].pop();

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
    node->DispatchConstructor();


    as->PushCounter();
    QString labelStartOverAgain = as->NewLabel("while");
    QString lblstartTrueBlock = as->NewLabel("ConditionalTrueBlock");

    QString labelElse = as->NewLabel("elseblock");
    QString labelElseDone = as->NewLabel("elsedoneblock");
    // QString labelFailed = as->NewLabel("conditionalfailed");

    if (node->m_isWhileLoop)
        as->Label(labelStartOverAgain);

    // Test all binary clauses:
    bool isSimplified = false;
    NodeBinaryClause* bn = dynamic_cast<NodeBinaryClause*>(node->m_binaryClause);
    if (node->verifyBlockBranchSize(as, node->m_block)) {
        isSimplified = !bn->cannotBeSimplified(as);
    }

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
        node->m_binaryClause->Accept(this);
        // Now, a should be either true or false

        as->Asm("cmp #1");
        as->Asm("beq " + lblstartTrueBlock); // All conditionals checked out!
        // Do we have an else block?
        if (node->m_elseBlock!=nullptr)
            as->Asm("jmp " + labelElse); // All conditionals false: skip to end (or else block)
        as->Asm("jmp " + labelElseDone);
    }
    else {
        // Simplified version <80 instructions & just one clause

        QString failedLabel = labelElseDone;
        if (node->m_elseBlock!=nullptr)
            failedLabel = labelElse;
        BuildSimple(bn,  failedLabel);
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
    if (node->m_elseBlock!=nullptr) {
        as->Label(labelElse);
        node->m_elseBlock->Accept(this);
//        m_elseBlock->Build(as);

    }
    as->Label(labelElseDone); // Jump here if not

    as->PopLabel("while");
    as->PopLabel("ConditionalTrueBlock");
    as->PopLabel("elseblock");
    as->PopLabel("elsedoneblock");
//    as->PopLabel("conditionalfailed");

    as->PopCounter(node->m_op.m_lineNumber);

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
    node->DispatchConstructor();


    //QString m_currentVar = ((NodeAssign*)m_a)->m_
    NodeAssign *nVar = dynamic_cast<NodeAssign*>(node->m_a);


    if (nVar==nullptr)
        ErrorHandler::e.Error("Index must be variable", node->m_op.m_lineNumber);

    QString var = dynamic_cast<NodeVar*>(nVar->m_left)->value;//  m_a->Build(as);
    node->m_a->Accept(this);
    if (node->m_loopCounter==1) as->Asm("tax");
    if (node->m_loopCounter==2) as->Asm("tay");
    //QString to = m_b->Build(as);
    QString to = "";
    if (dynamic_cast<const NodeNumber*>(node->m_b) != nullptr)
        to = QString::number(((NodeNumber*)node->m_b)->m_val);
    if (dynamic_cast<const NodeVar*>(node->m_b) != nullptr)
        to = ((NodeVar*)node->m_b)->value;
  //  if (m_b->m_op.m_type==TokenType::INTEGER ||m_b->m_op.m_type==TokenType::INTEGER_CONST )
  //      to = "#" + to;
//    as->StartForLoop(var, to);

    as->m_stack["for"].push(var);
//    as->m_labelStack["for"].push();
    as->Label(as->NewLabel("for"));


    bool isSmall = node->verifyBlockBranchSize(as, node->m_block);

    if (node->m_forcePage == 1)
        isSmall = false;

    if (node->m_forcePage == 2)
        isSmall = true;

    if (isSmall)
        SmallLoop(node,dynamic_cast<NodeVar*>(nVar->m_left));
    else
        LargeLoop(node,dynamic_cast<NodeVar*>(nVar->m_left));


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
    as->Asm(m+  "(" + node->value+"),y");

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

    node->DispatchConstructor();

    QString  val = node->value;
    Pmm::Data::d.lineNumber = node->m_op.m_lineNumber;
    Symbol* s = as->m_symTab->Lookup(node->value, node->m_op.m_lineNumber);
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
        if (s->getTokenType()==TokenType::POINTER && as->m_term=="")
            isOK = false;
        if ((s->getTokenType()==TokenType::ADDRESS || s->getTokenType()==TokenType::INCBIN)  && as->m_term=="") {
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
    NodeVar* var = dynamic_cast<NodeVar*>(other);
    NodeNumber* num = dynamic_cast<NodeNumber*>(other);
    QString operand = "ldx ";
    if (!isx) operand="ldy ";
    if (var!=nullptr && var->m_expr == nullptr) {
        as->Asm(operand + var->value);
        return true;
    }
    if (num!=nullptr) {
        as->Asm(operand  + num->StringValue());
        return true;
    }
    return false;

}

void ASTDispather6502::LoadByteArray(NodeVar *node) {
    // Optimizer: if expression is number, just return direct
    as->Comment("Load Byte array");
    QString m = as->m_term;

    as->ClearTerm();
    if (!LoadXYVarOrNum(node, node->m_expr,true))
    {
        //       as->Asm("pha");
        node->m_expr->Accept(this);
        as->Term();
        as->Asm("tax");
        //          as->Asm("pla");
    }
    if (m=="")
        m="lda ";
    as->Asm(m+  node->value+",x");

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
    TokenType::Type t = as->m_symTab->Lookup(node->value, node->m_op.m_lineNumber)->getTokenType();
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
            as->Asm("lda " +node->value);
        }
        return;
    }
    if (t == TokenType::INTEGER) {
        node->m_isWord = true;
        as->Comment("Integer assignment in nodevar");
        as->Asm("ldy " +node->value);
        as->Asm("lda " +node->value+"+1");
        return;
    }
    ErrorHandler::e.Error(TokenType::getType(t) + " assignment not supported yet for exp: " + node->value);
    return;
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
    as->m_symTab->Lookup(node->value, node->m_op.m_lineNumber);
    //          ErrorHandler::e.Error("Could not find variable '" +value +"' for storing.", m_op.m_lineNumber);

    // Is array
    if (node->m_expr != nullptr) {
        NodeNumber* number = dynamic_cast<NodeNumber*>(node->m_expr);
        if (number!=nullptr && node->getType(as)!=TokenType::POINTER) { // IS NUMBER optimize}
            as->Asm("sta " + node->value + "+"+ QString::number(number->m_val));
            return;
        }
        else {
            //if regular array

            NodeVar* var = dynamic_cast<NodeVar*>(node->m_expr);
            NodeNumber* num = dynamic_cast<NodeNumber*>(node->m_expr);

            //                qDebug() << "Var name: " << value;
            QString secondReg="x";
            QString pa = "";
            QString pb= "";
            if (node->getType(as)==TokenType::POINTER) {
                secondReg="y";
                pa="(";
                pb=")";
            }

            // Optimize for number or pure var
            if (node->m_expr->getType(as)==TokenType::INTEGER_CONST || var!=nullptr) {
                as->ClearTerm();
                as->Term("ld"+secondReg +" ");
                node->m_expr->Accept(this);
                as->Term();
                as->Asm("sta " +pa + node->value+ pb + "," + secondReg);

                return;
            }
            // Just regular var optimize
            // Regular expression
            as->Asm("pha");
            as->ClearTerm();
            node->m_expr->Accept(this);
            //                node->m_expr->Build(as);
            as->Term();
            as->Asm("ta" + secondReg);
            as->Asm("pla");
            as->Asm("sta " +pa + node->value+pb+","+ secondReg);

        }
        return;
    }
    else {
        // Not array
        if (as->m_symTab->Lookup(node->value, node->m_op.m_lineNumber)->getTokenType() == TokenType::BYTE) {

            as->Asm("sta " + node->value);
            return;
        }
        if (as->m_symTab->Lookup(node->value, node->m_op.m_lineNumber)->getTokenType() == TokenType::ADDRESS) {

            as->Asm("sta " + node->value);
            return;
        }
        if (as->m_symTab->Lookup(node->value, node->m_op.m_lineNumber)->getTokenType() == TokenType::INTEGER) {
            as->Asm("sta " + node->value);
            as->Asm("sty " + node->value + "+1");
            return;
        }

    }

}

void ASTDispather6502::AssignString(NodeAssign *node) {

    NodeString* right = (NodeString*)dynamic_cast<const NodeString*>(node->m_right);
    NodeVar* left = (NodeVar*)dynamic_cast<const NodeVar*>(node->m_left);
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

void ASTDispather6502::AssignPointer(NodeAssign *node) {
    NodeVar* bVar = dynamic_cast<NodeVar*>(node->m_right);
    NodeNumber* bNum = dynamic_cast<NodeNumber*>(node->m_right);
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
        as->Term();
        node->m_right->Accept(this);
        as->Term();
     //  as->Comment(";end");
        as->Asm("sta " + aVar->value);
        as->Asm("sty "+ aVar->value+"+1");
        return;
    }




    if (bVar!=nullptr) {

        if (bVar->getType(as)!=TokenType::POINTER) {
//            if (bVar->m_op.m_type==TokenType::ADDRESS) {
                as->Asm("lda #<" + bVar->value);
                as->Asm("ldx #>" + bVar->value);
                as->Asm("sta " + aVar->value);
                as->Asm("stx "+ aVar->value+"+1");
                return;
  //          }
        }
        else
        {
            as->Asm("lda " + bVar->value);
            as->Asm("ldx " + bVar->value + "+1");
            as->Asm("sta " + aVar->value);
            as->Asm("stx "+ aVar->value+"+1");
            return;
        }

    }
    if (node->m_right->isPureNumeric()) {
        as->Asm("lda #" + QString::number(((int)node->m_right->numValue()) & 255));
        as->Asm("ldx #" + QString::number(((int)node->m_right->numValue()>>8) & 255) );
        as->Asm("sta " + aVar->value);
        as->Asm("stx "+ aVar->value+"+1");
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
    NodeNumber* rrnum = dynamic_cast<NodeNumber*>(rterm->m_right);

    if (rrvar==nullptr && rrnum==nullptr)
        return false;

    if (!(rterm->m_op.m_type==TokenType::PLUS || rterm->m_op.m_type==TokenType::MINUS))
        return false;


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
//    exit(1);
//    return false;
    if (var->isWord(as) && !rterm->m_right->isWord(as)) {
        QString lbl = as->NewLabel("WordAdd");
        as->Comment("WORD optimization: a=a+b");
        //var->Accept(this);
        as->Asm("lda " + var->value + "+0");
        as->Term();
//        as->Asm("clc");
        as->BinOP(rterm->m_op.m_type);

        rterm->m_right->Accept(this);
        as->Term();
        if (rterm->m_op.m_type==TokenType::PLUS) {
            as->Asm("bcc "+lbl);
            as->Asm("inc " + var->value + "+1");
        }
        else {
            as->Asm("bcs "+lbl);
            as->Asm("dec " + var->value + "+1");
        }

        as->Label(lbl);
        as->Asm("sta " + var->value + "+0");
//        as->Asm("sty " + var->value +"+1");

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

    if (rvar->value!=var->value)
        return false;

    NodeNumber* num = dynamic_cast<NodeNumber*>(rterm->m_right);

    // If a = a + b

    if (!var->isWord(as)) {
        if (!(num!=nullptr && num->m_val==1))
            return isSimpleAeqAOpB(var, node);
    }
    else
        return isSimpleAeqAOpB16Bit(var, node);

//    if (var->isWord(as))
  //      return false;

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

        if (LoadXYVarOrNum(var, var->m_expr,true)) {
            as->Comment("Optimize byte array " + operand);

            as->Asm(operand + var->value+",x");
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
    node->DispatchConstructor();

    as->PushCounter();

    AssignVariable(node);

    as->PopCounter(node->m_op.m_lineNumber);

}



QString ASTDispather6502::AssignVariable(NodeAssign *node) {


    NodeVar* v = (NodeVar*)dynamic_cast<const NodeVar*>(node->m_left);
    //        qDebug() << "AssignVariable: " <<v->value << " : " << TokenType::getType( v->getType(as));

    NodeNumber* num = (NodeNumber*)dynamic_cast<const NodeNumber*>(node->m_left);

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
    Symbol* s = as->m_symTab->Lookup(v->value, node->m_op.m_lineNumber, v->isAddress());
    //        if (s==nullptr)
    //          ErrorHandler::e.Error("Could not find variable :" + v->value,m_op.m_lineNumber);



    TokenType::Type t = s->getTokenType();
    //qDebug() << TokenType::getType(m_left->getType(as)) << " " << v->m_expr;

    if (node->m_left->getType(as)==TokenType::POINTER && v->m_expr==nullptr) {
        AssignPointer(node);
        return v->value;
    }

    if ((NodeString*)dynamic_cast<const NodeString*>(node->m_right))
    {
        AssignString(node);
        return v->value;
    }
    if (node->m_right==nullptr)
        ErrorHandler::e.Error("Node assign: right hand must be expression", node->m_op.m_lineNumber);
    if (node->m_left->getType(as)==TokenType::INTEGER) {
        as->Asm("ldy #0");
        node->m_right->m_forceType = TokenType::INTEGER; // FORCE integer on right-hand side
    }
    // For constant i:=i+1;
    if (IsSimpleIncDec(v,  node))
        return v->value;

    node->m_right->Accept(this);

    as->Term();
    StoreVariable(v);

     return v->value;
}
