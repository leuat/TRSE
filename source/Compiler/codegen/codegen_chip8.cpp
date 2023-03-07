#include "codegen_chip8.h"


CodeGenChip8::CodeGenChip8()
{
}

void CodeGenChip8::PrintBop(TokenType::Type type, QString x0, QString x1, QString value, bool is16bit)
{
    QString a = "";

    if (is16bit)
        a="16";
    if (type==TokenType::Type::PLUS)
        Binop("add"+a,x0,x1,value);
    if (type==TokenType::Type::MINUS)
        Binop("sub"+a,x0,x1,value);
/*    if (type==TokenType::Type::MUL)
        Binop("mul",x0,x1,value);
    if (type==TokenType::Type::DIV)
        Binop("div",x0,x1,value);*/
    if (type==TokenType::Type::BITOR)
        Binop("or"+a,x0,x1,value);
    if (type==TokenType::Type::XOR)
        Binop("xor"+a,x0,x1,value);
    if (type==TokenType::Type::BITAND)
        Binop("and"+a,x0,x1,value);

}



void CodeGenChip8::dispatch(QSharedPointer<NodeBinOP> node)
{
    as->Comment("Binary operation of type: "+TokenType::getType(node->m_op.m_type));
    if (node->isWord(as)) {
        node->m_left->Accept(this);
        QString a = getReg();
        PushReg();
        QString b = getReg();
        PushReg();
        QString c = getReg();
        PushReg();
        QString d = getReg();
        PopReg();
        node->m_right->Accept(this);
        PopReg();
        PopReg();
        PrintBop(node->m_op.m_type,a,b,c+","+d,true);

    }
    else {
        node->m_left->Accept(this);
        QString a = getReg();
        PushReg();
        QString b = getReg();
        node->m_right->Accept(this);
        PopReg();
        PrintBop(node->m_op.m_type,a,b,"",false);

    }
}

void CodeGenChip8::dispatch(QSharedPointer<NodeNumber>node)
{
    QString ax = getReg();
    if (!node->isWord(as))
        as->Asm("ld "+ax+", " + node->getValue(as));
    else
    {
        PushReg();
        QString bx = getReg();
        as->Asm("ld "+ax+", "+bx+","+ node->getValue(as));
        PopReg();

    }
}

void CodeGenChip8::dispatch(QSharedPointer<NodeVar> node)
{
    if (m_inlineParameters.contains(node->value)) {
  //      qDebug()<< "INLINE node override : "<< node->value;
        m_inlineParameters[node->value]->Accept(this);
        return;
    }
    if (node->isReference()) {
        //for this code to work I need to add LO and HI to Wernsey's Assembler 
        as->Asm("LD V0, "+node->getValue(as));
        as->Asm("LD V1, "+node->getValue(as));
        return;

    } else if (node->m_expr!=nullptr) {
        if (node->isPointer(as)) {
            //I could make it support words, but that's for later
            node->m_expr->setForceType(TokenType::BYTE); 
            node->m_expr->Accept(this);
            QString v0_save = getReg();PushReg();
            as->Asm("LD "+ v0_save + ", V0");
            as->Asm("LD I, System_ptr");
            as->Asm("LD V1, [I]");
            as->Asm("call System_loadPointer");
            as->Asm("ADD I, "+ v0_save);

            if (node->getArrayType(as)==TokenType::INTEGER) {
                as->Asm("ADD I, " + v0_save);
                PopReg();
                as->Asm("LD V1, [I]");
                QString x0 = getReg(); PushReg();
                QString x1 = getReg(); 
                as->Asm("LD " +x0+", V0");
                as->Asm("LD " +x1+", V1");
                PopReg();
            } else {
                PopReg();
                as->Asm("LD V0, [I]");
                QString x0 = getReg();
                as->Asm("LD " +x0+", V0");
            }


            return;

        } else {
            // Regular array
            node->m_expr->setForceType(TokenType::BYTE);
            node->m_expr->Accept(this);
            as->Asm("LD I, "+node->getValue(as));
            as->Asm("ADD I, V0");
            if (node->getArrayType(as)==TokenType::INTEGER) {
                as->Asm("ADD I, V0");
                as->Asm("LD V1, [I]");
                QString x0 = getReg(); PushReg();
                QString x1 = getReg(); 
                as->Asm("LD " +x0+", V0");
                as->Asm("LD " +x1+", V1");
                PopReg();
            } else {
                as->Asm("LD V0, [I]");
                QString x0 = getReg();
                as->Asm("LD " +x0+", V0");

            }
            return;
        }
    }

    ldr(node);



}


