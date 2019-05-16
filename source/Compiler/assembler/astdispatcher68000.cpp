#include "astdispatcher68000.h"


ASTDispather68000::ASTDispather68000()
{

}

void ASTDispather68000::dispatch(NodeBinOP *node)
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
        qDebug() << "IS PURE NUMERIC BinOp";
        int val = node->BothPureNumbersBinOp(as);
        QString s = "#";
        if (node->m_left->isAddress() || node->m_right->isAddress())
            s = "";
        QString d0 = as->m_regAcc.Get();
        as->Asm("move "+s+node->getValue() +","+d0);
        as->m_regAcc.Pop(d0);
//        StoreVariable(as,)
        return;
    }

  //  qDebug() << "Varstack count: " << as->m_varStack.m_vars.count();
    as->BinOP(node->m_op.m_type);
    QString op = as->m_varStack.pop();



    /*
      // WORKS
    QString d0 = as->m_regAcc.Get();
    as->m_varStack.push(d0);

    node->m_right->Accept(this);
    QString right = as->m_varStack.pop();
    node->m_left->Accept(this);

    TransformVariable(as,"move.l",d0,as->m_varStack.pop());
    TransformVariable(as,op+".l",d0,right);
*/


//    qDebug() << "Start BINOP";
    QString d0 = "";
    bool start = false;
    if (node->m_left->isPureNumeric() || node->m_left->isPureVariable())
        start = true;


    if (start) {
        d0 = as->m_regAcc.Get();
        qDebug() << "NEW register: " << d0;
        as->m_varStack.push(d0);
    }
    else {
        node->m_left->Accept(this);
//        d0 = as->m_regAcc.getLatest();
        d0 = as->m_regAcc.peekLatest();
        qDebug() << "LEFT accepted, current register : " << d0;
//        if (node->m_right->isPureNumeric() || node->m_right->isPureVariable())
        if (as->m_regAcc.m_latest.count()!=1)
        {
            qDebug() << "Popping: " << d0;
            as->m_regAcc.Pop(d0);
            d0 = as->m_regAcc.peekLatest();

        }
  //      d0 = as->m_regAcc.getLatest();

    }

    if (start) {
        node->m_left->Accept(this);
        TransformVariable(as,"move.l",d0,as->m_varStack.pop());
    }

    node->m_right->Accept(this);
    QString right = as->m_varStack.pop();

    if (op.toLower().contains("mul") || op.toLower().contains("div"))
        op = op+".w"; else op=op+".l";
    TransformVariable(as,op,d0,right);

//    as->m_regAcc.Pop(d0);



/*    if (as->m_varStack.m_vars.count()==0) {
        QString d0 = as->m_regAcc.Get();
        qDebug() << "D0 org and RIGHT start: " << d0;

        node->m_right->Accept(this);
        QString right = as->m_varStack.pop();
        TransformVariable(as,"move.l",d0,as->m_varStack.pop());

        qDebug() << "D0 org and RIGHT end: " << d0;
        as->m_varStack.push(d0);

        node->m_left->Accept(this);

        qDebug() << "End 0 vars" << as->m_varStack.m_vars;
        TransformVariable(as,op+".l",d0,right);
        qDebug() << "End 0 vars" << as->m_varStack.m_vars;
    }
    else {
        qDebug() << "Start 1 vars" << as->m_varStack.m_vars;

        QString d0 = as->m_varStack.pop();
        qDebug() << d0 << op;
        node->m_right->Accept(this);
        qDebug() << "Woot";
        //node->m_left->Accept(this);
//        TransformVariable(as,"move.l",d0,as->m_varStack.pop());
        TransformVariable(as,op+".l",d0,as->m_varStack.pop());
        qDebug() << "Trans";
        as->m_varStack.push(d0);

    }
*/
/*    if (node->m_op.m_type==TokenType::MUL || node->m_op.m_type==TokenType::DIV) {
        HandleMulDiv(node);
        return;
    }*/
