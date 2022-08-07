
#include "codegen_tripe.h"

CodeGenTRIPE::CodeGenTRIPE()
{

}

/*
 *
 *  NODE BINOP dispatcher
 *
 *  */



void CodeGenTRIPE::HandleGenericBinop(QSharedPointer<Node> node) {


}

void CodeGenTRIPE::Doublette(Assembler *as, QSharedPointer<Node> a, QSharedPointer<Node> b, QString cmd)
{
    QString l = "";
    if (a->isPure())
        l = TripeValue(as,a);
    else {
        a->Accept(this);
        l = m_curTemp.pop();
    }

    QString r = "";
    if (b->isPure()) {
        r = TripeValue(as,b);
    }
    else {
        b->Accept(this);
        r = m_curTemp.pop();
    }
    as->Asm(cmd+"\t"+ l+"\t"+r);

}


void CodeGenTRIPE::dispatch(QSharedPointer<NodeBinOP>node)
{

    node->DispatchConstructor(as,this);


    QString v = getTempName("t_"+getIntType(as,node)+"_");
    if (node->m_left->isWord(as) && !node->m_right->isWord(as))
        node->m_right->setForceType(TokenType::INTEGER);
    as->ClearTerm();
    as->BinOP(node->m_op.m_type,true);
    QString cmd = as->m_term + "\t"+v;
    as->ClearTerm();

//    as->Asm(s+"\t"+ v+"\t"+ l+"\t"+r);
    Doublette(as, node->m_left, node->m_right,cmd);
   // return v;


}



/*
 *
 *  NODE NUMBER dispatcher
 *
 *  */

void CodeGenTRIPE::dispatch(QSharedPointer<NodeNumber>node)
{
    node->DispatchConstructor(as,this);

}
/*
 *
 *  SMALL NODE dispatcherS
 *
 *  */






void CodeGenTRIPE::dispatch(QSharedPointer<Node> node)
{
    node->DispatchConstructor(as,this);
    node->m_currentLineNumber = node->m_op.m_lineNumber;


}






void CodeGenTRIPE::dispatch(QSharedPointer<NodeVarType> node)
{
    node->DispatchConstructor(as,this);

}

void CodeGenTRIPE::dispatch(QSharedPointer<NodeBinaryClause> node)
{
    node->DispatchConstructor(as,this);

}

void CodeGenTRIPE::dispatch(QSharedPointer<NodeString> node)
{
    node->DispatchConstructor(as,this);
//    exit(1);
    if (node->m_val.length()>=1 && node->m_val[0].length()>=1) {
        as->ClearTerm();

//        CStringItem it  = ((AsmTRIPE*)as)->m_cstr[QString(node->m_val[0][0]) ];
//        as->Asm("lda #"+QString::number(it.m_screenCode));
        as->Term();
        return;
    }
    as->String(node->m_val,true);
}

void CodeGenTRIPE::DeclarePointer(QSharedPointer<NodeVarDecl> node) {

    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);
    QString initVal = t->initVal;

//    qDebug() << "POINTER INIT VAL " <<initVal;
    if (initVal=="" && Syntax::s.m_currentSystem->useZeroPagePointers()) {
        initVal = as->PushZeroPointer();
        node->m_pushedPointers++;
//        as->Asm(".data "+Syntax::s.m_currentSystem->getTripePointerType()+":"+Util::numToHex0(m_curZP));
 //       m_curZP+=2;

    }

    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
//    as->Asm(".data uint64: "+initVal);
    as->Asm("declptr\t"+v->value + "\t uint16:0");

}


