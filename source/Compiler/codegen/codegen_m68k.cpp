#include "codegen_m68k.h"
#include "source/Compiler/ast/nodefactory.h"

CodeGen68k::CodeGen68k()
{

}


void CodeGen68k::dispatch(QSharedPointer<NodeBinOP>node) {
    node->DispatchConstructor(as,this);


    if (node->isPureNumeric()) {
        //qDebug() << "IS PURE NUMERIC BinOp";
        as->Comment("NodeBinop : both are pure numeric optimization : "+node->getValue(as));
        as->m_varStack.push(node->getValue(as));
        return;
    }


    as->BinOP(node->m_op.m_type,node->isSigned(as));


    if (node->m_left->isWord(as) && node->m_right->isByte(as) && node->m_op.m_type==TokenType::MUL)
        node->SwapNodes();

    QString op = as->m_varStack.pop();
    QString d0 = as->m_regAcc.Get();
    bool adv = false;
    if (node->m_op.m_type == TokenType::SHR || node->m_op.m_type == TokenType::SHL) {
        if (!node->m_right->isPureNumeric())
            adv = true;
    }
    if (adv)
        as->Comment("Advanced binop: Evaluate expr first");

//    as->Comment("BOP NEW register: " + d0);
//    if (m_clearFlag) {

     TransformVariable(as,"moveq",d0,"#0");
    //    m_clearFlag=false;
  //  }
    node->m_left->Accept(this);
    QString endtype = getEndType(as,node->m_left, node->m_right);
    if (node->m_left->isByte(as))
        endtype =".b";

    QString d1 = as->m_varStack.pop();
    if (m_regs.contains(d1) && m_regs.contains(d0))
        endtype = ".l";
    TransformVariable(as,"move"+endtype,d0 + "     ; BOP move",d1);



//    qDebug() << "NodeBinOp : " << op;
  //  as->Comment("NodeBinop  : " +op);
    if (op.toLower().contains("mul") || op.toLower().contains("div")) {
            op+=".w";
        if (node->isSigned(as))
            op = op.replace("u","s");
        auto nv = qSharedPointerDynamicCast<NodeVar>(node->m_left);
        if (nv)
            as->Comment("ORG TYPE of "+nv->value+" "+TokenType::getType(nv->getOrgType(as)));
        if (!node->m_left->isWord(as) || !node->m_right->isWord(as)) {
            adv = true;
            as->Comment("LHS is byte, so initiate advanced op");


        }
    }

    else op=op + getEndType(as,node->m_left, node->m_right);//+m_lastSize;//+".l";

//    as->Comment("d0 used:" +d0);
    if (adv) {
        QString d1 = as->m_regAcc.Get();

        node->m_right->Accept(this);
        TransformVariable(as,"move"+getEndType(as,node->m_right, node->m_right),d1 + "     ; Advanced move",as->m_varStack.pop());

        TransformVariable(as,op,d0,d1);

        as->m_regAcc.Pop(d1);

    }
    else {
        node->m_right->Accept(this);
        TransformVariable(as,op,d0,as->m_varStack.pop());
    }


    as->m_varStack.push(d0);



    as->m_regAcc.Pop(d0);
}


/*void CodeGen68k::dispatchOld(QSharedPointer<NodeBinOP>node)
{
    node->DispatchConstructor(as,this);

    if (node->isPureNumeric()) {
        //qDebug() << "IS PURE NUMERIC BinOp";
        as->Comment("NodeBinop : both are pure numeric optimization : "+node->getValue(as));
        as->m_varStack.push(node->getValue(as));
        return;
        int val = node->BothPureNumbersBinOp(as);
        QString s = "";
        if (node->m_left->isAddress() || node->m_right->isAddress())
            s = "";
        //qDebug() << "A0";
        as->Comment("NodeBinop : both are pure numeric optimization");
        QString d0 = as->m_regAcc.Get();
        //qDebug() << "A05";
        as->Asm("move "+s+node->getValue(as) +","+d0);
        //qDebug() << "A07";
        as->m_regAcc.Pop(d0);
        as->m_varStack.push(d0);
        //qDebug() << "A1";
//        StoreVariable(as,)
        return;
    }

//    qDebug() << "Varstack count: " << as->m_varStack.m_vars.length();
  //  qDebug() << "Binop: " << as->m_varStack.m_vars.length();
    as->BinOP(node->m_op.m_type);
    QString op = as->m_varStack.pop();


//    as->Comment(" ** StartFree registers: " +QString::number(as->m_regAcc.m_free.length()));
    QString d0 = "";
    bool start = false;
    if (node->m_left->isPureNumeric() || node->m_left->isPureVariable())
        start = true;


    if (start) {
        QString l = as->m_regAcc.m_latest;
        d0 = as->m_regAcc.Get();
        as->Comment("BOP NEW register: " + d0);
        if (d0==l) {
            QString d1 = as->m_regAcc.Get();
            as->m_regAcc.Pop(d0);
            d0=d1;

        }
        if (m_clearFlag)
            TransformVariable(as,"moveq",d0,"#0");

        as->m_regAcc.m_latest = d0;
    }
    else {
        node->m_left->Accept(this);
        d0 = as->m_regAcc.m_latest;
    //    as->Comment("BOP Peeklatest : " + d0);

    }

    if (start) {
//        NodeVar nv =
        node->m_left->Accept(this);
  //      LoadVariable(node->m_left);
        as->Comment("BOP Start : " + d0);

        TransformVariable(as,"move",d0,as->m_varStack.pop(),node->m_left);
    }


    node->m_right->Accept(this);
    QString right = as->m_varStack.pop();



    if (op.toLower().contains("mul") || op.toLower().contains("div"))
        op = op+".w"; else op=op +m_lastSize;//+".l";

    as->Comment("d0 used:" +d0);

    TransformVariable(as,op,d0,right);


    as->m_varStack.push(d0);
    as->m_regAcc.m_latest =d0;

    as->Comment("BOP popping: " + d0);
    as->m_regAcc.Pop(d0);

}
*/