//    HandleRestBinOp(node);
    // The rest is only plus and minus, and etc

}

void ASTDispather68000::dispatch(NodeNumber *node)
{
    as->m_varStack.push(node->getValue());

}

void ASTDispather68000::dispatch(NodeAsm *node)
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

void ASTDispather68000::dispatch(NodeString *node)
{

}

void ASTDispather68000::dispatch(NodeUnaryOp *node)
{

}

void ASTDispather68000::dispatch(NodeCompound *node)
{
    node->DispatchConstructor();

    as->BeginBlock();
    for (Node* n: node->children)
        n->Accept(this);


    as->EndBlock();

}

void ASTDispather68000::dispatch(NodeVarDecl *node)
{
    node->DispatchConstructor();


    node->ExecuteSym(as->m_symTab);


    NodeVar* v = (NodeVar*)node->m_varNode;
    NodeVarType* t = (NodeVarType*)node->m_typeNode;
    if (t->m_flags.contains("chipmem")) {
        as->m_currentBlock = &as->m_chipMem;
    }

    if (t->m_op.m_type==TokenType::ARRAY) {
        as->DeclareArray(v->value, t->m_arrayVarType.m_value, t->m_op.m_intVal, t->m_data, t->m_position);
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

        IncBin(as,node);
    }
    else
    if (t->m_op.m_type==TokenType::POINTER) {
        if (node->m_curMemoryBlock!=nullptr)
            ErrorHandler::e.Error("Pointers can not be declared within a user-defined memory block :",node->m_op.m_lineNumber);
//        DeclarePointer(node);
    }else {
        node->m_dataSize=1;
        if (t->value.toLower()=="integer") node->m_dataSize = 2;
        if (t->value.toLower()=="long") node->m_dataSize = 4;
        as->DeclareVariable(v->value, t->value, t->initVal);
    }



    as->m_currentBlock = nullptr;


}

