#include "abstractcodegen.h"
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
#include "source/Compiler/ast/nodecast.h"
#include "source/Compiler/ast/nodecompound.h"
#include "source/Compiler/ast/nodebuiltinmethod.h"
#include "source/Compiler/ast/nodeunaryop.h"
#include "source/Compiler/ast/nodecontrolstatement.h"
#include "source/Compiler/ast/nodefactory.h"
#include "source/Compiler/codegen/methods/factorymethods.h"
#include "source/Compiler/codegen/methods/abstractmethods.h"

#include "source/LeLib/util/fc8/FC8Compression.h"
#include "source/Compiler/systems/systemcustom.h"

AbstractCodeGen::AbstractCodeGen()
{

}


void AbstractCodeGen::UpdateDispatchCounter()
{
    int nc = Node::s_nodeCount;
    if (nc==0) nc++;
    int p = std::min((int)((100*m_currentNode)/nc),100);

    if (m_currentPercent!=p) {
        m_currentPercent = p;
        emit EmitTick("&"+QString::number(p));

    }
    m_currentNode++;
//    qDebug() <<p << m_currentNode << Node::s_nodeCount;

}

void AbstractCodeGen::dispatch(QSharedPointer<NodeAssign> node)  {
    node->DispatchConstructor(as,this);
    AssignVariable(node);
}

void AbstractCodeGen::dispatch(QSharedPointer<NodeCast> node)
{
    node->DispatchConstructor(as,this);
/*    if (node->m_op.m_type==TokenType::INTEGER)
        node->m_right->setForceType(TokenType::INTEGER);
    if (node->m_op.m_type==TokenType::BYTE)
       node->m_right->setForceType(TokenType::BYTE);*/
    node->m_right->Accept(this);
//    as->Comment("WriteType : "+TokenType::getType(node->m_right->m_castType));
    Cast(node->m_right->getOrgType(as), node->m_op.m_type, node->m_right->m_castType);

}
/*
 *
 *  BLOCKS (VAR ... BEGIN ...  END)
 *
 * */
