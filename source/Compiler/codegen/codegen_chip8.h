#ifndef CodeGenchip8
#define CodeGenchip8



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

enum CHIP8_TYPE{
    OG,
    SCHIP,
    CHIPX,
    MEGA,
    XO_CHIP
};
class CodeGenChip8 : public AbstractCodeGen
{
private: 
    CHIP8_TYPE chip_type = OG;
public:
    CodeGenChip8();

    void dispatch(QSharedPointer<NodeBinOP>node) override;
    virtual void dispatch(QSharedPointer<NodeNumber>node) override;
    void dispatch(QSharedPointer<NodeString> node) override;
    void dispatch(QSharedPointer<NodeVarType> node) override;
    void dispatch(QSharedPointer<NodeBinaryClause> node) override;
    void dispatch(QSharedPointer<NodeVar> node) override;
    void dispatch(QSharedPointer<Node> node) override;
    void dispatch(QSharedPointer<NodeRepeatUntil> node) override;
    void dispatch(QSharedPointer<NodeComment> node) override;
    void dispatch(QSharedPointer<NodeProgram> node) override;



    void LoadVariable(QSharedPointer<NodeVar> n) override ;
    QString LoadAddress(QSharedPointer<Node> n) override;
    QString LoadAddress(QSharedPointer<Node> n, QString reg) override;
    void LoadVariable(QSharedPointer<NodeProcedure> node) override;

    virtual void LoadPointer(QSharedPointer<Node> n);
    void LoadVariable(QSharedPointer<Node> n) override;
    void LoadVariable(QSharedPointer<NodeNumber> n) override;


    QString getIndexScaleVal(QSharedPointer<Node> var);

    int m_lvl = 0;
    QStringList m_regs = QStringList({"V2","V3","V4","V5","V6","V7", "V8", "V9", "VA", "VB", "VC", "VD", "VE"});
    QString m_cmp = "cmp ";
    QString m_jne = "jz ";

    QString getWordByteType(QSharedPointer<Node> n) {
        if (n->isWord(as)) return "word"; else return "byte";
    }

    QString getJmp(bool isOffPage) override {
        return "jp";
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


    virtual QString getChip8Value(QSharedPointer<Node> n) {
        if (n->isPureVariable() && !n->isReference())
            return "["+n->getValue(as)+"]";
        return n->getValue(as);

    }


    virtual QString getBinaryOperation(QSharedPointer<NodeBinOP> bop);

    QString PushReg();
    void PopReg();





    void DeclarePointer(QSharedPointer<NodeVarDecl> node) override;



//    void IncBin(Assembler* as, QSharedPointer<NodeVarDecl> node);

    virtual void BuildConditional(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool page) override;


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


    void PrintBop(TokenType::Type type, QString x0, QString x1);
    void PrintBop16(TokenType::Type type, QString x0_hi, QString x0_lo, QString x1_hi, QString x1_lo);

    void StoreVariable(QSharedPointer<NodeVar> node) override;

    bool StoreVariableSimplified(QSharedPointer<NodeAssign> assignNode) override;

    void AssignString(QSharedPointer<NodeAssign>node) override;

    bool AssignPointer(QSharedPointer<NodeAssign>node) override;

    void GenericAssign(QSharedPointer<NodeAssign> node) override;

    bool IsAssignPointerWithIndex(QSharedPointer<NodeAssign> node) override;

    bool IsAssignArrayWithIndex(QSharedPointer<NodeAssign> node) override;

    bool IsSimpleIncDec(QSharedPointer<NodeAssign> node) override;

    bool IsSimpleAssignPointer(QSharedPointer<NodeAssign>node) override;

    void OptimizeBinaryClause(QSharedPointer<Node> node) override;

    virtual void AssignFromRegister(QSharedPointer<NodeAssign> node) override;

    virtual void AssignToRegister(QSharedPointer<NodeAssign> node) override;
    

};
#endif
