#ifndef NODEPROCEDURE_H
#define NODEPROCEDURE_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodeproceduredecl.h"
#include "source/Compiler/ast/nodevar.h"
#include "source/Compiler/ast/nodeassign.h"

class NodeProcedure : public Node {
public:
    NodeProcedureDecl* m_procedure;
    QVector<Node*> m_parameters;

    NodeProcedure(NodeProcedureDecl* proc, QVector<Node*> params, Token t );

    void Delete() override;


    QString Build(Assembler* as) override;

    PVar Execute(SymbolTable *symTab, uint lvl) override;
    void ExecuteSym(SymbolTable* symTab) override;

};


#endif // NODEPROCEDURE_H
