#include "astdispatcherx86.h"


ASTdispatcherX86::ASTdispatcherX86()
{

}

void ASTdispatcherX86::dispatch(QSharedPointer<NodeBinOP>node)
{
    if (node->m_left->isWord(as))
        node->m_right->setForceType(TokenType::INTEGER);
    if (node->m_right->isWord(as))
        node->m_left->setForceType(TokenType::INTEGER);


    if (node->isPureNumeric()) {
        as->Asm("mov "+getAx(node)+", " + node->getValue(as) + "; binop is pure numeric");
        return;
    }
    if (node->isPureVariable()) {
        as->Asm("mov "+getAx(node)+", [" + node->getValue(as)+"] ; binop is pure variable");
        return;
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
        if (node->m_op.m_type == TokenType::DIV) {
//            node->m_right->setForceType(TokenType::BYTE);
            as->Asm("cwd");
        }
        node->m_right->Accept(this);

        QString ax = getAx(node->m_right);
        PopX();
        as->BinOP(node->m_op.m_type);

        as->Asm(as->m_term + " " +  ax);
        as->m_term = "";
        return;
    }
    node->m_left->Accept(this);
    QString bx = getAx(node->m_left);

    PushX();
    node->m_right->Accept(this);

    QString ax = getAx(node->m_right);
    PopX();
    as->BinOP(node->m_op.m_type);

    as->Asm(as->m_term + " " +  bx +"," +ax);
    as->m_term = "";


}

void ASTdispatcherX86::dispatch(QSharedPointer<NodeNumber>node)
{
    QString ax = getAx(node);
    if (as->m_term!="") {
        as->m_term +=node->getValue(as);
        return;
    }

    as->Asm("mov "+ax+", " + node->getValue(as));
}

void ASTdispatcherX86::dispatch(QSharedPointer<NodeVar> node)
{
    QString ending = "]";
    if (node->m_expr!=nullptr) {
        as->Asm("mov ah,0");
        node->m_expr->Accept(this);
        as->Asm("mov di,ax");
        if (node->getArrayType(as)==TokenType::INTEGER)
            as->Asm("shl di,1 ; Accomodate for word");
        ending = "+di]";
    }

    if (as->m_term!="") {
        as->m_term +=node->getValue(as);
        return;
    }
    QString ax = getAx(node);


    as->Asm("mov "+ax+", [" + node->getValue(as)+ending);
//    if (node->isArrayIndex())
//        qDebug() << TokenType::getType(node->getArrayType(as));
    if (node->m_forceType==TokenType::INTEGER) {
        bool accomodate = false;
        if (node->isArrayIndex()) {
            if (node->getArrayType(as)!=TokenType::INTEGER) {
            accomodate = true;
            }
        }
        else
        if (node->getOrgType(as)!=TokenType::INTEGER)
            accomodate = true;

        if (accomodate)
            as->Asm("mov ah,0 ; forcetype clear high bit");
    }
//    qDebug() << "ORG " <<TokenType::getType(node->getOrgType(as)) << "   : " << node->getValue(as);
  //  qDebug() << "FT " <<TokenType::getType(node->m_forceType);

}

void ASTdispatcherX86::dispatch(QSharedPointer<NodeAsm>node)
{
    node->DispatchConstructor(as);

    QStringList txt = node->m_asm.split("\n");
    as->Comment("");
    as->Comment("****** Inline assembler section");
    for (QString t: txt) {
        as->Write(t,0);
    }
    as->Asm("");

}

void ASTdispatcherX86::dispatch(QSharedPointer<NodeString> node)
{

}

void ASTdispatcherX86::dispatch(QSharedPointer<NodeUnaryOp> node)
{

}

void ASTdispatcherX86::dispatch(QSharedPointer<NodeCompound> node)
{
    node->DispatchConstructor(as);

    as->BeginBlock();
    for (QSharedPointer<Node> n: node->children)
        n->Accept(this);


    as->EndBlock();

}

