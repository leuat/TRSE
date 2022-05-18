#include "codegen_x86.h"


CodeGenX86::CodeGenX86()
{

}

void CodeGenX86::dispatch(QSharedPointer<NodeBinOP>node)
{
        if (node->m_left->isWord(as) && !node->m_right->isPointer(as))
            node->m_right->setForceType(TokenType::INTEGER);
        if (node->m_right->isWord(as) && !node->m_left->isPointer(as))
            node->m_left->setForceType(TokenType::INTEGER);

/*        if (node->m_left->isWord(as))
            node->m_right->setForceType(TokenType::INTEGER);
        if (node->m_right->isWord(as))
            node->m_left->setForceType(TokenType::INTEGER);
*/
    as->ClearTerm();
    if (!node->isPointer(as)) {
        if (node->isPureNumeric()) {
            as->Asm("mov "+getAx(node)+", " + node->getValue(as) + "; binop is pure numeric");
            return;
        }
        if (node->isPureVariable()) {
            as->Asm("mov "+getAx(node)+", [" + node->getValue(as)+"] ; binop is pure variable");
            return;
        }
    }
/*    if (!node->m_left->isPure() && node->m_right->isPure()) {
        QSharedPointer<Node> t = node->m_right;
        node->m_right = node->m_left;
        node->m_left = t;
        qDebug() << "SWITCH";
    }
*/
    if (node->m_op.m_type == TokenType::MUL || node->m_op.m_type == TokenType::DIV) {
        node->m_left->Accept(this);
        QString bx = getAx(node->m_left);

        PushX();
        QString sign = "";
        bool isSigned = false;
        if (node->m_left->isSigned(as)||node->m_right->isSigned(as)) {
            sign = "i";
            isSigned = true;
        }
        if (node->m_op.m_type == TokenType::DIV) {
            //            node->m_right->setForceType(TokenType::BYTE);
            if (node->m_right->isWord(as)) {
                as->Asm("xor dx,dx");
                if (isSigned)
                    as->Asm("cwd");
            }
            else {
                if (isSigned) as->Asm("cbw");
                as->Asm("xor ah,ah");
            }
        }
        node->m_right->Accept(this);

        QString ax = getAx(node->m_right);
        PopX();
        as->BinOP(node->m_op.m_type);
        if (bx[0]!='a')  {
            as->Asm("push ax");
            as->Asm("mov ax,"+bx);
        }
        as->Asm(sign+as->m_term +" " +  ax);
        if (bx[0]!='a')  {
            as->Asm("mov "+bx+",ax");
            as->Asm("pop ax");
        }
        as->m_term = "";
        return;
    }
    if (node->m_left->isPointer(as) || node->m_right->isPointer(as)) {
        // Treat as pointers
        // Make sure left is pointer
        if (!node->m_left->isPointer(as)) {
            auto n = node->m_right;
            node->m_right = node->m_left;
            node->m_left = n;
        }
        as->ClearTerm();
        as->BinOP(node->m_op.m_type);
        QString bop = as->m_term;
        as->ClearTerm();

        if (node->m_right->isPointer(as)) {
            if (!node->m_right->isPure()) {
//                ErrorHandler::e.Error("Pointers can only add / sub with other pure pointers.", node->m_op.m_lineNumber);
                as->Comment("Interesting..");
                as->ClearTerm();
                node->m_right->Accept(this);
                as->Asm("push ax");

                QString ax =getAx(node->m_right);
                as->Comment("Interesting ends");
                node->m_left->Accept(this);

                QString bx =getAx(node->m_left);
                as->Asm("pop bx");
                as->Asm(bop + " ax,bx");
                return;

            }

            QString r = node->m_right->getValue(as);
            if (!node->m_right->isPointer(as)) {
                node->m_right->setForceType(TokenType::INTEGER);
                as->Comment("RHS is pure integer");
                return;
            }
            as->Comment("RHS is pure pointer as well! "+r);
            node->m_left->Accept(this); // Should always be a pointer
            as->Term();

            as->Asm("mov ax,es");
            as->Asm(bop+" ax,["+r+"+2]");
            as->Asm("mov es,ax");
            as->Asm(bop+" di,["+r+"]");
            return;
        }
        if (!node->m_left->isPure()) {
            as->Comment("Damn hard binop");
            PushX();
            PushX();
            node->m_right->setForceType(TokenType::INTEGER);
            QString bx = getAx(node->m_right);
            node->m_right->Accept(this);
            as->Asm("push "+bx);
            PopX();
            PopX();
            node->m_left->Accept(this); // Should always be a PURE pointer
            as->Asm("pop "+bx);
            as->Asm(bop+" " +bx+",ax ; generic binop when rhs is NOT pointer");
            as->Asm("mov ax,"+bx);
            return;
        }

        as->Comment("RHS is NOT pointer, only updating DI since LHS is pure");

        as->ClearTerm();
//        node->m_right->setForceType(TokenType::INTEGER);
        node->m_right->Accept(this);
        as->Term();
        if (!node->m_left->isPure()) {
//            ErrorHandler::e.Error("Left-hand side of equation must be pure pointer and not a binary operation!", node->m_op.m_lineNumber);

        }

        node->m_left->Accept(this); // Should always be a PURE pointer
        as->Asm(bop+" di,ax ; generic binop when rhs is NOT pointer");

        return;
    }

    if (node->m_right->isPure() && (node->m_op.m_type==TokenType::PLUS || node->m_op.m_type==TokenType::MINUS || node->m_op.m_type==TokenType::BITAND || node->m_op.m_type==TokenType::BITOR || node->m_op.m_type==TokenType::XOR)) {
        as->Comment("RHS is pure optimization");

        as->ClearTerm();
        node->m_left->Accept(this);
        QString ax = getAx(node);
        as->Term();
        as->Comment("Forcetype IS POINTER: "+QString::number(node->m_forceType==TokenType::POINTER));
        as->BinOP(node->m_op.m_type);
        if (node->m_left->isReference())
            ax="di";
        as->Asm(as->m_term + " "+ax+", "+getX86Value(as,node->m_right));
        as->ClearTerm();
        return;

    }


    as->Comment("Generic add/sub");
    node->m_left->Accept(this);
    QString bx = getAx(node->m_left);
    if (m_isPurePointer)
        bx = "di";


    PushX();
    if (node->m_op.m_type==TokenType::SHR || node->m_op.m_type==TokenType::SHL)
        PushX();
    node->m_right->Accept(this);
//    as->Term();
    QString ax = getAx(node->m_right);
    if (node->m_op.m_type==TokenType::SHR || node->m_op.m_type==TokenType::SHL) {
       PopX();
       ax = "cl";
    }
    PopX();
    as->BinOP(node->m_op.m_type);

    as->Asm(as->m_term + " " +  bx +"," +ax);
    as->m_term = "";


}