void CodeGenTRIPE::dispatch(QSharedPointer<NodeVarDecl> node)
{
    node->DispatchConstructor(as,this);


    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);

    QSharedPointer<Appendix> old = as->m_currentBlock;

    if (t->m_flags.contains("wram")) {
        as->m_currentBlock = as->m_wram;
    }

    if (t->m_flags.contains("bank")) {
        QString bnk = t->m_flags[t->m_flags.indexOf("bank")+1];//Banks always placed +1
        if (!as->m_banks.contains(bnk)) {
            as->m_banks[bnk] = QSharedPointer<Appendix>(new Appendix());
            if (Syntax::s.m_currentSystem->m_system==AbstractSystem::MEGA65)
                as->m_banks[bnk]->m_pos = "$0000";
            else
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
    if (t->m_op.m_type==TokenType::INCSID || t->m_op.m_type==TokenType::INCNSF) {
//        IncSid(node);
        return;
    }
    //  qDebug() << as->m_currentBlock;
    as->m_currentBlock = old;



}







QString CodeGenTRIPE::getIncbin() {
    return ".incbin";
}

void CodeGenTRIPE::PrintCompare(QSharedPointer<Node> node, QString lblSuccess, QString lblFailed)
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
void CodeGenTRIPE::BuildToCmp(QSharedPointer<Node> node)
{
//    Doublette(as,node->m_left,node->m_right,"cmp");

}

QString CodeGenTRIPE::TripeNumber(Assembler* as,QSharedPointer<Node> node)
{
    QString s = Util::numToHex0(node->getValueAsInt(as));
    QString t = getIntType(as, node);

    return t+":"+s;

}

QString CodeGenTRIPE::TripeValue(Assembler *as, QSharedPointer<Node> node)
{
    if (node->isPureNumeric())
        return TripeNumber(as,node);
    if (node->isPureVariable())
        return node->getValue(as);
    return node->getValue(as);

}


void CodeGenTRIPE::BuildSimple(QSharedPointer<Node> node, QString lblSuccess, QString lblFailed, bool page)
{

    as->Comment("Binary clause Simplified: " + node->m_op.getType());
    //    as->Asm("pha"); // Push that baby

    BuildToCmp(node);

    PrintCompare(node, lblSuccess,lblFailed);



}

QString CodeGenTRIPE::getReturn() {
    return "return";
}

QString CodeGenTRIPE::getCallSubroutine() {
    return "call";
}

QString CodeGenTRIPE::ProcedureEndWithoutReturn() {
    return "";

}


QString CodeGenTRIPE::getInitProcedure() {
    return "";
}





void CodeGenTRIPE::Compare(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, bool isLarge, QString loopDone, QString loopNotDone, bool inclusive) {

    Doublette(as,nodeA->m_left,nodeB,"cmp");
 //   BuildToCmp()
//    PrintCompare(nodeA->m_left, lblSuccess,lblFailed);
    as->Asm("bne "+loopNotDone);

}




 //       if (node->m_loopCounter!=0)
   //         ErrorHandler::e.Error("Error: Loop with step other than 1,-1 cannot have loopy/loopx flag");
        // Is word




void CodeGenTRIPE::LoadPointer(QSharedPointer<NodeVar> node) {
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

void CodeGenTRIPE::dispatch(QSharedPointer<NodeVar> node)
{

}




void CodeGenTRIPE::LoadByteArray(QSharedPointer<NodeVar> node) {

}

void CodeGenTRIPE::LoadVariable(QSharedPointer<Node> node)
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


void CodeGenTRIPE::LoadVariable(QSharedPointer<NodeVar> node) {

    /*        if (as->m_symTab->Lookup(value)==nullptr)
                ErrorHandler::e.Error("Could not find variable '" +value +"' for storing.",m_op.m_lineNumber);
    */
    TokenType::Type t = as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber)->getTokenType();
    QString type = as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber)->m_type;
    if (as->m_symTab->m_records.contains(type))
        t = TokenType::ADDRESS; // Working with a CLASS directly (not pointer)

    if (node->isStackVariable()) {
//        LoadStackVariable(node);
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
        return;
    }
    if (t == TokenType::INTEGER) {
        node->m_isWord = true;
        if (node->m_expr!=nullptr)
            LoadByteArray(node);
        return;
    }
    ErrorHandler::e.Error(TokenType::getType(t) + " assignment not supported yet for exp: " + getValue(node));
    return;
}


void CodeGenTRIPE::LoadVariable(QSharedPointer<NodeNumber>node)
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

void CodeGenTRIPE::LoadVariable(QSharedPointer<NodeProcedure> node)
{
    as->Asm("lda #<"+node->m_procedure->m_procName);
    Disable16bit();
    as->Asm("ldy #>"+node->m_procedure->m_procName);
    Enable16bit();
}


void CodeGenTRIPE::StoreVariable(QSharedPointer<NodeVar> node) {
    //        as->Comment("VarNode StoreVariable");
    //          ErrorHandler::e.Error("Could not find variable '" +value +"' for storing.", m_op.m_lineNumber);



}


