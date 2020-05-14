#include "abstractastdispatcher.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodeconditional.h"
#include "source/Compiler/ast/nodeforloop.h"
#include "source/Compiler/ast/nodeblock.h"

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

