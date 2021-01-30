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
#include "source/Compiler/ast/nodebuiltinmethod.h"
#include "source/Compiler/ast/nodeunaryop.h"
#include "source/Compiler/ast/nodecontrolstatement.h"
#include "source/Compiler/assembler/abstractmethods.h"
#include "source/Compiler/assembler/factorymethods.h"
#include "source/LeLib/util/fc8/FC8Compression.h"

AbstractASTDispatcher::AbstractASTDispatcher()
{

}
/*
 *
 *  BLOCKS (VAR ... BEGIN ...  END)
 *
 * */
void AbstractASTDispatcher::dispatch(QSharedPointer<NodeBlock> node) {
    if (m_ticks++%8==0)
        emit EmitTick(".");


    node->DispatchConstructor(as);
    LineNumber(node->m_op.m_lineNumber);
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


    node->PopZeroPointers(as);
    as->PopCounter(ln);
}
/*
 *
 *  FOR LOOPS
 *
 * */

void AbstractASTDispatcher::dispatch(QSharedPointer<NodeForLoop> node)
{
    node->DispatchConstructor(as);

    QSharedPointer<NodeAssign> nVar = qSharedPointerDynamicCast<NodeAssign>(node->m_a);
    // Must be a variable
    if (nVar==nullptr)
        ErrorHandler::e.Error("Index must be variable", node->m_op.m_lineNumber);

    // Get name
    if (nVar->m_left->m_isRegister )
        ErrorHandler::e.Error("Index cannot be register", node->m_op.m_lineNumber);

    auto v = qSharedPointerDynamicCast<NodeVar>(nVar->m_left);
    if (v == nullptr )
        ErrorHandler::e.Error("Index cannot be register", node->m_op.m_lineNumber);

     QString var = v->getValue(as);//  m_a->Build(as);
    // Perform assigment
    node->m_a->Accept(this);

    // Define main for label
    QString lblFor =as->NewLabel("forloop");
    QString lblForEnd =as->NewLabel("forloopend");
    QString lblForCounter =as->NewLabel("forloopcounter");

    QString lblLoopStart = as->NewLabel("loopstart");
    QString lblLoopEnd = as->NewLabel("loopend");

    as->Label(lblFor);
    bool offpage = isOffPage(node, node->m_block, nullptr);


    Token t_cond = node->m_op;
 /*
    QSharedPointer<NodeBinaryClause> bc = new QSharedPointer<NodeBinaryClause>(
                new NodeBinaryClause(t_bc,node->m_a,node->m_b))

    QSharedPointer<NodeConditional> cond = QSharedPointer<NodeConditional>(
                new NodeConditional(t_cond,offpage,clause,node->m_block,true,nullptr));
    cond->Accept(this);
   */


    // Maintain b has same type as a
    if (nVar->m_left->isWord(as))
        node->m_b->setForceType(TokenType::INTEGER);

    // Perform block
    node->m_block->Accept(this);


    // Perform counter increase and jimps (individual for each target cpu)
    as->Label(lblForCounter);
    as->Label(lblLoopStart);
    CompareAndJumpIfNotEqual(node->m_a, node->m_b,  node->m_step, lblFor, offpage,node->m_inclusive);

    as->Label(lblForEnd);
    as->Label(lblLoopEnd);
    as->PopLabel("forloop");
    as->PopLabel("forloopend");
    as->PopLabel("forloopcounter");

    as->PopLabel("loopend");// for BREAK and CONT
    as->PopLabel("loopstart"); // for BREAK and CONT

}

void AbstractASTDispatcher::dispatch(QSharedPointer<NodeControlStatement> node)
{
/*    QString labelStartOverAgain = as->NewLabel("while");
    QString labelElseDone = as->NewLabel("elsedoneblock");
  */
    if (node->m_op.m_type == TokenType::BREAK) {

        if (as->m_labelStack["loopend"].m_vars.count()!=0) // for loops
            as->Asm(getJmp(true) + " " + as->getLabel("loopend"));
        else
        ErrorHandler::e.Error("'Break' can only be used within a for / while loop", node->m_op.m_lineNumber);
    }
    if (node->m_op.m_type == TokenType::CONTINUE) {
        if (as->m_labelStack["loopstart"].m_vars.count()!=0)
            as->Asm(getJmp(true) + " " + as->getLabel("loopstart"));
        else
         ErrorHandler::e.Error("'Continue' can only be used within a for / while loop", node->m_op.m_lineNumber);
    }
    if (node->m_op.m_type == TokenType::RETURN)
        as->Asm(getReturn());
}

