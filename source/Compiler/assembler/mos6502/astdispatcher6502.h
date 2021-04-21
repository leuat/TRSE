#ifndef ASTdispatcher6502_H
#define ASTdispatcher6502_H

#include "source/Compiler/assembler/abstractastdispatcher.h"
#include "source/Compiler/ast/nodebinop.h"
#include "source/Compiler/ast/nodenumber.h"
#include "source/Compiler/ast/nodestring.h"
#include "source/Compiler/ast/nodeunaryop.h"
#include "source/Compiler/ast/nodecompound.h"
#include "source/Compiler/ast/nodevar.h"
#include "source/Compiler/ast/nodeassign.h"
#include "source/Compiler/ast/nodevardecl.h"
#include "source/Compiler/ast/nodeblock.h"
#include "source/Compiler/ast/nodeprogram.h"
#include "source/Compiler/ast/nodevartype.h"
#include "source/Compiler/ast/nodeproceduredecl.h"
#include "source/Compiler/ast/nodeprocedure.h"
#include "source/Compiler/ast/nodeconditional.h"
#include "source/Compiler/ast/nodeforloop.h"
#include "source/Compiler/ast/nodebuiltinmethod.h"
#include "source/Compiler/ast/nodewhileloop.h"
#include "source/Compiler/ast/nodeasm.h"
#include "source/Compiler/ast/nodebinaryclause.h"
#include "source/Compiler/ast/nodecase.h"
#include "source/Compiler/ast/noderepeatuntil.h"
#include "methods6502.h"
#include <QChar>
#include "source/Compiler/assembler/factorymethods.h"

class ASTDispatcher6502 : public AbstractASTDispatcher
{
public:
    ASTDispatcher6502();


    void dispatch(QSharedPointer<NodeBinOP>node) override;
    void dispatch(QSharedPointer<NodeNumber>node) override;
    void dispatch(QSharedPointer<NodeString> node) override;
    void dispatch(QSharedPointer<NodeVarDecl> node) override;
 //   void dispatch(QSharedPointer<NodeBlock> node) override;
    void dispatch(QSharedPointer<NodeVarType> node) override;
    void dispatch(QSharedPointer<NodeBinaryClause> node) override;
  //  void dispatch(QSharedPointer<NodeProcedure> node) override;
//    void dispatch(QSharedPointer<NodeProcedureDecl> node) override;
   // void dispatch(QSharedPointer<NodeConditional> node) override;
   // void dispatch(QSharedPointer<NodeForLoop> node) override;
    void dispatch(QSharedPointer<NodeVar> node) override;
    void dispatch(QSharedPointer<Node> node) override;
    void dispatch(QSharedPointer<NodeAssign> node) override;
    void dispatch(QSharedPointer<NodeRepeatUntil> node) override;
    void dispatch(QSharedPointer<NodeComment> node) override;
    QString resolveTemporaryClassPointer(QString name,int mul,int& res) override;


    bool Evaluate16bitExpr(QSharedPointer<Node> node, QString& lo, QString& hi);


    QString getValue(QSharedPointer<Node> n) override;

    QString getValue8bit(QSharedPointer<Node> n, bool isHi);

    QString getJmp(bool isOffPage) override {
        return "jmp";
    }

    bool UseBlocks() override { return true;}

    void HackPointer(Assembler* as, QSharedPointer<Node> node);




    /*
     *
     * NodeBinOP Specific functions
     *
    */

    void EightBitDiv(QSharedPointer<Node> node);
    void EightBitMul(QSharedPointer<Node> node);
    void Mul16x8(QSharedPointer<Node> node);
    void Div16x8(QSharedPointer<Node> node);
    void RightIsPureNumericMulDiv16bit(QSharedPointer<Node> node);

    void HandleShiftLeftRight(QSharedPointer<NodeBinOP> node);
    void HandleShiftLeftRightInteger(QSharedPointer<NodeBinOP> node, bool isSimpleAeqAopB);

    void HandleGenericBinop16bit(QSharedPointer<Node> node);
    void HandleVarBinopB16bit(QSharedPointer<Node> node);
    bool HandleSingleAddSub(QSharedPointer<Node> node);
    void HandleMulDiv(QSharedPointer<Node> node);
    void HandleRestBinOp(QSharedPointer<Node> node);

