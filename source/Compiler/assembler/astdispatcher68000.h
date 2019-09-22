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
#include "source/Compiler/ast/nodecase.h"
#include "methods68000.h"

class ASTDispather68000 : public AbstractASTDispatcher
{
public:
    ASTDispather68000();
    QString m_lastSize;

    void dispatch(NodeBinOP *node) override;
//    void dispatchOld(NodeBinOP *node) override;
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
    void dispatch(NodeCase* node) override;
    void dispatch(NodeRepeatUntil *node) override;
    void dispatch(NodeBuiltinMethod *node) override;



    void StoreVariable(NodeVar* n) override;
    void LoadVariable(NodeVar* n) override ;
    void LoadAddress(Node* n) override;
    void LoadAddress(Node *n, QString reg) override;

    void LoadPointer(Node* n);
    void LoadVariable(Node* n) override;
    void LoadVariable(NodeNumber* n) override;


    void TransformVariable(Assembler* as, QString op, QString n, QString val, Node* t);
    void TransformVariable(Assembler* as, QString op, NodeVar* n, QString val);
    void TransformVariable(Assembler* as, QString op, QString n, NodeVar* val);
    void TransformVariable(Assembler* as, QString op, QString n, QString val);

    QString getEndType(Assembler* as, Node* v) override;

    QString AssignVariable(NodeAssign *node);
    void IncBin(Assembler* as, NodeVarDecl *node);

    void BuildSimple(Node *node, QString lblFailed);

    void BuildToCmp(Node *node);

    void DeclarePointer(NodeVarDecl *node);
    bool m_clearFlag = false;
    QString getEndType(Assembler *as, Node *v1,Node *v2) override;

};

#endif // ASTDISPATCHER68000_H
