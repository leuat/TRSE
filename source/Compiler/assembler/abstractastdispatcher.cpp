#include "abstractastdispatcher.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodeconditional.h"
#include "source/Compiler/ast/nodeforloop.h"
#include "source/Compiler/ast/nodeblock.h"
#include "source/Compiler/ast/nodeproceduredecl.h"
#include "source/Compiler/ast/nodebuiltinmethod.h"
#include "source/Compiler/ast/nodeprocedure.h"
#include "source/Compiler/ast/nodeprogram.h"
#include "source/Compiler/ast/nodevardecl.h"
#include "source/Compiler/ast/nodeasm.h"
#include "source/Compiler/ast/nodecompound.h"

AbstractASTDispatcher::AbstractASTDispatcher()
{

}

void AbstractASTDispatcher::dispatch(QSharedPointer<NodeBlock> node) {
    if (m_ticks++%8==0)
        emit EmitTick(".");


    node->DispatchConstructor(as);

 //   AbstractASTDispatcher::dispatch(node);


    // In case memory block is acive
    //as->EndMemoryBlock();
    int ln = node->m_op.m_lineNumber-1;
    if (ln==0) ln++;
    as->PushBlock(node->m_currentLineNumber);




    bool blockLabel = false;
    bool blockProcedure = false;
    bool hasLabel = false;

    QString label = as->NewLabel("block");


    if (!node->m_ignoreDeclarations) {

        if (node->m_decl.count()!=0) {
            if (node->m_isMainBlock && !as->m_ignoreInitialJump)
                as->Asm(getJmp(true)+" " + label);
            hasLabel = true;
        }


        for (QSharedPointer<Node> n: node->m_decl) {
            // Print label at end of vardecl
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
            n->Accept(this);

        }
        as->VarDeclEnds();
    }
    as->PushCounter();

    if (node->m_isMainBlock) {
        int ret = node->MaintainBlocks(as);

        as->m_currentBlockName="MainProgram";
    }


    if (!blockLabel && hasLabel)
        as->Label(label);

    if (node->forceLabel!="")
        as->Label(node->forceLabel);

    if (node->m_isMainBlock && Syntax::s.m_currentSystem->m_system == AbstractSystem::NES)
        as->IncludeFile(":resources/code/nes_init.asm");


    if (node->m_compoundStatement!=nullptr)
        node->m_compoundStatement->Accept(this);

    as->PopBlock(node->m_currentLineNumber);
    if (node->m_isMainBlock && Syntax::s.m_currentSystem->m_system == AbstractSystem::NES) {
        as->StartMemoryBlock("$FFFA");
        as->IncludeFile(":resources/code/nes_end.asm");
        as->EndMemoryBlock();
    }
    if (node->m_isMainBlock)
        as->Label("EndSymbol");


    node->PopZeroPointers(as);
    as->PopCounter(ln);
}

void AbstractASTDispatcher::dispatch(QSharedPointer<NodeForLoop> node)
{
    node->DispatchConstructor(as);


    //QString m_currentVar = ((NodeAssign*)m_a)->m_
    QSharedPointer<NodeAssign> nVar = qSharedPointerDynamicCast<NodeAssign>(node->m_a);


    if (nVar==nullptr)
        ErrorHandler::e.Error("Index must be variable", node->m_op.m_lineNumber);

    QString var = qSharedPointerDynamicCast<NodeVar>(nVar->m_left)->getValue(as);//  m_a->Build(as);
    //    qDebug() << "Starting for";
    node->m_a->Accept(this);

    QString lblFor =as->NewLabel("forloop");
    as->Label(lblFor);

    if (nVar->m_left->isWord(as))
        node->m_b->setForceType(TokenType::INTEGER);

    node->m_block->Accept(this);


    bool offpage = isOffPage(node, node->m_block, nullptr);

    CompareAndJumpIfNotEqual(node->m_a, node->m_b,  node->m_step, lblFor, offpage,node->m_inclusive);


    as->PopLabel("forloop");


}

QString AbstractASTDispatcher::getValue(QSharedPointer<Node> n) {
    return n->getValue(as);
}

void AbstractASTDispatcher::dispatch(QSharedPointer<NodeConditional> node)
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


    bool offpage = isOffPage(node, node->m_block, node->m_elseBlock);