    void RightIsPureNumericMulDiv8bit(QSharedPointer<Node> node);

    /*
     *
     * NodeVarDecl
     *
     *
    */

    void Load16bitVariable(QSharedPointer<Node> node, QString reg="y");

    void IncSid(QSharedPointer<NodeVarDecl> node);

//    void IncBin(QSharedPointer<NodeVarDecl> node);

    void PrintCompare(QSharedPointer<Node> node, QString lblSuccess, QString lblFailed);
    void DeclarePointer(QSharedPointer<NodeVarDecl> node);


    void InlineProcedure(QSharedPointer<NodeProcedure> p);

    /*
     *
     * NodeBinaryClause functions
     *
    */
//    void BinaryClause(QSharedPointer<Node> node);

    void BuildToCmp(QSharedPointer<Node> node);

//    void BuildSimple(QSharedPointer<Node> node, QString lblSuccess, QString lblFailed);
    void BuildSimple(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool page) override;

    void BinaryClauseInteger(QSharedPointer<Node> node,QString lblSuccess, QString lblFailed, bool page);




  //  void LogicalClause(QSharedPointer<Node> node);


    /*
     *
     * NodeForLoop
     *
     *
     */


//    void Compare(QSharedPointer<NodeForLoop> node, QSharedPointer<NodeVar> var, bool isLarge, QString loopDone, QString loopNotDone, bool inclusive);

    void IncreaseCounter(QSharedPointer<Node> step, QSharedPointer<NodeVar> var);

    void LargeLoop(QSharedPointer<NodeForLoop> node, QSharedPointer<NodeVar> var, bool inclusive);

    void SmallLoop(QSharedPointer<NodeForLoop> node, QSharedPointer<NodeVar> var, bool inclusive);

    void CompareAndJumpIfNotEqualAndIncrementCounter(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive) override;
    void CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QString lblJump, bool isOffPage) override;

    void Compare(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, bool isLarge, QString loopDone, QString loopNotDone, bool inclusive);

    /*
     *
     * NodeVar
     *
     *
     *
   */
    void LoadPointer(QSharedPointer<NodeVar> node);

    bool LoadXYVarOrNum(QSharedPointer<NodeVar> node, QSharedPointer<Node> other, bool isx);

    void LoadByteArray(QSharedPointer<NodeVar> node);


    void LoadVariable(QSharedPointer<NodeVar> node) override;
    void LoadAddress(QSharedPointer<Node> node) override;
    void LoadVariable(QSharedPointer<Node> node) override;
    void LoadVariable(QSharedPointer<NodeNumber> node) override;
    void LoadVariable(QSharedPointer<NodeProcedure> node) override;

    void StoreVariable(QSharedPointer<NodeVar> node) override;
    void StoreVariableSimplified(QSharedPointer<NodeVar> node, QSharedPointer<Node> expr);

    /*
     *
     * NodeAssign
     *
     *
     *
    */

    void AssignString(QSharedPointer<NodeAssign>node, bool isPointer);

    void AssignPointer(QSharedPointer<NodeAssign>node);

    bool isSimpleAeqAOpB(QSharedPointer<NodeVar> var, QSharedPointer<NodeAssign>node);
    bool isSimpleAeqAOpB16Bit(QSharedPointer<NodeVar> var, QSharedPointer<NodeAssign>node);

    bool IsSimpleIncDec(QSharedPointer<NodeVar> var, QSharedPointer<NodeAssign>node);

    bool IsSimpleAndOr(QSharedPointer<NodeBinaryClause> node, QString labelSuccess, QString labelFail);
    bool IsSimpleAssignPointerExpression(QSharedPointer<NodeVar> var, QSharedPointer<NodeAssign>node);
    void AssignVariable(QSharedPointer<NodeAssign>node);

//    void HandleNodeAssignCopyRecord(QSharedPointer<NodeAssign>node);


    void OptimizeBinaryClause(QSharedPointer<Node> node,Assembler* as) override;



    QString getReturn() override { return "rts";}
    QString getReturnInterrupt() override { return "rti";}


    void dispatch(QSharedPointer<NodeUnaryOp> node) override;

};

#endif // ASTdispatcher6502_H