void CodeGenX86::dispatch(QSharedPointer<NodeNumber>node)
{
    QString ax = getAx(node);
    if (as->m_term!="") {
        as->m_term +=node->getValue(as);
        return;
    }

    as->Asm("mov "+ax+", " + node->getValue(as));
}

void CodeGenX86::dispatch(QSharedPointer<NodeVar> node)
{
    if (m_inlineParameters.contains(node->value)) {
  //      qDebug()<< "INLINE node override : "<< node->value;
        m_inlineParameters[node->value]->Accept(this);
        return;
    }
    QString ending = "]";
    if (node->m_expr!=nullptr) {

        if (node->getArrayType(as)==TokenType::POINTER && node->m_classApplied==false) {
            as->Comment("Looking up array of pointer : "+node->value);

            node->m_expr->setForceType(TokenType::INTEGER);
            node->m_expr->Accept(this);
            as->Asm("mov bx,ax");
            ShlAx("bx",2,false);
//            as->Asm("shl bx,2");

            as->Asm("lea si,["+node->getValue(as)+ "]");
            as->Asm("mov di,[ds:si+bx]");
            as->Asm("mov ax,[ds:si+bx+2]");
            as->Asm("mov es,ax");
            //            as->Asm("mov di,cx");

            return;
        }

        if (node->isPointer(as)) {
            //            as->Asm("push ax");
            as->Asm("les di,["+node->getValue(as)+ "]");
            if (node->m_expr->isPureNumeric()) {
//                as->Comment("Data size:"+QString::number(node->getArrayDataSize(as)));
                QString scale = "*"+QString::number(node->getArrayDataSize(as));

                as->Asm("mov ax, word [es:di + "+node->m_expr->getValue(as)+scale+"]");
                if (node->m_writeType==TokenType::LONG || node->isLong(as)) {
                    as->Asm("mov bx, word [es:di + "+node->m_expr->getValue(as)+"+2]");
                    as->Asm("mov es,bx");
                    as->Asm("mov di,ax");
                    return;
                }
                if (!node->isWord(as))
                    as->Asm("mov ah,0"); // Force byte
                return;

            }

            if (node->m_expr->isPureVariable() && !node->m_expr->hasArrayIndex() && node->m_expr->isWord(as)) {
                as->Asm("add di,word ["+node->m_expr->getValue(as)+"]");
                if (node->isWord(as))
                    as->Asm("add di,word ["+node->m_expr->getValue(as)+"]");
                as->Asm("mov ax, word [es:di]");
                if (!node->isWord(as))
                    as->Asm("mov ah,0");
                return;

            }
            node->m_expr->setForceType(TokenType::INTEGER);
            node->m_expr->Accept(this);
            as->Asm("add di,ax");
            if (node->getArrayType(as)==TokenType::INTEGER)
                //                as->Asm("shl di,1 ; Accomodate for word");
                as->Asm("add di,ax");


            //          as->Asm("pop ax");
            if (node->isWord(as))
                as->Asm("mov ax, word [es:di]; Is word");
            else
                if (node->isLong(as) || node->getWriteType()==TokenType::LONG) {
                    as->Asm("mov ax, word [es:di] ; is long" );
                    as->Asm("mov cx, word [es:di+2]" );
                    as->Asm("mov es,cx");
                    as->Asm("mov di,ax");

                }
            else
                as->Asm("mov al, byte [es:di]; Is byte" );
            return;
        }
        if (node->is8bitValue(as))
            as->Asm("mov ah,0 ; Accomodate for byte");
/*
        node->m_expr->setForceType(TokenType::INTEGER);
        node->m_expr->Accept(this);
        as->Asm("mov di,ax");
        if (node->getArrayType(as)==TokenType::INTEGER)
            as->Asm("shl di,1 ; Accomodate for word");
        ending = "+di]";
        */
        node->m_expr->setForceType(TokenType::INTEGER);


        if (!node->isPointer(as) && node->m_expr!=nullptr && node->getWriteType()==TokenType::LONG) {
            // Read / write long bytes from non-pointer
            node->m_expr->setForceType(TokenType::INTEGER);
            as->Comment("Writetype LONG to non-pointer");
            if (node->m_expr->isPureNumeric()) {
                as->Asm("mov si,"+node->getValue(as)+ "");
                as->Asm("mov di,[ds:si+"+node->m_expr->getValue(as)+"]");
                as->Asm("mov ax,[ds:si+"+node->m_expr->getValue(as)+"+2]");
                as->Asm("mov es,ax");
                return;
            }
            node->m_expr->Accept(this);
            as->Asm("mov bx,ax");
//            as->Asm("shl bx,2");

            as->Asm("mov si,"+node->getValue(as)+ "");
            as->Asm("mov di,[ds:si+bx]");
            as->Asm("mov ax,[ds:si+bx+2]");
            as->Asm("mov es,ax");
            return;
        }



        if (node->m_expr->isPure()) {
            if (node->m_expr->isPureNumeric()) {
                as->Asm("mov di,"+Util::numToHex(node->m_expr->getValueAsInt(as)*node->getArrayDataSize(as)));
            }
            else {
                as->Asm("mov di,"+getX86Value(as,node->m_expr));
                if (node->getArrayType(as)==TokenType::INTEGER)
                    as->Asm("shl di,1 ; Accomodate for word");
                if (node->getArrayType(as)==TokenType::LONG)
                    as->Asm("shl di,2 ; Accomodate for word");
            }
        }
        else {
            node->m_expr->Accept(this);
            as->Asm("mov di,ax");
            if (node->getArrayType(as)==TokenType::INTEGER)
                as->Asm("shl di,1 ; Accomodate for word");
            if (node->getArrayType(as)==TokenType::LONG)
                as->Asm("shl di,2 ; Accomodate for word");
        }
        ending = "+di]";

    }

/*    if (node->m_forceType==TokenType::POINTER && !node->isPointer(as)) {
        as->Comment("Force type is POINTER, converting");
        as->Asm("mov es,0");
        as->Asm("mov di, [" + node->getValue(as)+"]");
        return;
    }
*/
    if (node->isReference()) {
        as->Asm("mov di, " + node->getValue(as)+"");
        as->Asm("push ds");
        as->Asm("pop es");
        return;
    }

    if (node->isPointer(as) && !node->hasArrayIndex()) {
        as->Asm("les di, [" + node->getValue(as)+"]");
        return;
    }

//    as->Comment(";HERE "+QString::number(node->isPointer(as)) );



    if (as->m_term!="") {
        as->m_term +=node->getValue(as);
        return;
    }
    QString ax = getAx(node);




    as->Asm("mov "+ax+", [" + node->getValue(as)+ending);
//    if (node->hasArrayIndex())
//        qDebug() << TokenType::getType(node->getArrayType(as));
    if (node->m_forceType==TokenType::INTEGER) {
        bool accomodate = false;
        if (node->hasArrayIndex()) {
            if (node->getArrayType(as)!=TokenType::INTEGER) {
            accomodate = true;
            }
        }
        else
        if (node->getOrgType(as)!=TokenType::INTEGER)
            accomodate = true;

        if (accomodate && !node->isPointer(as)) {
            QString aa = QString(getAx(node)[0]);
            as->Asm("mov "+QString(ax[0])+"h,0 ; forcetype clear high bit");
        }
    }
//    qDebug() << "ORG " <<TokenType::getType(node->getOrgType(as)) << "   : " << node->getValue(as);
  //  qDebug() << "FT " <<TokenType::getType(node->m_forceType);

}


