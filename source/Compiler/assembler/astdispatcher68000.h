#ifndef ASTDISPATCHER68000_H
#define ASTDISPATCHER68000_H

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

class ASTDispather68000 : public AbstractASTDispatcher
{
public:
    ASTDispather68000();


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



    void StoreVariable(NodeVar* n);
    void LoadVariable(NodeVar* n);
    void LoadVariable(Node* n);
    void LoadVariable(NodeNumber* n);


    void TransformVariable(Assembler* as, QString op, NodeVar* n, QString val);
    void TransformVariable(Assembler* as, QString op, QString n, QString val);

    QString getEndType(Assembler* as, Node* v) {
        if (v->isWord(as))
            return ".w";
        if (v->isLong(as))
            return ".l";
        if (v->isByte(as))
            return ".b";
    }

    QString AssignVariable(NodeAssign *node);



};

#endif // ASTDISPATCHER68000_H