void ASTdispatcherX86::dispatch(QSharedPointer<NodeVarDecl> node)
{
    node->DispatchConstructor(as);


    node->ExecuteSym(as->m_symTab);


    if (qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode)->m_flags.count()==0) {
    int ret = node->MaintainBlocks(as);



    if (ret==3) node->m_curMemoryBlock = nullptr;
//    qDebug() << "NodeVarDecl new memory block "  << ret;
    if (node->m_curMemoryBlock!=nullptr)
  //      qDebug() << node->m_curMemoryBlock->m_start;
    if (as->m_currentBlock!=nullptr) {
    //    qDebug() <<as->m_currentBlock->m_pos;
        if (node->m_curMemoryBlock==nullptr) {
            bool ok;
            QString p = as->m_currentBlock->m_pos;
            int pos = p.remove("$").toInt(&ok, 16);
            node->m_curMemoryBlock = QSharedPointer<MemoryBlock>(new MemoryBlock(pos,pos,MemoryBlock::ARRAY, node->m_blockInfo.m_blockName));
            as->blocks.append(node->m_curMemoryBlock);
        }
    }
    else
        node->m_curMemoryBlock=nullptr;
 /*   if (ret==2) {
        m_curMemoryBlock = nullptr;

    }*/
    }


    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);

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
 //       as->DecaareP
        as->Asm(v->getValue(as)+ ": dw  0,0" );
//        as->DeclareVariable(v->getValue(as), "long", "0", t->m_position);

        as->m_symTab->Lookup(v->getValue(as), node->m_op.m_lineNumber)->m_arrayType=t->m_arrayVarType.m_type;

    }else {
        node->m_dataSize=1;
        if (t->getValue(as).toLower()=="integer") node->m_dataSize = 2;
        if (t->getValue(as).toLower()=="long") node->m_dataSize = 4;
        as->DeclareVariable(v->getValue(as), t->value, t->initVal, t->m_position);
    }


    if (node->m_curMemoryBlock!=nullptr) {
        node->m_curMemoryBlock->m_end+=node->m_dataSize;
    }

    as->m_currentBlock = nullptr;

}

void ASTdispatcherX86::dispatch(QSharedPointer<NodeBlock> node)
{
    node->DispatchConstructor(as);

    as->PushBlock(node->m_currentLineNumber);


    bool blockLabel = false;
    bool blockProcedure = false;
    bool hasLabel = false;

    QString label = as->NewLabel("block");


    if (node->m_decl.count()!=0) {
        as->Asm(m_jmp+ label);
        hasLabel = true;
        //           as->PushBlock(m_decl[0]->m_op.m_lineNumber-1);
    }
    for (QSharedPointer<Node> n: node->m_decl) {
        if (qSharedPointerDynamicCast<QSharedPointer<NodeVarDecl>>(n)==nullptr) {
            if (!blockProcedure) // Print label at end of vardecl
            {
                if (n->m_op.m_lineNumber!=0) {
                    //                      as->PopBlock(n->m_op.m_lineNumber);
                    blockProcedure = true;
                    //   qDebug() << "pop" << n->m_op.m_lineNumber << " " << TokenType::getType(n->getType(as));
                }

            }

        }
        n->Accept(this);

    }
    as->VarDeclEnds();

    if (!blockLabel && hasLabel) {
        as->Label(label);

    }

    if (node->forceLabel!="")
        as->Label(node->forceLabel);


    if (node->m_compoundStatement!=nullptr)
        node->m_compoundStatement->Accept(this);


    as->PopBlock(node->m_currentLineNumber);

}

void ASTdispatcherX86::dispatch(QSharedPointer<NodeProgram> node)
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

void ASTdispatcherX86::dispatch(QSharedPointer<NodeVarType> node)
{

}

void ASTdispatcherX86::dispatch(QSharedPointer<NodeBinaryClause> node)
{

}

void ASTdispatcherX86::dispatch(QSharedPointer<NodeProcedure> node)
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

    as->Asm("call " + node->m_procedure->m_procName);

}

void ASTdispatcherX86::dispatch(QSharedPointer<NodeProcedureDecl> node)
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

    if (!isInitFunction) {
    if (node->m_block!=nullptr) {
        QSharedPointer<NodeBlock> b = qSharedPointerDynamicCast<NodeBlock>(node->m_block);
        if (b!=nullptr)
            b->forceLabel=node->m_procName;
        node->m_block->Accept(this);
//        node->m_block->Build(as);
    }
    }
    as->Asm("ret");



}

void ASTdispatcherX86::dispatch(QSharedPointer<NodeConditional> node)
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

    BuildSimple(bn,  failedLabel,node->m_forcePage==1);

    // Start main block
    as->Label(lblstartTrueBlock); // This means skip inside

    node->m_block->Accept(this);

    if (node->m_elseBlock!=nullptr)
        as->Asm(m_jmp + labelElseDone);

    // If while loop, return to beginning of conditionals
    if (node->m_isWhileLoop)
        as->Asm(m_jmp + labelStartOverAgain);

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

void ASTdispatcherX86::dispatch(QSharedPointer<NodeForLoop> node)
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
    if (node->m_b->isPure())
