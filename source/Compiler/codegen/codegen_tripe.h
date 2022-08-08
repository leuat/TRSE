#ifndef CodeGenTRIPE_H
#define CodeGenTRIPE_H

#include "source/Compiler/codegen/abstractcodegen.h"
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
#include "source/Compiler/codegen/methods/methods6502.h"
#include "source/Compiler/ast/nodefactory.h"
#include <QChar>
#include "source/Compiler/codegen/methods/factorymethods.h"

class CodeGenTRIPE : public AbstractCodeGen
{
public:
    CodeGenTRIPE();
    int m_stackShift = 0;

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
    void dispatch(QSharedPointer<NodeRepeatUntil> node) override;
    void dispatch(QSharedPointer<NodeComment> node) override;
    void dispatch(QSharedPointer<NodeUnaryOp> node) override;


//    QString  BinopTemp(Assembler*as, QSharedPointer<Node> node);

    QStack<QString> m_curTemp;
//    QMap<QString,QString> m_temporaryVariables;
    QVector<QString> m_temporaryVariables;

    QString resolveTemporaryClassPointer(QString name,int mul,int& res) override;
    int block16bit=0;

    QString getIntType(Assembler* as, QSharedPointer<Node> node);
    QString getTempName(QString t);

    void Doublette(Assembler* as, QSharedPointer<Node> a,QSharedPointer<Node> b, QString cmd);


    QString getJmp(bool isOffPage) override {
        return "jump";
    }

    bool UseBlocks() override { return true;}

    void HackPointer(Assembler* as, QSharedPointer<Node> node);




    /*
     *
     * NodeBinOP Specific functions
     *
    */



    void HandleGenericBinop(QSharedPointer<Node> node);

    /*
     *
     * NodeVarDecl
     *
     *
    */

    void LoadVariable(QSharedPointer<Node> node) override;

    void DeclarePointer(QSharedPointer<NodeVarDecl> node) override;

    void InlineProcedure(QSharedPointer<NodeProcedure> p);

    virtual QString getIncbin() override;

    /*
     *
     * NodeBinaryClause functions
     *
    */
//    void BinaryClause(QSharedPointer<Node> node);

    void BuildToCmp(QSharedPointer<Node> node);

    QString TripeNumber(Assembler* as,QSharedPointer<Node> node);
    QString TripeValue(Assembler* as,QSharedPointer<Node> node);

    void BuildSimple(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool page) override;

//    void BinaryClauseInteger(QSharedPointer<Node> node,QString lblSuccess, QString lblFailed, bool page);

    bool IsSimpleAssignInteger(QSharedPointer<NodeAssign> node) override;





  //  void LogicalClause(QSharedPointer<Node> node);


    /*
     *
     * NodeForLoop
     *
     *
     */


//    void Compare(QSharedPointer<NodeForLoop> node, QSharedPointer<NodeVar> var, bool isLarge, QString loopDone, QString loopNotDone, bool inclusive);


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


    void LoadByteArray(QSharedPointer<NodeVar> node);


    void LoadVariable(QSharedPointer<NodeVar> node) override;
    void LoadVariable(QSharedPointer<NodeNumber> node) override;
    void LoadVariable(QSharedPointer<NodeProcedure> node) override;


    /*
     *
     * NodeAssign
     *
     *
     *
    */
    void StoreVariable(QSharedPointer<NodeVar> node) override;

//    void LoadStackVariable(QSharedPointer<NodeVar> node);

    bool StoreVariableSimplified(QSharedPointer<NodeAssign> assignNode) override;


    bool StoreStackParameter(QSharedPointer<NodeAssign> n) override;

    void AssignString(QSharedPointer<NodeAssign>node) override;

    bool AssignPointer(QSharedPointer<NodeAssign>node) override;



    void OptimizeBinaryClause(QSharedPointer<Node> node,Assembler* as) override;

    virtual void AssignFromRegister(QSharedPointer<NodeAssign> node) override;

    virtual void AssignToRegister(QSharedPointer<NodeAssign> node) override;


    QString getReturn() override;
    QString getReturnInterrupt() override { return "rti";}

    QString getCallSubroutine() override;

    virtual QString ProcedureEndWithoutReturn() override;

    virtual QString getInitProcedure() override;

    void PrintCompare(QSharedPointer<Node> node, QString lblSuccess, QString lblFailed);



    //    void HandleNodeAssignCopyRecord(QSharedPointer<NodeAssign>node);



};

#endif // CodeGen6502_H