void CodeGen68k::dispatch(QSharedPointer<NodeNumber>node)
{
    as->m_varStack.push(node->getValue(as));

}


void CodeGen68k::dispatch(QSharedPointer<NodeString> node)
{

}

void CodeGen68k::dispatch(QSharedPointer<NodeUnaryOp> node)
{
  //  node->DispatchConstructor(as,this);
//    node->Accept(this);
//    if (node->isMinusOne())
    if (!(node->m_op.m_type==TokenType::MINUS))
        node->m_right->Accept(this);
    else {
        QSharedPointer<NodeNumber>n = qSharedPointerDynamicCast<NodeNumber>(node->m_right);
        if (n!=nullptr)
           n->m_val*=-1;
        node->m_right->Accept(this);
    }

    as->Comment("Unary op for : " + node->m_right->getValue(as));
}


void CodeGen68k::dispatch(QSharedPointer<NodeVarDecl> node)
{

    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);


    node->DispatchConstructor(as,this);

    if (t->m_flags.contains("chipmem"))
        as->m_currentBlock = as->m_chipMem;

    if (t->m_flags.contains("aligned")) {
        as->Asm(" 	CNOP 0,256");
    }


    AbstractCodeGen::dispatch(node);

    as->m_currentBlock = nullptr;




}

void CodeGen68k::dispatch(QSharedPointer<NodeBlock> node)
{

    node->DispatchConstructor(as,this);

    as->PushBlock(node->m_currentLineNumber);


    bool blockLabel = false;
    bool blockProcedure = false;
    bool hasLabel = false;

    QString label = as->NewLabel("block");


    if (node->m_decl.length()!=0) {
        as->Asm("jmp " + label);
        hasLabel = true;
        //           as->PushBlock(m_decl[0]->m_op.m_lineNumber-1);
    }
    for (QSharedPointer<Node> n: node->m_decl) {
        if (qSharedPointerDynamicCast<NodeVarDecl>(n)==nullptr) {
            if (!blockProcedure) // Print label at end of vardecl
            {
                if (n->m_op.m_lineNumber!=0) {
                    //                      as->PopBlock(n->m_op.m_lineNumber);
                    blockProcedure = true;
                    //   qDebug() << "pop" << n->m_op.m_lineNumber << " " << TokenType::getType(n->getType(as));
                }

            }

        }
        //if (dynamic_cast<QSharedPointer<NodeProcedureDecl>>(n)==nullptr)
        //qDebug() << "VarDeclBuild:" ;
        n->Accept(this);

    }
    as->VarDeclEnds();
    if (node->m_decl.length()!=0)
        as->Asm(" 	CNOP 0,4");

    if (!blockLabel && hasLabel) {
        as->Label(label);

    }

    if (node->forceLabel!="")
        as->Label(node->forceLabel);

    // (Amiga) interrupt.
    if (node->m_forceInterupt!="") {
//        as->Asm(" 	CNOP 0,4");
        if (Syntax::s.m_currentSystem->m_system == AbstractSystem::AMIGA) {

            as->Asm("movem.l d0-d7/a0-a6,-(sp)");
            as->Asm("btst #5,$dff01f ;Check if it's the VB!");
            as->Asm("; Save all Dx and Ax regs");
            as->Asm("; Interrupt requested at level 3.");
            as->Asm("beq.w "+node->m_forceInterupt);
            as->Asm("move.w #$0020,$dff09c ; Acknowledge VB interrupt");
            as->Asm("move.w #$0020,$dff09c ; Twice for compatibility");
        }
        if (Syntax::s.m_currentSystem->m_system == AbstractSystem::ATARI520ST) {
    //        as->Asm("move.w  sr,-(a7)                ; backup status register");
     //       as->Asm("or.w    #$0700,sr               ; disable interrupts");
            as->Asm("movem.l d0-d7/a0-a6,-(a7)");

        }

    }



    if (node->m_compoundStatement!=nullptr)
        node->m_compoundStatement->Accept(this);


    as->PopBlock(node->m_currentLineNumber);


}

void CodeGen68k::dispatch(QSharedPointer<NodeVarType> node)
{

}

void CodeGen68k::dispatch(QSharedPointer<NodeBinaryClause> node)
{

}


