#ifndef ABSTRACTASTDISPATCHER_H
#define ABSTRACTASTDISPATCHER_H

#include "source/Compiler/assembler/assembler.h"

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


class AbstractASTDispatcher
{
public:

    Assembler* as = nullptr;

    AbstractASTDispatcher();
        // Declare overloads for each kind of a file to dispatch
    virtual void dispatch(NodeBinOP *node) = 0;
    virtual void dispatch(NodeNumber *node) = 0;
    virtual void dispatch(Node *node) = 0;
    virtual void dispatch(NodeString *node) = 0;
    virtual void dispatch(NodeUnaryOp *node) = 0;
    virtual void dispatch(NodeCompound *node) = 0;
    virtual void dispatch(NodeVar *node) = 0;
    virtual void dispatch(NodeAssign *node) = 0;
    virtual void dispatch(NodeVarDecl *node) = 0;
    virtual void dispatch(NodeBlock *node) = 0;
    virtual void dispatch(NodeProgram *node) = 0;
    virtual void dispatch(NodeVarType *node) = 0;
    virtual void dispatch(NodeProcedure *node) = 0;
    virtual void dispatch(NodeProcedureDecl *node) = 0;
    virtual void dispatch(NodeConditional *node) = 0;
    virtual void dispatch(NodeForLoop *node) = 0;
    virtual void dispatch(NodeBuiltinMethod *node) = 0;
    virtual void dispatch(NodeAsm *node) = 0;
    virtual void dispatch(NodeBinaryClause *node) = 0;


    virtual void StoreVariable(NodeVar* n) = 0;
    virtual void LoadVariable(NodeVar* n) = 0;
    virtual void LoadVariable(Node* n) = 0;
    virtual void LoadVariable(NodeNumber* n) = 0;

};

#endif // ABSTRACTASTDISPATCHER_H