//    HandleCompoundBinaryClause(bn, failedLabel, lblstartTrueBlock,page);
    QString localFailed = failedLabel;
    // Z80 has jumps in instruction
    if (offpage && Syntax::s.m_currentSystem->m_processor!=AbstractSystem::Z80) {
        localFailed = as->NewLabel("localfailed");
    }
    HandleCompoundBinaryClause(bn, localFailed, lblstartTrueBlock,offpage);
    // OFFPAGE branching:
    if (offpage && Syntax::s.m_currentSystem->m_processor!=AbstractSystem::Z80) {
        as->Asm(getJmp(offpage) + " "+lblstartTrueBlock);
        as->Label(localFailed);
        as->PopLabel("localfailed");
        as->Asm(getJmp(offpage) + " " + failedLabel);
    }


    /*
        // Handle AND & OR
        if (bn->isCompoundClause()) {
            HandleCompoundBinaryClause(bn, failedLabel, lblstartTrueBlock,node->m_forcePage==1);
        }
        else
            BuildSimple(bn,  failedLabel,node->m_forcePage==1);
    */
    // Start main block
    as->Label(lblstartTrueBlock+": ;Main true block ;keep "); // This means skip inside
    node->m_block->Accept(this);

    if (node->m_elseBlock!=nullptr)
        as->Asm(getJmp(offpage)+" " + labelElseDone);

    // If while loop, return to beginning of conditionals
//    qDebug() << "IS OFFPAGE: " << getJmp(offpage) << offpage;
    if (node->m_isWhileLoop)
        as->Asm(getJmp(offpage)+" " + labelStartOverAgain);

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

void AbstractASTDispatcher::HandleCompoundBinaryClause(QSharedPointer<Node> node, QString lblFailed, QString lblSuccess, bool offpage)
{
    //    QSharedPointer<NodeBinaryClause> bc = qSharedPointerDynamicCast<NodeBinaryClause>(node);
    if (!node->isCompoundClause()) { // IS LEAF
        BuildSimple(node,  lblSuccess, lblFailed, offpage);
        return;
    }


    if (node->m_op.m_type == TokenType::AND) {
        HandleCompoundBinaryClause(node->m_left,  lblFailed, lblSuccess, offpage);
        as->Comment("; logical AND");
        HandleCompoundBinaryClause(node->m_right,  lblFailed,lblSuccess,offpage);
    }
    if (node->m_op.m_type == TokenType::OR) {
        QString lblLocalFailed = as->NewLabel("localfailed");
        HandleCompoundBinaryClause(node->m_left,  lblLocalFailed,lblSuccess,offpage);
        // Success! please continue!
        as->Asm(getJmp(offpage)+" "+lblSuccess);
        as->Label(lblLocalFailed+": ;keep");
        as->Comment("; logical OR, second chance");
        HandleCompoundBinaryClause(node->m_right,  lblFailed,lblSuccess,offpage);
        as->PopLabel("localfailed");
    }

}

bool AbstractASTDispatcher::isOffPage(QSharedPointer<Node> node, QSharedPointer<Node> b1, QSharedPointer<Node> b2) {
    bool onPage = node->verifyBlockBranchSize(as, b1,b2,this);

    if (node->m_forcePage == 1)
        onPage = false;

    if (node->m_forcePage == 2)
        onPage = true;

    return !onPage;
}