void CodeGenX86::dispatch(QSharedPointer<NodeString> node)
{

}



void CodeGenX86::dispatch(QSharedPointer<NodeVarType> node)
{

}

void CodeGenX86::dispatch(QSharedPointer<NodeBinaryClause> node)
{

}


void CodeGenX86::dispatch(QSharedPointer<Node> node)
{

}

void CodeGenX86::dispatch(QSharedPointer<NodeRepeatUntil> node)
{
    ErrorHandler::e.Error("Repeat-until not implemented yet", node->m_op.m_lineNumber);

}
void CodeGenX86::dispatch(QSharedPointer<NodeComment> node)
{

}

void CodeGenX86::StoreVariable(QSharedPointer<NodeVar> n)
{

}

bool CodeGenX86::StoreVariableSimplified(QSharedPointer<NodeAssign> node)
{
    auto var = node->m_left;
    QString type =getWordByteType(as,var);

    if (node->m_right->isPureNumeric() && !node->m_left->isPointer(as) && !node->m_left->hasArrayIndex()) {
        as->Asm("mov ["+var->getValue(as)+ "], "+type+ " "+node->m_right->getValue(as));
        return true;
    }
    return false;

}

void CodeGenX86::LoadVariable(QSharedPointer<NodeVar> n)
{
    n->Accept(this);
}
void CodeGenX86::LoadVariable(QSharedPointer<NodeProcedure> node)
{
    ErrorHandler::e.Error("Procedure address not implemented yet! Please bug the developer", node->m_op.m_lineNumber);
}

