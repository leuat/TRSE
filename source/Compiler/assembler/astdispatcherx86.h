#ifndef ASTdispatcherX86_H
#define ASTdispatcherX86_H



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
#include "source/Compiler/ast/noderepeatuntil.h"
#include "source/Compiler/ast/nodeasm.h"
#include "source/Compiler/ast/nodebinaryclause.h"
#include "source/Compiler/ast/nodecase.h"
#include "source/Compiler/assembler/methodsx86.h"
#include "source/Compiler/assembler/factorymethods.h"

class ASTdispatcherX86 : public AbstractASTDispatcher
{
public:
    ASTdispatcherX86();

    void dispatch(QSharedPointer<NodeBinOP>node) override;
//    void dispatchOld(QSharedPointer<NodeBinOP>node) override;
    virtual void dispatch(QSharedPointer<NodeNumber>node) override;
    void dispatch(QSharedPointer<NodeString> node) override;
   // void dispatch(QSharedPointer<NodeVarDecl> node) override;
 //   void dispatch(QSharedPointer<NodeBlock> node) override;
 //   void dispatch(QSharedPointer<NodeProgram> node) override;
    void dispatch(QSharedPointer<NodeVarType> node) override;
    void dispatch(QSharedPointer<NodeBinaryClause> node) override;
    //void dispatch(QSharedPointer<NodeProcedure> node) override;
   // void dispatch(QSharedPointer<NodeProcedureDecl> node) override;
 //   void dispatch(QSharedPointer<NodeConditional> node) override;
    //void dispatch(QSharedPointer<NodeForLoop> node) override;
    void dispatch(QSharedPointer<NodeVar> node) override;
    void dispatch(QSharedPointer<Node> node) override;
    void dispatch(QSharedPointer<NodeAssign> node) override;
    void dispatch(QSharedPointer<NodeCase> node) override;
    void dispatch(QSharedPointer<NodeRepeatUntil> node) override;
//    void dispatch(QSharedPointer<NodeBuiltinMethod> node) override;
    void dispatch(QSharedPointer<NodeComment> node) override;



    void StoreVariable(QSharedPointer<NodeVar> n) override;
    void LoadVariable(QSharedPointer<NodeVar> n) override ;
    void LoadAddress(QSharedPointer<Node> n) override;
    void LoadAddress(QSharedPointer<Node> n, QString reg) override;
    void LoadVariable(QSharedPointer<NodeProcedure> node) override;

    void LoadPointer(QSharedPointer<Node> n);
    void LoadVariable(QSharedPointer<Node> n) override;
    void LoadVariable(QSharedPointer<NodeNumber> n) override;


    int m_lvl = 0;
    QStringList m_regs = QStringList({"a","b","c","d"});

    QString m_jmp = "jmp ";
    QString m_mov = "mov ";
    QString m_cmp = "cmp ";
    QString m_jne = "jne ";

    QString getJmp(bool isOffPage) override {
        return "jmp";
    }
    QString getCallSubroutine() override { return "call"; }
    bool m_isPurePointer = false;
    virtual QString getAx(QSharedPointer<Node> n);
        virtual QString getAx(QString a, QSharedPointer<Node> n);


    virtual QString getX86Value(Assembler* as, QSharedPointer<Node> n) {
        if (n->isPureVariable() && !n->isReference())
            return "["+n->getValue(as)+"]";
        return n->getValue(as);

    }


    virtual QString getBinaryOperation(QSharedPointer<NodeBinOP> bop) {
        if (bop->m_op.m_type == TokenType::PLUS)
            return "add";
        if (bop->m_op.m_type == TokenType::MINUS)
            return "sub";
        if (bop->m_op.m_type == TokenType::DIV)
            return "div";
        if (bop->m_op.m_type == TokenType::MUL)
            return "mul";
        return " UNKNOWN BINARY OPERATION";
    }

    void PushX() {
        if (m_lvl==3)
            ErrorHandler::e.Error("Error in X86 dispatcher PopX : trying to push regstack from max");
        m_lvl++;
    }
    void PopX() {
        if (m_lvl==0)
            ErrorHandler::e.Error("Error in X86 dispatcher PopX : trying to pop regstack from zero");
        m_lvl--;
    }
    QString getEndType(Assembler* as, QSharedPointer<Node> v) override;

    virtual QString AssignVariable(QSharedPointer<NodeAssign> node);



    void DeclarePointer(QSharedPointer<NodeVarDecl> node) override;
    QString getEndType(Assembler *as, QSharedPointer<Node> v1,QSharedPointer<Node> v2) override;



//    void IncBin(Assembler* as, QSharedPointer<NodeVarDecl> node);

    virtual void BuildSimple(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool page) override;

    virtual void BuildToCmp(QSharedPointer<Node> node);

    void CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive) override;


    QString getReturn() override { return "ret";}
    QString getReturnInterrupt() override { return "iret";}


    void AssignString(QSharedPointer<NodeAssign> node, bool isPointer);

};
#endif // ASTdispatcherX86_H