void CodeGenChip8::dispatch(QSharedPointer<NodeString> node)
{

}



void CodeGenChip8::dispatch(QSharedPointer<NodeVarType> node)
{

}

void CodeGenChip8::dispatch(QSharedPointer<NodeBinaryClause> node)
{

}


void CodeGenChip8::dispatch(QSharedPointer<Node> node)
{

}

void CodeGenChip8::dispatch(QSharedPointer<NodeRepeatUntil> node)
{
    ErrorHandler::e.Error("Repeat-until not implemented yet", node->m_op.m_lineNumber);

}
void CodeGenChip8::dispatch(QSharedPointer<NodeComment> node)
{

}

void CodeGenChip8::StoreVariable(QSharedPointer<NodeVar> n)
{

}

bool CodeGenChip8::StoreVariableSimplified(QSharedPointer<NodeAssign> node)
{
    auto var = node->m_left;
    QString type =getWordByteType(as,var);
    if (node->m_right->isPure() && !node->m_left->isPointer(as) && !node->m_left->hasArrayIndex()) {
        as->Comment("Store variable simplified");
        if (var->isWord(as))
            node->m_right->setForceType(TokenType::INTEGER);

        node->m_right->Accept(this);
        QString x0 = getReg();
        str(var);

        return true;
    }
    return false;

}

void CodeGenChip8::LoadVariable(QSharedPointer<NodeVar> n)
{
    n->Accept(this);
}

QString CodeGenChip8::LoadAddress(QSharedPointer<Node> n)
{
    QString reg = PushReg();
    as->Asm("ld "+reg+","+n->getValue(as));
    return reg;
}

QString CodeGenChip8::LoadAddress(QSharedPointer<Node> n, QString reg)
{
    return "";
}

void CodeGenChip8::LoadVariable(QSharedPointer<NodeProcedure> node)
{
    ErrorHandler::e.Error("Procedure address not implemented yet! Please bug the developer", node->m_op.m_lineNumber);
}

void CodeGenChip8::LoadPointer(QSharedPointer<Node> n)
{

}

void CodeGenChip8::LoadVariable(QSharedPointer<Node> n)
{
    n->Accept(this);

}

void CodeGenChip8::LoadVariable(QSharedPointer<NodeNumber>n)
{

}

QString CodeGenChip8::getShift(QSharedPointer<NodeVar> var)
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

QString CodeGenChip8::getIndexScaleVal(Assembler *as, QSharedPointer<Node> var)
{
    if (var->isWord(as))
        return "2";
    if (var->isLong(as))
        return "4";
    return "1";
}

QString CodeGenChip8::getCallSubroutine() {
    return "call";
}

QString CodeGenChip8::getReg(int dd) {
    if (m_lvl+dd>=m_regs.count()) {
        qDebug() << "ERROR CodeGenChip8 reg stack error shouldn't happen!";
        exit(1);
    }
    return m_regs[m_lvl+dd];

}

void CodeGenChip8::ldr(QString x0, QString x1)
{
    as->Asm("ld I,"+x1);
    as->Asm("ld V0, [I]");
    if (x0 != "V0") as->Asm("ld "+x0+", V0");
}

void CodeGenChip8::str(QString x0, QString x1)
{
    as->Asm("ld I,"+x1);
    if (x0 != "V0") as->Asm("ld V0, "+x0);
    as->Asm("ld [I], V0");
}

void CodeGenChip8::str(QSharedPointer<Node> var)
{
    if (var->isWord(as)) {
        QString x0 = getReg(); PushReg();
        QString x1 = getReg(); 
        as->Asm("LD I, "+var->getValue(as));
        if (x0!="V0") as->Asm("ld V0, "+x0);
        if (x1!="V1") as->Asm("ld V1, "+x1);
        as->Asm("LD [I], V1");
        PopReg();

    }
    else {
        QString x0 = getReg();
        as->Asm("ld I,"+var->getValue(as));
        if (x0!="V0") as->Asm("ld V0, "+x0);
        
        as->Asm("ld [I], V0");

    }
}