void CodeGenX86::LoadPointer(QSharedPointer<Node> n)
{

}

void CodeGenX86::LoadVariable(QSharedPointer<Node> n)
{
    n->Accept(this);

}

void CodeGenX86::LoadVariable(QSharedPointer<NodeNumber>n)
{

}

QString CodeGenX86::getIndexScaleVal(Assembler *as, QSharedPointer<Node> var)
{
    if (var->isWord(as))
        return "2";
    if (var->isLong(as))
        return "4";
    return "1";
}

QString CodeGenX86::getAx(QSharedPointer<Node> n) {
    QString a = m_regs[m_lvl];

    if (n->m_forceType==TokenType::INTEGER)
        return a+"x";
    if (n->m_forceType==TokenType::BYTE)
        return a+"l";

    if (n->getType(as)==TokenType::INTEGER || n->getType(as)==TokenType::POINTER)
        return a+"x";
    if (n->getType(as)==TokenType::ADDRESS)
        return a+"x";
    if (n->getType(as)==TokenType::INTEGER_CONST)
        if (n->isWord(as))
            return a+"x";
    //        if (n->isPureNumeric())
    //          if (n->getValue()
    return a+"l";
}

QString CodeGenX86::getAx(QString a, QSharedPointer<Node> n) {


    if (n->m_forceType==TokenType::INTEGER)
        return a+"x";
    if (n->isWord(as))
        return a+"x";

    return a+"l";


}

