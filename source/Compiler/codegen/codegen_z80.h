#ifndef CodeGenZ80_H
#define CodeGenZ80_H



#include "abstractcodegen.h"
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
#include "source/Compiler/codegen/methods/methodsx86.h"
#include "source/Compiler/codegen/codegen_x86.h"
#include "source/Compiler/codegen/methods/factorymethods.h"

class CodeGenZ80 : public CodeGenX86
{
public:
    CodeGenZ80();

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

//    void dispatch(QSharedPointer<NodeForLoop> node);


//    void AssignVariable(QSharedPointer<NodeAssign> node) override;

    /*
     *
     * NodeAssign
     *
     *
     *
    */

    void ExDeHl();
    void SbcHlDe();

    void AssignVariable(QSharedPointer<NodeAssign> node) override;

    void LoadPointer(QSharedPointer<Node> node) override;


    void AssignString(QSharedPointer<NodeAssign>node) override;

    bool AssignPointer(QSharedPointer<NodeAssign>node) override;

    bool IsSimpleIncDec(QSharedPointer<NodeAssign> node) override;

    bool IsSimpleAssignPointer(QSharedPointer<NodeAssign>node) override;

    virtual void GenericAssign(QSharedPointer<NodeAssign> node) override;


    virtual void AssignFromRegister(QSharedPointer<NodeAssign> node) override;

    virtual void AssignToRegister(QSharedPointer<NodeAssign> node) override;

    void StoreVariable(QSharedPointer<NodeVar> node) override;

    bool StoreVariableSimplified(QSharedPointer<NodeAssign> assignNode) override;

    bool isGB() {return Syntax::s.m_currentSystem->m_system == AbstractSystem::GAMEBOY; }

    QString getAx(QSharedPointer<Node> n) override;
    QString getA(QSharedPointer<Node> n);

    QString getX86Value(Assembler *as, QSharedPointer<Node> n) override;

    QString getBinaryOperation(QSharedPointer<NodeBinOP> bop) override;

    void LoadAddress(QSharedPointer<Node> n) override;
    void LoadInteger(QSharedPointer<Node> n);
    void StoreAddress(QSharedPointer<Node> n);
    QString getHL();

    void BuildSimple(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool page) override;

    void BuildToCmp(QSharedPointer<Node> node) override;


    QString getPlusMinus(Token t);
    void Handle16bitShift(QSharedPointer<NodeBinOP>node);


    QString getJmp(bool isOffPage) override;
    bool UseBlocks() override;


//    void dispatch(QSharedPointer<NodeConditional> node) override;
    void HandleCompoundBinaryClause(QSharedPointer<Node> node, QString lblFailed,QString lblSuccess, bool forcePage);

    void CompareAndJumpIfNotEqualAndIncrementCounter(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive) override;
    void CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB,QString lblJump, bool isOffpage) override;

    QString getReturn() override { return "ret";}
    QString getReturnInterrupt() override { return "reti";}

    void HandleAeqAopB16bit(QSharedPointer<NodeBinOP> bop, QSharedPointer<NodeVar> var);

    void LoadVariable(QSharedPointer<NodeProcedure> node) override;

    void LoadVariable(QSharedPointer<NodeVar> n) override;
    void LoadVariable(QSharedPointer<Node> n) override;


    void BinaryClauseInteger(QSharedPointer<Node> node,QString lblSuccess, QString lblFailed, bool page);



};
#endif // CodeGenX86_H
