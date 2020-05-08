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
#include "source/Compiler/assembler/astdispatcherx86.h"
#include "source/Compiler/assembler/factorymethods.h"

class ASTdispatcherZ80 : public ASTdispatcherX86
{
public:
    ASTdispatcherZ80();

    void dispatch(QSharedPointer<NodeBinOP>node) override;
//    void dispatchOld(QSharedPointer<NodeBinOP>node) override;
    void dispatch(QSharedPointer<NodeBinaryClause> node) override;
    //void dispatch(QSharedPointer<NodeConditional> node) override;
  //  void dispatch(QSharedPointer<NodeForLoop> node) override;
    void dispatch(QSharedPointer<NodeVarDecl> node) override;
/*    void dispatch(QSharedPointer<NodeCase> node) override;
    void dispatch(QSharedPointer<NodeRepeatUntil> node) override;*/

    void dispatch(QSharedPointer<NodeVar> node) override;
    void dispatch(QSharedPointer<NodeNumber>node) override;

    void dispatch(QSharedPointer<NodeForLoop> node);


    QString AssignVariable(QSharedPointer<NodeAssign> node) override;



    QString getAx(QSharedPointer<Node> n) override;
    QString getA(QSharedPointer<Node> n);

    QString getX86Value(Assembler *as, QSharedPointer<Node> n) override;

    QString getBinaryOperation(QSharedPointer<NodeBinOP> bop) override;

    void LoadAddress(QSharedPointer<Node> n) override;
    void LoadInteger(QSharedPointer<Node> n);
    void StoreAddress(QSharedPointer<Node> n);
    QString getHL() {
        QString hl ="hl";
        if (m_useNext!="")
            hl = m_useNext;
        m_useNext="";
       return hl;
    }

    void BuildSimple(QSharedPointer<Node> node, QString lblFailed, bool offPage=false);

    void BuildToCmp(QSharedPointer<Node> node) override;

    void HandleAssignPointers(QSharedPointer<NodeAssign> node);

    QString getPlusMinus(Token t);
    void Handle16bitShift(QSharedPointer<NodeBinOP>node);


};
#endif // ASTdispatcherX86_H