void AbstractCodeGen::dispatch(QSharedPointer<NodeBlock> node) {
    /*   if (m_ticks++%8==0)
        emit EmitTick(".");
*/
    node->DispatchConstructor(as,this);
    as->ClearTerm();
    LineNumber(node->m_op.m_lineNumber);
 //   AbstractCodeGen::dispatch(node);


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
            if (node->m_isMainBlock && !as->m_ignoreInitialJump) {
                as->Asm(getJmp(true)+" "+as->jumpLabel(label));
            }

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

    as->Asm(node->m_initCode);

    if (node->m_isMainBlock)
        as->Label("main_block_begin_");
    if (node->m_compoundStatement!=nullptr)
        node->m_compoundStatement->Accept(this);
    if (node->m_isMainBlock)
        as->Label("main_block_end_");

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

void AbstractCodeGen::dispatch(QSharedPointer<NodeForLoop> node)
{
    node->DispatchConstructor(as,this);

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

//    qDebug() <<(Syntax::s.m_currentSystem->m_processor==AbstractSystem::MOS6502);
  //  qDebug() <<nVar->isWord(as) << nVar->getValue(as);
    if (Syntax::s.m_currentSystem->m_processor==AbstractSystem::MOS6502 && nVar->m_left->isWord(as)) {
        ErrorHandler::e.Warning("Using integer '"+nVar->m_left->getValue(as)+"' as a for loop index can result in unpredictable behavior on the 6502. Please keep to using byte indicies, and use pointers to cover data > 255 bytes. See the TRSE tutorials for examples.", node->m_op.m_lineNumber);
    }

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
    CompareAndJumpIfNotEqualAndIncrementCounter(node->m_a, node->m_b,  node->m_step, lblFor, offpage,node->m_inclusive);

    as->Label(lblForEnd);
    as->Label(lblLoopEnd);
    as->PopLabel("forloop");
    as->PopLabel("forloopend");
    as->PopLabel("forloopcounter");

    as->PopLabel("loopend");// for BREAK and CONT
    as->PopLabel("loopstart"); // for BREAK and CONT

}

void AbstractCodeGen::dispatch(QSharedPointer<NodeControlStatement> node)
{
/*    QString labelStartOverAgain = as->NewLabel("while");
    QString labelElseDone = as->NewLabel("elsedoneblock");
  */
    node->DispatchConstructor(as,this);

    if (node->m_op.m_type == TokenType::BREAK) {

        if (as->m_labelStack["loopend"].m_vars.count()!=0) // for loops
            as->Asm(getJmp(true) + " " + as->jumpLabel(as->getLabel("loopend")));
        else
        ErrorHandler::e.Error("'Break' can only be used within a for / while loop", node->m_op.m_lineNumber);
    }
    if (node->m_op.m_type == TokenType::CONTINUE) {
        if (as->m_labelStack["loopstart"].m_vars.count()!=0)
            as->Asm(getJmp(true) + " " + as->jumpLabel(as->getLabel("loopstart")));
        else
         ErrorHandler::e.Error("'Continue' can only be used within a for / while loop", node->m_op.m_lineNumber);
    }
    if (node->m_op.m_type == TokenType::RETURN)
        as->Asm(getReturn());
}

void AbstractCodeGen::Cast(QString from, QString to)
{
    TokenType::Type f,t;
    from = from.toLower();
    to = to.toLower();
    if (from=="integer") f=TokenType::INTEGER;
    if (from=="byte") f=TokenType::BYTE;
    if (from=="long") f=TokenType::LONG;
    if (to=="integer") t=TokenType::INTEGER;
    if (to=="byte") t=TokenType::BYTE;
    if (to=="long") t=TokenType::LONG;
    Cast(f,t);
}

QString AbstractCodeGen::getBank(QSharedPointer<NodeVarType> t) {
    QString bnk = t->m_flags[t->m_flags.indexOf("bank")+1];//Banks always placed +1
    if (!as->m_banks.contains(bnk)) {
        as->m_banks[bnk] = QSharedPointer<Appendix>(new Appendix());
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::MEGA65)
            as->m_banks[bnk]->m_pos = "$0000";
        else
            as->m_banks[bnk]->m_pos = "$4000";
        as->m_banks[bnk]->m_isMainBlock = true;
    }
    return bnk;
}


/*
 *
 * Performs generic validation of assign stataement
 *
 */
void AbstractCodeGen::ValidateAssignStatement(QSharedPointer<NodeAssign> node)
{
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(node->m_left);

    if (v==nullptr && num == nullptr)
        ErrorHandler::e.Error("Left value not variable or memory address! ", node->m_op.m_lineNumber);
    if (num!=nullptr && num->getType(as)!=TokenType::ADDRESS)
        ErrorHandler::e.Error("Left value must be either variable or memory address, not a constant.", node->m_op.m_lineNumber);

    // Verify pointer integrity
    if (node->m_left->getType(as)==TokenType::POINTER && node->m_right->isRecord(as) && !node->m_right->isClass(as)) {
        if (!node->m_right->isRecordData(as)) {
               ErrorHandler::e.Error("Cannot assign a pointer data to a record.", node->m_op.m_lineNumber);

            as->Comment("Assigning pointer to record/class");
        }

    }
    if (node->m_right->isRecord(as) && (!node->m_right->isRecordData(as)) && !node->m_right->isClass(as)) {
//        if (!Syntax::s.m_currentSystem->m_allowRecordPointers)
            ErrorHandler::e.Error("Cannot assign a record of type '"+node->m_right->getTypeText(as)+"' to a single variable. ",node->m_op.m_lineNumber);
    }

    // Double check
    if (node->m_right==nullptr)
         ErrorHandler::e.Error("Right hand of assign statement must be expression.", node->m_op.m_lineNumber);

    // Ignore this one, as it is used a lot
/*    if (v->getTypeText(as).toLower()=="array" && v->m_expr==nullptr) {

    }
  */
}

/*
 *
 * Default generic assign statement (default for the c64)
 *
 */

