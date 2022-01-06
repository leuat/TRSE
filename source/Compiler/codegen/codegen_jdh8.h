#ifndef CodeGenJDH8_H
#define CodeGenJDH8_H



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
#include "source/Compiler/codegen/methods/abstractmethods.h"
#include "source/Compiler/codegen/abstractcodegen.h"

class CodeGenJDH8 : public AbstractCodeGen
{
public:
    CodeGenJDH8();

    void dispatch(QSharedPointer<NodeBinOP>node) override;
//    void dispatchOld(QSharedPointer<NodeBinOP>node) override;
    virtual void dispatch(QSharedPointer<NodeNumber>node) override;
    void dispatch(QSharedPointer<NodeString> node) override;
    void dispatch(QSharedPointer<NodeVarType> node) override;
    void dispatch(QSharedPointer<NodeBinaryClause> node) override;
    void dispatch(QSharedPointer<NodeVar> node) override;
    void dispatch(QSharedPointer<Node> node) override;
    //void dispatch(QSharedPointer<NodeCase> node) override;
    void dispatch(QSharedPointer<NodeRepeatUntil> node) override;
//    void dispatch(QSharedPointer<NodeBuiltinMethod> node) override;
    void dispatch(QSharedPointer<NodeComment> node) override;



    void LoadVariable(QSharedPointer<NodeVar> n) override ;
    QString LoadAddress(QSharedPointer<Node> n) override;
    QString LoadAddress(QSharedPointer<Node> n, QString reg) override;
    void LoadVariable(QSharedPointer<NodeProcedure> node) override;

    virtual void LoadPointer(QSharedPointer<Node> n);
    void LoadVariable(QSharedPointer<Node> n) override;
    void LoadVariable(QSharedPointer<NodeNumber> n) override;
    QString getShift(QSharedPointer<NodeVar> n);


    QString getIndexScaleVal(Assembler* as,QSharedPointer<Node> var);

    int m_lvl = 0;
    QStringList m_regs = QStringList({"a","b","c","d"});

    QString m_jmp = "jmp ";
    QString m_mov = "mw ";
    QString m_cmp = "cmp ";
    QString m_jne = "jz ";

    QString getWordByteType(Assembler* as, QSharedPointer<Node> n) {
        if (n->isWord(as)) return "word"; else return "byte";
    }

    QString getJmp(bool isOffPage) override {
        return "jmp";
    }
    QString getCallSubroutine() override;
    bool m_isPurePointer = false;

    virtual QString getReg(int dd=0);

    void ldr (QString x0, QString x1);
    void str (QString x0, QString x1);

    void str(QSharedPointer<Node> var);
    void ldr(QSharedPointer<Node> var);
    void Binop (QString bop, QString x0, QString x1, QString x2);

    virtual bool UseBlocks() override { return true;}

    QString BinopVariableNumber(QSharedPointer<Node> n, QString value, TokenType::Type type);
    QString getIncbin() override{
        return "@include";
    }


    virtual QString getJDH8Value(Assembler* as, QSharedPointer<Node> n) {
        if (n->isPureVariable() && !n->isReference())
            return "["+n->getValue(as)+"]";
        return n->getValue(as);

    }


    virtual QString getBinaryOperation(QSharedPointer<NodeBinOP> bop);

    QString PushReg();
    void PopReg();
    QString getEndType(Assembler* as, QSharedPointer<Node> v) override;




    void DeclarePointer(QSharedPointer<NodeVarDecl> node) override;
    QString getEndType(Assembler *as, QSharedPointer<Node> v1,QSharedPointer<Node> v2) override;



//    void IncBin(Assembler* as, QSharedPointer<NodeVarDecl> node);

    virtual void BuildSimple(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool page) override;

    virtual void BuildToCmp(QSharedPointer<Node> node);

    virtual void CompareAndJumpIfNotEqualAndIncrementCounter(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive) override;
    virtual void CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB,QString lblJump, bool isOffpage) override;


    QString getReturn() override;
    QString getReturnInterrupt() override { return "iret";}

    /*
     *
     * Node Assign stuff
     *
     *
     */


    void PrintBop(TokenType::Type type, QString x0, QString x1, QString value, bool is16bit=false);

    void StoreVariable(QSharedPointer<NodeVar> node) override;

    bool StoreVariableSimplified(QSharedPointer<NodeAssign> assignNode) override;

    void AssignString(QSharedPointer<NodeAssign>node) override;

    bool AssignPointer(QSharedPointer<NodeAssign>node) override;

    void GenericAssign(QSharedPointer<NodeAssign> node) override;

    bool IsAssignPointerWithIndex(QSharedPointer<NodeAssign> node) override;

    bool IsAssignArrayWithIndex(QSharedPointer<NodeAssign> node) override;

    bool IsSimpleIncDec(QSharedPointer<NodeAssign> node) override;

    bool IsSimpleAssignPointer(QSharedPointer<NodeAssign>node) override;

    void OptimizeBinaryClause(QSharedPointer<Node> node,Assembler* as) override;

    virtual void AssignFromRegister(QSharedPointer<NodeAssign> node) override;

    virtual void AssignToRegister(QSharedPointer<NodeAssign> node) override;


    void ProcedureStart(Assembler* as) override;

    void ProcedureEnd(Assembler* as) override;



};
#endif