QString CodeGenX86::getBinaryOperation(QSharedPointer<NodeBinOP> bop) {
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

void CodeGenX86::PushX() {
    if (m_lvl==3)
        ErrorHandler::e.Error("Error in X86 dispatcher PopX : trying to push regstack from max");
    m_lvl++;
}

void CodeGenX86::PopX() {
    if (m_lvl==0)
        ErrorHandler::e.Error("Error in X86 dispatcher PopX : trying to pop regstack from zero");
    m_lvl--;
}

QString CodeGenX86::getEndType(Assembler *as, QSharedPointer<Node> v)
{
    return "";
}




void CodeGenX86::AssignString(QSharedPointer<NodeAssign> node) {


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

bool CodeGenX86::AssignPointer(QSharedPointer<NodeAssign> node)
{
    return false;
}

void CodeGenX86::GenericAssign(QSharedPointer<NodeAssign> node)
{
    node->m_right->Accept(this);
    as->Term();
    as->Asm("mov ["+qSharedPointerDynamicCast<NodeVar>(node->m_left)->getValue(as) + "], "+getAx(node->m_left));
    return;

}

bool CodeGenX86::IsAssignPointerWithIndex(QSharedPointer<NodeAssign> node)
{
    // Set pointer value
    auto var = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    if (var->isPointer(as) && var->hasArrayIndex()) {

        // TO DO: Optimize special cases

        as->ClearTerm();
        as->Comment("Assigning pointer with index, type:" + TokenType::getType(var->m_writeType) + " right is pointer: " +QString::number(node->m_right->isPointer(as)));
        if (var->isWord(as))
            node->m_right->setForceType(TokenType::INTEGER);

        node->m_right->Accept(this);

        as->Term();
        if (var->m_expr->isPureNumeric() && node->getWriteType()!=TokenType::LONG) {
            as->Asm("les di, ["+var->getValue(as)+"]");
            as->Asm("mov [es:di+"+var->m_expr->getValue(as)+"*"+getIndexScaleVal(as,var)+"],"+getAx("a",var));
            return true;

        }
        if (var->m_expr->isPureVariable() && var->m_expr->isWord(as)) {
            as->Asm("les di, ["+var->getValue(as)+"]");
            as->Asm("add di,["+var->m_expr->getValue(as)+"]");
            if (var->isWord(as))
                as->Asm("add di,["+var->m_expr->getValue(as)+"]");

            as->Asm("mov [es:di],"+getAx("a",var));

            return true;

        }

        if (node->m_right->isPointer(as)) {
            as->Asm("push es");
            as->Asm("push di");
        }
        else
        as->Asm("push ax");
        as->Asm("les di, ["+var->getValue(as)+"]");

        var->m_expr->setForceType(TokenType::INTEGER);
        var->m_expr->Accept(this);
        as->Term();
        if (var->isWord(as))
            ShlAx("ax",1,false);
        as->Asm("mov bx,ax");

        if (node->m_right->isPointer(as)) {
            as->Asm("pop ax");
            as->Asm("pop cx");
            as->Asm("mov [es:di+bx],ax");
            as->Asm("mov [es:di+bx+2],cx");
            return true;

        }

        as->Asm("pop ax");

        as->Asm("mov [es:di+bx],"+getAx("a",var));
        return true;

    }
    return false;
}

bool CodeGenX86::IsAssignArrayWithIndex(QSharedPointer<NodeAssign> node)
{
    auto var = qSharedPointerDynamicCast<NodeVar>(node->m_left);

    if (var->hasArrayIndex()) {
        // Is an array
        as->Asm(";Is array index");
        if (var->getArrayType(as)==TokenType::POINTER) {
            as->Comment("Assign value to pointer array");
            node->m_right->Accept(this);
            var->m_expr->Accept(this);
            ShlAx("ax",2,false);
            as->Asm("mov bx,ax");
            as->Asm("lea si,["+var->getValue(as)+"]");
            as->Asm("mov [ds:si+bx],di ; store in pointer array");
            as->Asm("mov [ds:si+bx+2],es");

            return true;
        }
        as->Comment("Assign value to regular array");

        node->m_right->Accept(this);
        // Handle var[ i ] :=
        if (var->m_expr->isPure()) {
            var->m_expr->setForceType(TokenType::INTEGER);
            if (var->m_expr->isPureNumeric()) {
                as->Asm("mov ["+var->getValue(as) + "+" + Util::numToHex(var->m_expr->getValueAsInt(as)*var->getArrayDataSize(as))+"],"+getAx(node->m_left) );
                return true;
            }
            else {
                as->Asm("mov di,"+getX86Value(as,var->m_expr));
                if (var->isWord(as))
                    ShlAx("di",1,false);
            }
        }
        else {
            as->Asm("push ax");
            var->m_expr->setForceType(TokenType::INTEGER);
            var->m_expr->Accept(this);
            as->Asm("mov di,ax");
            if (var->isWord(as))
                ShlAx("di",1,false);
            as->Asm("pop ax");
        }
        as->Asm("mov ["+var->getValue(as) + "+di], "+getAx(node->m_left));


        return true;
    }

    return false;
}

bool CodeGenX86::IsSimpleIncDec(QSharedPointer<NodeAssign> node)
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
            as->Comment("Type : "+TokenType::getType(var->getType(as)));
            as->Asm(getBinaryOperation(bop) + " ["+var->getValue(as)+"], "+getAx(var));
            return true;
        }
        // Check for a:=a+

    }

    return false;
}

