#include "codegen_arm.h"


CodeGenARM::CodeGenARM()
{

}

void CodeGenARM::dispatch(QSharedPointer<NodeBinOP> node)
{
    as->Comment("Binary operation of type: "+TokenType::getType(node->m_op.m_type));
    QString reg = getReg();
    node->m_right->Accept(this);
    QString x0 = getReg();
    PushReg();
    node->m_left->Accept(this);
    QString x1 = getReg();
    PopReg();
    PrintBop(node->m_op.m_type,reg,x1,x0);

}

void CodeGenARM::dispatch(QSharedPointer<NodeNumber>node)
{
    QString ax = getReg();
    as->Asm("mov "+ax+", #" + node->getValue(as));
}

void CodeGenARM::dispatch(QSharedPointer<NodeVar> node)
{
    if (m_inlineParameters.contains(node->value)) {
  //      qDebug()<< "INLINE node override : "<< node->value;
        m_inlineParameters[node->value]->Accept(this);
        return;
    }
    if (node->isReference()) {
        QString reg = getReg();
        as->Asm("adrp "+reg+", "+node->getValue(as)+"@PAGE");
        as->Asm("add "+reg+","+reg+","+node->getValue(as)+"@PAGEOFF");
        return;

    }
    QString ending = "]";
    if (node->m_expr!=nullptr) {


    }

    QString x0 = getReg();
    QString x1 = LoadAddress(node);
    ldr(x0,x1);
    PopReg();



}


void CodeGenARM::dispatch(QSharedPointer<NodeString> node)
{

}



void CodeGenARM::dispatch(QSharedPointer<NodeVarType> node)
{

}

void CodeGenARM::dispatch(QSharedPointer<NodeBinaryClause> node)
{

}


void CodeGenARM::dispatch(QSharedPointer<Node> node)
{

}

void CodeGenARM::dispatch(QSharedPointer<NodeRepeatUntil> node)
{
    ErrorHandler::e.Error("Repeat-until not implemented yet", node->m_op.m_lineNumber);

}
void CodeGenARM::dispatch(QSharedPointer<NodeComment> node)
{

}

void CodeGenARM::StoreVariable(QSharedPointer<NodeVar> n)
{

}

bool CodeGenARM::StoreVariableSimplified(QSharedPointer<NodeAssign> node)
{
    auto var = node->m_left;
    QString type =getWordByteType(as,var);
    as->Comment("Store variable simplified");
    if (node->m_right->isPure() && !node->m_left->isPointer(as) && !node->m_left->isArrayIndex()) {
        node->m_right->Accept(this);
        QString x0 = getReg();
        QString x1 = LoadAddress(var);
        str(x0,x1);
        PopReg();
/*
        as->Asm("adrp x1, "+var->getValue(as)+"@PAGE");
        as->Asm("add x1, x1,"+var->getValue(as)+"@PAGEOFF");
        as->Asm("mov x2, #"+node->m_right->getValue(as));
        as->Asm("str x2, [x1]");*/
        return true;
    }
    return false;

}

void CodeGenARM::LoadVariable(QSharedPointer<NodeVar> n)
{
    n->Accept(this);
}

QString CodeGenARM::LoadAddress(QSharedPointer<Node> n)
{
    QString reg = PushReg();
    as->Asm("adrp "+reg+", "+n->getValue(as)+"@PAGE");
    as->Asm("add "+reg+","+reg+","+n->getValue(as)+"@PAGEOFF");
    return reg;
}

QString CodeGenARM::LoadAddress(QSharedPointer<Node> n, QString reg)
{
    return "";
}

void CodeGenARM::LoadVariable(QSharedPointer<NodeProcedure> node)
{
    ErrorHandler::e.Error("Procedure address not implemented yet! Please bug the developer", node->m_op.m_lineNumber);
}

void CodeGenARM::LoadPointer(QSharedPointer<Node> n)
{

}

void CodeGenARM::LoadVariable(QSharedPointer<Node> n)
{
    n->Accept(this);

}

void CodeGenARM::LoadVariable(QSharedPointer<NodeNumber>n)
{

}

QString CodeGenARM::getIndexScaleVal(Assembler *as, QSharedPointer<Node> var)
{
    if (var->isWord(as))
        return "2";
    if (var->isLong(as))
        return "4";
    return "1";
}

QString CodeGenARM::getCallSubroutine() {
    return "bl";
}

QString CodeGenARM::getReg() {
    return m_regs[m_lvl];

}

void CodeGenARM::ldr(QString x0, QString x1)
{
    as->Asm("ldr "+x0+",["+x1+"]");
}

void CodeGenARM::str(QString x0, QString x1)
{
    as->Asm("str "+x0+",["+x1+"]");
}

