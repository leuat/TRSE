#ifndef AbstractCodeGen_H
#define AbstractCodeGen_H

#include "source/Compiler/assembler/assembler.h"
#include <QStack>
class Node;
class NodeBinOP;
class NodeNumber;
class NodeString;
class NodeUnaryOp;
class NodeCompound;
class NodeVar;
class NodeAssign;
class NodeVarDecl;
class NodeBlock;
class NodeProgram;
class NodeVarType;
class NodeProcedureDecl;
class NodeProcedure;
class NodeConditional;
class NodeForLoop;
class NodeBuiltinMethod;
class NodeAsm;
class NodeBinaryClause;
class NodeCase;
class NodeCast;
class NodeRepeatUntil;
class NodeComment;
class NodeControlStatement;


class ProcedureParameter {
    QString m_name;
    TokenType::Type m_type;
    ProcedureParameter() {

    }
    ProcedureParameter(QString n, TokenType::Type t) {
        m_type=t;
        m_name = n;
    }
};

class AbstractCodeGen : public QObject
{
    Q_OBJECT
private:
    int m_ticks = 0;
    uint m_currentNode = 0;
    int m_currentPercent = -1;
    QStack<ProcedureParameter> m_parameters;
    bool m_outputLineNumbers = true;
    bool m_outputSource = false; // Turned off for now
    QStringList m_rasSource;
    bool m_isFarAway = false;
protected:
    Assembler* as = nullptr;
    QMap< QString,QSharedPointer<Node>> m_inlineParameters;
    bool m_flag1 = false;
    QString m_useNext="";
public:

    inline void dontOutputLineNumbers(){m_outputLineNumbers=false;}
    inline void outputLineNumbers(){m_outputLineNumbers=true;}
    inline void useThisNext(QString s){m_useNext=s;}
    inline void setAssembler(Assembler* as ){this->as=as;}
    AbstractCodeGen();
    void UpdateDispatchCounter();
       // Declare overloads for each kind of a file to dispatch
    virtual void dispatch(QSharedPointer<NodeBinOP> node) = 0;
    virtual void dispatch(QSharedPointer<NodeNumber> node) = 0;
    virtual void dispatch(QSharedPointer<Node> node) = 0;
    virtual void dispatch(QSharedPointer<NodeString> node) = 0;
    virtual void dispatch(QSharedPointer<NodeVar> node) = 0;
    virtual void dispatch(QSharedPointer<NodeAssign> node);
    virtual void dispatch(QSharedPointer<NodeBlock> node);
    virtual void dispatch(QSharedPointer<NodeVarType> node) = 0;
    virtual void dispatch(QSharedPointer<NodeForLoop> node);
    virtual void dispatch(QSharedPointer<NodeBinaryClause> node) = 0;
    virtual void dispatch(QSharedPointer<NodeCase> node);
    virtual void dispatch(QSharedPointer<NodeCast> node);
    virtual void dispatch(QSharedPointer<NodeRepeatUntil> node) = 0;
    virtual void dispatch(QSharedPointer<NodeComment> node) = 0;
    virtual void dispatch(QSharedPointer<NodeControlStatement> node);
    virtual void dispatch(QSharedPointer<NodeConditional> node);
    virtual void dispatch(QSharedPointer<NodeProcedureDecl> node);
    virtual void dispatch(QSharedPointer<NodeProcedure> node);
    virtual void dispatch(QSharedPointer<NodeProgram> node);
    virtual void dispatch(QSharedPointer<NodeVarDecl> node);
    virtual void dispatch(QSharedPointer<NodeAsm>node);
    virtual void dispatch(QSharedPointer<NodeCompound> node);
    virtual void dispatch(QSharedPointer<NodeBuiltinMethod> node);
    virtual void dispatch(QSharedPointer<NodeUnaryOp> node);






    // M6809 only. Loads an variable and transfers it to the internal index register.
    virtual void LoadIndex(QSharedPointer<Node> n,TokenType::Type arrayType) {}
    // Stores the current register values to a variable
    virtual void StoreVariable(QSharedPointer<NodeVar> n) = 0;
    // Loads a variable into a register(s)
    virtual void LoadVariable(QSharedPointer<NodeVar> n) = 0;
    // Loads a pointer address into an internal register
    virtual void LoadVariable(QSharedPointer<Node> n) = 0;
    // Loads a number into an internal register
    virtual void LoadVariable(QSharedPointer<NodeNumber> n) = 0;
    // Loads the address of a procedure into a internal register
    virtual void LoadVariable(QSharedPointer<NodeProcedure> node) = 0;
    // Loads the address of a label into the internal registers. Functions not used on the 6502.
    virtual QString LoadAddress(QSharedPointer<Node> node) {return "";}
    virtual QString LoadAddress(QSharedPointer<Node> node,QString reg) {return "";}
    // Returns the jmp instruction for the current cpu
    virtual QString getJmp(bool isOffPage) = 0;
    // Returns the rts/ret instruction for the current cpu
    virtual QString getReturn() = 0;
    // Returns the rti instruction for the current CPU
    virtual QString getReturnInterrupt() = 0;
    // Returns the binary include command for the current assembler
    virtual QString getIncbin() { return "incbin"; }
    // Returns the call/jsr subroutine instruction for the current CPU
    virtual QString getCallSubroutine() { return "jsr"; }
    // Does the current system support splitting code into custom memory blocks?
    virtual bool UseBlocks() { return false;}

