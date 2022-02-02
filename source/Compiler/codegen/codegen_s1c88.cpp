#include "codegen_s1c88.h"


CodeGenS1C88::CodeGenS1C88()
{

}

void CodeGenS1C88::PrintBop(TokenType::Type type, QString x0, QString x1, QString value, bool is16bit)
{
    QString a = "";

    if (type==TokenType::Type::PLUS)
        Binop("add",x0,x1,value);
    if (type==TokenType::Type::MINUS)
        Binop("sub",x0,x1,value);
/*    if (type==TokenType::Type::MUL)
        Binop("mul",x0,x1,value);
    if (type==TokenType::Type::DIV)
        Binop("div",x0,x1,value);*/
    if (type==TokenType::Type::BITOR)
        Binop("or",x0,x1,value);
    if (type==TokenType::Type::XOR)
        Binop("xor",x0,x1,value);
    if (type==TokenType::Type::BITAND)
        Binop("and",x0,x1,value);

}



void CodeGenS1C88::dispatch(QSharedPointer<NodeBinOP> node)
{
    as->Comment("Binary operation of type: "+TokenType::getType(node->m_op.m_type));
    if (node->isWord(as)) {
//        node->SwapNodes();
        node->m_left->Accept(this);
        QString a = m_reg16.Get();
        m_reg16.Push();
        node->m_right->Accept(this);
        QString b = m_reg16.Get();
        m_reg16.Pop();
        PrintBop(node->m_op.m_type,a,b,"",true);

    }
    else {
        node->m_left->Accept(this);
        QString a = m_reg8.Get();
        m_reg8.Push();
        QString b = m_reg8.Get();
        node->m_right->Accept(this);
        m_reg8.Pop();
        PrintBop(node->m_op.m_type,a,b,"",false);

    }

}

void CodeGenS1C88::dispatch(QSharedPointer<NodeNumber>node)
{
    if (!node->isWord(as)) {
        QString ax = m_reg8.Get();
        as->Asm("movb "+ax+", " + node->getValue(as));
    }
    else
    {
        QString ax = m_reg16.Get();
        as->Asm("movw "+ax+", " +node->getValue(as));
    }
}

void CodeGenS1C88::dispatch(QSharedPointer<NodeVar> node)
{
    if (m_inlineParameters.contains(node->value)) {
  //      qDebug()<< "INLINE node override : "<< node->value;
        m_inlineParameters[node->value]->Accept(this);
        return;
    }
//    as->Comment("Is 16 bit: " +QString::number(node->isWord(as)));
    ldr(node);



}


void CodeGenS1C88::dispatch(QSharedPointer<NodeString> node)
{

}



void CodeGenS1C88::dispatch(QSharedPointer<NodeVarType> node)
{

}

void CodeGenS1C88::dispatch(QSharedPointer<NodeBinaryClause> node)
{

}


void CodeGenS1C88::dispatch(QSharedPointer<Node> node)
{

}

void CodeGenS1C88::dispatch(QSharedPointer<NodeRepeatUntil> node)
{
    ErrorHandler::e.Error("Repeat-until not implemented yet", node->m_op.m_lineNumber);

}
void CodeGenS1C88::dispatch(QSharedPointer<NodeComment> node)
{

}

void CodeGenS1C88::StoreVariable(QSharedPointer<NodeVar> n)
{

}

bool CodeGenS1C88::StoreVariableSimplified(QSharedPointer<NodeAssign> node)
{
    auto var = node->m_left;
    QString type =getWordByteType(as,var);
    if (node->m_right->isPure() && !node->m_left->isPointer(as) && !node->m_left->hasArrayIndex()) {
        as->Comment("Store variable simplified");
        if (var->isWord(as))
            node->m_right->setForceType(TokenType::INTEGER);

        node->m_right->Accept(this);
        str(var);

        return true;
    }
    return false;

}

void CodeGenS1C88::LoadVariable(QSharedPointer<NodeVar> n)
{
    n->Accept(this);
}

QString CodeGenS1C88::LoadAddress(QSharedPointer<Node> n)
{
    return "LoadAddress not implemented";
}

QString CodeGenS1C88::LoadAddress(QSharedPointer<Node> n, QString reg)
{
    return "";
}

void CodeGenS1C88::LoadVariable(QSharedPointer<NodeProcedure> node)
{
    ErrorHandler::e.Error("Procedure address not implemented yet! Please bug the developer", node->m_op.m_lineNumber);
}

void CodeGenS1C88::LoadPointer(QSharedPointer<Node> n)
{

}

void CodeGenS1C88::LoadVariable(QSharedPointer<Node> n)
{
    n->Accept(this);

}

void CodeGenS1C88::LoadVariable(QSharedPointer<NodeNumber>n)
{

}