QString AbstractASTDispatcher::getValue(QSharedPointer<Node> n) {
    return n->getValue(as);
}


/*
 *
 *  IF (A AND (B OR C))  ,  WHILE
 *
 * */



void AbstractASTDispatcher::dispatch(QSharedPointer<NodeConditional> node)
{
    QString labelStartOverAgain = as->NewLabel("while");
    QString lblstartTrueBlock = as->NewLabel("ConditionalTrueBlock");

    QString labelElse = as->NewLabel("elseblock");
    QString labelElseDone = as->NewLabel("elsedoneblock");

    // While loops are identical to "ifs" with a loop

    QString lblLoopStart = "";
    QString lblLoopEnd = "";


    if (node->m_isWhileLoop)  {
        as->Label(labelStartOverAgain);
        lblLoopStart = as->NewLabel("loopstart");
        lblLoopEnd = as->NewLabel("loopend");
        as->Label(lblLoopStart);
    }

    // Test all binary clauses:
    QSharedPointer<NodeBinaryClause> bn = qSharedPointerDynamicCast<NodeBinaryClause>(node->m_binaryClause);

    QString failedLabel = labelElseDone;
    // If else block exist, point fail tot hat one
    if (node->m_elseBlock!=nullptr)
        failedLabel = labelElse;


    bool offpage = isOffPage(node, node->m_block, node->m_elseBlock);


    QString localFailed = failedLabel;

    if (offpage && Syntax::s.m_currentSystem->m_processor!=AbstractSystem::Z80) {
        localFailed = as->NewLabel("localfailed");
    }
    // Main recursive method that tests everything. Evaluates all logical
    // clauses and jumps to the corresponding fail/success label
    HandleCompoundBinaryClause(bn, localFailed, lblstartTrueBlock,offpage);

    // OFFPAGE branching for z80:

    if (offpage && Syntax::s.m_currentSystem->m_processor!=AbstractSystem::Z80) {
        as->Asm(getJmp(offpage) + " "+lblstartTrueBlock);
        as->Label(localFailed);
        as->PopLabel("localfailed");
        as->Asm(getJmp(offpage) + " " + failedLabel);
    }

   // Start main block
    as->Label(lblstartTrueBlock+": ;Main true block ;keep "); // This means skip inside
    node->m_block->Accept(this);

    if (node->m_elseBlock!=nullptr)
        as->Asm(getJmp(offpage)+" " + labelElseDone);

    // If while loop, return to beginning of conditionals
    if (node->m_isWhileLoop)
        as->Asm(getJmp(offpage)+" " + labelStartOverAgain);

    // Else block
    if (node->m_elseBlock!=nullptr) {
        as->Label(labelElse);
        node->m_elseBlock->Accept(this);
        //        m_elseBlock->Build(as);

    }
    as->Term();
//    as->Comment("; ELSEDONE HERE "+labelElseDone);
    as->Label(labelElseDone); // Jump here if not
    if (lblLoopEnd!="")
        as->Label(lblLoopEnd);

    as->PopLabel("while");
    as->PopLabel("ConditionalTrueBlock");
    as->PopLabel("elseblock");
    as->PopLabel("elsedoneblock");

    if (node->m_isWhileLoop)  { // for BREAK and CONT
        as->PopLabel("loopstart");
        as->PopLabel("loopend");
    }

    //    as->PopLabel("conditionalfailed");

}

/*
 *
 *  RECURSIVE METHOD FOR CHECKING BINARY CLAUSES ( A OR (B AND (C OR (D ....
 *  LEAF EVALUATED WITH BUILDSIMPLE
 *
 * */



