#ifndef ASTDISPATHER6502_H
#define ASTDISPATHER6502_H

#include "abstractastdispatcher.h"
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

class ASTDispather6502 : public AbstractASTDispatcher
{
public:
    ASTDispather6502();


    void dispatch(NodeBinOP *node) override;
    void dispatch(NodeNumber *node) override;
    void dispatch(NodeAsm *node) override;
    void dispatch(NodeString *node) override;
    void dispatch(NodeUnaryOp *node) override;
    void dispatch(NodeCompound *node) override;
    void dispatch(NodeVarDecl *node) override;
    void dispatch(NodeBlock *node) override;
    void dispatch(NodeProgram *node) override;
    void dispatch(NodeVarType *node) override;
    void dispatch(NodeBinaryClause *node) override;
    void dispatch(NodeProcedure *node) override;
    void dispatch(NodeProcedureDecl *node) override;
    void dispatch(NodeConditional *node) override;
    void dispatch(NodeForLoop *node) override;
    void dispatch(NodeVar *node) override;
    void dispatch(Node *node) override;
    void dispatch(NodeAssign *node) override;
    void dispatch(NodeCase *node) override;
    void dispatch(NodeRepeatUntil *node) override;


    void dispatch(NodeBuiltinMethod *node);



    /*
     *
     * NodeBinOP Specific functions
     *
    */

    void EightBitDiv(Node* node);
    void EightBitMul(Node* node);
    void Mul16x8(Node* node);
    void Div16x8(Node* node);
    void RightIsPureNumericMulDiv16bit(Node* node);

    void HandleShiftLeftRight(NodeBinOP* node);
    void HandleShiftLeftRightInteger(NodeBinOP* node, bool isSimpleAeqAopB);

    void HandleGenericBinop16bit(Node* node);
    void HandleVarBinopB16bit(Node* node);
    bool HandleSingleAddSub(Node* node);
    void HandleMulDiv(Node* node);
    void HandleRestBinOp(Node* node);

    void RightIsPureNumericMulDiv8bit(Node* node);

    /*
     *
     * NodeVarDecl
     *
     *
    */

    void IncSid(NodeVarDecl* node);

    void IncBin(NodeVarDecl* node);

    void PrintCompare(Node* node, QString lblSuccess, QString lblFailed);
    void DeclarePointer(NodeVarDecl* node);



    /*
     *
     * NodeBinaryClause functions
     *
    */
    void BinaryClause(Node* node);

    void BuildToCmp(Node* node);

    void BuildSimple(Node* node, QString lblSuccess, QString lblFailed);

    void BinaryClauseInteger(Node* node);

    void LogicalClause(Node* node);


    /*
     *
     * NodeForLoop
     *
     *
     */


    void Compare(NodeForLoop* node, NodeVar* var, bool isLarge, QString loopDone, QString loopNotDone, bool inclusive);

    void IncreaseCounter(NodeForLoop* node, NodeVar* var);

    void LargeLoop(NodeForLoop* node, NodeVar* var, bool inclusive);

    void SmallLoop(NodeForLoop* node, NodeVar* var, bool inclusive);

    /*
     *
     * NodeVar
     *
     *
     *
   */
    void LoadPointer(NodeVar* node);

    bool LoadXYVarOrNum(NodeVar* node, Node *other, bool isx);

    void LoadByteArray(NodeVar* node);


    void LoadVariable(NodeVar* node) override;
    void LoadAddress(Node* node) override;
    void LoadVariable(Node* node) override;
    void LoadVariable(NodeNumber* node) override;

    void StoreVariable(NodeVar* node) override;

    /*
     *
     * NodeAssign
     *
     *
     *
    */

    void AssignString(NodeAssign* node);

    void AssignPointer(NodeAssign* node);

    bool isSimpleAeqAOpB(NodeVar *var, NodeAssign* node);
    bool isSimpleAeqAOpB16Bit(NodeVar *var, NodeAssign* node);

    bool IsSimpleIncDec(NodeVar *var, NodeAssign* node);

    bool IsSimpleAndOr(NodeBinaryClause* node, QString labelSuccess, QString labelFail);

    QString AssignVariable(NodeAssign* node);




};

#endif // ASTDISPATHER6502_H