void CodeGenTRIPE::AssignString(QSharedPointer<NodeAssign> node) {

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


}
/*QString CodeGenTRIPE::BinopTemp(Assembler *as, QSharedPointer<Node> node)
{

}
*/


bool CodeGenTRIPE::IsSimpleAssignInteger(QSharedPointer<NodeAssign> node) {
    if (!node->m_left->hasArrayIndex()) {

        if (node->m_right->isPure()) {
            as->Asm("mov "+TripeValue(as,node->m_left)+" "+TripeValue(as,node->m_right));
        }
        else if (qSharedPointerDynamicCast<NodeBinOP>(node->m_right)!=nullptr) {
//            ErrorHandler::e.Error("BinOp not yet supported",node->m_op.m_lineNumber);
            //QString tempVar = BinopTemp(as,node->m_right);
            node->m_right->Accept(this);
            QString tempVar = m_curTemp.pop();
            as->Asm("mov "+TripeValue(as,node->m_left)+" "+tempVar);

        }
        return true;
    }

    return false;
}



bool CodeGenTRIPE::AssignPointer(QSharedPointer<NodeAssign> node) {

    auto var = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    if (var->isPointer(as) && var->hasArrayIndex()) {
        if (node->m_right->isPure() && var->m_expr->isPure()) {
            as->Asm("store_p "+TripeValue(as,var)+" "+TripeValue(as,var->m_expr) + " " +TripeValue(as, node->m_right));

        }
        else {
            ErrorHandler::e.Error("Tripe: non-pure pointer index not yet supported",node->m_op.m_lineNumber);
        }
    }

    return false;

}





void CodeGenTRIPE::dispatch(QSharedPointer<NodeRepeatUntil> node)
{


}

void CodeGenTRIPE::dispatch(QSharedPointer<NodeComment> node)
{
    node->DispatchConstructor(as,this);


}

QString CodeGenTRIPE::resolveTemporaryClassPointer(QString name, int mul, int& res)
{
    return "";
}

QString CodeGenTRIPE::getIntType(Assembler *as, QSharedPointer<Node> node) {
    QString t = "uint8";
    if (node->isWord(as))
        t = "uint16";
    else
        if (node->isByte(as))
            t = "uint8";
    if (node->isLong(as))
        t = "uint32";
    return t;
}

QString CodeGenTRIPE::getTempName(QString t)
{
    int i=1;
    QString s = t+QString::number(i);
    while (m_curTemp.contains(s))
        s = t+QString::number(++i);

    m_curTemp.push(s);
    QString name = "decl\t"+s+"\t"+t.split("_")[1]+":0";
    if (!as->m_tempVars.contains(name))
       as->m_tempVars.append(name);
    return s;

}






void CodeGenTRIPE::AssignFromRegister(QSharedPointer<NodeAssign> node)
{
}

void CodeGenTRIPE::AssignToRegister(QSharedPointer<NodeAssign> node)
{
}

void CodeGenTRIPE::OptimizeBinaryClause(QSharedPointer<Node> node, Assembler* as)
{
}

void CodeGenTRIPE::dispatch(QSharedPointer<NodeUnaryOp> node)
{

}



void CodeGenTRIPE::CompareAndJumpIfNotEqualAndIncrementCounter(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive)
{
    /*    if (!isOffPage)
        SmallLoop(node,qSharedPointerDynamicCast<NodeVar>(nVar->m_left), inclusive);
    else
        LargeLoop(node,qSharedPointerDynamicCast<NodeVar>(nVar->m_left), inclusive);

*/

    if (!isOffPage) {
        QString loopDone = as->NewLabel("loopdone");
//        as->Comment("Compare is onpage");

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


void CodeGenTRIPE::CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QString lblJump, bool isOffPage)
{
    Doublette(as,nodeA,nodeB,"cmp");
    as->Asm("bne " +lblJump);
    return;
}




bool CodeGenTRIPE::StoreVariableSimplified(QSharedPointer<NodeAssign> assignNode)
{
    return false;
}

bool CodeGenTRIPE::StoreStackParameter(QSharedPointer<NodeAssign> n)
{


    return false;

}
