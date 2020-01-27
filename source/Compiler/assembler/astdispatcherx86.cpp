#include "astdispatcherx86.h"


ASTDispatcherX86::ASTDispatcherX86()
{

}

void ASTDispatcherX86::dispatch(NodeBinOP *node)
{
    if (node->isPureNumeric()) {
        as->Asm("mov "+getAx(node)+", " + node->getValue(as) + "; binop is pure numeric");
        return;
    }
    if (node->isPureVariable()) {
        as->Asm("mov "+getAx(node)+", [" + node->getValue(as)+"] ; binop is pure variable");
        return;
    }
/*    if (!node->m_left->isPure() && node->m_right->isPure()) {
        Node* t = node->m_right;
        node->m_right = node->m_left;
        node->m_left = t;
        qDebug() << "SWITCH";
    }
*/
    if (node->m_op.m_type == TokenType::MUL || node->m_op.m_type == TokenType::DIV) {
        node->m_left->Accept(this);
        QString bx = getAx(node->m_left);

        PushX();
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

void ASTDispatcherX86::dispatch(NodeNumber *node)
{
    QString ax = getAx(node);
    if (as->m_term!="") {
        as->m_term +=node->getValue(as);
        return;
    }

    as->Asm("mov "+ax+", " + node->getValue(as));
}

void ASTDispatcherX86::dispatch(NodeVar *node)
{
    QString ax = getAx(node);
    if (as->m_term!="") {
        as->m_term +=node->getValue(as);
        return;
    }

    as->Asm("mov "+ax+", [" + node->getValue(as)+"]");

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
    node->DispatchConstructor(as);


    node->ExecuteSym(as->m_symTab);



    NodeVar* v = (NodeVar*)node->m_varNode;
    NodeVarType* t = (NodeVarType*)node->m_typeNode;

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
        DeclarePointer(node);
        as->m_symTab->Lookup(v->getValue(as), node->m_op.m_lineNumber)->m_arrayType=t->m_arrayVarType.m_type;

    }else {
        node->m_dataSize=1;
        if (t->getValue(as).toLower()=="integer") node->m_dataSize = 2;
        if (t->getValue(as).toLower()=="long") node->m_dataSize = 4;
        as->DeclareVariable(v->getValue(as), t->value, t->initVal);
    }



    as->m_currentBlock = nullptr;

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


void ASTDispatcherX86::dispatch(Node *node)
{

}

void ASTDispatcherX86::dispatch(NodeAssign *node)
{
    node->DispatchConstructor(as);

    AssignVariable(node);

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

    if (node->m_left->isWord(as)) {
        node->m_right->m_forceType == TokenType::INTEGER;
    }

    node->m_right->Accept(this);
    as->Asm("mov ["+dynamic_cast<NodeVar*>(node->m_left)->getValue(as) + "], "+getAx(node));

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