void AbstractCodeGen::GenericAssign(QSharedPointer<NodeAssign> node) {
    node->m_right->Accept(this);
    as->Term();
    as->Comment("Calling storevariable on generic assign expression");
//    StoreVariable(VarOrNum(node->m_left));
  //  as->Comment("RHS is byte: "+Util::numToHex(node->m_right->isByte(as)) + " "+node->m_right->getTypeText(as) + " " +TokenType::getType(node->m_right->getType(as)));
    //as->Comment("LHS is byte: "+Util::numToHex(node->m_left->isByte(as)) + " "+node->m_left->getTypeText(as) + " " +TokenType::getType(node->m_left->getType(as)));
//    as->Comment("CastType: "+TokenType::getType(n));

    if (node->m_right->getTypeText(as)=="BYTE")
        Cast(TokenType::BYTE,node->m_right->m_castType);
    StoreVariable(VarOrNum(node->m_left));
}

void AbstractCodeGen::IncreaseCounter(QSharedPointer<Node> step, QSharedPointer<Node> var) {

    // no STEP included in FOR TO DO, we assume STEP 1

    if (step==nullptr) step = NodeFactory::CreateNumber(var->m_op,1);
    auto bop = NodeFactory::CreateBinop(var->m_op,TokenType::PLUS,var,step);
    auto assign = NodeFactory::CreateAssign(var->m_op,var,bop);
    assign->Accept(this);

}

void AbstractCodeGen::SmallLoop(QSharedPointer<NodeForLoop> node, QSharedPointer<NodeVar> var, bool inclusive)
{
    QString loopDone = as->NewLabel("forLoopDone");
    //  Compare(as);
    //  as->Asm("beq "+loopDone);

    node->m_block->Accept(this);
    as->m_stack["for"].pop();
    IncreaseCounter(node->m_step,var);

    as->Label(loopDone);

    as->m_labelStack["for"].pop();
    as->m_labelStack["forLoopDone"].pop();

}

void AbstractCodeGen::LargeLoop(QSharedPointer<NodeForLoop> node, QSharedPointer<NodeVar> var, bool inclusive)
{
    QString loopForFix = as->NewLabel("forLoopFix");
    QString loopDone = as->NewLabel("forLoopDone");
    QString loopNotDone = as->NewLabel("forLoopNotDone");

    as->Label(loopForFix);
    node->m_block->Accept(this);
    as->m_stack["for"].pop();

    IncreaseCounter(node->m_step,var);
    //    Compare(node, var, true, loopDone, loopNotDone, inclusive);

    as->Label(loopNotDone);
    as->Asm(getJmp(true) + " " + as->jumpLabel(as->getLabel("for")));

    as->Label(loopDone);

    as->m_labelStack["for"].pop();
    as->m_labelStack["forLoopFix"].pop();
    as->m_labelStack["forLoopDone"].pop();
    as->m_labelStack["forLoopNotDone"].pop();

}
/*
 *
 * Used in the beginning of node assignment: if the variable
 * assigned to is a *number* (memory address), simply replace
 * name of the variable with the number value. Used in
 * screen_bg_col:=red etc where "screen_bg_col" is a memory address (const)
 *
 */
QSharedPointer<NodeVar> AbstractCodeGen::VarOrNum(QSharedPointer<Node> node)
{
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node);
    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(node);

    if (num!=nullptr) {
        as->Comment("Assigning memory location");
        v = QSharedPointer<NodeVar>(new NodeVar(num->m_op)); // Create a variable copy
        v->value = num->HexValue();
    }
    return v;
}
/*
 *
 * How to handle assignments between pure objects, ie
 * Monster = record
 * ...
 * end;
 * a,b : Monster;
 *
 * a:=b;
 *
 * NB Classes not supported yet
 *
 */
