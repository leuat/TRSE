#include "codegen_chip8.h"


CodeGenChip8::CodeGenChip8()
{

}
void CodeGenChip8::dispatch(QSharedPointer<NodeProgram> node){

    AbstractCodeGen::dispatch(node);
    QString injected_assembly=" \
    Inject_ptr: dw 0 \n\
    Inject_LoadPointer: \n\
        LD I, Inject_ptr \n\
        LD V1, [I] \n\
        LD V2, #10 \n\
        SUBN V2, V0 \n\
        SE VF, 0 \n\
        RET \n\
        ADD V0, #A0 \n\
        LD I, Inject_load_inst \n\
        LD [I], V1 \n\
    Inject_load_inst: \n\
        LD I, 0 \n\
    ";
    if (chip_type==XO_CHIP){
        injected_assembly+=" \
        LD I, Inject_ptr \n\
        LD V1, [I] \n\
        LD I, XO_CHIP_load_inst \n\
        LD [I], V1 \n\
        dw #f000 \n\
        XO_CHIP_load_inst: \n\
        dw 0 \n";
    }
    as->Asm(injected_assembly);
}
void CodeGenChip8::PrintBop(TokenType::Type type, QString x0, QString x1)
{
    if (type==TokenType::Type::PLUS)
        as->Asm("add "+x0+","+x1);
    else if (type==TokenType::Type::MINUS)
        as->Asm("sub "+x0+","+x1);
/*    if (type==TokenType::Type::MUL)
        Binop("mul",x0,x1,value);
    if (type==TokenType::Type::DIV)
        Binop("div",x0,x1,value);*/
    else if (type==TokenType::Type::BITOR)
        as->Asm("or "+x0+","+x1);
    else if (type==TokenType::Type::XOR)
        as->Asm("xor "+x0+","+x1);
    else if (type==TokenType::Type::BITAND)
        as->Asm("and "+x0+","+x1);

}

void CodeGenChip8::PrintBop16(TokenType::Type type, QString x0_hi, QString x0_lo, QString x1_hi, QString x1_lo)
{
    QString op;
    if (type==TokenType::Type::PLUS)
        op="add ";
    if (type==TokenType::Type::MINUS)
        op="sub ";
    if (type==TokenType::Type::BITOR)
        op="or ";
    if (type==TokenType::Type::XOR)
        op="xor ";
    if (type==TokenType::Type::BITAND)
        op="and ";
    as->Asm(op+x0_lo+","+x1_lo);
    as->Asm(op+x0_hi+", VF");
    as->Asm(op+x0_hi+","+ x1_hi);

}