void CodeGenChip8::ldr(QSharedPointer<Node> var)
{
    auto nv = qSharedPointerDynamicCast<NodeVar>(var);
//    as->Comment("Loading var is ")
//    as->Comment("Left right : "+QString::number(var->isWord(as)));

/*    if (nv && (var->m_forceType==TokenType::INTEGER && nv->getOrgType(as)==TokenType::BYTE)) {
        QString x0 = getReg();
        QString x1 = m_regs[m_lvl+1];
        as->Asm("ld I,"+x0+",0");
        as->Asm("ld "+x1+",[I]");
        return;
    }
*/
    if (var->isWord(as)) {
        QString x0 = getReg(); PushReg();
        QString x1 = getReg();
        as->Asm("ld I,"+var->getValue(as));
        as->Asm("LD V1, [I]");
        if (x0!="V0") as->Asm("ld "+x0+", V0");
        if (x1!="V1") as->Asm("ld "+x1+", V1");
        PopReg();
        
    }
    else {
        QString x0 = getReg();
        as->Asm("ld I,"+var->getValue(as));
        as->Asm("ld V0,[I]");
        if (x0!="V0") as->Asm("ld "+x0+",V0");
        

    }

}

void CodeGenChip8::Binop(QString bop, QString x0, QString x1, QString x2)
{
    if (x2=="")
        as->Asm(bop+" "+x0+","+x1);
    else
        as->Asm(bop+" "+x0+","+x1+","+x2);

}

QString CodeGenChip8::BinopVariableNumber(QSharedPointer<Node> n, QString value, TokenType::Type type)
{
    return "";
}




QString CodeGenChip8::getBinaryOperation(QSharedPointer<NodeBinOP> bop) {
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

QString CodeGenChip8::PushReg() {
    if (m_lvl==12)
        ErrorHandler::e.Error("Error in Chip8 dispatcher PopReg : trying to push regstack from max");

    m_lvl++;
    return m_regs[m_lvl];
}

void CodeGenChip8::PopReg() {
    if (m_lvl==0)
        ErrorHandler::e.Error("Error in Chip8 dispatcher PopReg : trying to pop regstack from zero");
    m_lvl--;
}

QString CodeGenChip8::getEndType(Assembler *as, QSharedPointer<Node> v)
{
    return "";
}




void CodeGenChip8::AssignString(QSharedPointer<NodeAssign> node) {


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
        as->Asm("mw cx, "+Util::numToHex(right->m_op.m_value.length()+2));
        as->Asm("rep movsb");
    }
    as->PopLabel("stringassignstr");
    as->PopLabel("stringassigncpy");
}

bool CodeGenChip8::AssignPointer(QSharedPointer<NodeAssign> node)
{
    if (node->m_left->isPointer(as) && node->m_left->hasArrayIndex()) {
        // Storing p[i] := something;
        auto var = qSharedPointerDynamicCast<NodeVar>(node->m_left);
        as->Comment("storing pointer");
        as->Comment("loading expression:");

        var->m_expr->setForceType(TokenType::INTEGER);
        var->m_expr->Accept(this);

        as->Comment("load value:");
        as->Asm("lw16 h,l,["+var->getValue(as)+"]");
        as->Asm("add16 h,l,a,b"); // add shift

        node->m_right->Accept(this);
        if (node->m_right->getArrayType(as)==TokenType::INTEGER) {
            ErrorHandler::e.Error("Integer pointers not implemented yet");
            as->Asm("sw16 h,l,a,b");
        }
        else
            as->Asm("sw h,l,a");

        return true;
    }
    return false;
}

void CodeGenChip8::GenericAssign(QSharedPointer<NodeAssign> node)
{
    as->Comment("Generic assign");
    node->m_right->Accept(this);
    str(node->m_left);

}

bool CodeGenChip8::IsAssignPointerWithIndex(QSharedPointer<NodeAssign> node)
{
    // Set pointer value
    return false;
}

bool CodeGenChip8::IsAssignArrayWithIndex(QSharedPointer<NodeAssign> node)
{
    return false;
}

bool CodeGenChip8::IsSimpleIncDec(QSharedPointer<NodeAssign> node)
{
    return false;
}

bool CodeGenChip8::IsSimpleAssignPointer(QSharedPointer<NodeAssign> node)
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

void CodeGenChip8::OptimizeBinaryClause(QSharedPointer<Node> node, Assembler *as)
{

}

void CodeGenChip8::AssignFromRegister(QSharedPointer<NodeAssign> node)
{
    ErrorHandler::e.Error("Cannot (yet) assign variables from registers on the Chip8",node->m_op.m_lineNumber);
}

