#ifndef NODEBLOCK_H
#define NODEBLOCK_H



#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodevar.h"
//#include "source/Compiler/ast/nodeproceduredecl.h"
#include "source/Compiler/ast/nodevardecl.h"

class NodeBlock : public Node {
public:
    QVector<Node*> m_decl;
    QString forceLabel = "";
    Node* m_compoundStatement = nullptr;
    SymbolTable* m_symTab = nullptr;
    bool m_useOwnSymTab;

    NodeBlock(Token t, QVector<Node*> decl, Node* comp, bool useOwnSymTab = true):Node() {
        m_compoundStatement = comp;
        m_decl = decl;
        m_op = t;
        m_useOwnSymTab = useOwnSymTab;
    }

    void SetParameter(QString name, PVar var);
    void Delete() override;

    void PopZeroPointers(Assembler* as);

   QString Build(Assembler* as) override;

    PVar Execute(SymbolTable* symTab, uint lvl) override;

    void ExecuteSym(SymbolTable* symTab) override;


};

#endif // NODEBLOCK_H