//    if (qSharedPointerDynamicCast<NodeNumber>(node->m_b) != nullptr)
        to = node->m_b->getValue(as);
  //  if (qSharedPointerDynamicCast<NodeVar>(node->m_b) != nullptr)
    //    to = (qSharedPointerDynamicCast<NodeVar>node->m_b)->getValue(as);

//    as->m_stack["for"].push(var);
    QString lblFor =as->NewLabel("forloop");
    as->Label(lblFor);
//    qDebug() << "end for";


    if (nVar->m_left->isWord(as))
        node->m_b->setForceType(TokenType::INTEGER);

    node->m_block->Accept(this);
    PushX();
    QString ax = getAx(nVar->m_left);
    PopX();
    as->Asm(m_mov+ax+",["+var+"]");
    as->Asm("add "+ax+",1");
    as->Asm(m_mov+"["+var+"],"+ax);
    LoadVariable(node->m_b);
    as->Asm(m_cmp+getAx(node->m_b)+","+ax);

    as->Asm(m_jne+lblFor);

    as->PopLabel("forloop");

}


void ASTdispatcherX86::dispatch(QSharedPointer<Node> node)
{

}

void ASTdispatcherX86::dispatch(QSharedPointer<NodeAssign> node)
{
/*    if (node==nullptr)
        return;*/
//    node->DispatchConstructor(as);
    node->m_currentLineNumber = node->m_op.m_lineNumber;


    AssignVariable(node);

}

void ASTdispatcherX86::dispatch(QSharedPointer<NodeCase> node)
{

}

void ASTdispatcherX86::dispatch(QSharedPointer<NodeRepeatUntil> node)
{

}

void ASTdispatcherX86::dispatch(QSharedPointer<NodeBuiltinMethod> node)
{
    node->DispatchConstructor(as);

    node->VerifyParams(as);


    QSharedPointer<AbstractMethods> methods = FactoryMethods::CreateMethods(Syntax::s.m_currentSystem->m_system);
    methods->m_node = node;
    methods->Assemble(as,this);



}

void ASTdispatcherX86::dispatch(QSharedPointer<NodeComment> node)
{

}

void ASTdispatcherX86::StoreVariable(QSharedPointer<NodeVar> n)
{

}

void ASTdispatcherX86::LoadVariable(QSharedPointer<NodeVar> n)
{
    n->Accept(this);
}

void ASTdispatcherX86::LoadAddress(QSharedPointer<Node> n)
{

}

void ASTdispatcherX86::LoadAddress(QSharedPointer<Node> n, QString reg)
{

}

void ASTdispatcherX86::LoadPointer(QSharedPointer<Node> n)
{

}

void ASTdispatcherX86::LoadVariable(QSharedPointer<Node> n)
{
    n->Accept(this);

}

void ASTdispatcherX86::LoadVariable(QSharedPointer<NodeNumber>n)
{

}

QString ASTdispatcherX86::getEndType(Assembler *as, QSharedPointer<Node> v)
{
return "";
}

QString ASTdispatcherX86::AssignVariable(QSharedPointer<NodeAssign> node)
{

    if (node->m_left->isWord(as)) {
        node->m_right->setForceType(TokenType::INTEGER);
    }




    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(node->m_left);





    if (var->isPointer(as)) {
        if (node->m_right->isPureVariable()) {
            as->Asm("lea si, ["+node->m_right->getValue(as)+"]");
            as->Asm("mov ["+var->getValue(as)+"], ds");
            as->Asm("mov ["+var->getValue(as)+"+2], si");
            return "";
        }
        else{
            node->m_right->Accept(this);

            as->Asm("mov ["+var->getValue(as)+"], ax");
            as->Asm("mov ["+var->getValue(as)+"+2], bx");
        }
        return "";
    }

    if (var->isArrayIndex()) {
        // Is an array
        node->m_right->Accept(this);
        as->Asm("push ax");
        var->m_expr->Accept(this);
        as->Asm("mov di,ax");
        if (var->isWord(as))
            as->Asm("shl di,1");
        as->Asm("pop ax");
        as->Asm("mov ["+var->getValue(as) + "+di], "+getAx(node->m_left));
        return "";
    }

//    if (var->getValue())
    // Simple a:=b;
    QString type ="byte";
    if (var->isWord(as))
        type = "word";

    if (node->m_right->isPureNumeric()) {
        as->Asm("mov ["+var->getValue(as)+ "], "+type+ " "+node->m_right->getValue(as));
        return "";
    }
    // Check for a:=a+2;
    QSharedPointer<NodeBinOP> bop =  qSharedPointerDynamicCast<NodeBinOP>(node->m_right);
   // as->Comment("Testing for : a:=a+ expr " + QString::number(bop!=nullptr));
   // if (bop!=nullptr)
     //  as->Comment(TokenType::getType(bop->getType(as)));
    if (bop!=nullptr && (bop->m_op.m_type==TokenType::PLUS || bop->m_op.m_type==TokenType::MINUS)) {
  //      as->Comment("PREBOP searching for "+var->getValue(as));
        if (bop->ContainsVariable(as,var->getValue(as))) {
            // We are sure that a:=a ....
            // first, check if a:=a + number
//            as->Comment("In BOP");
            if (bop->m_right->isPureNumeric()) {
                as->Comment("'a:=a + const'  optimization ");
                as->Asm(getBinaryOperation(bop) + " ["+var->getValue(as)+"], "+type + " "+bop->m_right->getValue(as));
                return "";
            }
            as->Comment("'a:=a + expression'  optimization ");
            bop->m_right->Accept(this);
            as->Asm(getBinaryOperation(bop) + " ["+var->getValue(as)+"], "+getAx(var));
            return "";
        }
        // Check for a:=a+

    }
/*    if (node->m_right->isPureVariable()) {
        as->Asm("mov ["+var->getValue(as)+ "],   " +getX86Value(as,node->m_right));
        return "";
    }
*/

    node->m_right->Accept(this);

    as->Asm("mov ["+qSharedPointerDynamicCast<NodeVar>(node->m_left)->getValue(as) + "], "+getAx(node->m_left));
    return "";
}