void AbstractASTDispatcher::dispatch(QSharedPointer<NodeProcedureDecl> node)
{

    node->DispatchConstructor(as);
    // Don't declare inline procedures

    if (node->m_isInline) {
        // Only declare variables in SYMTAB
        for (QSharedPointer<Node> n: qSharedPointerDynamicCast<NodeBlock>(node->m_block)->m_decl) {
            // Print label at end of vardecl
            auto vd = qSharedPointerDynamicCast<NodeVarDecl>(n);
            if (vd!=nullptr)
                vd->ExecuteSym(as->m_symTab);

        }
        return;
    }

    as->m_symTab->SetCurrentProcedure(node->m_procName+"_");
    int ln = node->m_currentLineNumber;
    //    as->PushCounter();
    //    if (node->m_curMemoryBlock!=nullptr)
    //      qDebug() << node->m_procName << "IS IN BLOCK " << node->m_curMemoryBlock->m_name << " STARTING AT " << Util::numToHex(node->m_curMemoryBlock->m_start);

    // In case memory block is acive


    //    qDebug() << node->m_procName << node->m_curMemoryBlock << as->m_currentBlock;


    int ret = node->MaintainBlocks(as);
    if (ret==3) node->m_curMemoryBlock=nullptr;
    if (as->m_currentBlock!=nullptr) {
        if (node->m_curMemoryBlock==nullptr) {
            bool ok;
            //            qDebug() << "Creating new block procedure for " << m_procName;
            QString p = as->m_currentBlock->m_pos;
            int pos = p.remove("$").toInt(&ok, 16);
            node->m_curMemoryBlock = QSharedPointer<MemoryBlock>(new MemoryBlock(pos,pos,MemoryBlock::ARRAY, node->m_blockInfo.m_blockName));
            as->blocks.append(node->m_curMemoryBlock);
        }
    }
    else {
        //node->m_curMemoryBlock=nullptr;
    }


    //MaintainBlocks(as);
    if (node->m_block==nullptr) {  // Is builtin procedure
        node->m_block = QSharedPointer<NodeBuiltinMethod>(new NodeBuiltinMethod(node->m_procName, QVector<QSharedPointer<Node>>(), nullptr));
        //        Node::s_uniqueSymbols[node->m_block] = node->m_block; // Mark for deletion

    }

    bool isInitFunction=false;
    bool isBuiltinFunction=false;
    if (Syntax::s.builtInFunctions.contains(node->m_procName)) {
        isBuiltinFunction = true;
        isInitFunction = Syntax::s.builtInFunctions[node->m_procName].m_initFunction;
        //        as->EndMemoryBlock();

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
    as->m_currentBlockName = node->m_procName;

    if (!isInitFunction) {
        //as->Asm("jmp afterProc_" + m_procName);


        //as->Label(m_procName);
    }

    if (node->m_block!=nullptr) {
        QSharedPointer<NodeBlock> b = qSharedPointerDynamicCast<NodeBlock>(node->m_block);
        if (b!=nullptr) {
            b->forceLabel=node->m_procName;
            b->m_isProcedure = true;
        }
        node->m_block->Accept(this);
        //        node->Delete();
        //        node->s_uniqueSymbols[node->m_block] = node->m_block;


        //        delete node;
        //        node->m_block->Build(as);
    }
    if (!isInitFunction) {
        if (node->m_type==0) {
            as->Asm(getReturn());
        }
        else as->Asm(getReturnInterrupt());
    }

    if (node->m_curMemoryBlock!=nullptr) {
        node->m_curMemoryBlock->m_end+=10;
    }

    as->m_symTab->ExitProcedureScope(false);
    //  as->PopCounter(ln);
}

void AbstractASTDispatcher::dispatch(QSharedPointer<NodeProcedure> node)
{
    node->DispatchConstructor(as);

    if (node->m_parameters.count()!=node->m_procedure->m_paramDecl.count())
        ErrorHandler::e.Error("Procedure '" + node->m_procedure->m_procName+"' requires "
                              + QString::number(node->m_procedure->m_paramDecl.count()) +" parameters, not "
                              + QString::number(node->m_parameters.count()) + ".", node->m_op.m_lineNumber);

    if (node->m_procedure->m_isInline) {
        InlineProcedure(node);
        return;
    }


    for (int i=0; i<node->m_parameters.count();i++) {
        // Assign all variables
        QSharedPointer<NodeVarDecl> vd = qSharedPointerDynamicCast<NodeVarDecl>(node->m_procedure->m_paramDecl[i]);
        QSharedPointer<NodeAssign>na = QSharedPointer<NodeAssign>(new NodeAssign(vd->m_varNode, node->m_parameters[i]->m_op, node->m_parameters[i]));
        na->Accept(this);
    }

    as->Asm(getCallSubroutine() + " " + node->m_procedure->m_procName);

}

void AbstractASTDispatcher::InlineProcedure(QSharedPointer<NodeProcedure> p)
{
    //    as->Comment("Inline procedure : "+p->m_procedure->m_procName);
    m_inlineParameters.clear();
    int cur = 0;
    for (auto v : p->m_procedure->m_paramDecl) {
        QSharedPointer<NodeVarDecl> nv = qSharedPointerDynamicCast<NodeVarDecl>(v);
        QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(nv->m_varNode);
        //    qDebug() << "Preparing inline : " <<var->value << cur;
        m_inlineParameters[var->value] = p->m_parameters[cur++];
    }
    qSharedPointerDynamicCast<NodeBlock>(p->m_procedure->m_block)->m_ignoreDeclarations = true;
    //    qDebug() << "Before Inline Proc" <<m_inlineParameters.keys();
    p->m_procedure->m_block->Accept(this);
    //  qDebug() << "Done Inline Proc";
    m_inlineParameters.clear();

}

void AbstractASTDispatcher::dispatch(QSharedPointer<NodeProgram> node)
{
    node->DispatchConstructor(as);

    NodeBuiltinMethod::m_isInitialized.clear();
    as->Program(node->m_name, node->m_param);
    for (QString s: node->m_initJumps)
        as->Asm(s);

    node->m_NodeBlock->m_isMainBlock = true;
    node->m_NodeBlock->Accept(this);

    as->EndProgram();
}

void AbstractASTDispatcher::dispatch(QSharedPointer<NodeVarDecl> node)
{
    node->DispatchConstructor(as);


    if (UseBlocks()) {
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

        //    qDebug() << "NODEVARDECL";
    }
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);

    // Don't declare va
    if (v->m_isGlobal)
        return;


    QString keep = v->value;

    v->value = as->m_symTab->getCurrentProcedure()+v->value;
    //    QSharedPointer<Symbol> s = as->m_symTab->Lookup(v->value, node->m_op.m_lineNumber);

    node->ExecuteSym(as->m_symTab);


    //    qDebug() << v->value;
    //    qDebug() << "NVA A";
    //    qDebug() << as->m_symTab->m_symbols.keys();
    //   v->value = keep;
    //        v->m_op.m_type =t->m_op.m_type;
    //v->m_type = t;
    //  qDebug() << "NVA B";

    if (t->m_op.m_type==TokenType::ARRAY) {
        as->DeclareArray(v->value, t->m_arrayVarType.m_value, t->m_op.m_intVal, t->m_data, t->m_position);
        //qDebug() << "IS: " << TokenType::types[as->m_symTab->Lookup(getValue(v))->getTokenType()];
        node->m_dataSize=t->m_op.m_intVal;
        QSharedPointer<Symbol> s = as->m_symTab->Lookup(v->value, node->m_op.m_lineNumber);
        s->isUsed=false;
        //if (!v->isRecord(as))
        // Symbol needs reorganizing, move type to typearray while main type now is address. stupid name, should be array
        s->m_arrayTypeText = s->m_type;
        //qDebug() << "DECLARE ARRAY "<<s->m_type;
        s->m_type="address";
        s->m_arrayType = t->m_arrayVarType.m_type;
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
                    //    if (node->m_curMemoryBlock!=nullptr && ((QSharedPointer<NodeVarType>)node->m_typeNode)->m_position!="")
                    //       ErrorHandler::e.Error("IncBin can not be declared within a user-defined memory block with an abslute address. :",node->m_op.m_lineNumber);

                    IncBin(node);
                }
                else
                    if (t->m_op.m_type==TokenType::POINTER) {
                        //if (node->m_curMemoryBlock!=nullptr)
                        //    ErrorHandler::e.Error("Pointers can not be declared within a user-defined memory block :",node->m_op.m_lineNumber);
                        DeclarePointer(node);
                        as->m_symTab->Lookup(v->getValue(as), node->m_op.m_lineNumber)->m_arrayType=t->m_arrayVarType.m_type;

                    }
                    else if (t->m_op.m_type==TokenType::INCSID || t->m_op.m_type==TokenType::INCNSF) {
                        // Ignore
                    }
                    else {
                        node->m_dataSize=1;
                        if (t->getValue(as).toLower()=="integer") node->m_dataSize = 2;
                        if (t->getValue(as).toLower()=="long") node->m_dataSize = 4;
                        QString typeVal = t->getValue(as);

                        if (t->m_flag==1 && (!as->m_symTab->m_records.contains(typeVal)))
                            typeVal="const";

                        as->DeclareVariable(v->value, typeVal, t->initVal,t->m_position);

                        // Increase by data counter IF
                        if (t->m_flag==1)
                            t->initVal = Util::numToHex(Util::NumberFromStringHex(t->initVal)+node->m_dataSize);
                    }


    if (node->m_curMemoryBlock!=nullptr) {
        node->m_curMemoryBlock->m_end+=node->m_dataSize;
    }

}