bool CodeGenX86::IsSimpleAssignPointer(QSharedPointer<NodeAssign> node)
{
    auto var = node->m_left;
    if (var->isPointer(as) && !var->hasArrayIndex()) {

        node->m_right->VerifyReferences(as);
        if (!node->m_right->isReference())
            if (!node->m_right->isPointer(as))
                if ((node->m_right->isWord(as) || node->m_right->isByte(as))) {
                    if (node->m_right->getWriteType()!=TokenType::LONG)
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
                    return true;
                }
                bop->m_right->Accept(this);
                as->Term();
                as->Asm(bopCmd + " ["+var->getValue(as)+"], ax");


                return true;
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
            return true;
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
        return true;


    }
    return false;

}

void CodeGenX86::OptimizeBinaryClause(QSharedPointer<Node> node, Assembler *as)
{

}

void CodeGenX86::AssignFromRegister(QSharedPointer<NodeAssign> node)
{
    ErrorHandler::e.Error("Cannot (yet) assign variables from registers on the x86",node->m_op.m_lineNumber);
}

void CodeGenX86::AssignToRegister(QSharedPointer<NodeAssign> node)
{
    QString vname = getValue(node->m_left);
    vname = vname.toLower();
    if (!node->m_right->isPure())
        ErrorHandler::e.Error("When assigning registers, RHS needs to be pure numeric or variable",node->m_op.m_lineNumber);

    QString reg = vname.remove(0,1);
//        as->Comment("Assigning register : " + vname);

    as->Asm("mov "+reg+", "+getX86Value(as,node->m_right));
    return;

}