void ASTdispatcherX86::DeclarePointer(QSharedPointer<NodeVarDecl> node)
{

}

QString ASTdispatcherX86::getEndType(Assembler *as, QSharedPointer<Node> v1, QSharedPointer<Node> v2)
{
    return "";
}

void ASTdispatcherX86::IncBin(Assembler *as, QSharedPointer<NodeVarDecl> node) {
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

void ASTdispatcherX86::BuildSimple(QSharedPointer<Node> node, QString lblFailed, bool offPage)
{

    as->Comment("Binary clause Simplified: " + node->m_op.getType());
    //    as->Asm("pha"); // Push that baby

    BuildToCmp(node);

    if (node->m_op.m_type==TokenType::EQUALS)
        as->Asm("jne " + lblFailed);
    if (node->m_op.m_type==TokenType::NOTEQUALS)
        as->Asm("je " + lblFailed);
    if (node->m_op.m_type==TokenType::LESS)
        as->Asm("jg " + lblFailed);
    if (node->m_op.m_type==TokenType::GREATER)
        as->Asm("jl " + lblFailed);



}

void ASTdispatcherX86::BuildToCmp(QSharedPointer<Node> node)
{
    if (node->m_left->getValue(as)!="") {
        if (node->m_right->isPureNumeric())
        {
            as->Comment("Compare with pure num / var optimization");
            //            TransformVariable(as,"cmp",node->m_left->getValue(as),node->m_right->getValue(as),node->m_left);
//            TransformVariable(as,"cmp",node->m_left->getValue(as),node->m_right->getValue(as),node->m_left);
            LoadVariable(node->m_left);
            as->Asm("cmp "+getAx(node->m_left)+"," + node->m_right->getValue(as));

            return;
        } else
        {
            as->Comment("Compare two vars optimization");
            if (node->m_right->isPureVariable()) {
                QString wtf = as->m_regAcc.Get();
                LoadVariable(node->m_right);
                //TransformVariable(as,"move",wtf,qSharedPointerDynamicCast<NodeVar>node->m_left);
                //TransformVariable(as,"cmp",wtf,as->m_varStack.pop());
                as->Asm("cmp  "+node->m_left->getValue(as) +"," + getAx(node->m_right));

                return;
            }
            else
                node->m_right->Accept(this);

            as->Asm("cmp  "+node->m_left->getValue(as) +"," + getAx(node->m_right));

//            TransformVariable(as,"cmp",qSharedPointerDynamicCast<NodeVar>node->m_left,as->m_varStack.pop());
            return;
        }
    }

    node->m_right->Accept(this);


    as->Asm("cmp  "+node->m_left->getValue(as) +"," + getAx(node->m_right));

//    TransformVariable(as,"cmp",qSharedPointerDynamicCast<NodeVar>node->m_left, as->m_varStack.pop());

    // Perform a full compare : create a temp variable
    //        QString tmpVar = as->m_regAcc.Get();//as->StoreInTempVar("binary_clause_temp");
    //        node->m_right->Accept(this);
    //      as->Asm("cmp " + tmpVar);
    //      as->PopTempVar();


}
