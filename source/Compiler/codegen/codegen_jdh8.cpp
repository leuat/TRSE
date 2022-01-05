#include "codegen_jdh8.h"


CodeGenJDH8::CodeGenJDH8()
{
}

void CodeGenJDH8::PrintBop(TokenType::Type type, QString x0, QString x1, QString value, bool is16bit)
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



void CodeGenJDH8::dispatch(QSharedPointer<NodeBinOP> node)
{
    as->Comment("Binary operation of type: "+TokenType::getType(node->m_op.m_type));
    if (node->isWord(as)) {
//        node->SwapNodes();
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

void CodeGenJDH8::dispatch(QSharedPointer<NodeNumber>node)
{
    QString ax = getReg();
    if (!node->isWord(as))
        as->Asm("mw "+ax+", " + node->getValue(as));
    else
    {
        PushReg();
        QString bx = getReg();
        as->Asm("mw16 "+ax+", "+bx+","+ node->getValue(as));
        PopReg();

    }
}

void CodeGenJDH8::dispatch(QSharedPointer<NodeVar> node)
{
    if (m_inlineParameters.contains(node->value)) {
  //      qDebug()<< "INLINE node override : "<< node->value;
        m_inlineParameters[node->value]->Accept(this);
        return;
    }
    if (node->isReference()) {
        as->Asm("lda "+node->getValue(as));
        as->Asm("mw16 a,b,h,l");
        return;

    }
    QString ending = "]";
    if (node->m_expr!=nullptr) {
        if (node->isPointer(as)) {
            // i := p[2] etc;
            node->m_expr->setForceType(TokenType::INTEGER);
            node->m_expr->Accept(this);
            as->Asm("lw16 h,l,["+node->getValue(as)+"]");
            as->Asm("add16 h,l,a,b");
            as->Asm("lw a,h,l ");
            return;

        }
        // Regular array
        QString x0 = getReg();
        QString expr = PushReg();
        as->Comment("Loading array");
        as->Comment("load expression:");
        node->m_expr->Accept(this);
        as->Comment("load value:");
        QString x1 = LoadAddress(node);
        as->Asm("add "+x1+","+x1+","+expr+getShift(node));
        ldr(x0,x1);

        PopReg();
        PopReg();
        return;
    }

    ldr(node);



}


void CodeGenJDH8::dispatch(QSharedPointer<NodeString> node)
{

}



void CodeGenJDH8::dispatch(QSharedPointer<NodeVarType> node)
{

}

void CodeGenJDH8::dispatch(QSharedPointer<NodeBinaryClause> node)
{

}


void CodeGenJDH8::dispatch(QSharedPointer<Node> node)
{

}

void CodeGenJDH8::dispatch(QSharedPointer<NodeRepeatUntil> node)
{
    ErrorHandler::e.Error("Repeat-until not implemented yet", node->m_op.m_lineNumber);

}
void CodeGenJDH8::dispatch(QSharedPointer<NodeComment> node)
{

}

void CodeGenJDH8::StoreVariable(QSharedPointer<NodeVar> n)
{

}

bool CodeGenJDH8::StoreVariableSimplified(QSharedPointer<NodeAssign> node)
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

void CodeGenJDH8::LoadVariable(QSharedPointer<NodeVar> n)
{
    n->Accept(this);
}

QString CodeGenJDH8::LoadAddress(QSharedPointer<Node> n)
{
    QString reg = PushReg();
    as->Asm("adrp "+reg+", "+n->getValue(as)+"@PAGE");
    as->Asm("add "+reg+","+reg+","+n->getValue(as)+"@PAGEOFF");
    return reg;
}

QString CodeGenJDH8::LoadAddress(QSharedPointer<Node> n, QString reg)
{
    return "";
}

void CodeGenJDH8::LoadVariable(QSharedPointer<NodeProcedure> node)
{
    ErrorHandler::e.Error("Procedure address not implemented yet! Please bug the developer", node->m_op.m_lineNumber);
}

void CodeGenJDH8::LoadPointer(QSharedPointer<Node> n)
{

}

void CodeGenJDH8::LoadVariable(QSharedPointer<Node> n)
{
    n->Accept(this);

}

void CodeGenJDH8::LoadVariable(QSharedPointer<NodeNumber>n)
{

}

QString CodeGenJDH8::getShift(QSharedPointer<NodeVar> var)
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

QString CodeGenJDH8::getIndexScaleVal(Assembler *as, QSharedPointer<Node> var)
{
    if (var->isWord(as))
        return "2";
    if (var->isLong(as))
        return "4";
    return "1";
}

QString CodeGenJDH8::getCallSubroutine() {
    return "call";
}

QString CodeGenJDH8::getReg(int dd) {
    if (m_lvl+dd>=m_regs.count()) {
        qDebug() << "ERROR CodeGenJDH8 reg stack error shouldn't happen!";
        exit(1);
    }
    return m_regs[m_lvl+dd];

}

void CodeGenJDH8::ldr(QString x0, QString x1)
{
    as->Asm("mw "+x1+",["+x0+"]");
}

void CodeGenJDH8::str(QString x0, QString x1)
{
    as->Asm("sw ["+x1+"],"+x0);
}

void CodeGenJDH8::str(QSharedPointer<Node> var)
{
    if (var->isWord(as)) {
        QString x0 = getReg();
        QString x1 = m_regs[m_lvl+1];
        as->Asm("sw16 ["+var->getValue(as)+"],"+x0+","+x1);
    }
    else {
        QString x0 = getReg();
        as->Asm("sw ["+var->getValue(as)+"],"+x0);

    }
}

void CodeGenJDH8::ldr(QSharedPointer<Node> var)
{
    if (var->isWord(as)) {
        QString x0 = getReg();
        QString x1 = m_regs[m_lvl+1];
        as->Asm("lw16 "+x0+","+x1+ ", ["+var->getValue(as)+"]");
    }
    else {
        QString x0 = getReg();
        as->Asm("lw "+x0+"["+var->getValue(as)+"]");

    }

}

void CodeGenJDH8::Binop(QString bop, QString x0, QString x1, QString x2)
{
    if (x2=="")
        as->Asm(bop+" "+x0+","+x1);
    else
        as->Asm(bop+" "+x0+","+x1+","+x2);

}

QString CodeGenJDH8::BinopVariableNumber(QSharedPointer<Node> n, QString value, TokenType::Type type)
{

}




QString CodeGenJDH8::getBinaryOperation(QSharedPointer<NodeBinOP> bop) {
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

QString CodeGenJDH8::PushReg() {
    if (m_lvl==12)
        ErrorHandler::e.Error("Error in JDH8 dispatcher PopReg : trying to push regstack from max");

    m_lvl++;
    return m_regs[m_lvl];
}

void CodeGenJDH8::PopReg() {
    if (m_lvl==0)
        ErrorHandler::e.Error("Error in JDH8 dispatcher PopReg : trying to pop regstack from zero");
    m_lvl--;
}

QString CodeGenJDH8::getEndType(Assembler *as, QSharedPointer<Node> v)
{
    return "";
}




void CodeGenJDH8::AssignString(QSharedPointer<NodeAssign> node) {


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

bool CodeGenJDH8::AssignPointer(QSharedPointer<NodeAssign> node)
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

void CodeGenJDH8::GenericAssign(QSharedPointer<NodeAssign> node)
{
    as->Comment("Generic assign");
    node->m_right->Accept(this);
    str(node->m_left);

}

bool CodeGenJDH8::IsAssignPointerWithIndex(QSharedPointer<NodeAssign> node)
{
    // Set pointer value
    return false;
}

bool CodeGenJDH8::IsAssignArrayWithIndex(QSharedPointer<NodeAssign> node)
{
    return false;
}

bool CodeGenJDH8::IsSimpleIncDec(QSharedPointer<NodeAssign> node)
{
    return false;
}

bool CodeGenJDH8::IsSimpleAssignPointer(QSharedPointer<NodeAssign> node)
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

void CodeGenJDH8::OptimizeBinaryClause(QSharedPointer<Node> node, Assembler *as)
{

}

void CodeGenJDH8::AssignFromRegister(QSharedPointer<NodeAssign> node)
{
    ErrorHandler::e.Error("Cannot (yet) assign variables from registers on the JDH8",node->m_op.m_lineNumber);
}

void CodeGenJDH8::AssignToRegister(QSharedPointer<NodeAssign> node)
{
    QString vname = getValue(node->m_left);
    vname = vname.toLower();
    if (!node->m_right->isPure())
        ErrorHandler::e.Error("When assigning registers, RHS needs to be pure numeric or variable",node->m_op.m_lineNumber);

    QString reg = vname.remove(0,1);
//        as->Comment("Assigning register : " + vname);

    as->Asm("mw "+reg+", "+getJDH8Value(as,node->m_right));
    return;

}

void CodeGenJDH8::ProcedureStart(Assembler *as) {

}

void CodeGenJDH8::ProcedureEnd(Assembler *as) {

}


/*void CodeGenJDH8::AssignVariable(QSharedPointer<NodeAssign> node)
{

    if (node->m_left->isWord(as)) {
        node->m_right->setForceType(TokenType::INTEGER);
    }

    as->ClearTerm();

    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(node->m_left);


    QSharedPointer<Symbol> s = as->m_symTab->Lookup(getValue(var), node->m_op.m_lineNumber, var->isAddress());

    if (!var->isPurePointer(as) && node->m_right->isPurePointer(as)) {
        ErrorHandler::e.Error("Cannot assign pointer to variable "+var->getValue(as),var->m_op.m_lineNumber);
    }
    if (!var->isPurePointer(as) && node->m_right->isPurePointer(as)) {
        ErrorHandler::e.Error("Cannot assign pointer to variable "+var->getValue(as),var->m_op.m_lineNumber);
    }

    QString vname = getValue(var);
//    as->Comm nt("IS REGISTER : "+Util::numToHex(v->m_isRegister) + " "+vname);
    if (var->m_isRegister) {
        vname = vname.toLower();
        if (!node->m_right->isPure())
            ErrorHandler::e.Error("When assigning registers, RHS needs to be pure numeric or variable",node->m_op.m_lineNumber);

        QString reg = vname.remove(0,1);
//        as->Comment("Assigning register : " + vname);

        as->Asm("mw "+reg+", "+getJDH8Value(as,node->m_right));
        return;
        //}
    }

    if (qSharedPointerDynamicCast<NodeString>(node->m_right)) {
        AssignString(node,node->m_left->isPointer(as));
        return;
    }


    if (var->m_writeType==TokenType::INTEGER) {
        node->m_right->setForceType(TokenType::INTEGER);
    }
    if (var->m_writeType==TokenType::LONG)
        node->m_right->setForceType(TokenType::LONG);


    if (var->isPointer(as) && !var->hasArrayIndex()) {
        node->m_right->VerifyReferences(as);
        if (!node->m_right->isReference())
            if (!node->m_right->isPointer(as))
                if (node->m_right->isWord(as) || node->m_right->isByte(as)) {
                ErrorHandler::e.Error("Trying to assign a non-pointer / non-reference / non-long to pointer '"+var->getValue(as)+"'",var->m_op.m_lineNumber);
                }

        as->Comment("Assigning pointer");

        QSharedPointer<NodeBinOP> bop =  qSharedPointerDynamicCast<NodeBinOP>(node->m_right);
//        node->m_right->setForceType(TokenType::POINTER);
        if (bop!=nullptr && (bop->m_op.m_type==TokenType::PLUS || bop->m_op.m_type==TokenType::MINUS || bop->m_op.m_type==TokenType::BITOR || bop->m_op.m_type==TokenType::BITAND || bop->m_op.m_type==TokenType::XOR )) {
            if (bop->m_left->getValue(as)==var->getValue(as)) {

                as->Comment("'p := p + v' optimization");
                as->ClearTerm();
                as->BinOP(bop->m_op.m_type);
                QString bopCmd = as->m_term;
                as->ClearTerm();
                if (bop->m_right->isPureNumeric()) {
                    as->Asm(bopCmd + " ["+var->getValue(as)+"], word "+bop->m_right->getValue(as));
                    return;
                }
                bop->m_right->Accept(this);
                as->Term();
                as->Asm(bopCmd + " ["+var->getValue(as)+"], ax");


                return;
            }
        }




        if (node->m_right->isPureVariable()) {
            if (node->m_right->isPointer(as)) {
                as->Asm("les di, ["+node->m_right->getValue(as)+"]");
                as->Asm("mw ["+var->getValue(as)+"+2], es");
                as->Asm("mw ["+var->getValue(as)+"], di");
            }
            else {
//                as->Asm("lea si, "+node->m_right->getValue(as));
//                as->Asm("cld");
                as->Asm("lea si, ["+node->m_right->getValue(as)+"]");
                //as->Asm("mw si, "+node->m_right->getValue(as));
                as->Asm("mw ["+var->getValue(as)+"+2], ds");
                as->Asm("mw ["+var->getValue(as)+"], si");
            }
            return;
        }
        else{
            as->Comment("Setting PURE POINTER "+QString::number(node->isPointer(as)));
//            m_isPurePointer = true;
 //           if (node->m_left->isPointer(as))
   //            node->m_right->setForceType(TokenType::POINTER);
            node->m_right->Accept(this);
  //          m_isPurePointer = false;
            as->Comment("Setting PURE POINTER ends");

            as->Asm("mw ["+var->getValue(as)+"+2], es");
            as->Asm("mw ["+var->getValue(as)+"], di");
        }
        return;
    }

    // Set pointer value
    if (var->isPointer(as) && var->hasArrayIndex()) {

        // TO DO: Optimize special cases

        as->ClearTerm();
        as->Comment("Assigning pointer with index, type:" + TokenType::getType(var->m_writeType));
        if (var->isWord(as))
            node->m_right->setForceType(TokenType::INTEGER);
        node->m_right->Accept(this);

        as->Term();
        as->Asm("les di, ["+var->getValue(as)+"]");
        if (var->m_expr->isPureNumeric()) {

            as->Asm("mw [es:di+"+var->m_expr->getValue(as)+"*"+getIndexScaleVal(as,var)+"],"+getReg("a",var));
            return;

        }
        if (var->m_expr->isPureVariable() && var->m_expr->isWord(as)) {
            as->Asm("add di,["+var->m_expr->getValue(as)+"]");
            if (var->isWord(as))
                as->Asm("add di,["+var->m_expr->getValue(as)+"]");

            as->Asm("mw [es:di],"+getReg("a",var));

            return;

        }

        as->Asm("push ax");
        var->m_expr->setForceType(TokenType::INTEGER);
        var->m_expr->Accept(this);
        as->Term();
        if (var->isWord(as))
            as->Asm("shl ax,1");
        as->Asm("mw bx,ax");

        as->Asm("pop ax");

        as->Asm("mw [es:di+bx],"+getReg("a",var));
        return;

    }



    if (var->hasArrayIndex()) {
        // Is an array
        as->Asm(";Is array index");
        if (var->getArrayType(as)==TokenType::POINTER) {
            as->Comment("Assign value to pointer array");
            node->m_right->Accept(this);
            var->m_expr->Accept(this);
            as->Asm("shl ax,2 ; pointer lookup");
            as->Asm("mw bx,ax");
            as->Asm("lea si,["+var->getValue(as)+"]");
            as->Asm("mw [ds:si+bx],di ; store in pointer array");
            as->Asm("mw [ds:si+bx+2],es");

            return;
        }
        as->Comment("Assign value to regular array");

        node->m_right->Accept(this);
        // Handle var[ i ] :=
        if (var->m_expr->isPure()) {
            var->m_expr->setForceType(TokenType::INTEGER);
            if (var->m_expr->isPureNumeric()) {
                as->Asm("mw ["+var->getValue(as) + "+" + Util::numToHex(var->m_expr->getValueAsInt(as)*var->getArrayDataSize(as))+"],"+getReg() );
                return;
            }
            else {
                as->Asm("mw di,"+getJDH8Value(as,var->m_expr));
                if (var->isWord(as))
                    as->Asm("shl di,1");
            }
        }
        else {
            as->Asm("push ax");
            var->m_expr->setForceType(TokenType::INTEGER);
            var->m_expr->Accept(this);
            as->Asm("mw di,ax");
            if (var->isWord(as))
                as->Asm("shl di,1");
            as->Asm("pop ax");
        }
        as->Asm("mw ["+var->getValue(as) + "+di], "+getReg());


        return;
    }

//    if (var->getValue())
    // Simple a:=b;
    QString type =getWordByteType(as,var);

    if (node->m_right->isPureNumeric()) {
        as->Asm("mw ["+var->getValue(as)+ "], "+type+ " "+node->m_right->getValue(as));
        return;
    }
    // Check for a:=a+2;
    QSharedPointer<NodeBinOP> bop =  qSharedPointerDynamicCast<NodeBinOP>(node->m_right);
    if (bop!=nullptr && (bop->m_op.m_type==TokenType::PLUS || bop->m_op.m_type==TokenType::MINUS || bop->m_op.m_type==TokenType::BITOR || bop->m_op.m_type==TokenType::BITAND || bop->m_op.m_type==TokenType::XOR )) {
        if (bop->ContainsVariable(as,var->getValue(as))) {
            // We are sure that a:=a ....
            // first, check if a:=a + number
//            as->Comment("In BOP");
            if (bop->m_right->isPureNumeric()) {
                as->Comment("'a:=a + const'  optimization ");
                as->Asm(getBinaryOperation(bop) + " ["+var->getValue(as)+"], "+type + " "+bop->m_right->getValue(as));
                return;
            }
            as->Comment("'a:=a + expression'  optimization ");
            bop->m_right->Accept(this);
            as->Asm(getBinaryOperation(bop) + " ["+var->getValue(as)+"], "+getReg());
            return;
        }
        // Check for a:=a+

    }
    as->ClearTerm();
    node->m_right->Accept(this);
    as->Term();
    as->Asm("mw ["+qSharedPointerDynamicCast<NodeVar>(node->m_left)->getValue(as) + "], "+getReg());
    return;
}
*/
void CodeGenJDH8::DeclarePointer(QSharedPointer<NodeVarDecl> node)
{
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);
//    as->Write(".align 4");
    as->Write(v->getValue(as)+ ": \n\t@dd  0",0);
  //  as->Write(".align 4");

    as->m_symTab->Lookup(v->getValue(as), node->m_op.m_lineNumber)->m_arrayType=t->m_arrayVarType.m_type;

}

QString CodeGenJDH8::getEndType(Assembler *as, QSharedPointer<Node> v1, QSharedPointer<Node> v2)
{
    return "";
}


void CodeGenJDH8::BuildSimple(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool page)
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

void CodeGenJDH8::BuildToCmp(QSharedPointer<Node> node)
{
/*    if (node->m_left->getValue(as)!="") {
        if (node->m_right->isPureNumeric())
        {
            as->Comment("Compare with pure num / var optimization");
            //            TransformVariable(as,"cmp",node->m_left->getValue(as),node->m_right->getValue(as),node->m_left);
//            TransformVariable(as,"cmp",node->m_left->getValue(as),node->m_right->getValue(as),node->m_left);
            if (node->m_left->isPure()) {
                as->Asm("cmp ["+node->m_left->getValue(as)+"],"+getWordByteType(as,node->m_left)+" " + node->m_right->getValue(as));
                return;
            }

            LoadVariable(node->m_left);
            as->Asm("cmp "+getReg()+"," + node->m_right->getValue(as));

            return;
        } else
        {
            as->Comment("Compare two vars optimization");

            if (node->m_right->isPureVariable()) {
                //QString wtf = as->m_regAcc.Get();
                LoadVariable(node->m_right);
                //TransformVariable(as,"move",wtf,qSharedPointerDynamicCast<NodeVar>node->m_left);
                //TransformVariable(as,"cmp",wtf,as->m_varStack.pop());
                as->Asm("cmp  ["+node->m_left->getValue(as) +"]," + getReg());

                return;
            }
            node->m_right->Accept(this);
            as->Term();

            as->Asm("cmp  "+node->m_left->getValue(as) +"," + getReg());

//            TransformVariable(as,"cmp",qSharedPointerDynamicCast<NodeVar>node->m_left,as->m_varStack.pop());
            return;
        }
    }*/
    QString ax = getReg();
    node->m_left->Accept(this);

    as->Term();
    if (node->m_right->isPureNumeric()) {
        as->Asm("cmp  "+ax+", " + getJDH8Value(as,node->m_right));
        return;

    }
    as->Comment("Evaluate full expression");
    QString bx = PushReg();
    node->m_right->Accept(this);
    as->Term();

    as->Asm("cmp "+ax+","+bx);
    PopReg();


}

void CodeGenJDH8::CompareAndJumpIfNotEqualAndIncrementCounter(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive)
{
    if (step!=nullptr)
        ErrorHandler::e.Error("For loops currently don't support step",nodeA->m_op.m_lineNumber);

//    QString x1 = BinopVariableNumber(nodeA->m_left,"1", TokenType::Type::PLUS);

    // variable
    auto n = nodeA->m_left;
    if (n->isWord(as)) {
        ldr(n);
        as->Asm("add16 a,b,1");
        str(n);
    }
    else {
        ldr(n);
        as->Asm("add a,1");
        str(n);
    }


    if (n->isWord(as)) {
        PushReg();
        PushReg();
        nodeB->Accept(this);
        PopReg();
        PopReg();


        as->Asm("eq16 a,b,c,d");
    }
    else {
        PushReg();
        nodeB->Accept(this);
        PopReg();

        as->Asm("cmp a,b");

    }
    as->Asm("jne "+lblJump);

}

void CodeGenJDH8::CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QString lblJump, bool isOffPage)
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

QString CodeGenJDH8::getReturn() { return "ret";}

