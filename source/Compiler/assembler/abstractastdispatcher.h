#ifndef ABSTRACTASTDISPATCHER_H
#define ABSTRACTASTDISPATCHER_H

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

};

#endif // ABSTRACTASTDISPATCHER_H
