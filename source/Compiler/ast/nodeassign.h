#ifndef NODEASSIGN_H
#define NODEASSIGN_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodevar.h"
#include "source/Compiler/ast/nodestring.h"
#include "source/Compiler/ast/nodevararray.h"
#include "source/Compiler/ast/nodebinop.h"


//class NodeBinop;

class NodeAssign : public Node {
public:
    Node* m_arrayIndex = nullptr;


    NodeAssign(Node* left, Token t, Node* r) {
        m_right = r;
        m_op = t;
        m_left = left;
    }

    PVar Execute(SymbolTable* symTab, uint lvl) override;
    void AssignString(Assembler *as);

    void AssignPointer(Assembler* as);

    bool isSimpleAeqAOpB(NodeVar* var, Assembler* as);


    bool IsSimpleIncDec(NodeVar* var, Assembler* as);

    QString AssignVariable(Assembler* as);

    QString Build(Assembler* as) override;
    void ExecuteSym(SymbolTable* symTab) override;


};

#endif // NODEASSIGN_H
