#include "astdispatcherx86.h"


ASTDispatcherX86::ASTDispatcherX86()
{

}

void ASTDispatcherX86::dispatch(NodeBinOP *node)
{

}

void ASTDispatcherX86::dispatch(NodeNumber *node)
{

}

void ASTDispatcherX86::dispatch(NodeAsm *node)
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

void ASTDispatcherX86::dispatch(NodeString *node)
{

}

void ASTDispatcherX86::dispatch(NodeUnaryOp *node)
{

}

void ASTDispatcherX86::dispatch(NodeCompound *node)
{
    node->DispatchConstructor(as);

    as->BeginBlock();
    for (Node* n: node->children)
        n->Accept(this);


    as->EndBlock();

}

void ASTDispatcherX86::dispatch(NodeVarDecl *node)
{

}

void ASTDispatcherX86::dispatch(NodeBlock *node)
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

void ASTDispatcherX86::dispatch(NodeProgram *node)
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

void ASTDispatcherX86::dispatch(NodeVarType *node)
{

}

void ASTDispatcherX86::dispatch(NodeBinaryClause *node)
{

}

void ASTDispatcherX86::dispatch(NodeProcedure *node)
{
    node->DispatchConstructor(as);


    if (node->m_parameters.count()!=node->m_procedure->m_paramDecl.count())
        ErrorHandler::e.Error("Procedure '" + node->m_procedure->m_procName+"' requires "
                              + QString::number(node->m_procedure->m_paramDecl.count()) +" parameters, not "
                              + QString::number(node->m_parameters.count()) + ".", node->m_op.m_lineNumber);

    for (int i=0; i<node->m_parameters.count();i++) {
        // Assign all variables
        NodeVarDecl* vd = (NodeVarDecl*)node->m_procedure->m_paramDecl[i];
        NodeAssign* na = new NodeAssign(vd->m_varNode, node->m_parameters[i]->m_op, node->m_parameters[i]);
        na->Accept(this);
//        na->Build(as);
    }

    as->Asm("call " + node->m_procedure->m_procName);

}

void ASTDispatcherX86::dispatch(NodeProcedureDecl *node)
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
        NodeBlock* b = dynamic_cast<NodeBlock*>(node->m_block);
        if (b!=nullptr)
            b->forceLabel=node->m_procName;
        node->m_block->Accept(this);
//        node->m_block->Build(as);
    }
    }
    as->Asm("ret");



}

void ASTDispatcherX86::dispatch(NodeConditional *node)
{

}

void ASTDispatcherX86::dispatch(NodeForLoop *node)
{

}

void ASTDispatcherX86::dispatch(NodeVar *node)
{

}

void ASTDispatcherX86::dispatch(Node *node)
{

}

void ASTDispatcherX86::dispatch(NodeAssign *node)
{

}

void ASTDispatcherX86::dispatch(NodeCase *node)
{

}

void ASTDispatcherX86::dispatch(NodeRepeatUntil *node)
{

}

void ASTDispatcherX86::dispatch(NodeBuiltinMethod *node)
{
    node->DispatchConstructor(as);

    node->VerifyParams(as);


    MethodsX86 methods;
    methods.m_node = node;
    methods.Assemble(as,this);



}

void ASTDispatcherX86::dispatch(NodeComment *node)
{

}

void ASTDispatcherX86::StoreVariable(NodeVar *n)
{

}

void ASTDispatcherX86::LoadVariable(NodeVar *n)
{

}

void ASTDispatcherX86::LoadAddress(Node *n)
{

}

void ASTDispatcherX86::LoadAddress(Node *n, QString reg)
{

}

void ASTDispatcherX86::LoadPointer(Node *n)
{

}

void ASTDispatcherX86::LoadVariable(Node *n)
{

}

void ASTDispatcherX86::LoadVariable(NodeNumber *n)
{

}

QString ASTDispatcherX86::getEndType(Assembler *as, Node *v)
{

}

QString ASTDispatcherX86::AssignVariable(NodeAssign *node)
{

}

void ASTDispatcherX86::IncBin(Assembler *as, NodeVarDecl *node)
{

}

void ASTDispatcherX86::BuildSimple(Node *node, QString lblFailed)
{

}

void ASTDispatcherX86::BuildToCmp(Node *node)
{

}

void ASTDispatcherX86::DeclarePointer(NodeVarDecl *node)
{

}

QString ASTDispatcherX86::getEndType(Assembler *as, Node *v1, Node *v2)
{

}