    // Resolves the string value of a temporary (*this) pointer for a class
    virtual QString resolveTemporaryClassPointer(QString name, int mul, int &res) { return "";}


   /*
    * method that is called at the start of every procedure, in case some stack manipulation is needed.
    * currently only used with the WDC65C816
    */
    virtual QString getInitProcedure() { return "";}
    /*
     * Inserts custom end .asm code when returning from a procedure. Only used on the WDC65C816.
     */
    virtual QString ProcedureEndWithoutReturn() { return "";}



    // Casts the internal register values from one type to another
    virtual void Cast(QString from, QString to);
    virtual void Cast(TokenType::Type from, TokenType::Type to) {}
    virtual void Cast(TokenType::Type from, TokenType::Type to, TokenType::Type writeType) {}

    // If the current system supports banks (Game boy, SNES etc), this method will
    // return the bank number of a given variable
    QString getBank(QSharedPointer<NodeVarType> t);


    /*
     *  Probably the most important method in all of TRSE
     *  Abstract AssignVariable
     *  Handles *all*  A:=B; operations in TRSE
    */

    virtual void AssignVariable(QSharedPointer<NodeAssign> node);

    /* Here follows methods that are used & requirede by assignvariable
       These are currently defined, but will be pure virtual when the method
       complete

    */
    // Method for assigning a record to another, ie "monster1 := monster2;"
    void AssignPureRecords(QSharedPointer<NodeAssign> node );

    /*
     This method is executed before all assign statements.
     The method will impose syntax / grammatical rules, and will raise
     errors if the assignment is incorrect.
     */
    void ValidateAssignStatement(QSharedPointer<NodeAssign> node);

    // Virtual methods
    // note that not all methods below are implemented on all systems.

    // assign a variable from an internal register: var := _ax;
    virtual void AssignFromRegister(QSharedPointer<NodeAssign> node) {}
    // assign a register to a variable: _ax := var;
    virtual void AssignToRegister(QSharedPointer<NodeAssign> node) {}
    // assigns a string that is created dynamically, for instance
    // str := "HELLO"
    virtual void AssignString(QSharedPointer<NodeAssign> node)  {}
    // Assigning a pure pointer without indices, ie  ptr := #someAddress + val*8 etc;
    virtual bool AssignPointer(QSharedPointer<NodeAssign> node)  { return false;}
    // returns true if assign statement is a:=a+1 or a:=a-1
    virtual bool IsSimpleIncDec(QSharedPointer<NodeAssign> node) {return false;}
    // returns true if assign statement is simple myInt := 1000 or myInt := someOtherInt;
    virtual bool IsSimpleAssignInteger(QSharedPointer<NodeAssign> node) {return false;}
    // returns true if assign statement is a simple p:=#var; or p[i]:=$10;
    virtual bool IsSimpleAssignPointer(QSharedPointer<NodeAssign>node) {return false;}
    // Stores a internal register value to a variable. "Simplified" means that the expression is
    // simple, so optimisations can be performed.
    virtual bool StoreVariableSimplified(QSharedPointer<NodeAssign> n) {return false;}
    // 6502 only. Stores a procedure parameter onto the stack.
    virtual bool StoreStackParameter(QSharedPointer<NodeAssign> n) {return false;}
    // Full-generic assign that is called when all other "simplified" optimisation storing has failed.
    // ie stores the current register value to var [ i*8 ]  etc;
    virtual void GenericAssign(QSharedPointer<NodeAssign> node);
    // Assigns the internal register variable to a pointer with index
    virtual bool IsAssignPointerWithIndex(QSharedPointer<NodeAssign> node)  { return false;}
    // Assigns the internal register variable to an array with index
    virtual bool IsAssignArrayWithIndex(QSharedPointer<NodeAssign> node) { return false;}


    // Method that will increase a variable in <var> with the value in <step>
    // used in for loops
    void IncreaseCounter(QSharedPointer<Node> step, QSharedPointer<Node> var);