void CodeGen68k::dispatch(QSharedPointer<NodeProcedureDecl> node)
{
    node->DispatchConstructor(as,this);



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
    QString endLabel = "";
    if (node->m_type==0) {
    }
    else {
        endLabel = as->NewLabel("endInterrupt");
        as->PopLabel("endInterrupt");
        if (node!=nullptr && node->m_block!=nullptr) {
            QSharedPointer<NodeBlock> nb = qSharedPointerDynamicCast<NodeBlock>(node->m_block);
            if (nb!=nullptr) {
                nb->m_forceInterupt = endLabel;
            }
        }
    }
  //as->Label("afterProc_" + m_procName);


    if (!isInitFunction) {
        //as->Asm("jmp afterProc_" + m_procName);


        //as->Label(m_procName);
    }
//    if (m_isInterrupt)
  //      as->Asm("dec $d019        ; acknowledge IRQ");
    if (node->m_block!=nullptr) {
        QSharedPointer<NodeBlock> b = qSharedPointerDynamicCast<NodeBlock>(node->m_block);
        if (b!=nullptr)
            b->forceLabel=node->m_procName;
        node->m_block->Accept(this);
//        node->m_block->Build(as);
    }
    if (!isInitFunction) {
        if (node->m_type==0) {
            as->Asm("rts");
        }
        else {
            //as->Asm("rti");
            if (endLabel!="")
                as->Label(endLabel);
            if (Syntax::s.m_currentSystem->m_system == AbstractSystem::AMIGA)
                as->Asm("movem.l (sp)+,d0-d7/a0-a6 ");
            if (Syntax::s.m_currentSystem->m_system == AbstractSystem::ATARI520ST) {
                as->Asm("movem.l (a7)+,d0-d7/a0-a6");
  //              as->Asm("move.w  (a7)+,sr                ; restore status register");
                as->Asm("bclr   #0,$fffffa0f ; Acknowledge end of irq");

            }
//            as->Asm("movem d0-a6,-(sp)");
            as->Asm("rte");
        }
      //as->Label("afterProc_" + m_procName);
    }



}
void CodeGen68k::dispatch(QSharedPointer<NodeVar> node)
{
//    LoadVariable(node);

    if (m_inlineParameters.contains(node->value)) {
  //      qDebug()<< "INLINE node override : "<< node->value;
        m_inlineParameters[node->value]->Accept(this);
        return;
    }


    if (node->m_expr!=nullptr) {
//        qDebug() << "HERE";
        LoadVariable(node);
        return;
    }
    as->m_varStack.push(node->getValue(as));
}

void CodeGen68k::dispatch(QSharedPointer<Node> node)
{

}

void CodeGen68k::dispatch(QSharedPointer<NodeRepeatUntil> node)
{
    ErrorHandler::e.Error("Repeat-until not implemented yet", node->m_op.m_lineNumber);

}


void CodeGen68k::dispatch(QSharedPointer<NodeComment> node)
{

}



void CodeGen68k::StoreVariable(QSharedPointer<NodeVar> n)
{
    as->Comment("Store variable : " + n->getValue(as));

    if (n->m_subNode!=nullptr){
        QSharedPointer<NodeVar> nv = qSharedPointerDynamicCast<NodeVar>(n->m_subNode);
        if (nv->m_expr!=nullptr) {
            n->m_expr = nv->m_expr;
            nv->m_expr = nullptr;
            n->m_ignoreRecordExpr = true;
            nv->m_ignoreRecordExpr = true;
        }
    }



    if (n->m_expr!=nullptr) {
        QString d0 = as->m_varStack.pop();


        if (n->m_expr->isPureNumeric()) {
            as->Comment("StoreVariable pure numeric optimization");
            int val = n->m_expr->getValueAsInt(as)*n->getArrayDataSize(as);
            as->Comment("expr : " +QString::number(n->m_expr->getValueAsInt(as)));
            as->Comment("data size : " +QString::number(n->getArrayDataSize(as)));
/*            if (n->getArrayType(as)==TokenType::INTEGER)
                val*=2;
            if (n->getArrayType(as)==TokenType::LONG)
                val*=4;*/
            QString a0 = as->m_regMem.Get();

            if (n->getType(as)==TokenType::POINTER)
                TransformVariable(as,"move.l",a0,n->getValue(as));
            else
                TransformVariable(as,"lea",a0,n->getValue(as));

//            as->Comment(";general X");
            TransformVariable(as,"move"+getEndType(as,n),Util::numToHex(val)+"("+a0+")",d0);

            as->m_regMem.Pop(a0);
            return;
        }

//        bool done = false;
        as->Term();
        as->m_regAcc.m_latest="";
//        QString d0 = as->m_varStack.pop();
        QString d2 = as->m_regAcc.Get();
        QString oldd2 = "";
        bool newD2 = false;
        if (d2 == d0) { // If they are equal, get another one {
            oldd2 =d2;
            d2 = as->m_regAcc.Get();
            newD2 = true;
        }
        QString a0 = as->m_regMem.Get();
        LoadVariable(n->m_expr);
        QString d1 = as->m_varStack.pop();
        if (n->getArrayType(as)==TokenType::INTEGER) {
            as->Asm("lsl #1,"+d1);
        }
        if (n->getArrayType(as)==TokenType::LONG) {
            as->Asm("lsl #2,"+d1);
        }
        if (n->getType(as)==TokenType::POINTER)
            TransformVariable(as,"move.l",a0,n->getValue(as));
        else
            TransformVariable(as,"lea",a0,n->getValue(as));

        TransformVariable(as,"move"+getEndType(as,n),"("+a0+","+d1+")",d0);

        as->m_regMem.Pop(a0);
        if (m_regs.contains(d2)) {
            as->m_regAcc.Pop(d2);
        }
        if (oldd2!="")
            as->m_regAcc.Pop(oldd2);

        return;
    }
    QString d0 = as->m_varStack.pop();

    TransformVariable(as,"move"+getEndType(as,n),n->getValue(as),d0);
}