void CodeGenChip8::dispatch(QSharedPointer<NodeBinOP> node)
{
    as->Comment("Binary operation of type: "+TokenType::getType(node->m_op.m_type));
    auto right_imm = qSharedPointerDynamicCast<NodeNumber>(node->m_right);
    if (node->isWord(as)) {
        node->m_left->Accept(this);
        QString a = getReg();
        PushReg();
        QString b = getReg();
        PushReg();
        QString c = getReg();
        PushReg();
        QString d = getReg();
        node->m_right->Accept(this);
        PopReg();
        PopReg();
        PopReg();
        PrintBop16(node->m_op.m_type,a,b,c,d);

    } else if (right_imm!=nullptr &&
        (node->m_op.m_type == TokenType::Type::PLUS || node->m_op.m_type == TokenType::Type::MINUS)){
            QString negate = node->m_op.m_type==TokenType::Type::MINUS?"-":"";
            node->m_left->Accept(this);
            QString a = getReg();
            as->Asm("ADD "+a+", "+negate+right_imm->StringValue());
        
    }  else {
        node->m_left->Accept(this);
        QString a = getReg();
        PushReg();
        QString b = getReg();
        node->m_right->Accept(this);
        PopReg();
        PrintBop(node->m_op.m_type,a,b);

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
        QString imm = node->getValue(as);
        as->Asm("ld "+ax+", " + imm+">>8" );
        as->Asm("ld "+bx+", " + imm+"&#ff" );
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
        QString x0 = getReg(); PushReg();
        QString x1 = getReg(); 
        as->Asm("LD "+x0+", "+node->getValue(as)+">>8");
        as->Asm("LD "+x1+", "+node->getValue(as)+"&#ff");
        PopReg();
        return;

    } else if (node->m_expr!=nullptr) {
        if (node->isPointer(as)) {
            //I could make it support words, but that's for later
            QString x0 = getReg(); PushReg();
            QString x1 = getReg(); PushReg();
            node->m_expr->setForceType(TokenType::BYTE); 
            node->m_expr->Accept(this);
            QString index;
            QString ptr_hi, ptr_lo;
            ptr_hi = getReg(); PushReg();
            ptr_lo = getReg(); PushReg();
            index = getReg(); 
            //TODO: replace V0 with the index register 
            as->Asm("LD V0, "+ptr_hi);
            as->Asm("LD V1, "+ptr_lo);
            as->Asm("LD I, Inject_ptr");
            as->Asm("LD [I], V1");
            as->Asm("call Inject_loadPointer");
            as->Asm("ADD I, "+ index);

            if (node->getArrayType(as)==TokenType::INTEGER) {
                as->Asm("ADD I, " + index);
                as->Asm("LD V1, [I]");
                as->Asm("LD " +x0+", V0");
                as->Asm("LD " +x1+", V1");
            } else {
                as->Asm("LD V0, [I]");
                QString x0 = getReg();
                as->Asm("LD " +x0+", V0");
            }
            PopReg(); PopReg(); PopReg(); PopReg();


            return;

        } else {
            // Regular array
            QString x0 = getReg(); PushReg();
            QString x1 = getReg(); PushReg();
            node->m_expr->setForceType(TokenType::BYTE);
            node->m_expr->Accept(this);
            QString index;
            index = getReg(); 
            as->Asm("LD I, "+node->getValue(as));
            as->Asm("ADD I, "+index);
            if (node->getArrayType(as)==TokenType::INTEGER) {
                as->Asm("ADD I, "+index);
                as->Asm("LD V1, [I]");
                as->Asm("LD " +x0+", V0");
                as->Asm("LD " +x1+", V1");
                PopReg();
            } else {
                as->Asm("LD V0, [I]");
                QString x0 = getReg();
                as->Asm("LD " +x0+", V0");

            }
            PopReg(); PopReg(); PopReg(); PopReg();
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
    QString type =getWordByteType(var);
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


QString CodeGenChip8::getIndexScaleVal( QSharedPointer<Node> var)
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






void CodeGenChip8::AssignString(QSharedPointer<NodeAssign> node) {
    //TODO: Convert to Chip8

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
    // TODO: Check if this is necessary
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

void CodeGenChip8::OptimizeBinaryClause(QSharedPointer<Node> node)
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

    as->Asm("ld "+reg+", "+getChip8Value(node->m_right));
    return;

}


void CodeGenChip8::DeclarePointer(QSharedPointer<NodeVarDecl> node)
{
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);
//    as->Write(".align 4");
    as->Write(v->getValue(as)+ ": \n\tdw  0",0);
  //  as->Write(".align 4");

    as->m_symTab->Lookup(v->getValue(as), node->m_op.m_lineNumber)->m_arrayType=t->m_arrayVarType.m_type;

}




void CodeGenChip8::BuildConditional(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool page)
{

    as->Comment("Binary clause Simplified: " + node->m_op.getType());



    as->Comment("Evaluate full expression");
    if (node->isWord(as)){

        if (node->m_op.m_type==TokenType::EQUALS){

            auto left_imm = qSharedPointerDynamicCast<NodeNumber>(node->m_left);
            auto right_imm = qSharedPointerDynamicCast<NodeNumber>(node->m_right);
            if (left_imm && right_imm){
                if (left_imm->m_val != right_imm->m_val)
                    as->Asm("jp "+as->jumpLabel(lblFailed));
                return;
                
            } else if (left_imm) { 
                node->m_right->Accept(this);
                QString ax_hi = getReg(); PushReg();
                QString ax_lo = getReg(); 
                as->Asm("sne " + ax_hi + "," + left_imm->StringValue()+">>8");
                as->Asm("se " + ax_lo + ","+ left_imm->StringValue()+"&#ff");

                PopReg();
            } else if (right_imm) {
                node->m_left->Accept(this);
                QString ax_hi = getReg(); PushReg();
                QString ax_lo = getReg(); 
                as->Asm("sne " + ax_hi + "," + right_imm->StringValue()+">>8");
                as->Asm("se " + ax_lo + ","+ right_imm->StringValue()+"&#ff");
                PopReg();
            } else {
                QString ax_lo, ax_hi, bx_hi, bx_lo; 
                ax_hi = getReg(); PushReg();
                ax_lo = getReg(); PushReg();
                bx_hi = getReg(); PushReg();
                bx_lo = getReg();
                as->Asm("sne " + ax_hi + "," + bx_hi);
                as->Asm("se " + ax_lo + ","+ bx_lo);
                PopReg(); PopReg(); PopReg();
            }

        } else if (node->m_op.m_type==TokenType::NOTEQUALS) {

            auto left_imm = qSharedPointerDynamicCast<NodeNumber>(node->m_left);
            auto right_imm = qSharedPointerDynamicCast<NodeNumber>(node->m_right);
            if (left_imm && right_imm){
                if (left_imm->m_val == right_imm->m_val)
                    as->Asm("jp "+as->jumpLabel(lblFailed));
                return;
                
            } else if (left_imm) { 
                node->m_right->Accept(this);
                QString ax_hi = getReg(); PushReg();
                QString ax_lo = getReg(); 
                as->Asm("se " + ax_hi + "," + left_imm->StringValue()+">>8");
                as->Asm("sne " + ax_lo + ","+ left_imm->StringValue()+"&#ff");

                PopReg();
            } else if (right_imm) {
                node->m_left->Accept(this);
                QString ax_hi = getReg(); PushReg();
                QString ax_lo = getReg(); 
                as->Asm("se " + ax_hi + "," + right_imm->StringValue()+">>8");
                as->Asm("sne " + ax_lo + ","+ right_imm->StringValue()+"&#ff");
                PopReg();
            } else {
                QString ax_lo, ax_hi, bx_hi, bx_lo; 
                ax_hi = getReg(); PushReg();
                ax_lo = getReg(); PushReg();
                bx_hi = getReg(); PushReg();
                bx_lo = getReg();
                as->Asm("se " + ax_hi + "," + bx_hi);
                as->Asm("sne " + ax_lo + ","+ bx_lo);
                PopReg(); PopReg(); PopReg();
            }
        } else {

            QString ax_lo, ax_hi, bx_hi, bx_lo; 
            node->m_left->Accept(this);
            ax_hi = getReg(); PushReg();
            ax_lo = getReg(); PushReg();
            node->m_right->Accept(this);
            bx_hi = getReg(); PushReg();
            bx_lo = getReg();
            if (node->m_op.m_type==TokenType::GREATEREQUAL){
                as->Asm("subn "+bx_hi+","+ax_hi);
                as->Asm("sne VF, 0");
                as->Asm("jp "+as->jumpLabel(lblSuccess));
                as->Asm("subn "+ax_hi+","+bx_hi);
                as->Asm("sne VF, 0");
                as->Asm("jp "+as->jumpLabel(lblFailed));
                as->Asm("subn "+bx_lo+","+ax_lo);
                as->Asm("sne VF, 0");

                


            } else if (node->m_op.m_type==TokenType::GREATER){
                as->Asm("subn "+bx_hi+","+ax_hi);
                as->Asm("sne VF, 0");
                as->Asm("jp "+as->jumpLabel(lblSuccess));
                as->Asm("subn "+ax_hi+","+bx_hi);
                as->Asm("sne VF, 0");
                as->Asm("jp "+as->jumpLabel(lblFailed));
                as->Asm("subn "+ax_lo+","+bx_lo);
                as->Asm("se VF, 0");


            } else if (node->m_op.m_type==TokenType::LESSEQUAL){
                as->Asm("subn "+bx_hi+","+ax_hi);
                as->Asm("sne VF, 0");
                as->Asm("jp "+as->jumpLabel(lblFailed));
                as->Asm("subn "+ax_hi+","+bx_hi);
                as->Asm("sne VF, 0");
                as->Asm("jp "+as->jumpLabel(lblSuccess));
                as->Asm("subn "+ax_lo+","+bx_lo);
                //(bx > ax) 
                as->Asm("sne VF, 0");


            } else if (node->m_op.m_type==TokenType::LESS){
                as->Asm("subn "+bx_hi+","+ax_hi);
                as->Asm("sne VF, 0");
                as->Asm("jp "+as->jumpLabel(lblFailed));
                as->Asm("subn "+ax_hi+","+bx_hi);
                as->Asm("sne VF, 0");
                as->Asm("jp "+as->jumpLabel(lblSuccess));
                as->Asm("subn "+bx_lo+","+ax_lo);
                //!(ax > bx) 
                as->Asm("se VF, 0");
            }
            PopReg(); PopReg(); PopReg();
        }
    } else {
        

        if (node->m_op.m_type==TokenType::EQUALS){

            auto left_imm = qSharedPointerDynamicCast<NodeNumber>(node->m_left);
            auto right_imm = qSharedPointerDynamicCast<NodeNumber>(node->m_right);
            if (right_imm && left_imm){
                if (left_imm->m_val != right_imm->m_val) 
                    as->Asm("jp "+as->jumpLabel(lblFailed));

            } else if (right_imm){
                node->m_left->Accept(this);
                QString ax= getReg();
                as->Asm("se " + ax + "," + right_imm->StringValue());
            }else if (left_imm){
                node->m_right->Accept(this);
                QString ax= getReg();
                as->Asm("se " + ax + "," + left_imm->StringValue());
            } else {
                QString ax= getReg(); PushReg();
                QString bx= getReg();
                as->Asm("se " + ax + "," + bx);
                PopReg();
            }   
            return;
        
        }else if (node->m_op.m_type==TokenType::NOTEQUALS) {
            auto left_imm = qSharedPointerDynamicCast<NodeNumber>(node->m_left);
            auto right_imm = qSharedPointerDynamicCast<NodeNumber>(node->m_right);
            if (right_imm && left_imm){
                if (left_imm->m_val == right_imm->m_val) 
                    as->Asm("jp "+as->jumpLabel(lblFailed));
            } else if (right_imm) {
                node->m_left->Accept(this);
                QString ax= getReg();
                as->Asm("sne " + ax + "," + right_imm->StringValue());
            } else if (left_imm) {
                node->m_right->Accept(this);
                QString ax= getReg();
                as->Asm("sne " + ax + "," + left_imm->StringValue());
            } else {
                QString ax= getReg(); PushReg();
                QString bx= getReg();
                as->Asm("sne " + ax + "," + bx);
                PopReg();
            }
            return;
        } else {

            node->m_left->Accept(this);
            QString ax = getReg(); PushReg();
            node->m_right->Accept(this);
            QString bx = getReg();
            if (node->m_op.m_type==TokenType::GREATEREQUAL){
                as->Asm("subn "+bx+","+ax);
                //(ax >= bx) 
                as->Asm("sne VF, 0");


            } else if (node->m_op.m_type==TokenType::GREATER){
                as->Asm("subn "+ax+","+bx);
                //!(bx >= ax) 
                as->Asm("se VF, 0");


            } else if (node->m_op.m_type==TokenType::LESSEQUAL){
                as->Asm("subn "+ax+","+bx);
                //(bx >= ax) 
                as->Asm("sne VF, 0");


            } else if (node->m_op.m_type==TokenType::LESS){
                as->Asm("subn "+bx+","+ax);
                //!(ax >= bx) 
                as->Asm("se VF, 0");
            }
            PopReg();
        }
    }
    as->Asm("jp "+as->jumpLabel(lblFailed));



}


void CodeGenChip8::CompareAndJumpIfNotEqualAndIncrementCounter(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive)
{

#if 0
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

#endif
}
void CodeGenChip8::CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QString lblJump, bool isOffPage)
{
    #if 0
    if (nodeA->isWord(as)) nodeB->setForceType(TokenType::INTEGER);
    LoadVariable(nodeA);
    QString ax = getReg();
    PushReg();
    LoadVariable(nodeB);
    QString bx = getReg();
    PopReg();
    as->Asm(m_cmp+ax+","+bx);
    as->Asm(m_jne+lblJump);
    #endif 
}

QString CodeGenChip8::getReturn() { return "ret";}