/*void CodeGenX86::AssignVariable(QSharedPointer<NodeAssign> node)
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

        as->Asm("mov "+reg+", "+getX86Value(as,node->m_right));
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

            as->Asm("mov [es:di+"+var->m_expr->getValue(as)+"*"+getIndexScaleVal(as,var)+"],"+getAx("a",var));
            return;

        }
        if (var->m_expr->isPureVariable() && var->m_expr->isWord(as)) {
            as->Asm("add di,["+var->m_expr->getValue(as)+"]");
            if (var->isWord(as))
                as->Asm("add di,["+var->m_expr->getValue(as)+"]");

            as->Asm("mov [es:di],"+getAx("a",var));

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

        as->Asm("mov [es:di+bx],"+getAx("a",var));
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
                as->Asm("mov ["+var->getValue(as) + "+" + Util::numToHex(var->m_expr->getValueAsInt(as)*var->getArrayDataSize(as))+"],"+getAx(node->m_left) );
                return;
            }
            else {
                as->Asm("mov di,"+getX86Value(as,var->m_expr));
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
        as->Asm("mov ["+var->getValue(as) + "+di], "+getAx(node->m_left));


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
            as->Asm(getBinaryOperation(bop) + " ["+var->getValue(as)+"], "+getAx(var));
            return;
        }
        // Check for a:=a+

    }
    as->ClearTerm();
    node->m_right->Accept(this);
    as->Term();
    as->Asm("mov ["+qSharedPointerDynamicCast<NodeVar>(node->m_left)->getValue(as) + "], "+getAx(node->m_left));
    return;
}
*/
void CodeGenX86::DeclarePointer(QSharedPointer<NodeVarDecl> node)
{
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::GAMEBOY || Syntax::s.m_currentSystem->m_system == AbstractSystem::COLECO)
        as->Write(v->getValue(as)+ ": ds  2" ,0);
    else
        if (Syntax::s.m_currentSystem->m_system == AbstractSystem::X86)
            as->Write(v->getValue(as)+ ": dw  0,0" ,0);
        else
            as->Write(v->getValue(as)+ ": dw  0",0);

    as->m_symTab->Lookup(v->getValue(as), node->m_op.m_lineNumber)->m_arrayType=t->m_arrayVarType.m_type;

}

QString CodeGenX86::getEndType(Assembler *as, QSharedPointer<Node> v1, QSharedPointer<Node> v2)
{
    return "";
}


void CodeGenX86::BuildSimple(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool page)
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