void AbstractCodeGen::AssignPureRecords(QSharedPointer<NodeAssign> node)
{
    auto v = node->m_left;
    if (!node->m_right->isRecord(as))
        ErrorHandler::e.Error("Right-hand side of assignment must also be record of type '"+v->getTypeText(as)+"'", v->m_op.m_lineNumber);
    if (v->getTypeText(as)!=node->m_right->getTypeText(as))
        ErrorHandler::e.Error("Right-hand side of assignment must also be of type '"+v->getTypeText(as)+"'", v->m_op.m_lineNumber);
    if (node->m_right->isRecordData(as))
        ErrorHandler::e.Error("Right-hand side of assignment must also be of type '"+v->getTypeText(as)+"'", v->m_op.m_lineNumber);

    // Copy record:
    if (!v->isClass(as))
        HandleNodeAssignCopyRecord(node);
    else
        HandleNodeAssignCopyClass(node);

}

/*
 *
 * One of the main hearts of TRSE: the abstract AssignVariable statement
 * generator. Current users: 6502 and Z80. Be sure to implement the rest soon!
 *
 * Handles everything of tye type A := B; in TRSE. Which is.. 96% of the language.
 *
 */

void AbstractCodeGen::AssignVariable(QSharedPointer<NodeAssign> node)
{
    // Begin by validating all nodes
    ValidateAssignStatement(node);
    as->ClearTerm();
    auto v = VarOrNum(node->m_left);
    node->m_left = v;
    bool ignoreLookup = false;

/*    auto пиздец = node->m_right;
    пиздец->Accept(this);
*/
    // Get variable name
    QString vname = getValue(v);
    // Make sure write type for classes are the same
    if (v->m_writeType==TokenType::NADA)
        v->m_writeType = node->m_right->getWriteType();



//    qDebug() <<v->value<<TokenType::getType(v->m_writeType) <<TokenType::getType(node->m_right->getWriteType());

/*    if (v->m_writeType==TokenType::INTEGER) {
        v->setForceType(TokenType::INTEGER);
        node->m_right->setForceType(TokenType::INTEGER);
    }
*/
    // Set force type for functions
    if (v->isByte(as))
        node->m_right->setCastType(TokenType::BYTE);
    if (v->isWord(as))
        node->m_right->setCastType(TokenType::INTEGER);
    if (v->isLong(as))
        node->m_right->setCastType(TokenType::LONG);


    // ****** REGISTERS TO
    if (v->m_isRegister) {
        as->Comment("Assigning to register");
        AssignToRegister(node);
        return;
    }

    // ****** REGISTERS FROM
    if (node->m_right->m_isRegister) {
        as->Comment("Assigning from register");
        AssignFromRegister(node);
        return;
    }

    // Ignore silly stuff such as a:=a; and zp:=zp;
    if (node->m_right->isPureVariable()) {
        if (vname == node->m_right->getValue(as))
            return;
    }


    // ****** RECORDS AND CLASSES DIRECT ASSIGNMENT
    if (v->isRecord(as) && !v->isRecordData(as) && !v->isClass(as)) {
        as->Comment("Assigning pure records/classes");
        AssignPureRecords(node);
        return;
    }
    // ****** STRINGS
    if (qSharedPointerDynamicCast<NodeString>(node->m_right) && !v->hasArrayIndex()) {
        as->Comment("Assigning a string : " + getValue(v));
        AssignString(node);
        return;
    }
    // ** Override
    // someInt := b[c];
    if (IsSimpleAssignInteger(node))
        return;


    // ****** Pointer handling
    if (AssignPointer(node)) {
        return;
    }
    // Lookup for a type check
    if (!ignoreLookup)
       QSharedPointer<Symbol> s = as->m_symTab->Lookup(getValue(v), node->m_op.m_lineNumber, v->isAddress());


    if (node->m_left->isWord(as)) {
//        as->Asm("ldy #0");    // AH:20190722: Does not appear to serve a purpose - takes up space in prg. Breaks TRSE scroll in 4K C64 demo if take this out
        node->m_right->setForceType(TokenType::INTEGER);
    }

    // stack parameters
     if (StoreStackParameter(node))
        return;



    // For constant i:=i+1;
    if (IsSimpleIncDec(node))
        return;

    //a:=5; some simplest case
     if (StoreVariableSimplified(node))
        return;

    // p := #data;
    if (IsSimpleAssignPointer(node))
        return;

    // p[i] := 10;
    if (IsAssignPointerWithIndex(node))
        return;


    // arr[i] := 10;
    if (IsAssignArrayWithIndex(node))
        return;



    GenericAssign(node);

}