void ASTDispather68000::dispatch(NodeBlock *node)
{

    node->DispatchConstructor();

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
        if (dynamic_cast<NodeVarDecl*>(n)==nullptr) {
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

    if (!blockLabel && hasLabel)
        as->Label(label);

    if (node->forceLabel!="")
        as->Label(node->forceLabel);

    if (node->m_compoundStatement!=nullptr)
        node->m_compoundStatement->Accept(this);


    as->PopBlock(node->m_currentLineNumber);


}

void ASTDispather68000::dispatch(NodeProgram *node)
{
    node->DispatchConstructor();

//    as->EndMemoryBlock();
    NodeBuiltinMethod::m_isInitialized.clear();
    as->Program(node->m_name, node->m_param);
  //  as->m_source << node->m_initJumps;
    node->m_NodeBlock->m_isMainBlock = true;
    node->m_NodeBlock->Accept(this);

//    qDebug() << as->m_currentBlock;
    as->EndProgram();

}

void ASTDispather68000::dispatch(NodeVarType *node)
{

}

void ASTDispather68000::dispatch(NodeBinaryClause *node)
{

}

void ASTDispather68000::dispatch(NodeProcedure *node)
{

}

void ASTDispather68000::dispatch(NodeProcedureDecl *node)
{

}

void ASTDispather68000::dispatch(NodeConditional *node)
{

}

void ASTDispather68000::dispatch(NodeForLoop *node)
{

}

void ASTDispather68000::dispatch(NodeVar *node)
{
    as->m_varStack.push(node->getValue());
}

void ASTDispather68000::dispatch(Node *node)
{

}

void ASTDispather68000::dispatch(NodeAssign *node)
{
    node->DispatchConstructor();

    as->PushCounter();

    AssignVariable(node);

    as->PopCounter(node->m_op.m_lineNumber);

}

void ASTDispather68000::dispatch(NodeBuiltinMethod *node)
{
    node->DispatchConstructor();

    node->VerifyParams(as);

  //  as->PushCounter();

    Methods68000 methods;
    methods.m_node = node;
    methods.Assemble(as,this);

//    as->PopCounter(node->m_op.m_lineNumber-1);

}

void ASTDispather68000::StoreVariable(NodeVar *n)
{

}


void ASTDispather68000::LoadVariable(NodeVar *n)
{

}

void ASTDispather68000::LoadVariable(Node *n)
{

}

void ASTDispather68000::LoadVariable(NodeNumber *n)
{

}

void ASTDispather68000::TransformVariable(Assembler* as, QString op, NodeVar *n, QString val)
{
    as->Asm(op+getEndType(as,n) +" "+val + "," + n->getValue());
    qDebug() << " ** OP : " << op+getEndType(as,n) +" "+val + "," + n->getValue();
}

void ASTDispather68000::TransformVariable(Assembler* as, QString op, QString n, QString val)
{
    as->Asm(op +" "+val + "," + n);
    qDebug() << " ** OP : " << op +" "+val + "," + n;
}

QString ASTDispather68000::AssignVariable(NodeAssign *node) {


    NodeVar* v = (NodeVar*)dynamic_cast<const NodeVar*>(node->m_left);
    //        qDebug() << "AssignVariable: " <<v->value << " : " << TokenType::getType( v->getType(as));

    NodeNumber* num = (NodeNumber*)dynamic_cast<const NodeNumber*>(node->m_left);

    if (v==nullptr && num == nullptr)
        ErrorHandler::e.Error("Left value not variable or memory address! ");
/*    if (num!=nullptr && num->getType(as)!=TokenType::ADDRESS)
        ErrorHandler::e.Error("Left value must be either variable or memory address");
  */


/*    if (num!=nullptr) {
        as->Comment("Assigning memory location (poke replacement)");
        v = new NodeVar(num->m_op); // Create a variable copy
        v->value = num->HexValue();
        //return num->HexValue();
    }
  */

    as->Comment("Assigning single variable : " + v->value);
    Symbol* s = as->m_symTab->Lookup(v->value, node->m_op.m_lineNumber, v->isAddress());

    TokenType::Type t = s->getTokenType();

/*    if (node->m_left->getType(as)==TokenType::POINTER && v->m_expr==nullptr) {
        AssignPointer(node);
        return v->value;
    }

    if ((NodeString*)dynamic_cast<const NodeString*>(node->m_right))
    {
        AssignString(node);
        return v->value;
    }*/
    if (node->m_right==nullptr)
        ErrorHandler::e.Error("Node assign: right hand must be expression", node->m_op.m_lineNumber);

    if (node->m_left->getType(as)==TokenType::INTEGER) {
        node->m_right->m_forceType = TokenType::INTEGER; // FORCE integer on right-hand side
    }
    if (node->m_left->getType(as)==TokenType::LONG) {
        node->m_right->m_forceType = TokenType::LONG; // FORCE integer on right-hand side
    }
    // For constant i:=i+1;
    //if (IsSimpleIncDec(v,  node))
    //    return v->value;

    //node->m_right->Accept(this);
    if (node->m_right->isPureVariable() || node->m_right->isPureNumeric()) {
        TransformVariable(as, "move",v,node->m_right->getValue());
        return "";
    }
    // Some expression instead
    node->m_right->Accept(this);
    TransformVariable(as, "move",v, as->m_varStack.pop());

 //   as->Term();
   // StoreVariable(v);

}

void ASTDispather68000::IncBin(Assembler* as, NodeVarDecl *node) {
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
//        Appendix app(t->m_position);

        Symbol* typeSymbol = as->m_symTab->Lookup(v->value, node->m_op.m_lineNumber);
        typeSymbol->m_org = Util::C64StringToInt(t->m_position);
        typeSymbol->m_size = size;
        //            qDebug() << "POS: " << typeSymbol->m_org;
        //app.Append("org " +t->m_position,1);
        as->Label(v->value);
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