void CodeGen68k::LoadVariable(QSharedPointer<NodeVar> n)
{
//    TokenType::Type t = as->m_symTab->Lookup(n->getValue(as), n->m_op.m_lineNumber)->getTokenType();

    if (n->m_expr!=nullptr) {
//        qDebug() << n->m_op.getType();
  //      exit(1);
        bool done = false;
        if (as->m_regAcc.m_latest.length()==2) {
            //as->Comment("Trying to clear: " + as->m_regAcc.m_latest + " YO");
//            TransformVariable(as,"move.l",as->m_regAcc.m_latest,"#0");
            done = true;
        }
//        move.b (a0,$1),d0             ; LoadVariable:: is array
        // move.b $1(a0),d0
        QString d0 = as->m_regAcc.Get();
        QString a0 = as->m_regMem.Get();
        m_clearFlag=true;
        as->Term();
        if (n->m_expr->isPureNumeric() && !n->isPointer(as)) {
            as->Comment("Shift is pure numeric optimization");
            if (n->isPointer(as))
                TransformVariable(as,"move.l",a0,n->getValue(as));
            else
                TransformVariable(as,"lea",a0,n->getValue(as));

            int shift = n->m_expr->getValueAsInt(as)*n->m_expr->getArrayDataSize(as);
            as->Asm("move"+getEndType(as,n)+" "+Util::numToHex(shift)+"("+a0+"),"+d0);
            as->m_varStack.push(d0);

            as->m_regMem.Pop(a0);
            as->m_regAcc.Pop(d0);

            return;
        }

        QString trp = "             ; LoadVariable:: is array";
        if (!done ) {
//            as->Comment("Clearing : " + d0 + " YO");
            m_clearFlag=true;
           // TransformVariable(as,"move.l",d0,"#0");
        }
        //qDebug() << "Loading array: expression";
        LoadVariable(n->m_expr);
        QString d1 = as->m_varStack.pop();
        if (n->getArrayType(as)==TokenType::INTEGER) {
            as->Comment("Array is integer, so multiply with 2");
            as->Asm("lsl #1,"+d1);
        }
        if (n->getArrayType(as)==TokenType::LONG) {
            as->Comment("Array is long, so multiply with 4");
            as->Asm("lsl #2,"+d1);
        }
        //qDebug() << "TYPE: "<< TokenType::getType(n->getArrayType(as));
        //qDebug() << "Popping varstack: " <<d1;
      //  as->Comment("Type : " + TokenType::getType(n->getType(as)));
//        as->Comment("Raw type: " + TokenType::getType(as->m_symTab->Lookup(n->value, n->m_op.m_lineNumber)->getTokenType()));
        //as->Comment("Is Pointer : " + QString::number(n->isPointer(as)));
        if (n->isPointer(as))
            TransformVariable(as,"move.l",a0 + trp,n->getValue(as));
        else
            TransformVariable(as,"lea",a0+trp,n->getValue(as));

//        TransformVariable(as,"lea",a0,n->getValue(as));
        TransformVariable(as,"move",d0+trp,"("+a0+","+d1+")",n);
        //qDebug() << "Cleaning up loadvar: " <<d1;
        as->m_varStack.push(d0);

        as->m_regMem.Pop(a0);
        as->m_regAcc.Pop(d0);

//        as->Comment("LoadVar array Popping "+a0 +" " +d0 + "   pushing " + d0);
  //      as->Comment("Free "+Util::toString(as->m_regAcc.m_free));
  //      as->Comment("Occ "+Util::toString(as->m_regAcc.m_occupied));

//        as->m_regAcc.Pop(d0);
//        LoadPointer(n);
        //qDebug() << "Done: ";

        return;
    }

    QString d0 = as->m_regAcc.Get();
    if (m_clearFlag || n->isByte(as)) {
//        as->Comment("LoadVariable:: Clearing");
        TransformVariable(as,"move.l",d0,"#0");

        m_clearFlag=0;
    }
    TransformVariable(as,"move"+getEndType(as,n),d0+"          ; Loadvar regular end",n->getValue(as));
    as->m_regAcc.Pop(d0);
    as->m_varStack.push(d0);
}

void CodeGen68k::LoadVariable(QSharedPointer<NodeProcedure> node)
{
    as->Asm("lea a0,"+node->m_procedure->m_procName + " ; NOT TESTED YET");
}

QString CodeGen68k::LoadAddress(QSharedPointer<Node> n)
{
    n->ForceAddress();
//    qDebug() << "CodeGen68k::Loadaddress  "+n->getValue(as);
if (!n->isPure()) {
//        n->setForceType(TokenType::LONG);
        n->Accept(this);
        return "";
    }

    QString a0 = as->m_regMem.Get();
    TransformVariable(as,"move.l",a0,n->getLiteral(as));
    as->m_varStack.push(a0);
    as->m_regMem.Pop(a0);
    return "";

}

QString CodeGen68k::LoadAddress(QSharedPointer<Node> n, QString a0)
{
    n->ForceAddress();
    if (n->isPureNumeric() && n->isAddress()) {
//        qDebug() << "CodeGen68k::Loadaddress  "+n->getValue(as) << n->HexValue() << qSharedPointerDynamicCast<NodeNumber>(n)->m_val;
        QString val = n->getValue(as);
        if (Syntax::s.m_currentSystem->m_processor==AbstractSystem::M68000) {
            if (val[0]=='-')
                val = Util::numToHex(0x100000000-abs(Util::NumberFromStringHex("$"+val)));
        }

        TransformVariable(as,"move.l",a0,"#"+val);
        return "";
    }
    TransformVariable(as,"move.l",a0,n->getLiteral(as));
    return "";
}

void CodeGen68k::LoadPointer(QSharedPointer<Node> n)
{
    QString d0 = as->m_regAcc.Get();
    //    n->Accept(m_)
    TransformVariable(as,"move"+getEndType(as,n),d0,n->getValue(as));
    as->m_regAcc.Pop(d0);
    as->m_varStack.push(d0);

}