void CodeGenX86::BuildToCmp(QSharedPointer<Node> node)
{
    if (node->m_left->getValue(as)!="") {
        if (node->m_right->isPureNumeric())
        {
            as->Comment("Compare with pure num / var optimization");
            //            TransformVariable(as,"cmp",node->m_left->getValue(as),node->m_right->getValue(as),node->m_left);
//            TransformVariable(as,"cmp",node->m_left->getValue(as),node->m_right->getValue(as),node->m_left);
            if (node->m_left->isPureNumeric() && node->m_right->isPureNumeric()) {
//                as->Asm("cmp "+getWordByteType(as,node->m_left) +" " +node->m_left->getValue(as)+","+getWordByteType(as,node->m_left)+" " + node->m_right->getValue(as));
                as->Comment("Compare two const numbers");
                if (node->m_left->getValueAsInt(as)==node->m_right->getValueAsInt(as))
                    as->Asm("xor ax,ax ; clear zero flag");
                else
                {
                    as->Asm("mov ax,1");
                    as->Asm("sub ax,2");
                }
                return;
            }
            if (node->m_left->isPure()) {
                as->Asm("cmp ["+node->m_left->getValue(as)+"],"+getWordByteType(as,node->m_left)+" " + node->m_right->getValue(as));
                return;
            }

            LoadVariable(node->m_left);
            as->Asm("cmp "+getAx(node->m_left)+"," + node->m_right->getValue(as));

            return;
        } else
        {
            as->Comment("Compare two vars optimization");

            if (node->m_right->isPureVariable()) {
                //QString wtf = as->m_regAcc.Get();
                LoadVariable(node->m_right);
                //TransformVariable(as,"move",wtf,qSharedPointerDynamicCast<NodeVar>node->m_left);
                //TransformVariable(as,"cmp",wtf,as->m_varStack.pop());
                as->Asm("cmp  ["+node->m_left->getValue(as) +"]," + getAx(node->m_right));

                return;
            }
            node->m_right->Accept(this);
            as->Term();

            as->Asm("cmp  "+node->m_left->getValue(as) +"," + getAx(node->m_right));

//            TransformVariable(as,"cmp",qSharedPointerDynamicCast<NodeVar>node->m_left,as->m_varStack.pop());
            return;
        }
    }
    QString ax = getAx(node->m_left);
    QString bx = "b"+ QString(ax[1]);
    node->m_left->Accept(this);
    as->Term();
    if (node->m_right->isPure()) {
        as->Asm("cmp  "+ax+", " + getX86Value(as,node->m_right));
        return;

    }
    as->Comment("Evaluate full expression");
    as->Asm("push ax");
    node->m_right->Accept(this);
    as->Term();
    as->Asm("pop bx");

    as->Asm("cmp "+ax+","+bx);


}

void CodeGenX86::CompareAndJumpIfNotEqualAndIncrementCounter(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive)
{
    QString var = nodeA->m_left->getValue(as);
    if (step!=nullptr) {
        step->setForceType(TokenType::INTEGER);
        step->Accept(this);
        as->Asm("mov cx,ax");
    }
    if (step==nullptr)
        as->Asm("add ["+var+"],"+getWordByteType(as,nodeA->m_left)+" 1");
    else
        as->Asm("add ["+var+"],cx");
/*
    if (nodeA->m_left->isByte(as))
        nodeB->setForceType(TokenType::BYTE);

    if (nodeA->m_left->isWord(as))
        nodeB->setForceType(TokenType::INTEGER);
*/
    LoadVariable(nodeB);
    if (isInclusive)
        as->Asm("add "+getAx(nodeB)+",1");


    as->Asm(m_cmp+getAx(nodeB)+","+getWordByteType(as,nodeA->m_left)+" ["+var+"]");
    as->Asm(m_jne+lblJump);

}

void CodeGenX86::CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QString lblJump, bool isOffPage)
{
    if (nodeA->isWord(as)) nodeB->setForceType(TokenType::INTEGER);
    LoadVariable(nodeA);
    QString ax = getAx(nodeA);
    PushX();
    LoadVariable(nodeB);
    QString bx = getAx(nodeB);
    PopX();
    as->Asm(m_cmp+ax+","+bx);
    as->Asm(m_jne+lblJump);

}

void CodeGenX86::ShlAx(QString ax, int val, bool pushpopcl)
{
    if (Syntax::s.m_currentSystem->is8088()) {
        if (pushpopcl)
            as->Asm("push cl");
        as->Asm("mov cl,"+QString::number(val));
        as->Asm("shl "+ax+",cl");
        if (pushpopcl)
            as->Asm("pop cl");
        return;

    }
   as->Asm("shl "+ax+","+QString::number(val));

}