QString CodeGenS1C88::getShift(QSharedPointer<NodeVar> var)
{
    if (var->isPointer(as) || var->m_expr!=nullptr) {
        if (var->getArrayType(as)==TokenType::LONG)
            return ",lsl 2";
        if (var->getArrayType(as)==TokenType::INTEGER)
            return ",lsl 1";
    }


    if (var->isWord(as))
        return ",lsl 1";
    if (var->isLong(as))
        return ",lsl 2";

    return "";
}

QString CodeGenS1C88::getIndexScaleVal(Assembler *as, QSharedPointer<Node> var)
{
    if (var->isWord(as))
        return "2";
    if (var->isLong(as))
        return "4";
    return "1";
}

QString CodeGenS1C88::getCallSubroutine() {
    return "call";
}


void CodeGenS1C88::ldr(QString x0, QString x1)
{
    as->Asm("mov "+x1+",["+x0+"]");
}

void CodeGenS1C88::str(QString x0, QString x1)
{
    as->Asm("mov ["+x1+"],"+x0);
}

void CodeGenS1C88::str(QSharedPointer<Node> var, QString forceReg)
{
    auto ar = m_reg16.Get();
    if (forceReg!="")
        ar = forceReg;

    if (var->isWord(as)) {
        as->Asm("movw ["+var->getValue(as)+"],"+ar);
    }
    else {
        as->Asm("movb ["+var->getValue(as)+"],"+m_reg8.Get());

    }
}

void CodeGenS1C88::ldr(QSharedPointer<Node> var, QString forceReg)
{
    auto nv = qSharedPointerDynamicCast<NodeVar>(var);
//    as->Comment("Loading var is ")
//    as->Comment("Left right : "+QString::number(var->isWord(as)));

/*    if (nv && (var->m_forceType==TokenType::INTEGER && nv->getOrgType(as)==TokenType::BYTE)) {
        QString x0 = getReg();
        QString x1 = m_regs[m_lvl+1];
        as->Asm("mw "+x0+",0");
        as->Asm("lw "+x1+",["+var->getValue(as)+"]");
        return;
    }
*/

    auto ar = m_reg16.Get();
    if (forceReg!="")
        ar = forceReg;

    if (var->isReference()) {
            as->Asm("mov "+ar+", "+var->getValue(as)+"");
        return;
    }


    if (var->hasArrayIndex()) {
        nv->m_expr->setForceType(TokenType::INTEGER);
        auto di = m_reg16.Get();
        as->Comment("Loading byte array");
        nv->m_expr->Accept(this);
        m_reg16.Push();
        auto ax = m_reg16.Get();
        m_reg16.Pop();

        if (var->getArrayType(as)==TokenType::INTEGER) {
            as->Comment("Integer array");
            as->Asm("add "+di+","+di+" ; integer array");
            if (var->isPointer(as))
                as->Asm("mov "+ax+",["+var->getValue(as)+"]");
            else
                as->Asm("mov "+ax+","+var->getValue(as));

            as->Asm("add "+ax+","+di);
            as->Asm("mov "+di+","+"["+ax+"]");
            return;
        }
        if (var->isPointer(as))
            as->Asm("mov "+ax+",["+var->getValue(as)+"]");
        else
            as->Asm("mov "+ax+","+var->getValue(as));
        as->Asm("add "+ax+","+di);
        as->Asm("mov "+m_reg8.Get()+","+"["+ax+"]");
        return;

    }

    if (var->isWord(as)) {
        if (((NodeVar*)var.get())->getOrgType(as)==TokenType::BYTE) {
            as->Asm("mov l, ["+var->getValue(as)+"]" );
            as->Asm("mov h, 0" );
            as->Asm("mov "+ar+",hl");
            return;

        }
        as->Asm("movw "+ar+", ["+var->getValue(as)+"] ; word" );
    }
    else {

        as->Asm("movb "+m_reg8.Get()+", ["+var->getValue(as)+"]");
/*        if (var->m_forceType==TokenType::INTEGER) {
            as->Asm("mov x")
        }
*/
    }

}

void CodeGenS1C88::Binop(QString bop, QString x0, QString x1, QString x2)
{
    if (x2=="")
        as->Asm(bop+" "+x0+","+x1);
    else
        as->Asm(bop+" "+x0+","+x1+","+x2);

}

QString CodeGenS1C88::BinopVariableNumber(QSharedPointer<Node> n, QString value, TokenType::Type type)
{
    return "";
}

void CodeGenS1C88::dispatch(QSharedPointer<NodeVarDecl> node)
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
 /*
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
    }*/
    if (v->m_isGlobal) {
        as->m_currentBlock = nullptr;
        return;
    }


    //    qDebug() << "" <<as->m_currentBlock;
    AbstractCodeGen::dispatch(node);
    //  qDebug() << as->m_currentBlock;
    as->m_currentBlock = old;
}