void CodeGen68k::LoadVariable(QSharedPointer<Node> n)
{
    //    qDebug() << "Don't call dispatcher::LoadVariable with NODE. ";
    //  exit(1);
    QSharedPointer<NodeVar> nv = qSharedPointerDynamicCast<NodeVar>(n);
    QSharedPointer<NodeNumber> nn = qSharedPointerDynamicCast<NodeNumber>(n);
    if (nv!=nullptr) {

        LoadVariable(qSharedPointerDynamicCast<NodeVar>(n));
    }
    else
        if (nn!=nullptr){
            LoadVariable(qSharedPointerDynamicCast<NodeNumber>(n));
    }
    else {
    //    as->Comment("LoadVariable: unknown, just accepting");
        n->Accept(this);
    }
}


void CodeGen68k::LoadVariable(QSharedPointer<NodeNumber>n)
{
    QString d0 = as->m_regAcc.Get();
    TransformVariable(as,"move",d0, n->getValue(as));
    as->m_regAcc.Pop(d0);
    as->m_varStack.push(d0);

}

void CodeGen68k::TransformVariable(Assembler *as, QString op, QString n, QString val, QSharedPointer<Node> t)
{
//    as->Asm(op+getEndType(as,t) +" "+val + "," + n);
    m_lastSize = getEndType(as,t);
    as->Asm(op+getEndType(as,t) +" "+val + "," + n);

}

void CodeGen68k::TransformVariable(Assembler* as, QString op, QSharedPointer<NodeVar> n, QString val)
{
    as->Asm(op+getEndType(as,n) +" "+val + "," + n->getValue(as));
    m_lastSize = getEndType(as,n);

//    qDebug() << " ** OP : " << op+getEndType(as,n) +" "+val + "," + n->getValue(as);
}

void CodeGen68k::TransformVariable(Assembler *as, QString op, QString n, QSharedPointer<NodeVar> val)
{
    as->Asm(op+getEndType(as,val) +" "+val->getValue(as) + "," + n);
    m_lastSize = getEndType(as,val);

//
}

void CodeGen68k::TransformVariable(Assembler* as, QString op, QString n, QString val)
{
    QString flag="";
    as->Asm(op+flag +" "+val + "," + n);
}

QString CodeGen68k::getEndType(Assembler *as, QSharedPointer<Node> v) {
//    getType(as)==TokenType::LONG
    if (v->m_forceType==TokenType::LONG)
        return ".l";


    QSharedPointer<NodeVar> nv = qSharedPointerDynamicCast<NodeVar>(v);

    // Classes return types
    if (nv!=nullptr) {
        if (nv->m_writeType==TokenType::BYTE)
            return ".b";
        if (nv->m_writeType==TokenType::INTEGER)
            return ".w";
        if (nv->m_writeType==TokenType::LONG)
            return ".l";
    }


    TokenType::Type t = v->getType(as);
//    as->Comment("Type: "+TokenType::getType(t) + "  for var "+v->getValue(as));
    if (nv!=nullptr && nv->m_expr!=nullptr) {
      //  qDebug() << nv->getValue(as);
        QSharedPointer<Symbol> s = as->m_symTab->Lookup(nv->getValue(as), v->m_op.m_lineNumber, v->isAddress());
        if (s!=nullptr) {
            t = s->m_arrayType;
//            as->Comment("GetEndType : is array of type : " +TokenType::getType(t));
            if (t==TokenType::IF) {
  //              as->Comment("Lookup : is type " + TokenType::getType(s->getTokenType()));
                if (s->getTokenType()==TokenType::STRING)
                    t = TokenType::BYTE;
            }

        }
    }
    if (v->isPureNumeric()) {
        int ival =v->getValueAsInt(as);
        if (ival>=256*256)
            return ".l";
        if (ival>=256)
            return ".w";
        return ".w";


    }




    QSharedPointer<NodeNumber> n =qSharedPointerDynamicCast<NodeNumber>(v);
    if (n!=nullptr) {

        return ".w";
    }

    if (t==TokenType::INTEGER)
        return ".w";
    if (t==TokenType::POINTER)
        return ".l";
    if (t==TokenType::LONG)
        return ".l";
    if (t==TokenType::BYTE)
        return ".b";

//    as->Comment("Current tokentype : "+TokenType::getType(t));

    return "";
}

bool CodeGen68k::HandleSimpleAeqAopConst(QSharedPointer<NodeAssign> node)
{
    QString var = node->m_left->getValue(as);
    QSharedPointer<NodeBinOP> bop = qSharedPointerDynamicCast<NodeBinOP>(node->m_right);
    if (bop==nullptr)
        return false;

    bop->SwapVariableFirst();
    //if (bop->m_left->)

    QSharedPointer<NodeVar> v2 =  qSharedPointerDynamicCast<NodeVar>(bop->m_left);
    // v2 MUst be variable
    if (v2==nullptr)
        return false;
    // Make sure that A:=A op ...
    if (v2->getValue(as)!=var)
        return false;
    // rvar must be pure numeric
    if (!bop->m_right->isPureNumeric())
        return false;

    if (v2->hasArrayIndex())
        return false;


    QString num = bop->m_right->getValue(as);



    // Good to go!
    as->BinOP(bop->m_op.m_type);
    QString op = as->m_varStack.pop();

    // Can be done directly
    if (bop->m_op.m_type==TokenType::PLUS || bop->m_op.m_type==TokenType::MINUS || bop->m_op.m_type==TokenType::OR || bop->m_op.m_type==TokenType::AND)
        as->Asm(op +getEndType(as,v2) + " "+num +","+var + " ; Optimization: simple A := A op Const ADD SUB OR AND");
    else {
        QString d0 = as->m_regAcc.Get();
        as->Asm("move"+getEndType(as,v2) + " "+var+","+d0);
        as->Asm(op +getEndType(as,v2) + " "+num +","+d0 + " ; Optimization: simple A := A op Const MUL DIV SHR etc");
        as->Asm("move"+getEndType(as,v2) + " "+d0+","+var);
        as->m_regAcc.Pop(d0);
    }

    return true;
}