    // handles for loops that are onpage, enabling the use of relative <128 byte branches
    void SmallLoop(QSharedPointer<NodeForLoop> node, QSharedPointer<NodeVar> var, bool inclusive);

    // handles for loops that are offpage, using slower jumps for >=128 byte branches
    void LargeLoop(QSharedPointer<NodeForLoop> node, QSharedPointer<NodeVar> var, bool inclusive);



    /* VarOrNum
     *
     * Used in the beginning of node assignment: if the variable
     * assigned to is a *number* (memory address), simply replace
     * name of the variable with the number value. Used in
     * screen_bg_col:=red etc where "screen_bg_col" is a memory address (const)
     *
     */
    QSharedPointer<NodeVar> VarOrNum(QSharedPointer<Node> node);

    /*
     * Main method for handling compound binary clauses, ie
     * if (a>b and (c!=d or (j!=0))) then ....
     * Makes sure that every clause is evaluated correctly, and jumps to the else block
     * as soon as any conditional fails
    */
    void HandleCompoundBinaryClause(QSharedPointer<Node> node, QString lblFailed, QString lblSuccess, bool offpage);
    /*
     * Method that builds a single conditional, such as
     * a > b
     * or
     * c != d
     * etc.
     * Used by "HandleCompoundBinaryClause"
    */
    virtual void BuildConditional(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool page)  = 0;

    /*
     *  Sometimes compound binary clauses can be optimised. Only used on the 6502 for now.
     *
     */
    virtual void OptimizeBinaryClause(QSharedPointer<Node> node) {}



    /*
     * This method estimates the code size of a NodeBlock (only on the 6502/Z80)
     * Returns "true" if the code size is >128
     * Used to decide whether to use SmallLoop or LargeLoop etc in for loops, but also
     * all conditionals/while loops etc
    */
    bool isOffPage(QSharedPointer<Node> node, QSharedPointer<Node> b1, QSharedPointer<Node> b2);

    /*
     * The following two methods are used in for loops. For a for loop such as:
     *    for i:=0 to 100 step 2 do ....   the parameters are
     *
     * nodeA : NodeAssign, the assign statement i:=0
     * nodeB : the target, "100" (NodeNumber in this case)
     * step : the step value "2"
     *
    */
    virtual void CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QString lblJump, bool isOffPage) = 0;
    virtual void CompareAndJumpIfNotEqualAndIncrementCounter(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive) =0;



    /*
     *  Inserts custom .asm code at every procedure start/end.
     *
     */
    virtual void ProcedureStart() { }
    virtual void ProcedureEnd() { }

    /*
     * Prints out the "incbin" command for the current cpu/assembler.
     * Will also perform a compression of data when the "compressed" flag is used.
     * Can also split data into multiple files etc.
     */
    virtual void IncBin(QSharedPointer<NodeVarDecl> node);

    /*
     * If a procedure is declared as inline, the code block is printed out
     * directly. This method handles all inline procedures.
     */
    void InlineProcedure(QSharedPointer<NodeProcedure> p);

    /*
     *  Will declare a pointer on the current CPU, ie
     *  ptr = $40 etc on the 6502
     *  ptr dc.w 0   on the z80
     *  ptr dc.w 0,0  on the x86
     */
    virtual void DeclarePointer(QSharedPointer<NodeVarDecl> node) = 0;

    /*
     *  methods that handles copying of pure records/objects, ie
     *  data1 := data2; // data1/data2 are classes/records
     */
    void HandleNodeAssignCopyRecord(QSharedPointer<NodeAssign> node);
    void HandleNodeAssignCopyClass(QSharedPointer<NodeAssign> node);

    /*
     * LineNumber will print out the current .ras line number in the generated .asm
     *
     */
    void LineNumber(int ln);



    /*
     * The following two methods are used in BuildConditional, and will
     * either return the lo/hi/super hi bytes of a comparison value.
     * Only used on the 6502.
    */
    virtual bool Evaluate16bitExpr(QSharedPointer<Node> node, QString &lo, QString &hi);
    virtual bool Evaluate24bitExpr(QSharedPointer<Node> node, QString &lo, QString &hi, QString &z);

    /*
     * The two following methods are syntactically the same as
     * n->getValue(as);
     * but will override the call if the current value is an inline function parameter
     */
    QString getValue(QSharedPointer<Node> n);
    QString getValue8bit(QSharedPointer<Node> n, int isHi);

    virtual void PopLostStack(int cnt) {}

    // Method that defines a temporary string. Only used on the 6502, marked obsolete.
    virtual QString DefineTempString(QSharedPointer<Node> node);

public:
signals:
    // Emits a compile tick to give user feedback
    void EmitTick(QString val);

};

#endif // AbstractCodeGen_H
