#ifndef AbstractASTDispatcher_H
#define AbstractASTDispatcher_H

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

class AbstractASTDispatcher : public QObject
{
    Q_OBJECT
public:

    Assembler* as = nullptr;
    int m_ticks = 0;
    AbstractASTDispatcher();
    QStack<ProcedureParameter> m_parameters;
    QString m_useNext="";
    QMap< QString,QSharedPointer<Node>> m_inlineParameters;
    bool m_flag1 = false;
    bool m_outputLineNumbers = true;
       // Declare overloads for each kind of a file to dispatch
    virtual void dispatch(QSharedPointer<NodeBinOP> node) = 0;
    virtual void dispatch(QSharedPointer<NodeNumber> node) = 0;
    virtual void dispatch(QSharedPointer<Node> node) = 0;
    virtual void dispatch(QSharedPointer<NodeString> node) = 0;
    virtual void dispatch(QSharedPointer<NodeVar> node) = 0;
    virtual void dispatch(QSharedPointer<NodeAssign> node) = 0;
    virtual void dispatch(QSharedPointer<NodeBlock> node);
    virtual void dispatch(QSharedPointer<NodeVarType> node) = 0;
    virtual void dispatch(QSharedPointer<NodeForLoop> node);
    virtual void dispatch(QSharedPointer<NodeBinaryClause> node) = 0;
    virtual void dispatch(QSharedPointer<NodeCase> node) = 0;
    virtual void dispatch(QSharedPointer<NodeRepeatUntil> node) = 0;
    virtual void dispatch(QSharedPointer<NodeComment> node) = 0;

    virtual QString getValue(QSharedPointer<Node> n);
    virtual QString getEndType(Assembler* as, QSharedPointer<Node> v) {return "";}
    virtual QString getEndType(Assembler *as, QSharedPointer<Node> v1, QSharedPointer<Node> v2) {return "";}

    virtual void StoreVariable(QSharedPointer<NodeVar> n) = 0;
    virtual void LoadVariable(QSharedPointer<NodeVar> n) = 0;
    virtual void LoadVariable(QSharedPointer<Node> n) = 0;
    virtual void LoadVariable(QSharedPointer<NodeNumber> n) = 0;
    virtual void LoadAddress(QSharedPointer<Node> node) {}
    virtual void LoadAddress(QSharedPointer<Node> node,QString reg) {}

    virtual QString getJmp(bool isOffPage) = 0;
    virtual QString getReturn() = 0;
    virtual QString getReturnInterrupt() = 0;
    virtual QString getIncbin() { return "incbin"; }
    virtual QString getCallSubroutine() { return "jsr"; }

    virtual bool UseBlocks() { return false;}

    void virtual dispatch(QSharedPointer<NodeConditional> node);

    void HandleCompoundBinaryClause(QSharedPointer<Node> node, QString lblFailed, QString lblSuccess, bool offpage);

    virtual void BuildSimple(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool page)  = 0;

    bool isOffPage(QSharedPointer<Node> node, QSharedPointer<Node> b1, QSharedPointer<Node> b2);

    virtual void CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive) = 0;


    virtual void dispatch(QSharedPointer<NodeProcedureDecl> node);


    virtual void dispatch(QSharedPointer<NodeProcedure> node);
    void InlineProcedure(QSharedPointer<NodeProcedure> p);


    virtual void dispatch(QSharedPointer<NodeProgram> node);

    virtual void dispatch(QSharedPointer<NodeVarDecl> node);

    virtual void IncBin(QSharedPointer<NodeVarDecl> node);


    virtual void DeclarePointer(QSharedPointer<NodeVarDecl> node) = 0;

    virtual void dispatch(QSharedPointer<NodeAsm>node);


    virtual void dispatch(QSharedPointer<NodeCompound> node);

    void HandleNodeAssignCopyRecord(QSharedPointer<NodeAssign> node);

    void dispatch(QSharedPointer<NodeBuiltinMethod> node);

    virtual void dispatch(QSharedPointer<NodeUnaryOp> node);

    void LineNumber(int ln);

    virtual void OptimizeBinaryClause(QSharedPointer<Node> node,Assembler* as) {}


public:
signals:
    void EmitTick(QString val);

};

#endif // AbstractASTDispatcher_H