void CodeGenARM::Binop(QString bop, QString x0, QString x1, QString x2)
{
    as->Asm(bop+" "+x0+","+x1+","+x2);

}

QString CodeGenARM::BinopVariableNumber(QSharedPointer<Node> n, QString value, TokenType::Type type)
{
    QString x0 = LoadAddress(n);
    QString x1 = PushReg();
    ldr(x1,x0);

    PrintBop(type,x1,x1,value);

    str(x1,x0);
    PopReg();
    PopReg();

    return x1;
}




QString CodeGenARM::getBinaryOperation(QSharedPointer<NodeBinOP> bop) {
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

QString CodeGenARM::PushReg() {
    if (m_lvl==12)
        ErrorHandler::e.Error("Error in ARM dispatcher PopReg : trying to push regstack from max");

    m_lvl++;
    return m_regs[m_lvl];
}

void CodeGenARM::PopReg() {
    if (m_lvl==0)
        ErrorHandler::e.Error("Error in ARM dispatcher PopReg : trying to pop regstack from zero");
    m_lvl--;
}

QString CodeGenARM::getEndType(Assembler *as, QSharedPointer<Node> v)
{
    return "";
}




void CodeGenARM::AssignString(QSharedPointer<NodeAssign> node) {


    QSharedPointer<NodeString> right = qSharedPointerDynamicCast<NodeString>(node->m_right);
    QSharedPointer<NodeVar> left = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    QString str = as->NewLabel("stringassignstr");
    QString lblCpy=as->NewLabel("stringassigncpy");
    bool isPointer = left->isPointer(as);
    QString strAssign = str + "\t db \"" + right->m_op.m_value + "\",0";
    as->m_tempVars<<strAssign;
    if (isPointer) {
        as->Asm("mov si, "+str+"");
        as->Asm("mov ["+left->getValue(as)+"+2], ds");
        as->Asm("mov ["+left->getValue(as)+"], si");

    }
    else {
        as->Comment("String copy!");
        if (left->isPointer(as))
            as->Asm("les di,["+left->getValue(as)+"]");
        else
            as->Asm("mov di,"+left->getValue(as)+"");
        as->Term();

        as->Asm("push ds");
        as->Asm("pop es");
        as->Asm("mov si,"+str);
        as->Asm("mov cx, "+Util::numToHex(right->m_op.m_value.count()+2));
        as->Asm("rep movsb");
    }
    as->PopLabel("stringassignstr");
    as->PopLabel("stringassigncpy");
}

bool CodeGenARM::AssignPointer(QSharedPointer<NodeAssign> node)
{
    return false;
}

void CodeGenARM::GenericAssign(QSharedPointer<NodeAssign> node)
{
    as->Comment("Generic assign");
    node->m_right->Accept(this);
    QString x0 = getReg();
    QString x1 = LoadAddress(node->m_left);
    str(x0,x1);
    PopReg();

}

bool CodeGenARM::IsAssignPointerWithIndex(QSharedPointer<NodeAssign> node)
{
    // Set pointer value
    return false;
}

bool CodeGenARM::IsAssignArrayWithIndex(QSharedPointer<NodeAssign> node)
{
    return false;
}

bool CodeGenARM::IsSimpleIncDec(QSharedPointer<NodeAssign> node)
{
    // Check for a:=a+2;
    auto var = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    QString type =getWordByteType(as,var);
    QSharedPointer<NodeBinOP> bop =  qSharedPointerDynamicCast<NodeBinOP>(node->m_right);
    if (bop!=nullptr && (bop->m_op.m_type==TokenType::PLUS || bop->m_op.m_type==TokenType::MINUS || bop->m_op.m_type==TokenType::BITOR || bop->m_op.m_type==TokenType::BITAND || bop->m_op.m_type==TokenType::XOR )) {
        if (bop->ContainsVariable(as,var->getValue(as))) {
            // We are sure that a:=a ....
            // first, check if a:=a + number
//            as->Comment("In BOP");
            if (bop->m_right->isPureNumeric()) {
                as->Comment("'a:=a + const'  optimization ");
                as->Asm(getBinaryOperation(bop) + " ["+var->getValue(as)+"], "+type + " "+bop->m_right->getValue(as));
                return true;
            }
            as->Comment("'a:=a + expression'  optimization ");
            bop->m_right->Accept(this);
            as->Asm(getBinaryOperation(bop) + " ["+var->getValue(as)+"], "+getReg());
            return true;
        }
        // Check for a:=a+

    }

    return false;
}

bool CodeGenARM::IsSimpleAssignPointer(QSharedPointer<NodeAssign> node)
{
    auto var = node->m_left;
    if (var->isPointer(as) && !var->isArrayIndex()) {

        node->m_right->VerifyReferences(as);
        if (!node->m_right->isReference())
            if (!node->m_right->isPointer(as))
                if (node->m_right->isWord(as) || node->m_right->isByte(as)) {
                    ErrorHandler::e.Error("Trying to assign a non-pointer / non-reference / non-long to pointer '"+var->getValue(as)+"'",var->m_op.m_lineNumber);
                }

        as->Comment("Assigning pointer");

        node->m_right->Accept(this);
        QString val = getReg();

        QString v = LoadAddress(var);
        PopReg();
        str(val,v);
        return true;

    }
    return false;

}

void CodeGenARM::OptimizeBinaryClause(QSharedPointer<Node> node, Assembler *as)
{

}

void CodeGenARM::AssignFromRegister(QSharedPointer<NodeAssign> node)
{
    ErrorHandler::e.Error("Cannot (yet) assign variables from registers on the ARM",node->m_op.m_lineNumber);
}

void CodeGenARM::AssignToRegister(QSharedPointer<NodeAssign> node)
{
    QString vname = getValue(node->m_left);
    vname = vname.toLower();
    if (!node->m_right->isPure())
        ErrorHandler::e.Error("When assigning registers, RHS needs to be pure numeric or variable",node->m_op.m_lineNumber);

    QString reg = vname.remove(0,1);
//        as->Comment("Assigning register : " + vname);

    as->Asm("mov "+reg+", "+getARMValue(as,node->m_right));
    return;

}


/*void CodeGenARM::AssignVariable(QSharedPointer<NodeAssign> node)
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

        as->Asm("mov "+reg+", "+getARMValue(as,node->m_right));
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


    if (var->isPointer(as) && !var->isArrayIndex()) {
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
                as->Asm("mov ["+var->getValue(as)+"+2], es");
                as->Asm("mov ["+var->getValue(as)+"], di");
            }
            else {
//                as->Asm("lea si, "+node->m_right->getValue(as));
//                as->Asm("cld");
                as->Asm("lea si, ["+node->m_right->getValue(as)+"]");
                //as->Asm("mov si, "+node->m_right->getValue(as));
                as->Asm("mov ["+var->getValue(as)+"+2], ds");
                as->Asm("mov ["+var->getValue(as)+"], si");
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

            as->Asm("mov ["+var->getValue(as)+"+2], es");
            as->Asm("mov ["+var->getValue(as)+"], di");
        }
        return;
    }

    // Set pointer value
    if (var->isPointer(as) && var->isArrayIndex()) {

        // TO DO: Optimize special cases

        as->ClearTerm();
        as->Comment("Assigning pointer with index, type:" + TokenType::getType(var->m_writeType));
        if (var->isWord(as))
            node->m_right->setForceType(TokenType::INTEGER);
        node->m_right->Accept(this);

        as->Term();
        as->Asm("les di, ["+var->getValue(as)+"]");
        if (var->m_expr->isPureNumeric()) {

            as->Asm("mov [es:di+"+var->m_expr->getValue(as)+"*"+getIndexScaleVal(as,var)+"],"+getReg("a",var));
            return;

        }
        if (var->m_expr->isPureVariable() && var->m_expr->isWord(as)) {
            as->Asm("add di,["+var->m_expr->getValue(as)+"]");
            if (var->isWord(as))
                as->Asm("add di,["+var->m_expr->getValue(as)+"]");

            as->Asm("mov [es:di],"+getReg("a",var));

            return;

        }

        as->Asm("push ax");
        var->m_expr->setForceType(TokenType::INTEGER);
        var->m_expr->Accept(this);
        as->Term();
        if (var->isWord(as))
            as->Asm("shl ax,1");
        as->Asm("mov bx,ax");

        as->Asm("pop ax");

        as->Asm("mov [es:di+bx],"+getReg("a",var));
        return;

    }



    if (var->isArrayIndex()) {
        // Is an array
        as->Asm(";Is array index");
        if (var->getArrayType(as)==TokenType::POINTER) {
            as->Comment("Assign value to pointer array");
            node->m_right->Accept(this);
            var->m_expr->Accept(this);
            as->Asm("shl ax,2 ; pointer lookup");
            as->Asm("mov bx,ax");
            as->Asm("lea si,["+var->getValue(as)+"]");
            as->Asm("mov [ds:si+bx],di ; store in pointer array");
            as->Asm("mov [ds:si+bx+2],es");

            return;
        }
        as->Comment("Assign value to regular array");

        node->m_right->Accept(this);
        // Handle var[ i ] :=
        if (var->m_expr->isPure()) {
            var->m_expr->setForceType(TokenType::INTEGER);
            if (var->m_expr->isPureNumeric()) {
                as->Asm("mov ["+var->getValue(as) + "+" + Util::numToHex(var->m_expr->getValueAsInt(as)*var->getArrayDataSize(as))+"],"+getReg() );
                return;
            }
            else {
                as->Asm("mov di,"+getARMValue(as,var->m_expr));
                if (var->isWord(as))
                    as->Asm("shl di,1");
            }
        }
        else {
            as->Asm("push ax");
            var->m_expr->setForceType(TokenType::INTEGER);
            var->m_expr->Accept(this);
            as->Asm("mov di,ax");
            if (var->isWord(as))
                as->Asm("shl di,1");
            as->Asm("pop ax");
        }
        as->Asm("mov ["+var->getValue(as) + "+di], "+getReg());


        return;
    }

//    if (var->getValue())
    // Simple a:=b;
    QString type =getWordByteType(as,var);

    if (node->m_right->isPureNumeric()) {
        as->Asm("mov ["+var->getValue(as)+ "], "+type+ " "+node->m_right->getValue(as));
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
    as->Asm("mov ["+qSharedPointerDynamicCast<NodeVar>(node->m_left)->getValue(as) + "], "+getReg());
    return;
}
*/
void CodeGenARM::DeclarePointer(QSharedPointer<NodeVarDecl> node)
{
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);

    as->Write(v->getValue(as)+ ": .word  0",0);

    as->m_symTab->Lookup(v->getValue(as), node->m_op.m_lineNumber)->m_arrayType=t->m_arrayVarType.m_type;

}