/*
QString AbstractCodeGen::getValue(QSharedPointer<Node> n) {
    return n->getValue(as);
}
*/

/*
 *
 *  IF (A AND (B OR C))  ,  WHILE
 *
 * */



void AbstractCodeGen::dispatch(QSharedPointer<NodeConditional> node)
{
    node->DispatchConstructor(as,this);

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
        as->Asm(getJmp(offpage) + " "+as->jumpLabel(lblstartTrueBlock));
        as->Label(localFailed);
        as->PopLabel("localfailed");
        as->Asm(getJmp(offpage) + " " + as->jumpLabel(failedLabel));
    }

   // Start main block
    as->Label(lblstartTrueBlock+": ;Main true block ;keep "); // This means skip inside
    node->m_block->Accept(this);

    if (node->m_elseBlock!=nullptr)
        as->Asm(getJmp(offpage)+" " + as->jumpLabel(labelElseDone));

    // If while loop, return to beginning of conditionals
    if (node->m_isWhileLoop)
        as->Asm(getJmp(offpage)+" " + as->jumpLabel(labelStartOverAgain));

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



void AbstractCodeGen::HandleCompoundBinaryClause(QSharedPointer<Node> node, QString lblFailed, QString lblSuccess, bool offpage)
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

        if (node->m_right->m_isBoolean) {
//            qDebug() << "Right is boolean!" << node->m_right->getValue(as);
            if (node->m_right->getValueAsInt(as)==1) {
                // ooh flip to FALSE and invert
                auto num = qSharedPointerDynamicCast<NodeNumber>(node->m_right);
                if (node->m_op.m_type == TokenType::EQUALS) {
                    num->m_strVal="0";
                    num->m_val = 0;
                    node->m_op.m_type = TokenType::NOTEQUALS;
                    //qDebug() << "Flipping IS TRUE to NOT EQ FALSE";
                }
                else
                if (node->m_op.m_type == TokenType::NOTEQUALS) {
                    num->m_strVal="0";
                    num->m_val = 0;
                    node->m_op.m_type = TokenType::EQUALS;
                    //qDebug() << "Flipping IS NOT TRUE to EQ FALSE";
                }

            }
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
        as->Asm(getJmp(offpage)+" "+as->jumpLabel(lblSuccess));
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


bool AbstractCodeGen::isOffPage(QSharedPointer<Node> node, QSharedPointer<Node> b1, QSharedPointer<Node> b2) {
  //  bool onPage = true;
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


void AbstractCodeGen::dispatch(QSharedPointer<NodeProcedureDecl> node)
{

    node->DispatchConstructor(as,this);
    // Don't x<<<< inline procedures
    if (node->m_isInline) {
        auto decls = qSharedPointerDynamicCast<NodeBlock>(node->m_block)->m_decl;
        // Only declare variables in SYMTAB
        for (QSharedPointer<Node> n: decls) {
            // Print label at end of vardecl
            auto vd = qSharedPointerDynamicCast<NodeVarDecl>(n);
            auto var = qSharedPointerDynamicCast<NodeVar>(vd->m_varNode);
/*            if (!(var->m_isGlobal))
            if (vd!=nullptr) {
                vd->ExecuteSym(as->m_symTab);
            }*/
            n->Accept(this);


        }

        return;
    }


    as->m_symTab->SetCurrentProcedure(node->m_procName+"_");
    //int ln = node->m_currentLineNumber;
//    LineNumber(ln+1);
    bool isInbank = false;
    auto orgBank = as->m_currentBlock;
    if (node->m_flags.contains("bank")) {
        QString bnk = node->m_flags[node->m_flags.indexOf("bank")+1];//Banks always placed +1
        isInbank = true;
        if (!as->m_banks.contains(bnk)) {
            as->m_banks[bnk] = QSharedPointer<Appendix>(new Appendix());
//            as->m_banks[bnk]->m_pos = "$4000";
            as->m_banks[bnk]->m_isMainBlock = false;
        }
        as->m_currentBlock = as->m_banks[bnk];
        m_isFarAway = true;

    }
    else
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
            b->m_initCode = getInitProcedure();
        }
        node->m_block->Accept(this);
    }
    // Return value
    if (node->m_returnValue!=nullptr) {
        if (node->m_returnType->getValue(as).toLower()=="integer") {
            node->m_returnValue->setForceType(TokenType::INTEGER);
        }
        if (node->m_returnType->getValue(as).toLower()=="byte") {
            node->m_returnValue->setForceType(TokenType::BYTE);
        }

        as->ClearTerm();
        node->m_returnValue->Accept(this);
        as->Term();
    }

    if (!isInitFunction) {

        if (node->m_type==0) {
            as->Asm(getReturn());
        }
        else {
 //           as->Asm(endInterrupt());
            as->Asm(getReturnInterrupt());
        }
    }

    if (node->m_curMemoryBlock!=nullptr) {
        node->m_curMemoryBlock->m_end+=10;
    }

    as->m_symTab->ExitProcedureScope(false);
    as->Label("end_procedure_"+node->m_procName);

    if (isInbank) {
        as->m_currentBlock = orgBank;
        m_isFarAway = false;
    }
    //  as->PopCounter(ln);
}


