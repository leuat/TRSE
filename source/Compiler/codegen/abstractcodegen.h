#ifndef AbstractCodeGen_H
#define AbstractCodeGen_H

#include "source/Compiler/assembler/assembler.h"
#include <QStack>
class Node;
class NodeBinOP;
class NodeNumber;
class NodeString;
class NodeUnaryOp;
class NodeCompound;
class NodeVar;
class NodeAssign;
class NodeVarDecl;
class NodeBlock;
class NodeProgram;
class NodeVarType;
class NodeProcedureDecl;
class NodeProcedure;
class NodeConditional;
class NodeForLoop;
class NodeBuiltinMethod;
class NodeAsm;
class NodeBinaryClause;
class NodeCase;
class NodeRepeatUntil;
class NodeComment;
class NodeControlStatement;


class ProcedureParameter {
    QString m_name;
    TokenType::Type m_type;
    ProcedureParameter() {

    }
    ProcedureParameter(QString n, TokenType::Type t) {
        m_type=t;
        m_name = n;
    }
};

class AbstractCodeGen : public QObject
{
    Q_OBJECT
public:

    Assembler* as = nullptr;
    int m_ticks = 0;
    uint m_currentNode = 0;
    int m_currentPercent = -1;
    AbstractCodeGen();
    QStack<ProcedureParameter> m_parameters;
    QString m_useNext="";
    QMap< QString,QSharedPointer<Node>> m_inlineParameters;
    bool m_flag1 = false;
    bool m_outputLineNumbers = true;
    bool m_outputSource = false; // Turned off for now
    QStringList m_rasSource;
    bool m_isFarAway = false;

    void UpdateDispatchCounter();
       // Declare overloads for each kind of a file to dispatch
    virtual void dispatch(QSharedPointer<NodeBinOP> node) = 0;
    virtual void dispatch(QSharedPointer<NodeNumber> node) = 0;
    virtual void dispatch(QSharedPointer<Node> node) = 0;
    virtual void dispatch(QSharedPointer<NodeString> node) = 0;
    virtual void dispatch(QSharedPointer<NodeVar> node) = 0;
    virtual void dispatch(QSharedPointer<NodeAssign> node);
    virtual void dispatch(QSharedPointer<NodeBlock> node);
    virtual void dispatch(QSharedPointer<NodeVarType> node) = 0;
    virtual void dispatch(QSharedPointer<NodeForLoop> node);
    virtual void dispatch(QSharedPointer<NodeBinaryClause> node) = 0;
    virtual void dispatch(QSharedPointer<NodeCase> node);
    virtual void dispatch(QSharedPointer<NodeRepeatUntil> node) = 0;
    virtual void dispatch(QSharedPointer<NodeComment> node) = 0;
    virtual void dispatch(QSharedPointer<NodeControlStatement> node);




//    virtual QString getValue(QSharedPointer<Node> n);
    virtual QString getEndType(Assembler* as, QSharedPointer<Node> v) {return "";}
    virtual QString getEndType(Assembler *as, QSharedPointer<Node> v1, QSharedPointer<Node> v2) {return "";}

    virtual void StoreVariable(QSharedPointer<NodeVar> n) = 0;
    virtual void LoadVariable(QSharedPointer<NodeVar> n) = 0;
    virtual void LoadVariable(QSharedPointer<Node> n) = 0;
    virtual void LoadVariable(QSharedPointer<NodeNumber> n) = 0;
    virtual QString LoadAddress(QSharedPointer<Node> node) {return "";}
    virtual QString LoadAddress(QSharedPointer<Node> node,QString reg) {return "";}
    virtual void LoadVariable(QSharedPointer<NodeProcedure> node) = 0;

    virtual QString getJmp(bool isOffPage) = 0;
    virtual QString getReturn() = 0;
    virtual QString getReturnInterrupt() = 0;
    virtual QString getIncbin() { return "incbin"; }
    virtual QString getCallSubroutine() { return "jsr"; }

    virtual bool UseBlocks() { return false;}

    virtual QString resolveTemporaryClassPointer(QString name, int mul, int &res) { return "";}
    virtual void PopLostStack(int num) {};

    virtual QString getInitProcedure() { return "";}

    virtual void ExDeHl() {}
    virtual void SbcHlDe() {}


    QString getBank(QSharedPointer<NodeVarType> t);


    /*
     *  Probably the most important method in all of TRSE
     *  Abstract AssignVariable
     *  Handles *all*  A:=B; operations in TRSE
     *  WIP
    */