bool CodeGen68k::HandleSimpleAeqBopConst(QSharedPointer<NodeAssign> node)
{
    QString var = node->m_left->getValue(as);
//    return false;
    // First: Check if right is simple,
    if (node->m_right->isPure() && !node->m_right->hasArrayIndex() && !node->m_left->hasArrayIndex()) {
        QString rval = node->m_right->getValue(as);

        as->Asm("move"+getEndType(as,node->m_left) + " "+rval+","+var + " ; Simple a:=b optimization");
        return true;
    }
    // Stop here for now
    return false;


    QSharedPointer<NodeBinOP> bop = qSharedPointerDynamicCast<NodeBinOP>(node->m_right);
    if (bop==nullptr)
        return false;

    bop->SwapVariableFirst();
    //if (bop->m_left->)

    QSharedPointer<NodeVar> v2 = qSharedPointerDynamicCast<NodeVar>(bop->m_left);
    // v2 MUst be variable
    if (v2==nullptr)
        return false;
    // Make sure that A:=A op ...
    if (v2->getValue(as)!=var)
        return false;
    // rvar must be pure numeric
    if (!bop->m_right->isPureNumeric())
        return false;

    QString num = bop->m_right->getValue(as);

    // Good to go!
    as->BinOP(bop->m_op.m_type);
    QString op = as->m_varStack.pop();

    // Can be done directly
    if (bop->m_op.m_type==TokenType::PLUS || bop->m_op.m_type==TokenType::MINUS || bop->m_op.m_type==TokenType::OR || bop->m_op.m_type==TokenType::AND)
        as->Asm(op +getEndType(as,v2) + " "+num +","+var + " ; Optimization: simple A := A op Const ADD SUB OR AND");
    else {
        QString d0 = as->m_regAcc.Get();
        as->Asm("move"+getEndType(as,v2) + " "+var+","+d0);
        as->Asm(op +getEndType(as,v2) + " "+num +","+d0 + " ; Optimization: simple A := A op Const MUL DIV SHR etc");
        as->Asm("move"+getEndType(as,v2) + " "+d0+","+var);
        as->m_regAcc.Pop(d0);
    }

    return true;

}

void CodeGen68k::CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QString lblJump, bool isOffPage)
{

    //IncreaseCounter(step,qSharedPointerDynamicCast<NodeVar>(nodeA->m_left));
    //Compare(nodeA, nodeB, step, false, loopDone, lblJump, isInclusive);

    QString var = nodeA->getValue(as);
    LoadVariable(nodeB);
    TransformVariable(as,"cmp"+getEndType(as,nodeA),as->m_varStack.pop(),var);
    as->Asm("bne "+lblJump);

}

void CodeGen68k::AssignStringPtr(QSharedPointer<NodeAssign> node, bool isPointer) {

    QSharedPointer<NodeString> right = qSharedPointerDynamicCast<NodeString>(node->m_right);
    QSharedPointer<NodeVar> left = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    //    QString lbl = as->NewLabel("stringassign");
    QString str = as->NewLabel("stringassignstr");
    QString lblCpy=as->NewLabel("stringassigncpy");

    //    as->Asm("jmp " + lbl);
    QString strAssign = str + "\t dc.b  \"" + right->m_op.m_value + "\",0";
    as->m_tempVars<<strAssign;
    //as->Label(str + "\t.dc \"" + right->m_op.m_value + "\",0");
    //  as->Label(lbl);

    //    qDebug() << "IS POINTER " << isPointer;
    if (isPointer) {
        //      qDebug() << "HERE";


        as->Asm("move.l #"+str+","+left->getValue(as));
    }
    else {
//                ErrorHandler::e.Error("String copying not yet implemented, can only be assigned as pointers.", node->m_op.m_lineNumber);
        as->Comment("String copy!");
        as->Asm("move.l #"+str+",a0");
        as->Asm("move.l #"+left->value+",a1");
        as->Label(lblCpy);
        as->Asm("move.b (a0)+,(a1)+");
        as->Asm("tst.b (a0)");
        as->Asm("bne.b "+lblCpy);
        as->Asm("move.b #0,(a1)");



        //        as-
        //LoadVariable(left);
    }
    as->PopLabel("stringassignstr");
    as->PopLabel("stringassigncpy");

}


void CodeGen68k::CompareAndJumpIfNotEqualAndIncrementCounter(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive)
{
    auto var = nodeA->m_left;
    if (step==nullptr) step = NodeFactory::CreateNumber(var->m_op,1);
    auto increaseCounter = NodeFactory::CreateAssign(var->m_op, var, NodeFactory::CreateBinop(var->m_op,TokenType::PLUS,var, step));
    as->Comment("Create increasecounter");
    increaseCounter->Accept(this);
    as->Comment("end increasecounter");
    LoadVariable(nodeB);
    QString d0 = as->m_varStack.pop();
    if (isInclusive)
        TransformVariable(as,"add",d0,"#1");

    TransformVariable(as,"cmp"+getEndType(as,var),d0,var->getValue(as));
    as->Asm("bne "+lblJump);

}