/*
 *
 *  PROCEDURE CALLING
 *
 * */



void AbstractCodeGen::dispatch(QSharedPointer<NodeProcedure> node)
{
    node->DispatchConstructor(as,this);
    if (node->isReference()) {
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

//    as->Comment("calling procedure: "+node->m_procedure->m_procName);

    if (node->m_procedure->m_isInline) {
        InlineProcedure(node);
        return;
    }

    int lostStack = 0;
    for (int i=0; i<node->m_parameters.count();i++) {
        // Assign all variables
//        node->m_parameters[i]->ApplyHack(as);
        QSharedPointer<NodeVarDecl> vd = qSharedPointerDynamicCast<NodeVarDecl>(node->m_procedure->m_paramDecl[i]);
        QSharedPointer<NodeAssign>na = QSharedPointer<NodeAssign>(new NodeAssign(vd->m_varNode, node->m_parameters[i]->m_op, node->m_parameters[i]));
        if (vd->m_varNode->isStackVariable()) {
            lostStack++;
            if (vd->isWord(as))
                lostStack++;
            na->m_isProcedureParameterAssign = true;
        }
        na->Accept(this);
    }

    ProcedureStart(as);
//    if (node->m_procedure->m_returnType!=nullptr)
  //      as->Comment("Return type: "+node->m_procedure->m_returnType->getValue(as) +" with forcetype " +TokenType::getType(node->m_forceType)) ;
    as->Asm(getCallSubroutine() + " " + as->jumpLabel(node->m_procedure->m_procName));

    if (node->m_procedure->m_returnType!=nullptr)
        if (node->m_procedure->m_returnType->m_op.m_type!=node->m_castType) {
            Cast(node->m_procedure->m_returnType->m_op.m_type, node->m_castType);
        }
    ProcedureEnd(as);
    PopLostStack(lostStack);
}
/*
 *
 *  INLINE PROCEDURE CALLING (AND DECLARATION)
 *
 * */


void AbstractCodeGen::InlineProcedure(QSharedPointer<NodeProcedure> p)
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


void AbstractCodeGen::dispatch(QSharedPointer<NodeProgram> node)
{
    node->DispatchConstructor(as,this);

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



void AbstractCodeGen::dispatch(QSharedPointer<NodeVarDecl> node)
{
    node->DispatchConstructor(as,this);


    LineNumber(node->m_op.m_lineNumber);
    if (UseBlocks()) {
        int ret = node->MaintainBlocks(as);

        if (ret==3) node->m_curMemoryBlock = nullptr;
        if (node->m_curMemoryBlock!=nullptr)
        {
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
    }
    auto keepBlock = as->m_currentBlock;
    if (Syntax::s.m_currentSystem->m_processor==AbstractSystem::ARM)
        as->m_currentBlock = as->m_tempVarsBlock;

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
        if (t->m_flags.contains("compressed")) {
            QString s ="";
            t->m_data = Syntax::s.m_currentSystem->CompressData(t->m_data, s);
        }


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
                node->m_dataSize+=s.length();
            node->m_dataSize++; // 0 end
        }
        else
            if (t->m_op.m_type==TokenType::CSTRING) {
                as->DeclareCString(v->value, t->m_data, t->m_flags);
                node->m_dataSize = 0;
                for (QString s: t->m_data)
                    node->m_dataSize+=s.length();
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
    if (Syntax::s.m_currentSystem->m_processor==AbstractSystem::ARM)
        as->m_currentBlock = keepBlock;

}

/*
 *
 *  INCLUDE BINARY FILE
 *
 * */



void AbstractCodeGen::IncBin(QSharedPointer<NodeVarDecl> node) {
    node->DispatchConstructor(as,this);

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
        as->Asm(getIncbin()+"\t \"" + filename + "\"");
        as->Label("end_incbin_"+v->value);

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
        if (Syntax::s.m_currentSystem->CL65Syntax()) {
            app->Append(".res "+t->m_position+"-*",0);

        }
        else
            app->Append(as->GetOrg() +t->m_position,1);

        app->Append(v->value+":",0);
        app->Append(getIncbin()+"\t \"" + filename + "\"",1);
        as->m_appendix.append(app);
        bool ok;
        int start=0;
        if (t->m_position.startsWith("$")) {
            start = t->m_position.remove("$").toInt(&ok, 16);
        }
        else start = t->m_position.toInt();

        app->Append("end_incbin_"+v->value+":",0);

        as->blocks.append(QSharedPointer<MemoryBlock>(new MemoryBlock(start,start+size, MemoryBlock::DATA,filename)));
//        app->Append("EndBlock"+t->m_position+":",0);
    }
}

/*
 *
 *  INLINE ASSEMBLER
 *
 * */


void AbstractCodeGen::dispatch(QSharedPointer<NodeAsm> node)
{
    node->DispatchConstructor(as,this);

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


void AbstractCodeGen::dispatch(QSharedPointer<NodeCompound> node)
{
    node->DispatchConstructor(as,this);
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


void AbstractCodeGen::HandleNodeAssignCopyRecord(QSharedPointer<NodeAssign> node)
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

void AbstractCodeGen::HandleNodeAssignCopyClass(QSharedPointer<NodeAssign> node)
{
    // Both are records of same type. Set up copy.
    QSharedPointer<SymbolTable>  stab = as->m_symTab->m_records[node->m_right->getTypeText(as)];
    as->Comment("Handle assign copy records");
    // NOT IMPLEMENTED, SEE PARSER FOR COPYING
 /*   for (QSharedPointer<Symbol> s: stab->m_symbols) {

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

    }*/

}

/*
 *
 *  BUILT-IN METHODS
 *
 * */



void AbstractCodeGen::dispatch(QSharedPointer<NodeBuiltinMethod> node)
{
    node->DispatchConstructor(as,this);

/*    if (m_inlineParameters.count()!=0) {
        // We have INLINE parameters! replace
        for (auto p : node->m_params) {
            p->ReplaceInline(as, m_inlineParameters);

        }
    }
*/
    if (m_inlineParameters.count()!=0)
        node->ReplaceInline(as, m_inlineParameters);



//    for (auto m:node->m_params)
  //      qDebug() << m->HexValue();

    node->VerifyParams(as);
    auto system = Syntax::s.m_currentSystem->m_system;
    if (qSharedPointerDynamicCast<SystemCustom>(Syntax::s.m_currentSystem) !=nullptr) {
        system = AbstractSystem::SystemFromString(Syntax::s.m_currentSystem->m_alternateMethods);
    }
    QSharedPointer<AbstractMethods> methods = FactoryMethods::CreateMethods(system);
    methods->m_node = node;
//    qDebug() << "BALLE"<<methods << Syntax::s.m_currentSystem->m_system << qSharedPointerDynamicCast<Methods6502>(methods);
    methods->Assemble(as,this);
}

void AbstractCodeGen::dispatch(QSharedPointer<NodeUnaryOp> node)
{
    node->DispatchConstructor(as,this);

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

void AbstractCodeGen::LineNumber(int ln) {
    if (m_outputLineNumbers)
        as->Comment("LineNumber: "+QString::number(ln));
    if (m_outputSource)
        if (ln<m_rasSource.count())
           as->Comment(m_rasSource[ln]);

}

bool AbstractCodeGen::Evaluate16bitExpr(QSharedPointer<Node> node, QString &lo, QString &hi)
{
    if (node->isPure()) {
        lo = getValue8bit(node,false);
        hi = getValue8bit(node,true);
        return true;
    }
    node->Accept(this);
    QString lbl = as->StoreInTempVar("rightvarInteger", "word");
    lo = lbl;
    hi = lbl+"+1";
    return false;
}

QString AbstractCodeGen::getValue(QSharedPointer<Node> n) {



    if (m_inlineParameters.contains(n->getValue(as)))
        return m_inlineParameters[n->getValue(as)]->getValue(as);

    //    HackPointer(as,n);

    return n->getValue(as);
}

QString AbstractCodeGen::getValue8bit(QSharedPointer<Node> n, int isHi) {

    if (m_inlineParameters.contains(n->getValue(as)))
        return m_inlineParameters[n->getValue(as)]->getValue8bit(as,isHi);

    return n->getValue8bit(as,isHi);
}


void AbstractCodeGen::dispatch(QSharedPointer<NodeCase> node)
{
    node->DispatchConstructor(as,this);
    bool hasElse = node->m_elseBlock!=nullptr;
    QString labelEnd = as->NewLabel("caseend");
    // Loop through all the conditionals in the case statement

    auto expr = node->m_variable;

    if (!expr->isPureVariable()) {
        // Uh oh, we need to store in a temp one
//        if (Syntax::s.m_currentSystem->m_processor!=AbstractSystem::MOS6502)
  //          ErrorHandler::e.Error("Cases can only operate on pure variables (for now). Not implemented on the z80 / m68k / x86 yet, please nag the developer!",node->m_op.m_lineNumber);

/*        QString name = as->m_internalZP[as->m_internalZP.count()-1];
        as->Comment("expr is not pure, need to save to temp var");
        as->ClearTerm();
        expr->Accept(this);
        as->Term();
        as->Asm("sta "+name);
        if (expr->isWord(as))
            as->Asm("sty "+name+"+1");

        expr = NodeFactory::CreateVariable(expr->m_op,name);
*/
        QString type = "byte";
        if (expr->isWord(as))
            type="integer";
        QString name = as->StoreInTempVar("case",type,false);

//        qDebug() << name;

        auto sym = QSharedPointer<Symbol>(new Symbol(name,type));
        as->m_symTab->m_ignoreAllprefixes = true;
        as->m_symTab->Define(sym);

        as->Comment("expr is not pure, need to save to temp var");
        auto right = expr;

        expr = NodeFactory::CreateVariable(expr->m_op,name);
        auto assign = NodeFactory::CreateAssign(expr->m_op,expr,right);

        AssignVariable(assign);
        as->m_symTab->m_ignoreAllprefixes = false;

    }


    for (int i=0;i<node->m_conditionals.count();i++) {
        QString labelNext = as->NewLabel("casenext");
        as->PopLabel("casenext");
        // perform the actual CPU-dependent comparison of the two numbers
        CompareAndJumpIfNotEqual(expr, node->m_conditionals[i], labelNext,false);
        // Print the current statement block
        node->m_statements[i]->Accept(this);
        // Jump to the end, done with case
        if (i!=node->m_conditionals.count()-1 || hasElse)
            as->Asm(getJmp(true)+" "+as->jumpLabel(labelEnd));

        as->Label(labelNext);
    }
    // Print else blockl
    if (hasElse)
        node->m_elseBlock->Accept(this);

    as->Label(labelEnd);

    as->PopLabel("caseend");
}