void AbstractASTDispatcher::HandleCompoundBinaryClause(QSharedPointer<Node> node, QString lblFailed, QString lblSuccess, bool offpage)
{
    //    QSharedPointer<NodeBinaryClause> bc = qSharedPointerDynamicCast<NodeBinaryClause>(node);
    if (!node->isCompoundClause()) { // IS LEAF
        // Flip A and B if B is complex and A is pure
        if (node->m_left->isPure() && !node->m_right->isPure()) {
            node->SwapNodes();
            if (node->m_op.m_type == TokenType::GREATER)
                node->m_op.m_type = TokenType::LESS;
            else
                if (node->m_op.m_type == TokenType::LESS)
                    node->m_op.m_type = TokenType::GREATER;
                else
            if (node->m_op.m_type == TokenType::LESSEQUAL)
                node->m_op.m_type = TokenType::GREATEREQUAL;
            else
            if (node->m_op.m_type == TokenType::GREATEREQUAL)
                node->m_op.m_type = TokenType::LESSEQUAL;
            as->Comment("Swapped comparison expressions");
        }

        OptimizeBinaryClause(node,as);
        BuildSimple(node,  lblSuccess, lblFailed, offpage);
        return;
    }

    if (node->m_op.m_type == TokenType::AND) {
        QString lblLocalSuccess = as->NewLabel("localsuccess");
//        HandleCompoundBinaryClause(node->m_left,  lblFailed, lblSuccess, offpage);
        HandleCompoundBinaryClause(node->m_left,  lblFailed, lblLocalSuccess, offpage);
        as->Label(lblLocalSuccess+": ;keep");
        as->Comment("; logical AND, second requirement");
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

/*
 *
 *  Determines if a node block is >~127 bytes or not (for relative branching) (offpage/onpage)
 *
 * */


bool AbstractASTDispatcher::isOffPage(QSharedPointer<Node> node, QSharedPointer<Node> b1, QSharedPointer<Node> b2) {
    bool onPage = node->verifyBlockBranchSize(as, b1,b2,this);

    if (node->m_forcePage == 1)
        onPage = false;

    if (node->m_forcePage == 2)
        onPage = true;

//    qDebug() << "FORCEPAGE " <<node->m_forcePage;

    return !onPage;
}

/*
 *
 *  PROCEDURE DECLARATION
 *
 * */


void AbstractASTDispatcher::dispatch(QSharedPointer<NodeProcedureDecl> node)
{

    node->DispatchConstructor(as);
    // Don't x<<<< inline procedures

    if (node->m_isInline) {
        // Only declare variables in SYMTAB
        for (QSharedPointer<Node> n: qSharedPointerDynamicCast<NodeBlock>(node->m_block)->m_decl) {
            // Print label at end of vardecl
            auto vd = qSharedPointerDynamicCast<NodeVarDecl>(n);

            if (!(qSharedPointerDynamicCast<NodeVar>(vd->m_varNode)->m_isGlobal))
            if (vd!=nullptr)
                vd->ExecuteSym(as->m_symTab);

        }
        return;
    }

    as->m_symTab->SetCurrentProcedure(node->m_procName+"_");
    int ln = node->m_currentLineNumber;
//    LineNumber(ln+1);

    if (UseBlocks()) {
        as->Comment("NodeProcedureDecl "+ QString::number(node->m_blockInfo.m_blockID));
        int ret = node->MaintainBlocks(as);
        if (ret==3) node->m_curMemoryBlock=nullptr;
        if (as->m_currentBlock!=nullptr) {
            if (node->m_curMemoryBlock==nullptr) {
                bool ok;
                //            qDebug() << "Creating new block procedure for " << m_procName;
                QString p = as->m_currentBlock->m_pos;
                int pos = p.remove("$").toInt(&ok, 16);
//                as->StartMemoryBlock()
                node->m_curMemoryBlock = QSharedPointer<MemoryBlock>(new MemoryBlock(pos,pos,MemoryBlock::CODE, node->m_blockInfo.m_blockName));
                as->userBlocks.append(node->m_curMemoryBlock);
            }
        }
        else {
            //node->m_curMemoryBlock=nullptr;
        }

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

    if (node->m_block!=nullptr) {
        QSharedPointer<NodeBlock> b = qSharedPointerDynamicCast<NodeBlock>(node->m_block);
        if (b!=nullptr) {
            b->forceLabel=node->m_procName;
            b->m_isProcedure = true;
        }
        node->m_block->Accept(this);
    }
    // Return value
    if (node->m_returnValue!=nullptr) {
        if (node->m_returnType->getValue(as).toLower()=="integer")
            node->m_returnType->setForceType(TokenType::INTEGER);
        node->m_returnValue->Accept(this);
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


/*
 *
 *  PROCEDURE CALLING
 *
 * */



void AbstractASTDispatcher::dispatch(QSharedPointer<NodeProcedure> node)
{
    node->DispatchConstructor(as);
    if (node->isReference()) {
//        qDebug() << "IS REFERENCE";
        LoadVariable(node);
        return;
    }

    if (node->m_parameters.count()!=node->m_procedure->m_paramDecl.count())
        ErrorHandler::e.Error("Procedure '" + node->m_procedure->m_procName+"' requires "
                              + QString::number(node->m_procedure->m_paramDecl.count()) +" parameters, not "
                              + QString::number(node->m_parameters.count()) + ".", node->m_op.m_lineNumber);

    // Now, check for pureness
    for (int i=0; i<node->m_parameters.count();i++) {
        QSharedPointer<NodeVarDecl> vd = qSharedPointerDynamicCast<NodeVarDecl>(node->m_procedure->m_paramDecl[i]);
        QSharedPointer<NodeVarType> vt = qSharedPointerDynamicCast<NodeVarType>(vd->m_typeNode);
        if (vt->m_flags.contains("pure"))
            if (!node->m_parameters[i]->isPure())
                ErrorHandler::e.Error("Procedure '"+node->m_procedure->m_procName+"' requires parameter "+QString::number(i) +" to be a pure variable or number", node->m_op.m_lineNumber);
        if (vt->m_flags.contains("pure_number"))
            if (!node->m_parameters[i]->isPureNumeric())
                ErrorHandler::e.Error("Procedure '"+node->m_procedure->m_procName+"' requires parameter "+QString::number(i) +" to be a pure number / constant", node->m_op.m_lineNumber);
        if (vt->m_flags.contains("pure_variable"))
            if (!node->m_parameters[i]->isPureVariable())
                ErrorHandler::e.Error("Procedure '"+node->m_procedure->m_procName+"' requires parameter "+QString::number(i) +" to be a pure variable", node->m_op.m_lineNumber);
//        qDebug() <<"typenode flags" <<node->m_procedure->m_procName<<vt->m_flags << vd->m_varNode->getValue(as);
    }



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
/*
 *
 *  INLINE PROCEDURE CALLING (AND DECLARATION)
 *
 * */


void AbstractASTDispatcher::InlineProcedure(QSharedPointer<NodeProcedure> p)
{
    m_inlineParameters.clear();
    int cur = 0;
    for (auto v : p->m_procedure->m_paramDecl) {
        QSharedPointer<NodeVarDecl> nv = qSharedPointerDynamicCast<NodeVarDecl>(v);
        QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(nv->m_varNode);
        if (!var->m_isRegister)
            m_inlineParameters[var->value] = p->m_parameters[cur];
        else {
            QSharedPointer<NodeAssign>na = QSharedPointer<NodeAssign>(new NodeAssign(nv->m_varNode,
                                                                                     p->m_parameters[cur]->m_op,
                                                                                     p->m_parameters[cur]));
            na->Accept(this);
        }
        cur++;
    }
    qSharedPointerDynamicCast<NodeBlock>(p->m_procedure->m_block)->m_ignoreDeclarations = true;

    p->m_procedure->m_block->Accept(this);
    m_inlineParameters.clear();

}
/*
 *
 *  MAIN PROGRAM NODE
 *
 * */


void AbstractASTDispatcher::dispatch(QSharedPointer<NodeProgram> node)
{
    node->DispatchConstructor(as);

    NodeBuiltinMethod::m_isInitialized.clear();
    as->Program(node->m_name, node->m_param);
    for (QString s: node->m_initJumps)
        as->Asm(s);

    node->m_NodeBlock->m_isMainBlock = true;
    node->m_NodeBlock->Accept(this);

//    as->EndMemoryBlock();
    as->EndProgram();
}


/*
 *
 *  VARIABLE DECLARATION
 *
 * */



void AbstractASTDispatcher::dispatch(QSharedPointer<NodeVarDecl> node)
{
    node->DispatchConstructor(as);


    LineNumber(node->m_op.m_lineNumber);
    if (UseBlocks()) {
        int ret = node->MaintainBlocks(as);

        if (ret==3) node->m_curMemoryBlock = nullptr;
        if (node->m_curMemoryBlock!=nullptr)
            if (as->m_currentBlock!=nullptr) {
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
    }

    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);

    // Don't declare va
    if (v->m_isGlobal)
        return;


    QString keep = v->value;
    if (!v->m_isRegister)
        v->value = as->m_symTab->getCurrentProcedure()+v->value;

    node->ExecuteSym(as->m_symTab);

    if (t->m_op.m_type==TokenType::ARRAY) {
        as->DeclareArray(v->value, t->m_arrayVarType.m_value, t->m_op.m_intVal, t->m_data, t->m_position);
        node->m_dataSize=t->m_op.m_intVal;
        QSharedPointer<Symbol> s = as->m_symTab->Lookup(v->value, node->m_op.m_lineNumber);
        s->isUsed=false;
        s->m_arrayTypeText = s->m_type;
        s->m_type="address";
        s->m_arrayType = t->m_arrayVarType.m_type;
    }else
        if (t->m_op.m_type==TokenType::STRING) {
            as->DeclareString(v->value, t->m_data, t->m_flags);
            node->m_dataSize = 0;
            for (QString s: t->m_data)
                node->m_dataSize+=s.count();
            node->m_dataSize++; // 0 end
        }
        else
            if (t->m_op.m_type==TokenType::CSTRING) {
                as->DeclareCString(v->value, t->m_data, t->m_flags);
                node->m_dataSize = 0;
                for (QString s: t->m_data)
                    node->m_dataSize+=s.count();
                node->m_dataSize++; // 0 end
            }
            else
                if (t->m_op.m_type==TokenType::INCBIN) {

                    IncBin(node);
                }
                else
                    if (t->m_op.m_type==TokenType::POINTER) {
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

                        if (t->m_flag==1)
                            t->initVal = Util::numToHex(Util::NumberFromStringHex(t->initVal)+node->m_dataSize);
                    }


    if (node->m_curMemoryBlock!=nullptr) {
        node->m_curMemoryBlock->m_end+=node->m_dataSize;
    }

}

/*
 *
 *  INCLUDE BINARY FILE
 *
 * */



void AbstractASTDispatcher::IncBin(QSharedPointer<NodeVarDecl> node) {
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);
    QString filename = as->m_projectDir + "/" + t->m_filename.replace("\\","/");
    if (!QFile::exists(filename)) {

        filename = Util::GetSystemPrefix()+ Data::data.unitPath + QDir::separator()+ AbstractSystem::StringFromSystem(Syntax::s.m_currentSystem->m_system)+QDir::separator()+  t->m_filename.replace("\\","/");        //Test in Units name
        if (!QFile::exists(filename))
            ErrorHandler::e.Error("Could not locate binary file for inclusion :" +t->m_filename, node->m_op.m_lineNumber);

    }

    int size=0;
    QFile f(filename);
    if (f.open(QIODevice::ReadOnly)){
        size = f.size();  //when file does open.
        f.close();
    }

    if (t->m_flags.contains("compressed"))
        filename = Syntax::s.m_currentSystem->CompressFile(filename);



    if (t->m_position=="") {
        as->Label(v->value);
        as->Asm(getIncbin()+" \"" + filename + "\"");
        if (as->m_currentBlock!=nullptr)
            if (as->m_mainBlock != as->m_currentBlock)
                as->m_currentBlock->m_incDataSize+=QFileInfo(filename).size();
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
        as->blocks.append(QSharedPointer<MemoryBlock>(new MemoryBlock(start,start+size, MemoryBlock::DATA,filename)));
        app->Append("EndBlock"+t->m_position,0);
    }
}

/*
 *
 *  INLINE ASSEMBLER
 *
 * */


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

/*
 *
 *  COMPOUND STATEMENTS (several lines of sentences)
 *
 * */


void AbstractASTDispatcher::dispatch(QSharedPointer<NodeCompound> node)
{
    node->DispatchConstructor(as);

    as->BeginBlock();
    for (QSharedPointer<Node> n: node->children) {
        LineNumber(n->m_op.m_lineNumber+1);
        n->Accept(this);
    }

    as->EndBlock();
}
/*
 *
 *  RECROD COPYING myMonster:=someMonster;
 *
 * */


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

/*
 *
 *  BUILT-IN METHODS
 *
 * */



void AbstractASTDispatcher::dispatch(QSharedPointer<NodeBuiltinMethod> node)
{
    node->DispatchConstructor(as);

/*    if (m_inlineParameters.count()!=0) {
        // We have INLINE parameters! replace
        for (auto p : node->m_params) {
            p->ReplaceInline(as, m_inlineParameters);

        }
    }
*/
    if (m_inlineParameters.count()!=0)
        node->ReplaceInline(as, m_inlineParameters);

    node->VerifyParams(as);

    QSharedPointer<AbstractMethods> methods = FactoryMethods::CreateMethods(Syntax::s.m_currentSystem->m_system);
    methods->m_node = node;
//    qDebug() << "BALLE"<<methods << Syntax::s.m_currentSystem->m_system << qSharedPointerDynamicCast<Methods6502>(methods);
    methods->Assemble(as,this);
}

void AbstractASTDispatcher::dispatch(QSharedPointer<NodeUnaryOp> node)
{
    node->DispatchConstructor(as);

//    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(node->m_right);

    if (node->m_right->isPureNumeric()) {
        int s = node->getValueAsInt(as);
        bool isWord = node->m_forceType==TokenType::INTEGER;
        QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(node->m_right);

        if (node->m_op.m_type==TokenType::MINUS) {
            if (!isWord)
                num->m_val=256-num->m_val;
            else
                num->m_val=65536-num->m_val;

        }

        num->Accept(this);
        num->m_val = s;
    }


}

void AbstractASTDispatcher::LineNumber(int ln) {
    if (m_outputLineNumbers)
        as->Comment("LineNumber: "+QString::number(ln));

}