void CodeGen68k::AssignVariable(QSharedPointer<NodeAssign> node) {

    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    //        qDebug() << "AssignVariable: " <<v->getValue(as) << " : " << TokenType::getType( v->getType(as));

    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(node->m_left);
    if (v==nullptr && num == nullptr)
        ErrorHandler::e.Error("Left value not variable or memory address! ");

    //as->Comment("Assigning single variable : " + v->getValue(as));
    if (node->m_right==nullptr)
        ErrorHandler::e.Error("Node assign: right hand must be expression", node->m_op.m_lineNumber);


    if (qSharedPointerDynamicCast<NodeString>(node->m_right)) {
        AssignStringPtr(node,node->m_left->isPointer(as));
        return;
    }


    if (node->m_left->getType(as)==TokenType::INTEGER) {
        node->m_right->m_forceType = TokenType::INTEGER; // FORCE integer on right-hand side
    }
    if (node->m_left->getType(as)==TokenType::LONG) {
        node->m_right->m_forceType = TokenType::LONG; // FORCE integer on right-hand side
    }
    if (node->m_left->getType(as)==TokenType::POINTER && !node->m_left->hasArrayIndex()) {
        node->m_right->m_forceType = TokenType::LONG; // FORCE integer on right-hand side
        node->m_right->ForceAddress();
    }
    if (node->m_left->getType(as)==TokenType::POINTER && node->m_left->hasArrayIndex()) {
        //if (node->m_left->getArrayType(as))
        node->m_right->m_forceType = node->m_left->getArrayType(as); // FORCE integer on right-hand side

    }
    if (node->m_left->isPointer(as))
        node->m_right->VerifyReferences(as);


    // Trying to assign a PURE record
  //  if (v == nullptr) {
//        ErrorHandler::e.Error("Left is null pointer, should not happen! ", node->m_op.m_lineNumber);
    //}
    if (v!=nullptr)
    if (v->isRecord(as) && !v->isRecordData(as) && !v->isClass(as)) {
        if (!node->m_right->isRecord(as))
            ErrorHandler::e.Error("Right-hand side of assignment must also be record of type '"+v->getTypeText(as)+"'", v->m_op.m_lineNumber);
        if (v->getTypeText(as)!=node->m_right->getTypeText(as))
            ErrorHandler::e.Error("Right-hand side of assignment must also be of type '"+v->getTypeText(as)+"'", v->m_op.m_lineNumber);
        if (node->m_right->isRecordData(as))
            ErrorHandler::e.Error("Right-hand side of assignment must also be of type '"+v->getTypeText(as)+"'", v->m_op.m_lineNumber);

        // Copy record:
//        HandleNodeAssignCopyRecord(node);
        ErrorHandler::e.Error("RECORD ASSIGNMENT NOT IMPLEMENTED YET", v->m_op.m_lineNumber);

        return;
    }

    // POINTER = RECORD errors
    if (node->m_left->getType(as)==TokenType::POINTER && node->m_right->isRecord(as) && !node->m_right->isClass(as)) {
        if (!node->m_left->hasArrayIndex())
            ErrorHandler::e.Error("Cannot assign a pointer to a record.", node->m_op.m_lineNumber);
        if (!node->m_right->isRecordData(as))
            ErrorHandler::e.Error("Cannot assign a pointer data to a record.", node->m_op.m_lineNumber);
 //       if (node->)
    }
    // Variable = POINTER
    if (node->m_right->isRecord(as) && (!node->m_right->isRecordData(as)) && !node->m_right->isClass(as)) {
        ErrorHandler::e.Error("Cannot assign a record of type '"+node->m_right->getTypeText(as)+"' to a single variable. ",node->m_op.m_lineNumber);
    }





    // Handle A = A op #num;
    if (HandleSimpleAeqAopConst(node))
        return;

    // Handle A = B (op #num);
    if (HandleSimpleAeqBopConst(node))
        return;




    if (node->m_right->hasArrayIndex()) {
  //      as->Comment("Assign: is Array index, forcetype " +TokenType::getType(node->m_right->m_forceType));
        LoadVariable(qSharedPointerDynamicCast<NodeVar>(node->m_right));

    }
    else {
//        as->Comment("Assign: Regular, forcetype " +TokenType::getType(node->m_right->m_forceType));
        node->m_right->Accept(this);
    }


    StoreVariable(v);
//    as->Comment("regacc : " +as->m_regAcc.m_latest);
    as->m_regAcc.m_latest ="";
//    as->m_regAcc.Pop(as->m_regAcc.m_latest);
    while (as->m_varStack.m_vars.length()!=0)
        as->m_varStack.pop();

    return;
}

void CodeGen68k::IncBin(QSharedPointer<NodeVarDecl> node) {
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);
    QString filename = as->m_projectDir + "/" + t->m_filename;
    if (!QFile::exists(filename))
        ErrorHandler::e.Error("Could not locate binary file for inclusion :" +filename);

    int size=0;

    if (t->m_flags.contains("compressed")) {
        QString old = filename;
        FC8 fc;
        filename = filename+ "_c";

  //      if (Util::fileChanged(filename) || !QFile(old).exists())
            fc.Encode(old,filename);
//        else qDebug() << "Ignoring compressing " << old;
    }



    QFile f(filename);
    if (f.open(QIODevice::ReadOnly)){
        size = f.size();  //when file does open.
        f.close();
    }
    QString wname = filename;
    if (t->m_position=="") {
        as->Asm(" 	CNOP 0,4");

        as->Label(v->getValue(as));
        as->Asm("incbin \"" + wname + "\"");
        as->Asm(" 	CNOP 0,4");
    }
    else {
        //            qDebug() << "bin: "<<v->getValue(as) << " at " << t->m_position;
//        Appendix app(t->m_position);
        QSharedPointer<Symbol> typeSymbol = as->m_symTab->Lookup(v->getValue(as), node->m_op.m_lineNumber);
        typeSymbol->m_org = Util::C64StringToInt(t->m_position);
        typeSymbol->m_size = size;
        //            qDebug() << "POS: " << typeSymbol->m_org;
        //app.Append("org " +t->m_position,1);
        as->Asm(" 	CNOP 0,4");

        as->Label(v->getValue(as));
        as->Asm("incbin \"" + wname + "\"");
        as->Asm(" 	CNOP 0,4");
/*        bool ok;
        int start=0;
        if (t->m_position.startsWith("$")) {
            start = t->m_position.remove("$").toInt(&ok, 16);
        }
        else start = t->m_position.toInt();
*/
  //      as->blocks.append(new MemoryBlock(start,start+size, MemoryBlock::DATA,t->m_filename));

    }
}