void AbstractASTDispatcher::IncBin(QSharedPointer<NodeVarDecl> node) {
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);
    QString filename = as->m_projectDir + "/" + t->m_filename.replace("\\","/");
    if (!QFile::exists(filename))
        ErrorHandler::e.Error("Could not locate binary file for inclusion :" +filename, node->m_op.m_lineNumber);

    int size=0;
    QFile f(filename);
    if (f.open(QIODevice::ReadOnly)){
        size = f.size();  //when file does open.
        f.close();
    }


    if (t->m_position=="") {
        as->Label(v->value);
        as->Asm(getIncbin()+" \"" + filename + "\"");
    }
    else {
        //            qDebug() << "bin: "<<getValue(v) << " at " << t->m_position;
        QSharedPointer<Appendix> app = QSharedPointer<Appendix>(new Appendix(t->m_position));

        QSharedPointer<Symbol> typeSymbol = as->m_symTab->Lookup(v->value, node->m_op.m_lineNumber);
        typeSymbol->m_org = Util::C64StringToInt(t->m_position);
        typeSymbol->m_size = size;
        //            qDebug() << "POS: " << typeSymbol->m_org;
        app->Append("org " +t->m_position,1);
        app->Append(v->value,0);
        app->Append(getIncbin()+" \"" + filename + "\"",1);
        as->m_appendix.append(app);
        bool ok;
        int start=0;
        if (t->m_position.startsWith("$")) {
            start = t->m_position.remove("$").toInt(&ok, 16);
        }
        else start = t->m_position.toInt();

        as->blocks.append(QSharedPointer<MemoryBlock>(new MemoryBlock(start,start+size, MemoryBlock::DATA,t->m_filename)));

    }
}

