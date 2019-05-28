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
#include "methods6502.h"

class ASTDispather6502 : public AbstractASTDispatcher
{
public:
    ASTDispather6502();


    void dispatch(NodeBinOP *node);
    void dispatch(NodeNumber *node);
    void dispatch(NodeAsm *node);
    void dispatch(NodeString *node);
    void dispatch(NodeUnaryOp *node);
    void dispatch(NodeCompound *node);
    void dispatch(NodeVarDecl *node);
    void dispatch(NodeBlock *node);
    void dispatch(NodeProgram *node);
    void dispatch(NodeVarType *node);
    void dispatch(NodeBinaryClause *node);
    void dispatch(NodeProcedure *node);
    void dispatch(NodeProcedureDecl *node);
    void dispatch(NodeConditional *node);
    void dispatch(NodeForLoop *node);
    void dispatch(NodeVar *node);
    void dispatch(Node *node);
    void dispatch(NodeAssign *node);


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


    void DeclarePointer(NodeVarDecl* node);



    /*
     *
     * NodeBinaryClause functions
     *
    */
    void BinaryClause(Node* node);

    void BuildToCmp(Node* node);

    void BuildSimple(Node* node, QString lblFailed);

    void BinaryClauseInteger(Node* node);

    void LogicalClause(Node* node);


    /*
     *
     * NodeForLoop
     *
     *
     */


    void Compare(NodeForLoop* node, NodeVar* var, bool isLarge, QString);

    void IncreaseCounter(NodeForLoop* node, NodeVar* var);

    void LargeLoop(NodeForLoop* node, NodeVar* var);

    void SmallLoop(NodeForLoop* node, NodeVar* var);

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

    QString AssignVariable(NodeAssign* node);




};

#endif // ASTDISPATHER6502_H
