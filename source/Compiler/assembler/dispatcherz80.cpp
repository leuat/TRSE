#include "dispatcherz80.h"


ASTdispatcherZ80::ASTdispatcherZ80()
{

}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeBinOP>node)
{


}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeNumber>node)
{
}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeVar> node)
{

}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeAsm>node)
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

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeString> node)
{

}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeUnaryOp> node)
{

}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeCompound> node)
{
    node->DispatchConstructor(as);

    as->BeginBlock();
    for (QSharedPointer<Node> n: node->children)
        n->Accept(this);


    as->EndBlock();

}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeVarDecl> node)
{
    node->DispatchConstructor(as);


    node->ExecuteSym(as->m_symTab);

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

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeBlock> node)
{
    node->DispatchConstructor(as);

    as->PushBlock(node->m_currentLineNumber);


    bool blockLabel = false;
    bool blockProcedure = false;
    bool hasLabel = false;

    QString label = as->NewLabel("block");


    if (node->m_decl.count()!=0) {
        as->Asm("jp " + label);
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

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeProgram> node)
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

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeVarType> node)
{

}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeBinaryClause> node)
{

}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeProcedure> node)
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

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeProcedureDecl> node)
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

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeConditional> node)
{

}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeForLoop> node)
{

}


void ASTdispatcherZ80::dispatch(QSharedPointer<Node> node)
{

}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeAssign> node)
{
/*    if (node==nullptr)
        return;*/
//    node->DispatchConstructor(as);
    node->m_currentLineNumber = node->m_op.m_lineNumber;


 //   AssignVariable(node);

}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeCase> node)
{

}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeRepeatUntil> node)
{

}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeBuiltinMethod> node)
{
    node->DispatchConstructor(as);

    node->VerifyParams(as);


    QSharedPointer<AbstractMethods> methods = FactoryMethods::CreateMethods(Syntax::s.m_currentSystem->m_system);
    methods->m_node = node;
    methods->Assemble(as,this);



}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeComment> node)
{

}

void ASTdispatcherZ80::StoreVariable(QSharedPointer<NodeVar> n)
{

}

void ASTdispatcherZ80::LoadVariable(QSharedPointer<NodeVar> n)
{
    n->Accept(this);
}

void ASTdispatcherZ80::LoadAddress(QSharedPointer<Node> n)
{

}

void ASTdispatcherZ80::LoadAddress(QSharedPointer<Node> n, QString reg)
{

}

void ASTdispatcherZ80::LoadPointer(QSharedPointer<Node> n)
{

}

void ASTdispatcherZ80::LoadVariable(QSharedPointer<Node> n)
{
    n->Accept(this);

}

void ASTdispatcherZ80::LoadVariable(QSharedPointer<NodeNumber>n)
{

}

void ASTdispatcherZ80::IncBin(Assembler *as, QSharedPointer<NodeVarDecl> node) {
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