    virtual void AssignVariable(QSharedPointer<NodeAssign> node);

    /* Here follows methods that are used & requirede by assignvariable
       These are currently defined, but will be pure virtual when the method
       complete

    */

    void AssignPureRecords(QSharedPointer<NodeAssign> node );
    void ValidateAssignStatement(QSharedPointer<NodeAssign> node);

    // Virtual methods

    // var := _ax;
    virtual void AssignFromRegister(QSharedPointer<NodeAssign> node) {}
    // _ax := var;
    virtual void AssignToRegister(QSharedPointer<NodeAssign> node) {}

    virtual void AssignString(QSharedPointer<NodeAssign> node)  {}
    // Assigning a pure pointer  ptr := #someAddress + blah;
    virtual bool AssignPointer(QSharedPointer<NodeAssign> node)  { return false;}

    virtual bool IsSimpleIncDec(QSharedPointer<NodeAssign> node) {return false;}

    virtual bool IsSimpleAssignInteger(QSharedPointer<NodeAssign> node) {return false;}

    virtual bool IsSimpleAssignPointer(QSharedPointer<NodeAssign>node) {return false;}

    virtual bool StoreVariableSimplified(QSharedPointer<NodeAssign> n) {return false;}

    virtual bool StoreStackParameter(QSharedPointer<NodeAssign> n) {return false;}

    virtual void GenericAssign(QSharedPointer<NodeAssign> node);

    virtual bool IsAssignPointerWithIndex(QSharedPointer<NodeAssign> node)  { return false;}

    virtual bool IsAssignArrayWithIndex(QSharedPointer<NodeAssign> node) { return false;}

    void IncreaseCounter(QSharedPointer<Node> step, QSharedPointer<Node> var);



    // handle a small loop
    void SmallLoop(QSharedPointer<NodeForLoop> node, QSharedPointer<NodeVar> var, bool inclusive);

    // handle a large loop
    void LargeLoop(QSharedPointer<NodeForLoop> node, QSharedPointer<NodeVar> var, bool inclusive);



    /* The rest
     */
    QSharedPointer<NodeVar> VarOrNum(QSharedPointer<Node> node);

    void virtual dispatch(QSharedPointer<NodeConditional> node);

    void HandleCompoundBinaryClause(QSharedPointer<Node> node, QString lblFailed, QString lblSuccess, bool offpage);

    virtual void BuildSimple(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool page)  = 0;

    bool isOffPage(QSharedPointer<Node> node, QSharedPointer<Node> b1, QSharedPointer<Node> b2);

    virtual void CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QString lblJump, bool isOffPage) = 0;
    virtual void CompareAndJumpIfNotEqualAndIncrementCounter(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive) =0;


    virtual void dispatch(QSharedPointer<NodeProcedureDecl> node);


    virtual void ProcedureStart(Assembler* as) {

    }

    virtual void ProcedureEnd(Assembler* as) {

    }
    virtual QString ProcedureEndWithoutReturn() {
        return "";
    }

    virtual void dispatch(QSharedPointer<NodeProcedure> node);
    void InlineProcedure(QSharedPointer<NodeProcedure> p);


    virtual void dispatch(QSharedPointer<NodeProgram> node);

    virtual void dispatch(QSharedPointer<NodeVarDecl> node);

    virtual void IncBin(QSharedPointer<NodeVarDecl> node);


    virtual void DeclarePointer(QSharedPointer<NodeVarDecl> node) = 0;

    virtual void dispatch(QSharedPointer<NodeAsm>node);


    virtual void dispatch(QSharedPointer<NodeCompound> node);

    void HandleNodeAssignCopyRecord(QSharedPointer<NodeAssign> node);
    void HandleNodeAssignCopyClass(QSharedPointer<NodeAssign> node);

    void dispatch(QSharedPointer<NodeBuiltinMethod> node);

    virtual void dispatch(QSharedPointer<NodeUnaryOp> node);

    void LineNumber(int ln);

    virtual void OptimizeBinaryClause(QSharedPointer<Node> node,Assembler* as) {}


    virtual bool Evaluate16bitExpr(QSharedPointer<Node> node, QString &lo, QString &hi);


    QString getValue(QSharedPointer<Node> n);

    QString getValue8bit(QSharedPointer<Node> n, int isHi);

    virtual void Disable16bit() {}
    virtual void Enable16bit() {}

public:
signals:
    void EmitTick(QString val);

};

#endif // AbstractCodeGen_H