void CodeGen68k::BuildConditional(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool page)
{

//    as->Comment("Binary clause Simplified: " + node->m_op.getType());
    //    as->Asm("pha"); // Push that baby

    BuildToCmp(node);
    QString bge = "bge";
    QString bgt = "bgt";
    QString ble = "ble";
    QString blt = "blt";
    if (!(node->m_left->isSigned(as) || node->m_right->isSigned(as))) {
        bge = "bhs";
        bgt = "bhi";
        ble = "bls";
        blt = "blo";
    }
    if (node->m_op.m_type==TokenType::EQUALS)
        as->Asm("bne " + lblFailed);
    if (node->m_op.m_type==TokenType::NOTEQUALS)
        as->Asm("beq " + lblFailed);

    if (node->m_op.m_type==TokenType::LESS)
        as->Asm(bge+" " + lblFailed);
    if (node->m_op.m_type==TokenType::GREATER)
        as->Asm(ble+" " + lblFailed);


    if (node->m_op.m_type==TokenType::LESSEQUAL)
        as->Asm(bgt+" " + lblFailed);
    if (node->m_op.m_type==TokenType::GREATEREQUAL)
        as->Asm(blt+" " + lblFailed);




/*    if (node->m_op.m_type==TokenType::LESS)
        as->Asm("bcc " + lblFailed);
    if (node->m_op.m_type==TokenType::GREATER)
        as->Asm("bcs " + lblFailed);
*/

}

void CodeGen68k::BuildToCmp(QSharedPointer<Node> node)
{
//    QString b="";

  /*  QSharedPointer<NodeVar> varb = dynamic_cast<QSharedPointer<NodeVar>>(node->m_right);
    if (varb!=nullptr && varb->m_expr==nullptr)
        b = varb->getValue(as);

    QSharedPointer<NodeNumber> numb = dynamic_cast<QSharedPointer<NodeNumber>>(node->m_right);
    if (numb!=nullptr)
        b = numb->StringValue();
*/
//    as->Term();

    if (node->m_left->isPure()) {
        if (node->m_right->isPureNumeric())
        {
  //          as->Comment("Compare with pure num / var optimization");
//            TransformVariable(as,"cmp",node->m_left->getValue(as),node->m_right->getValue(as),node->m_left);
            TransformVariable(as,"cmp",node->m_left->getValue(as),node->m_right->getValue(as),node->m_left);
            return;
        } else
        {
    //        as->Comment("Compare two vars optimization");
            if (node->m_right->isPureVariable()) {
                QString wtf = as->m_regAcc.Get();
                LoadVariable(qSharedPointerDynamicCast<NodeVar>(node->m_right));
                TransformVariable(as,"move",wtf,qSharedPointerDynamicCast<NodeVar>(node->m_left));
                TransformVariable(as,"cmp"+getEndType(as,node->m_left),wtf,as->m_varStack.pop());
//                TransformVariable(as,"cmp",wtf,as->m_varStack.pop());
                as->m_regAcc.Pop(wtf);
                return;
            }
                 else
                node->m_right->Accept(this);

            TransformVariable(as,"cmp",qSharedPointerDynamicCast<NodeVar>(node->m_left),as->m_varStack.pop());
            return;
        }
    }

    node->m_left->Accept(this);
    QString d0 = as->m_varStack.pop();
    QString tmp = as->m_regAcc.Get();
    node->m_right->Accept(this);
    QString d1 = as->m_varStack.pop();
    as->m_regAcc.Pop(tmp);
//    qDebug() << "VARSTACK" <<d0 << d1;

    as->Asm("cmp"+getEndType(as,node->m_left)+" "+d1+"," +d0);
    //TransformVariable(as,"cmp",node->m_left, as->m_varStack.pop());

        // Perform a full compare : create a temp variable
//        QString tmpVar = as->m_regAcc.Get();//as->StoreInTempVar("binary_clause_temp");
//        node->m_right->Accept(this);
  //      as->Asm("cmp " + tmpVar);
    //      as->PopTempVar();


}

void CodeGen68k::DeclarePointer(QSharedPointer<NodeVarDecl> node) {

    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);
    QString initVal = t->initVal;
    if (initVal=="") initVal = "0";

    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    as->Label(v->getValue(as) + " dc.l "+ initVal);

}

QString CodeGen68k::CodeGen68k::getEndType(Assembler *as, QSharedPointer<Node> v1, QSharedPointer<Node> v2)
{
    if (v1->isReference() || v2->isReference())
        return ".l";
    QString t1 = getEndType(as,v1);
    QString t2 = getEndType(as,v2);
    if (t1 ==".l" || t2==".l")
        return ".l";
    if (t1 ==".w" || t2==".w")
        return ".w";
    return ".b";
}