void AbstractASTDispatcher::dispatch(QSharedPointer<NodeAsm> node)
{
    node->DispatchConstructor(as);

    QStringList txt = node->m_asm.split("\n");
    as->Comment("****** Inline assembler section");
    for (QString t: txt) {
        as->Write(t,0);
    }
    as->Asm("");
}

void AbstractASTDispatcher::dispatch(QSharedPointer<NodeCompound> node)
{
    node->DispatchConstructor(as);

    as->BeginBlock();
    for (QSharedPointer<Node> n: node->children)
        n->Accept(this);

    as->EndBlock();
}

void AbstractASTDispatcher::HandleNodeAssignCopyRecord(QSharedPointer<NodeAssign> node)
{
    // Both are records of same type. Set up copy.
    QSharedPointer<SymbolTable>  stab = as->m_symTab->m_records[node->m_right->getTypeText(as)];
    as->Comment("Handle assign copy records");
    for (QSharedPointer<Symbol> s: stab->m_symbols) {
        QSharedPointer<NodeVar> l = QSharedPointer<NodeVar>(new NodeVar(Token(TokenType::ID,getValue(node->m_left))));
        l->m_op.m_lineNumber = node->m_op.m_lineNumber;
        l->m_expr = qSharedPointerDynamicCast<NodeVar>(node->m_left)->m_expr;
        QSharedPointer<NodeVar> lp = QSharedPointer<NodeVar>(new NodeVar(Token(TokenType::ID,s->m_name)));
        l->m_subNode = lp;

        QSharedPointer<NodeVar> r = QSharedPointer<NodeVar>(new NodeVar(Token(TokenType::ID,getValue(node->m_right))));
        QSharedPointer<NodeVar> rp = QSharedPointer<NodeVar>(new NodeVar(Token(TokenType::ID,s->m_name)));
        r->m_subNode = rp;
        r->m_op.m_lineNumber = node->m_op.m_lineNumber;
        r->m_expr = qSharedPointerDynamicCast<NodeVar>(node->m_right)->m_expr;

        QSharedPointer<NodeAssign> ns = QSharedPointer<NodeAssign>(new NodeAssign(l,node->m_op, r));
        ns->Accept(this);
        //    Node::s_uniqueSymbols[ns] = ns; // Mark for deletion

    }
}

