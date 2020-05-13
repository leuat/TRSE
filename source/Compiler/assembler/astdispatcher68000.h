#ifndef ASTdispatcher68000_H
#define ASTdispatcher68000_H

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
#include "source/Compiler/assembler/factorymethods.h"
#include "source/Compiler/syntax.h"

class ASTDispatcher68000 : public AbstractASTDispatcher
{
public:
    ASTDispatcher68000();
    QString m_lastSize;

    QStringList m_regs = QStringList()<<"d0" <<"d1"<<"d2" <<"d3"<<"d4" <<"d5"<<"d6";

    void dispatch(QSharedPointer<NodeBinOP>node) override;
//    void dispatchOld(QSharedPointer<NodeBinOP>node) override;
    void dispatch(QSharedPointer<NodeNumber>node) override;
    void dispatch(QSharedPointer<NodeAsm>node) override;
    void dispatch(QSharedPointer<NodeString> node) override;
    void dispatch(QSharedPointer<NodeUnaryOp> node) override;
    void dispatch(QSharedPointer<NodeCompound> node) override;
    void dispatch(QSharedPointer<NodeVarDecl> node) override;
    void dispatch(QSharedPointer<NodeBlock> node) override;
    void dispatch(QSharedPointer<NodeProgram> node) override;
    void dispatch(QSharedPointer<NodeVarType> node) override;
    void dispatch(QSharedPointer<NodeBinaryClause> node) override;
    void dispatch(QSharedPointer<NodeProcedure> node) override;
    void dispatch(QSharedPointer<NodeProcedureDecl> node) override;
   // void dispatch(QSharedPointer<NodeConditional> node) override;
    void dispatch(QSharedPointer<NodeForLoop> node) override;
    void dispatch(QSharedPointer<NodeVar> node) override;
    void dispatch(QSharedPointer<Node> node) override;
    void dispatch(QSharedPointer<NodeAssign> node) override;
    void dispatch(QSharedPointer<NodeCase> node) override;
    void dispatch(QSharedPointer<NodeRepeatUntil> node) override;
    void dispatch(QSharedPointer<NodeBuiltinMethod> node) override;
    void dispatch(QSharedPointer<NodeComment> node) override;




    void StoreVariable(QSharedPointer<NodeVar> n) override;
    void LoadVariable(QSharedPointer<NodeVar> n) override ;
    void LoadAddress(QSharedPointer<Node> n) override;
    void LoadAddress(QSharedPointer<Node> n, QString reg) override;

    void LoadPointer(QSharedPointer<Node> n);
    void LoadVariable(QSharedPointer<Node> n) override;
    void LoadVariable(QSharedPointer<NodeNumber> n) override;

    QString getJmp(bool isOffPage) override {
        return "jmp";
    }

    void TransformVariable(Assembler* as, QString op, QString n, QString val, QSharedPointer<Node> t);
    void TransformVariable(Assembler* as, QString op, QSharedPointer<NodeVar> n, QString val);
    void TransformVariable(Assembler* as, QString op, QString n, QSharedPointer<NodeVar> val);
    void TransformVariable(Assembler* as, QString op, QString n, QString val);

    QString getEndType(Assembler* as, QSharedPointer<Node> v) override;

    QString AssignVariable(QSharedPointer<NodeAssign> node);
    void IncBin(Assembler* as, QSharedPointer<NodeVarDecl> node);

//    void BuildSimple(QSharedPointer<Node> node, QString lblFailed);
    void BuildSimple(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool page) override;

    void BuildToCmp(QSharedPointer<Node> node);

    void DeclarePointer(QSharedPointer<NodeVarDecl> node);
    bool m_clearFlag = false;
    QString getEndType(Assembler *as, QSharedPointer<Node> v1,QSharedPointer<Node> v2) override;

    bool HandleSimpleAeqAopConst(QSharedPointer<NodeAssign>node);
    bool HandleSimpleAeqBopConst(QSharedPointer<NodeAssign>node);


    void CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive) override;

};

#endif // ASTdispatcher68000_H
