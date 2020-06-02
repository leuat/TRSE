#include "astdispatcher68000.h"


ASTDispatcher68000::ASTDispatcher68000()
{

}


void ASTDispatcher68000::dispatch(QSharedPointer<NodeBinOP>node) {
    node->DispatchConstructor(as);


    if (node->isPureNumeric()) {
        //qDebug() << "IS PURE NUMERIC BinOp";
        as->Comment("NodeBinop : both are pure numeric optimization : "+node->getValue(as));
        as->m_varStack.push(node->getValue(as));
        return;
    }


    as->BinOP(node->m_op.m_type);
    QString op = as->m_varStack.pop();
    QString d0 = as->m_regAcc.Get();
    as->Comment("BOP NEW register: " + d0);
//    if (m_clearFlag) {

        TransformVariable(as,"moveq",d0,"#0");
    //    m_clearFlag=false;
  //  }
    node->m_left->Accept(this);
    TransformVariable(as,"move"+getEndType(as,node->m_left, node->m_right),d0 + "     ; BOP move",as->m_varStack.pop());

//    qDebug() << "NodeBinOp : " << op;
  //  as->Comment("NodeBinop  : " +op);
    if (op.toLower().contains("mul") || op.toLower().contains("div"))
        op = op+".w"; else op=op + getEndType(as,node->m_left, node->m_right);//+m_lastSize;//+".l";

//    as->Comment("d0 used:" +d0);
    node->m_right->Accept(this);

    TransformVariable(as,op,d0,as->m_varStack.pop());
    as->m_varStack.push(d0);



    as->m_regAcc.Pop(d0);
}


/*void ASTDispatcher68000::dispatchOld(QSharedPointer<NodeBinOP>node)
{
    node->DispatchConstructor(as);

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

//    qDebug() << "Varstack count: " << as->m_varStack.m_vars.count();
  //  qDebug() << "Binop: " << as->m_varStack.m_vars.count();
    as->BinOP(node->m_op.m_type);
    QString op = as->m_varStack.pop();


//    as->Comment(" ** StartFree registers: " +QString::number(as->m_regAcc.m_free.count()));
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

void ASTDispatcher68000::dispatch(QSharedPointer<NodeNumber>node)
{
    as->m_varStack.push(node->getValue(as));

}


void ASTDispatcher68000::dispatch(QSharedPointer<NodeString> node)
{

}

void ASTDispatcher68000::dispatch(QSharedPointer<NodeUnaryOp> node)
{
  //  node->DispatchConstructor(as);
//    node->Accept(this);
//    if (node->isMinusOne())
    if (!(node->m_op.m_type==TokenType::MINUS))
        node->m_right->Accept(this);
    else {
        QSharedPointer<NodeNumber>n = qSharedPointerDynamicCast<NodeNumber>(node->m_right);
        n->m_val*=-1;
        node->m_right->Accept(this);
    }

    as->Comment("Unary op for : " + node->m_right->getValue(as));
}


void ASTDispatcher68000::dispatch(QSharedPointer<NodeVarDecl> node)
{

    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);


    node->DispatchConstructor(as);

    if (t->m_flags.contains("chipmem"))
        as->m_currentBlock = as->m_chipMem;


    AbstractASTDispatcher::dispatch(node);

    as->m_currentBlock = nullptr;



/*
    node->DispatchConstructor(as);





    node->ExecuteSym(as->m_symTab);



    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);
  //  qDebug() << t->m_flags;
    if (t->m_flags.contains("chipmem")) {
        as->m_currentBlock = as->m_chipMem;
//        qDebug() << "CURRENTBLOCK IS CHIPMEM";
    }

    if (t->m_op.m_type==TokenType::ARRAY) {
        as->DeclareArray(v->getValue(as), t->m_arrayVarType.m_value, t->m_op.m_intVal, t->m_data, t->m_position);
        node->m_dataSize=t->m_op.m_intVal;
        as->m_symTab->Lookup(v->getValue(as), node->m_op.m_lineNumber)->m_type="address";
        as->m_symTab->Lookup(v->getValue(as), node->m_op.m_lineNumber)->m_arrayType=t->m_arrayVarType.m_type;
    }else
    if (t->m_op.m_type==TokenType::STRING) {
        as->DeclareString(v->getValue(as), t->m_data);

    }
    else
    if (t->m_op.m_type==TokenType::CSTRING) {
        as->DeclareCString(v->getValue(as), t->m_data);
        node->m_dataSize = 0;
        for (QString s: t->m_data)
            node->m_dataSize+=s.count();
        node->m_dataSize++; // 0 end
    }
    else
    if (t->m_op.m_type==TokenType::INCBIN) {
//        if (node->m_curMemoryBlock!=nullptr)
  //          ErrorHandler::e.Error("IncBin can not be declared within a user-defined memory block :",node->m_op.m_lineNumber);

        IncBin(node);

    }
    else
    if (t->m_op.m_type==TokenType::POINTER) {
    //    if (node->m_curMemoryBlock!=nullptr)
      //      ErrorHandler::e.Error("Pointers can not be declared within a user-defined memory block :",node->m_op.m_lineNumber);
        DeclarePointer(node);
        as->m_symTab->Lookup(v->getValue(as), node->m_op.m_lineNumber)->m_arrayType=t->m_arrayVarType.m_type;

    }else {
        node->m_dataSize=1;
        if (t->getValue(as).toLower()=="integer") node->m_dataSize = 2;
        if (t->getValue(as).toLower()=="long") node->m_dataSize = 4;
        as->DeclareVariable(v->getValue(as), t->value, t->initVal, t->m_position);
    }



    as->m_currentBlock = nullptr;
    */

}