QString CodeGenS1C88::getBinaryOperation(QSharedPointer<NodeBinOP> bop) {
    if (bop->m_op.m_type == TokenType::PLUS)
        return "add";
    if (bop->m_op.m_type == TokenType::MINUS)
        return "sub";
    if (bop->m_op.m_type == TokenType::BITOR)
        return "or";
    if (bop->m_op.m_type == TokenType::BITAND)
        return "and";
    if (bop->m_op.m_type == TokenType::XOR)
        return "xor";
    if (bop->m_op.m_type == TokenType::DIV)
        return "idiv";
    if (bop->m_op.m_type == TokenType::MUL)
        return "imul";
    return " UNKNOWN BINARY OPERATION";
}


QString CodeGenS1C88::getEndType(Assembler *as, QSharedPointer<Node> v)
{
    return "";
}




void CodeGenS1C88::AssignString(QSharedPointer<NodeAssign> node) {


    QSharedPointer<NodeString> right = qSharedPointerDynamicCast<NodeString>(node->m_right);
    QSharedPointer<NodeVar> left = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    QString str = as->NewLabel("stringassignstr");
    QString lblCpy=as->NewLabel("stringassigncpy");
    bool isPointer = left->isPointer(as);
    QString strAssign = str + "\t db \"" + right->m_op.m_value + "\",0";
    as->m_tempVars<<strAssign;
    if (isPointer) {
        as->Asm("mw si, "+str+"");
        as->Asm("mw ["+left->getValue(as)+"+2], ds");
        as->Asm("mw ["+left->getValue(as)+"], si");

    }
    else {
        as->Comment("String copy!");
        if (left->isPointer(as))
            as->Asm("les di,["+left->getValue(as)+"]");
        else
            as->Asm("mw di,"+left->getValue(as)+"");
        as->Term();

        as->Asm("push ds");
        as->Asm("pop es");
        as->Asm("mw si,"+str);
        as->Asm("mw cx, "+Util::numToHex(right->m_op.m_value.count()+2));
        as->Asm("rep movsb");
    }
    as->PopLabel("stringassignstr");
    as->PopLabel("stringassigncpy");
}

bool CodeGenS1C88::AssignPointer(QSharedPointer<NodeAssign> node)
{
    if (node->m_left->isPointer(as) && node->m_left->hasArrayIndex()) {
        // Storing p[i] := something;
        auto var = qSharedPointerDynamicCast<NodeVar>(node->m_left);
        as->Comment("storing pointer");
        as->Comment("loading expression:");
        m_reg16.Push();
        auto bx = m_reg16.Get();
        var->m_expr->setForceType(TokenType::INTEGER);
        var->m_expr->Accept(this);
        m_reg16.Pop();
        auto ax = m_reg16.Get();

        as->Comment("load value:");
        as->Asm("movw "+ax+",["+var->getValue(as)+"]");
        as->Asm("add "+ax+","+bx); // add shift

        if (node->m_right->getArrayType(as)==TokenType::INTEGER) {
            ErrorHandler::e.Error("Integer pointers not implemented yet");
            //as->Asm("sw16 h,l,a,b");
        }
        else {

            as->Asm("movw "+bx+","+ax);
            node->m_right->Accept(this);
            as->Asm("movb ["+bx+"],"+m_reg8.Get());
        }

        return true;
    }
    return false;
}

void CodeGenS1C88::GenericAssign(QSharedPointer<NodeAssign> node)
{
    as->Comment("Generic assign");
    node->m_right->Accept(this);
    str(node->m_left);

}

bool CodeGenS1C88::IsAssignPointerWithIndex(QSharedPointer<NodeAssign> node)
{
    // Set pointer value
    return false;
}

bool CodeGenS1C88::IsAssignArrayWithIndex(QSharedPointer<NodeAssign> node)
{
    return false;
}

bool CodeGenS1C88::IsSimpleIncDec(QSharedPointer<NodeAssign> node)
{
    return false;
}

bool CodeGenS1C88::IsSimpleAssignPointer(QSharedPointer<NodeAssign> node)
{
    auto var = node->m_left;
    if (var->isPointer(as) && !var->hasArrayIndex()) {

        node->m_right->VerifyReferences(as);
        if (!node->m_right->isReference())
            if (!node->m_right->isPointer(as))
                if (node->m_right->isByte(as)) {
                    ErrorHandler::e.Error("Trying to assign a non-pointer / non-reference / non-long to pointer '"+var->getValue(as)+"'",var->m_op.m_lineNumber);
                }

        as->Comment("Assigning pointer");
        node->m_right->Accept(this);
        str(node->m_left);
        return true;

    }
    return false;

}

