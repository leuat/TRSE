#ifndef ASTDISPATCHERX86_H
#define ASTDISPATCHERX86_H



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

class ASTDispatcherX86 : public AbstractASTDispatcher
{
public:
    ASTDispatcherX86();

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
    void dispatch(NodeComment *node) override;



    void StoreVariable(NodeVar* n) override;
    void LoadVariable(NodeVar* n) override ;
    void LoadAddress(Node* n) override;
    void LoadAddress(Node *n, QString reg) override;

    void LoadPointer(Node* n);
    void LoadVariable(Node* n) override;
    void LoadVariable(NodeNumber* n) override;


    int m_lvl = 0;
    QStringList m_regs = QStringList({"a","b","c","d"});

    QString getAx(Node* n) {
        QString a = m_regs[m_lvl];
        if (n->m_forceType==TokenType::INTEGER)
            return a+"x";
        if (n->getType(as)==TokenType::INTEGER)
            return a+"x";
        if (n->getType(as)==TokenType::ADDRESS)
            return a+"x";
        if (n->getType(as)==TokenType::INTEGER_CONST)
            if (n->isWord(as))
            return a+"x";
//        if (n->isPureNumeric())
  //          if (n->getValue()
        return a+"l";

    }
    QString getX86Value(Assembler* as, Node* n) {
        if (n->isPureVariable())
            return "["+n->getValue(as)+"]";
        return n->getValue(as);

    }

    QString getBinaryOperation(NodeBinOP* bop) {
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
    QString getEndType(Assembler* as, Node* v) override;

    QString AssignVariable(NodeAssign *node);



    void DeclarePointer(NodeVarDecl *node);
    QString getEndType(Assembler *as, Node *v1,Node *v2) override;



    void IncBin(Assembler* as, NodeVarDecl *node);

    void BuildSimple(Node *node, QString lblFailed);

    void BuildToCmp(Node *node);


};
#endif // ASTDISPATCHERX86_H