void ASTDispatcher68000::dispatch(QSharedPointer<NodeBlock> node)
{

    node->DispatchConstructor(as);

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
    if (node->m_decl.count()!=0)
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

/*void ASTDispatcher68000::dispatch(QSharedPointer<NodeProgram> node)
{
    node->DispatchConstructor(as);

//    as->EndMemoryBlock();
    NodeBuiltinMethod::m_isInitialized.clear();
    as->Program(node->m_name, node->m_param);
  //  as->m_source << node->m_initJumps;
    node->m_NodeBlock->m_isMainBlock = true;
    node->m_NodeBlock->Accept(this);

//    qDebug() << as->m_currentBlock;
    as->EndProgram();

}
*/
void ASTDispatcher68000::dispatch(QSharedPointer<NodeVarType> node)
{

}

void ASTDispatcher68000::dispatch(QSharedPointer<NodeBinaryClause> node)
{

}

/*void ASTDispatcher68000::dispatch(QSharedPointer<NodeProcedure> node)
{
    node->DispatchConstructor(as);


    if (node->m_parameters.count()!=node->m_procedure->m_paramDecl.count())
        ErrorHandler::e.Error("Procedure '" + node->m_procedure->m_procName+"' requires "
                              + QString::number(node->m_procedure->m_paramDecl.count()) +" parameters, not "
                              + QString::number(node->m_parameters.count()) + ".", node->m_op.m_lineNumber);

    for (int i=0; i<node->m_parameters.count();i++) {
        // Assign all variables
        QSharedPointer<NodeVarDecl> vd = qSharedPointerDynamicCast<NodeVarDecl>(node->m_procedure->m_paramDecl[i]);
        QSharedPointer<NodeAssign> na = QSharedPointer<NodeAssign>(new NodeAssign(vd->m_varNode, node->m_parameters[i]->m_op, node->m_parameters[i]));
        na->Accept(this);
//        na->Build(as);
    }

    as->Asm("jsr " + node->m_procedure->m_procName);

}
*/
void ASTDispatcher68000::dispatch(QSharedPointer<NodeProcedureDecl> node)
{
    node->DispatchConstructor(as);



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
/*
void ASTDispatcher68000::dispatch(QSharedPointer<NodeConditional> node)
{
    QString labelStartOverAgain = as->NewLabel("while");
    QString lblstartTrueBlock = as->NewLabel("ConditionalTrueBlock");

    QString labelElse = as->NewLabel("elseblock");
    QString labelElseDone = as->NewLabel("elsedoneblock");
    // QString labelFailed = as->NewLabel("conditionalfailed");

//    qDebug() << "HMM";

    if (node->m_isWhileLoop)
        as->Label(labelStartOverAgain);

    // Test all binary clauses:
    QSharedPointer<NodeBinaryClause> bn = qSharedPointerDynamicCast<NodeBinaryClause>(node->m_binaryClause);


        QString failedLabel = labelElseDone;
        if (node->m_elseBlock!=nullptr)
            failedLabel = labelElse;

    BuildSimple(bn,  failedLabel);

    // Start main block
    as->Label(lblstartTrueBlock); // This means skip inside

    node->m_block->Accept(this);

    if (node->m_elseBlock!=nullptr)
        as->Asm("bra " + labelElseDone);

    // If while loop, return to beginning of conditionals
    if (node->m_isWhileLoop)
        as->Asm("bra " + labelStartOverAgain);

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


}
*/
void ASTDispatcher68000::dispatch(QSharedPointer<NodeForLoop> node)
{
    node->DispatchConstructor(as);


    //QString m_currentVar = ((NodeAssign*)m_a)->m_
    QSharedPointer<NodeAssign> nVar = qSharedPointerDynamicCast<NodeAssign>(node->m_a);


    if (nVar==nullptr)
        ErrorHandler::e.Error("Index must be variable", node->m_op.m_lineNumber);

    QString var = qSharedPointerDynamicCast<NodeVar>(nVar->m_left)->getValue(as);//  m_a->Build(as);
//    qDebug() << "Starting for";
    node->m_a->Accept(this);
  //  qDebug() << "accepted";

//    LoadVariable(node->m_a);
  //  TransformVariable()
    //QString to = m_b->Build(as);
    QString to = "";
    if (qSharedPointerDynamicCast<NodeNumber>(node->m_b) != nullptr)
        to = QString::number(((qSharedPointerDynamicCast<NodeNumber>(node->m_b)))->m_val);
    if (qSharedPointerDynamicCast<NodeVar>(node->m_b) != nullptr)
        to = qSharedPointerDynamicCast<NodeVar>(node->m_b)->getValue(as);

//    as->m_stack["for"].push(var);
    QString lblFor =as->NewLabel("forloop");
    as->Label(lblFor);
//    qDebug() << "end for";



    node->m_block->Accept(this);
    TransformVariable(as,"add",var, "#1");
    LoadVariable(node->m_b);
    TransformVariable(as,"cmp",as->m_varStack.pop(),var);
    as->Asm("bne "+lblFor);

    as->PopLabel("forloop");


}

void ASTDispatcher68000::dispatch(QSharedPointer<NodeVar> node)
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

void ASTDispatcher68000::dispatch(QSharedPointer<Node> node)
{

}

void ASTDispatcher68000::dispatch(QSharedPointer<NodeAssign> node)
{
    node->DispatchConstructor(as);

//    as->PushCounter();

    AssignVariable(node);

  //  as->PopCounter(node->m_op.m_lineNumber);

}

void ASTDispatcher68000::dispatch(QSharedPointer<NodeCase> node)
{
    ErrorHandler::e.Error("case not implemented for m68K yet", node->m_op.m_lineNumber);
}

void ASTDispatcher68000::dispatch(QSharedPointer<NodeRepeatUntil> node)
{
    ErrorHandler::e.Error("Repeat-until not implemented yet", node->m_op.m_lineNumber);

}


void ASTDispatcher68000::dispatch(QSharedPointer<NodeComment> node)
{

}



void ASTDispatcher68000::StoreVariable(QSharedPointer<NodeVar> n)
{
        as->Comment("StoreVar : " +QString::number(n->m_expr==nullptr));
        if (n->m_expr!=nullptr) {
    //        qDebug() << n->m_op.getType();
      //      exit(1);
            bool done = false;
/*            if (as->m_regAcc.m_latest.count()==2) {
                TransformVariable(as,"moveq.l",as->m_regAcc.m_latest + " ; StoreVariable regAcc latest","#0");
                done = true;
            }*/
            as->Term();
            as->m_regAcc.m_latest="";
            //as->Comment("PREV POP "+as->m_varStack.pop());
  //          QString prevPop = as->m_varStack.pop();
            QString d0 = as->m_varStack.pop();
    //        QString data = as->m_regAcc.Get(); // Get d1
//            QString d0 = as->m_regAcc.Get();
            QString d2 = as->m_regAcc.Get();
            QString oldd2 = "";
            bool newD2 = false;
            if (d2==d0) { // If they are equal, get another one {
                oldd2 =d2;
                d2 = as->m_regAcc.Get();
                newD2 = true;
            }

            //d0 = d2;
            as->Comment("Storevar START getting new unused var "+d0);
//            QString d0 = as->m_regAcc.Get();
            QString a0 = as->m_regMem.Get();
 //           QStringList ls = QStringList() << "d0";
/*            if (!ls.contains(prevPop) )
                TransformVariable(as,"move"+getEndType(as,n),data + "; clear #1",prevPop);
*/
//            if (!done && prevPop.toLower().startsWith("d"))
//                TransformVariable(as,"moveq.l",d0 + "; clear #1","#0");

            //qDebug() << "Loading array: expression";
            LoadVariable(n->m_expr);
            QString d1 = as->m_varStack.pop();
            if (n->getArrayType(as)==TokenType::INTEGER) {
                as->Asm("lsl #1,"+d1);
            }
            if (n->getArrayType(as)==TokenType::LONG) {
                as->Asm("lsl #2,"+d1);
            }


            //qDebug() << "Popping varstack: " <<d1;
//            as->Comment("Type: " + TokenType::getType(n->getType(as)));
            if (n->getType(as)==TokenType::POINTER)
                TransformVariable(as,"move.l",a0,n->getValue(as));
            else
                TransformVariable(as,"lea",a0,n->getValue(as));

            TransformVariable(as,"move"+getEndType(as,n),"("+a0+","+d1+")",d0);

            as->m_regMem.Pop(a0);
            if (m_regs.contains(d2)) {
                as->m_regAcc.Pop(d2);
            }

            return;
        }
        QString d0 = as->m_varStack.pop();

        TransformVariable(as,"move"+getEndType(as,n),n->getValue(as),d0);
}


void ASTDispatcher68000::LoadVariable(QSharedPointer<NodeVar> n)
{
//    TokenType::Type t = as->m_symTab->Lookup(n->getValue(as), n->m_op.m_lineNumber)->getTokenType();

    if (n->m_expr!=nullptr) {
//        qDebug() << n->m_op.getType();
  //      exit(1);
        bool done = false;
        if (as->m_regAcc.m_latest.count()==2) {
            as->Comment("Trying to clear: " + as->m_regAcc.m_latest + " YO");
//            TransformVariable(as,"move.l",as->m_regAcc.m_latest,"#0");
            done = true;
        }
        QString trp = "             ; LoadVariable:: is array";
        QString d0 = as->m_regAcc.Get();
        QString a0 = as->m_regMem.Get();
        if (!done ) {
            as->Comment("Clearing : " + d0 + " YO");
            m_clearFlag=true;
           // TransformVariable(as,"move.l",d0,"#0");
        }
        //qDebug() << "Loading array: expression";
        m_clearFlag=true;
        LoadVariable(n->m_expr);
        QString d1 = as->m_varStack.pop();
        if (n->getArrayType(as)==TokenType::INTEGER) {
            as->Asm("lsl #1,"+d1);
        }
        if (n->getArrayType(as)==TokenType::LONG) {
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
//        as->m_regAcc.Pop(d0);
//        LoadPointer(n);
        //qDebug() << "Done: ";

        return;
    }

    QString d0 = as->m_regAcc.Get();
    if (m_clearFlag) {
        as->Comment("LoadVariable:: Clearing");
        TransformVariable(as,"move.l",d0,"#0");

        m_clearFlag=0;
    }
    TransformVariable(as,"move"+getEndType(as,n),d0+"          ; Loadvar regular end",n->getValue(as));
    as->m_regAcc.Pop(d0);
    as->m_varStack.push(d0);
}

void ASTDispatcher68000::LoadAddress(QSharedPointer<Node> n)
{
/*    QSharedPointer<NodeVar> v = dynamic_cast<QSharedPointer<NodeVar>>(n);
    if (v==nullptr) {
        n->ForceAddress();
        LoadVariable(n);
        return;
    }*/
    n->ForceAddress();
    QString a0 = as->m_regMem.Get();
    as->Comment("LoadAddress: move start with literal: " +n->getLiteral(as));
    TransformVariable(as,"move.l",a0,n->getLiteral(as));
    as->m_varStack.push(a0);
    as->m_regMem.Pop(a0);

//    QString v = n->getLiteral(as);

}

void ASTDispatcher68000::LoadAddress(QSharedPointer<Node> n, QString a0)
{
    /*    QSharedPointer<NodeVar> v = dynamic_cast<QSharedPointer<NodeVar>>(n);
        if (v==nullptr) {
            n->ForceAddress();
            LoadVariable(n);
            return;
        }*/
    n->ForceAddress();
//    as->Comment("LoadAddress: move start with literal: " +n->getLiteral(as));
    if (n->isPureNumeric() && n->isAddress()) {
        TransformVariable(as,"move.l",a0,"#"+n->getValue(as));
        return;
    }
    TransformVariable(as,"move.l",a0,n->getLiteral(as));

    //    QString v = n->getLiteral(as);

}

void ASTDispatcher68000::LoadPointer(QSharedPointer<Node> n)
{
    QString d0 = as->m_regAcc.Get();
    //    n->Accept(m_)
    TransformVariable(as,"move"+getEndType(as,n),d0,n->getValue(as));
    as->m_regAcc.Pop(d0);
    as->m_varStack.push(d0);

}

void ASTDispatcher68000::LoadVariable(QSharedPointer<Node> n)
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
        as->Comment("LoadVariable: unknown, just accepting");
        n->Accept(this);
    }
}


void ASTDispatcher68000::LoadVariable(QSharedPointer<NodeNumber>n)
{
    QString d0 = as->m_regAcc.Get();
    TransformVariable(as,"move",d0, n->getValue(as));
    as->m_regAcc.Pop(d0);
    as->m_varStack.push(d0);

}

void ASTDispatcher68000::TransformVariable(Assembler *as, QString op, QString n, QString val, QSharedPointer<Node> t)
{
//    as->Asm(op+getEndType(as,t) +" "+val + "," + n);
    m_lastSize = getEndType(as,t);
    as->Asm(op+getEndType(as,t) +" "+val + "," + n);

}

void ASTDispatcher68000::TransformVariable(Assembler* as, QString op, QSharedPointer<NodeVar> n, QString val)
{
    as->Asm(op+getEndType(as,n) +" "+val + "," + n->getValue(as));
    m_lastSize = getEndType(as,n);

//    qDebug() << " ** OP : " << op+getEndType(as,n) +" "+val + "," + n->getValue(as);
}

void ASTDispatcher68000::TransformVariable(Assembler *as, QString op, QString n, QSharedPointer<NodeVar> val)
{
    as->Asm(op+getEndType(as,val) +" "+val->getValue(as) + "," + n);
    m_lastSize = getEndType(as,val);

//
}

void ASTDispatcher68000::TransformVariable(Assembler* as, QString op, QString n, QString val)
{
    QString flag="";
/*    if (val.count()==2 && val.startsWith("d") && n.count()==2 && n.startsWith("d"))
        flag = ".l";
    if (op.contains("."))
        flag = "";*/
    as->Asm(op+flag +" "+val + "," + n);
    //qDebug() << " ** OP : " << op +" "+val + "," + n;
}

QString ASTDispatcher68000::getEndType(Assembler *as, QSharedPointer<Node> v) {
//    getType(as)==TokenType::LONG
    if (v->m_forceType==TokenType::LONG)
        return ".l";

    QSharedPointer<NodeVar> nv = qSharedPointerDynamicCast<NodeVar>(v);
    TokenType::Type t = v->getType(as);
    if (nv!=nullptr && nv->m_expr!=nullptr) {
      //  qDebug() << nv->getValue(as);
        QSharedPointer<Symbol> s = as->m_symTab->Lookup(nv->getValue(as), v->m_op.m_lineNumber, v->isAddress());
        if (s!=nullptr) {
            t = s->m_arrayType;
//            as->Comment("GetEndType : is array of type : " +TokenType::getType(t));
            if (t==TokenType::IF) {
  //              as->Comment("Lookup : is type " + TokenType::getType(s->getTokenType()));
//                t = TokenType::LONG;  // Default pointer is INTEGER
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

bool ASTDispatcher68000::HandleSimpleAeqAopConst(QSharedPointer<NodeAssign> node)
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

bool ASTDispatcher68000::HandleSimpleAeqBopConst(QSharedPointer<NodeAssign> node)
{
    QString var = node->m_left->getValue(as);
//    return false;
    // First: Check if right is simple,
    if (node->m_right->isPure() && !node->m_right->isArrayIndex() && !node->m_left->isArrayIndex()) {
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

void ASTDispatcher68000::CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive)
{

}


QString ASTDispatcher68000::AssignVariable(QSharedPointer<NodeAssign> node) {

    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    //        qDebug() << "AssignVariable: " <<v->getValue(as) << " : " << TokenType::getType( v->getType(as));

    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(node->m_left);
    if (v==nullptr && num == nullptr)
        ErrorHandler::e.Error("Left value not variable or memory address! ");

    //as->Comment("Assigning single variable : " + v->getValue(as));
    if (node->m_right==nullptr)
        ErrorHandler::e.Error("Node assign: right hand must be expression", node->m_op.m_lineNumber);




    if (node->m_left->getType(as)==TokenType::INTEGER) {
        node->m_right->m_forceType = TokenType::INTEGER; // FORCE integer on right-hand side
    }
    if (node->m_left->getType(as)==TokenType::LONG) {
        node->m_right->m_forceType = TokenType::LONG; // FORCE integer on right-hand side
    }
    if (node->m_left->getType(as)==TokenType::POINTER && !node->m_left->isArrayIndex()) {
        node->m_right->m_forceType = TokenType::LONG; // FORCE integer on right-hand side
        node->m_right->ForceAddress();
    }
    if (node->m_left->getType(as)==TokenType::POINTER && node->m_left->isArrayIndex()) {
        //if (node->m_left->getArrayType(as))
        node->m_right->m_forceType = node->m_left->getArrayType(as); // FORCE integer on right-hand side
    }



    // Handle A = A op #num;
    if (HandleSimpleAeqAopConst(node))
        return "";

    // Handle A = B (op #num);
    if (HandleSimpleAeqBopConst(node))
        return "";




    if (node->m_right->isArrayIndex()) {
        as->Comment("Assign: is Array index, forcetype " +TokenType::getType(node->m_right->m_forceType));
        LoadVariable(qSharedPointerDynamicCast<NodeVar>(node->m_right));

    }
    else {
        as->Comment("Assign: Regular, forcetype " +TokenType::getType(node->m_right->m_forceType));
        node->m_right->Accept(this);
    }


    StoreVariable(v);
//    as->Comment("regacc : " +as->m_regAcc.m_latest);
    as->m_regAcc.m_latest ="";
//    as->m_regAcc.Pop(as->m_regAcc.m_latest);
    while (as->m_varStack.m_vars.count()!=0)
        as->m_varStack.pop();

    return "";
}

void ASTDispatcher68000::IncBin(QSharedPointer<NodeVarDecl> node) {
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);
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
        as->Asm(" 	CNOP 0,4");

        as->Label(v->getValue(as));
        as->Asm("incbin \"" + filename + "\"");
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
        as->Asm("incbin \"" + filename + "\"");
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

void ASTDispatcher68000::BuildSimple(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool page)
{

    as->Comment("Binary clause Simplified: " + node->m_op.getType());
    //    as->Asm("pha"); // Push that baby

    BuildToCmp(node);

    if (node->m_op.m_type==TokenType::EQUALS)
        as->Asm("bne " + lblFailed);
    if (node->m_op.m_type==TokenType::NOTEQUALS)
        as->Asm("beq " + lblFailed);
    if (node->m_op.m_type==TokenType::LESS)
        as->Asm("bgt " + lblFailed);
    if (node->m_op.m_type==TokenType::GREATER)
        as->Asm("blt " + lblFailed);

/*    if (node->m_op.m_type==TokenType::LESS)
        as->Asm("bcc " + lblFailed);
    if (node->m_op.m_type==TokenType::GREATER)
        as->Asm("bcs " + lblFailed);
*/

}

void ASTDispatcher68000::BuildToCmp(QSharedPointer<Node> node)
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
            as->Comment("Compare with pure num / var optimization");
//            TransformVariable(as,"cmp",node->m_left->getValue(as),node->m_right->getValue(as),node->m_left);
            TransformVariable(as,"cmp",node->m_left->getValue(as),node->m_right->getValue(as),node->m_left);
            return;
        } else
        {
            as->Comment("Compare two vars optimization");
            if (node->m_right->isPureVariable()) {
                QString wtf = as->m_regAcc.Get();
                LoadVariable(qSharedPointerDynamicCast<NodeVar>(node->m_right));
                TransformVariable(as,"move",wtf,qSharedPointerDynamicCast<NodeVar>(node->m_left));
                TransformVariable(as,"cmp",wtf,as->m_varStack.pop());
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

    as->Asm("cmp "+d1+"," +d0);
    //TransformVariable(as,"cmp",node->m_left, as->m_varStack.pop());

        // Perform a full compare : create a temp variable
//        QString tmpVar = as->m_regAcc.Get();//as->StoreInTempVar("binary_clause_temp");
//        node->m_right->Accept(this);
  //      as->Asm("cmp " + tmpVar);
    //      as->PopTempVar();


}

void ASTDispatcher68000::DeclarePointer(QSharedPointer<NodeVarDecl> node) {

    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);
    QString initVal = t->initVal;
    if (initVal=="") initVal = "0";

    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    as->Label(v->getValue(as) + " dc.l "+ initVal);

}

QString ASTDispatcher68000::ASTDispatcher68000::getEndType(Assembler *as, QSharedPointer<Node> v1, QSharedPointer<Node> v2)
{
    QString t1 = getEndType(as,v1);
    QString t2 = getEndType(as,v2);
    if (t1 ==".l" || t2==".l")
        return ".l";
    if (t1 ==".w" || t2==".w")
        return ".w";
    return ".b";
}