QString CodeGenARM::getEndType(Assembler *as, QSharedPointer<Node> v1, QSharedPointer<Node> v2)
{
    return "";
}


void CodeGenARM::BuildSimple(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool page)
{

    as->Comment("Binary clause Simplified: " + node->m_op.getType());
    //    as->Asm("pha"); // Push that baby

    BuildToCmp(node);
    QString jg ="jg ";
    QString jl ="jl ";
    QString jge ="jge ";
    QString jle ="jle ";
    if (!(node->m_left->isSigned(as) || node->m_right->isSigned(as))) {
        jg = "ja ";
        jl = "jb ";
        jge = "jae ";
        jle = "jbe ";
    }
    if (node->m_op.m_type==TokenType::EQUALS)
        as->Asm("jne " + lblFailed);
    if (node->m_op.m_type==TokenType::NOTEQUALS)
        as->Asm("je " + lblFailed);
    if (node->m_op.m_type==TokenType::LESS)
        as->Asm(jge  + lblFailed);
    if (node->m_op.m_type==TokenType::GREATER)
        as->Asm(jle  + lblFailed);

    if (node->m_op.m_type==TokenType::LESSEQUAL)
        as->Asm(jg  + lblFailed);
    if (node->m_op.m_type==TokenType::GREATEREQUAL)
        as->Asm(jl  + lblFailed);



}