void CodeGenS1C88::OptimizeBinaryClause(QSharedPointer<Node> node, Assembler *as)
{

}

void CodeGenS1C88::AssignFromRegister(QSharedPointer<NodeAssign> node)
{
    ErrorHandler::e.Error("Cannot (yet) assign variables from registers on the S1C88",node->m_op.m_lineNumber);
}

void CodeGenS1C88::AssignToRegister(QSharedPointer<NodeAssign> node)
{
}

void CodeGenS1C88::ProcedureStart(Assembler *as) {

}

void CodeGenS1C88::ProcedureEnd(Assembler *as) {

}


void CodeGenS1C88::DeclarePointer(QSharedPointer<NodeVarDecl> node)
{
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);
//    as->Write(".align 4");
    as->Write(v->getValue(as)+ ": \n\t.dw  0",0);
  //  as->Write(".align 4");

    as->m_symTab->Lookup(v->getValue(as), node->m_op.m_lineNumber)->m_arrayType=t->m_arrayVarType.m_type;

}

QString CodeGenS1C88::getEndType(Assembler *as, QSharedPointer<Node> v1, QSharedPointer<Node> v2)
{
    return "";
}


void CodeGenS1C88::BuildSimple(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool page)
{

    as->Comment("Binary clause Simplified: " + node->m_op.getType());
    //    as->Asm("pha"); // Push that baby

    BuildToCmp(node);
    QString jg ="jg ";
    QString jl ="jl ";
    QString jge ="jge ";
    QString jle ="jle ";
/*    if (!(node->m_left->isSigned(as) || node->m_right->isSigned(as))) {
        jg = "bhi ";
        jl = "blo ";
        jge = "bhi ";
        jle = "blo ";
    }*/
    if (node->m_op.m_type==TokenType::EQUALS)
        as->Asm("jnz " + as->jumpLabel(lblFailed));
    if (node->m_op.m_type==TokenType::NOTEQUALS)
        as->Asm("jz " + as->jumpLabel(lblFailed));
    if (node->m_op.m_type==TokenType::LESS)
        as->Asm(jge  + as->jumpLabel(lblFailed));
    if (node->m_op.m_type==TokenType::GREATER)
        as->Asm(jle  + as->jumpLabel(lblFailed));

    if (node->m_op.m_type==TokenType::LESSEQUAL)
        as->Asm(jg  + as->jumpLabel(lblFailed));
    if (node->m_op.m_type==TokenType::GREATEREQUAL)
        as->Asm(jl  + as->jumpLabel(lblFailed));



}

void CodeGenS1C88::BuildToCmp(QSharedPointer<Node> node)
{
    if (!node->isWord(as)) {
        node->m_left->Accept(this);
        QString ax = m_reg8.Get();

        m_reg8.Push();
        QString bx = m_reg8.Get();
        node->m_right->Accept(this);
      //  as->Term();
        m_reg8.Pop();
        as->Asm("cmp "+ax+","+bx);

    }
    else {
        as->Asm("16 bit compare not implemented yet");
    }
//    PopReg();

}

void CodeGenS1C88::CompareAndJumpIfNotEqualAndIncrementCounter(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive)
{
    if (step!=nullptr)
        ErrorHandler::e.Error("For loops currently don't support step",nodeA->m_op.m_lineNumber);

//    QString x1 = BinopVariableNumber(nodeA->m_left,"1", TokenType::Type::PLUS);

    // variable
    auto n = nodeA->m_left;
    if (n->isWord(as)) {

        ldr(n,"hl");
        as->Asm("add hl,1");
        str(n,"hl");
    }
    else {
        ldr(n);
        as->Asm("addb a,1");
        str(n);
    }


    if (n->isWord(as)) {
        m_reg16.Push();
        nodeB->Accept(this);
        m_reg16.Pop();


        as->Asm("cmp hl,x1");
    }
    else {
        m_reg8.Push();
        auto a = m_reg8.Get();
        nodeB->Accept(this);
        m_reg8.Pop();
        auto b = m_reg8.Get();

        as->Asm("cmp "+b+","+a);

    }
    as->Asm("jnz "+lblJump);

}

void CodeGenS1C88::CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QString lblJump, bool isOffPage)
{
    if (nodeA->isWord(as)) nodeB->setForceType(TokenType::INTEGER);
    LoadVariable(nodeA);
    QString ax = m_reg8.Get();
    m_reg8.Push();
    LoadVariable(nodeB);
    QString bx = m_reg8.Get();
    m_reg8.Pop();
    as->Asm(m_cmp+ax+","+bx);
    as->Asm(m_jne+lblJump);

}

QString CodeGenS1C88::getReturn() { return "ret";}