void CodeGenChip8::AssignToRegister(QSharedPointer<NodeAssign> node)
{
    QString vname = getValue(node->m_left);
    vname = vname.toLower();
    if (!node->m_right->isPure())
        ErrorHandler::e.Error("When assigning registers, RHS needs to be pure numeric or variable",node->m_op.m_lineNumber);

    QString reg = vname.remove(0,1);
//        as->Comment("Assigning register : " + vname);

    as->Asm("ld "+reg+", "+getChip8Value(as,node->m_right));
    return;

}

void CodeGenChip8::ProcedureStart(Assembler *as) {

}

void CodeGenChip8::ProcedureEnd(Assembler *as) {

}

void CodeGenChip8::DeclarePointer(QSharedPointer<NodeVarDecl> node)
{
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);
//    as->Write(".align 4");
    as->Write(v->getValue(as)+ ": \n\t@dd  0",0);
  //  as->Write(".align 4");

    as->m_symTab->Lookup(v->getValue(as), node->m_op.m_lineNumber)->m_arrayType=t->m_arrayVarType.m_type;

}

QString CodeGenChip8::getEndType(Assembler *as, QSharedPointer<Node> v1, QSharedPointer<Node> v2)
{
    return "";
}


void CodeGenChip8::BuildSimple(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool page)
{

    as->Comment("Binary clause Simplified: " + node->m_op.getType());
    //    as->Asm("pha"); // Push that baby

    BuildToCmp(node);
    QString jg ="jg ";
    QString jl ="jl ";
    QString jge ="jge ";
    QString jle ="jle ";
    if (!(node->m_left->isSigned(as) || node->m_right->isSigned(as))) {
        jg = "bhi ";
        jl = "blo ";
        jge = "bhi ";
        jle = "blo ";
    }
    if (node->m_op.m_type==TokenType::EQUALS)
        as->Asm("jne " + as->jumpLabel(lblFailed));
    if (node->m_op.m_type==TokenType::NOTEQUALS)
        as->Asm("jeq " + as->jumpLabel(lblFailed));
    if (node->m_op.m_type==TokenType::LESS)
        as->Asm(jge  + as->jumpLabel(lblFailed));
    if (node->m_op.m_type==TokenType::GREATER)
        as->Asm(jle  + as->jumpLabel(lblFailed));

    if (node->m_op.m_type==TokenType::LESSEQUAL)
        as->Asm(jg  + as->jumpLabel(lblFailed));
    if (node->m_op.m_type==TokenType::GREATEREQUAL)
        as->Asm(jl  + as->jumpLabel(lblFailed));



}

void CodeGenChip8::BuildToCmp(QSharedPointer<Node> node)
{
    QString ax = getReg();
    node->m_left->Accept(this);

    as->Term();
    if (node->m_right->isPureNumeric()) {
        as->Asm("cmp  "+ax+", " + getChip8Value(as,node->m_right));
        return;

    }
    as->Comment("Evaluate full expression");
    QString bx = PushReg();
    node->m_right->Accept(this);
    as->Term();

    as->Asm("cmp "+ax+","+bx);
    PopReg();


}

void CodeGenChip8::CompareAndJumpIfNotEqualAndIncrementCounter(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive)
{
    if (step!=nullptr)
        ErrorHandler::e.Error("For loops currently don't support step",nodeA->m_op.m_lineNumber);

//    QString x1 = BinopVariableNumber(nodeA->m_left,"1", TokenType::Type::PLUS);

    // variable
    auto n = nodeA->m_left;
    if (n->isWord(as)) {
        ldr(n);
        as->Asm("; 16 bit add not implemented");
        str(n);
    }
    else {
        ldr(n);
        as->Asm("add "+getReg()+",1");
        str(n);
    }


    if (n->isWord(as)) {
 /*       PushReg();
        PushReg();
        nodeB->Accept(this);
        PopReg();
        PopReg();


        as->Asm("eq16 a,b,c,d");*/
        as->Comment("; 16 bit counter not implemented yet");
    }
    else {
        PushReg();
        nodeB->Accept(this);
        QString b = getReg();
        PopReg();
        QString a = getReg();

        as->Asm("se "+a+","+b);

    }
    as->Asm("jp "+lblJump);

}

void CodeGenChip8::CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QString lblJump, bool isOffPage)
{
    if (nodeA->isWord(as)) nodeB->setForceType(TokenType::INTEGER);
    LoadVariable(nodeA);
    QString ax = getReg();
    PushReg();
    LoadVariable(nodeB);
    QString bx = getReg();
    PopReg();
    as->Asm(m_cmp+ax+","+bx);
    as->Asm(m_jne+lblJump);

}

QString CodeGenChip8::getReturn() { return "ret";}

