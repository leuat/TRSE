#ifndef ASTdispatcherZ80_H
#define ASTdispatcherZ80_H



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
#include "source/Compiler/assembler/methodsx86.h"
#include "source/Compiler/assembler/factorymethods.h"

class ASTdispatcherZ80 : public AbstractASTDispatcher
{
public:
    ASTdispatcherZ80();

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
    void dispatch(QSharedPointer<NodeConditional> node) override;
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
    void IncBin(Assembler *as, QSharedPointer<NodeVarDecl> node);



    int m_lvl = 0;



};
#endif // ASTdispatcherX86_H
