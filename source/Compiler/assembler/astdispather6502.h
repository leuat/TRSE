#ifndef ASTDISPATHER6502_H
#define ASTDISPATHER6502_H

#include "abstractastdispatcher.h"
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

    void InitSid(NodeVarDecl* node,QString projectDir);

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


    void Compare(NodeForLoop* node);

    void IncreaseCounter(NodeForLoop* node);

    void LargeLoop(NodeForLoop* node);

    void SmallLoop(NodeForLoop* node);

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


    void LoadVariable(NodeVar* node);

    void StoreVariable(NodeVar* node);

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

    bool IsSimpleIncDec(NodeVar *var, NodeAssign* node);

    QString AssignVariable(NodeAssign* node);




};

#endif // ASTDISPATHER6502_H
