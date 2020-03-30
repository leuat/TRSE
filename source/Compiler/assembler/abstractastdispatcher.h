#ifndef ABSTRACTASTDISPATCHER_H
#define ABSTRACTASTDISPATCHER_H

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

class AbstractASTDispatcher
{
public:

    Assembler* as = nullptr;

    AbstractASTDispatcher();
    QStack<ProcedureParameter> m_parameters;
        // Declare overloads for each kind of a file to dispatch
    virtual void dispatch(QSharedPointer<NodeBinOP> node) = 0;
    virtual void dispatch(QSharedPointer<NodeNumber> node) = 0;
    virtual void dispatch(QSharedPointer<Node> node) = 0;
    virtual void dispatch(QSharedPointer<NodeString> node) = 0;
    virtual void dispatch(QSharedPointer<NodeUnaryOp> node) = 0;
    virtual void dispatch(QSharedPointer<NodeCompound> node) = 0;
    virtual void dispatch(QSharedPointer<NodeVar> node) = 0;
    virtual void dispatch(QSharedPointer<NodeAssign> node) = 0;
    virtual void dispatch(QSharedPointer<NodeVarDecl> node) = 0;
    virtual void dispatch(QSharedPointer<NodeBlock> node) = 0;
    virtual void dispatch(QSharedPointer<NodeProgram> node) = 0;
    virtual void dispatch(QSharedPointer<NodeVarType> node) = 0;
    virtual void dispatch(QSharedPointer<NodeProcedure> node) = 0;
    virtual void dispatch(QSharedPointer<NodeProcedureDecl> node) = 0;
    virtual void dispatch(QSharedPointer<NodeConditional> node) = 0;
    virtual void dispatch(QSharedPointer<NodeForLoop> node) = 0;
    virtual void dispatch(QSharedPointer<NodeBuiltinMethod> node) = 0;
    virtual void dispatch(QSharedPointer<NodeAsm> node) = 0;
    virtual void dispatch(QSharedPointer<NodeBinaryClause> node) = 0;
    virtual void dispatch(QSharedPointer<NodeCase> node) = 0;
    virtual void dispatch(QSharedPointer<NodeRepeatUntil> node) = 0;
    virtual void dispatch(QSharedPointer<NodeComment> node) = 0;

    virtual QString getEndType(Assembler* as, QSharedPointer<Node> v) {return "";}
    virtual QString getEndType(Assembler *as, QSharedPointer<Node> v1, QSharedPointer<Node> v2) {return "";}

    virtual void StoreVariable(QSharedPointer<NodeVar> n) = 0;
    virtual void LoadVariable(QSharedPointer<NodeVar> n) = 0;
    virtual void LoadVariable(QSharedPointer<Node> n) = 0;
    virtual void LoadVariable(QSharedPointer<NodeNumber> n) = 0;
    virtual void LoadAddress(QSharedPointer<Node> node) {}
    virtual void LoadAddress(QSharedPointer<Node> node,QString reg) {}

};

#endif // ABSTRACTASTDISPATCHER_H