void CodeGenARM::BuildToCmp(QSharedPointer<Node> node)
{
    if (node->m_left->getValue(as)!="") {
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
    }
    QString ax = getReg();
    QString bx = "b"+ QString(ax[1]);
    node->m_left->Accept(this);
    as->Term();
    if (node->m_right->isPure()) {
        as->Asm("cmp  "+ax+", " + getARMValue(as,node->m_right));
        return;

    }
    as->Comment("Evaluate full expression");
    as->Asm("push ax");
    node->m_right->Accept(this);
    as->Term();
    as->Asm("pop bx");

    as->Asm("cmp "+ax+","+bx);


}

void CodeGenARM::CompareAndJumpIfNotEqualAndIncrementCounter(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive)
{
    QString var = nodeA->m_left->getValue(as);
    if (step!=nullptr)
        ErrorHandler::e.Error("For loops currently don't support step",nodeA->m_op.m_lineNumber);

    QString x1 = BinopVariableNumber(nodeA->m_left,"#1", TokenType::Type::PLUS);

//    PushReg();
//    PushReg();
    QString ax = getReg();
    LoadVariable(nodeB);
//    PopReg();
//    PopReg();

    as->Asm(m_cmp+x1+","+ax);
    as->Asm("bne "+lblJump+"b");

}

void CodeGenARM::CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QString lblJump, bool isOffPage)
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

QString CodeGenARM::getReturn() { return "ret";}

void CodeGenARM::PrintBop(TokenType::Type type, QString x0, QString x1, QString value)
{
    if (type==TokenType::Type::PLUS)
        Binop("add",x0,x1,value);
    if (type==TokenType::Type::MINUS)
        Binop("sub",x0,x1,value);
    if (type==TokenType::Type::MUL)
        Binop("mul",x0,x1,value);
    if (type==TokenType::Type::DIV)
        Binop("div",x0,x1,value);
    if (type==TokenType::Type::BITOR)
        Binop("or",x0,x1,value);
    if (type==TokenType::Type::BITAND)
        Binop("and",x0,x1,value);

}


