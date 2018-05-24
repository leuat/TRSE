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

    NodeProcedure(NodeProcedureDecl* proc, QVector<Node*> params, Token t ) {
        m_parameters = params;
        m_procedure = proc;
        m_op = t;
    }

    void Delete() {
        ErrorHandler::e.DebugLow("Memory: Deleting in NodeProcedure", level);
        if (m_procedure) {
            //m_procedure->Delete();
            //delete m_procedure;
            //m_procedure = nullptr;
        }
        for (Node* n : m_parameters) {
            n->Delete();
            delete n;
        }
        m_parameters.clear();
        ErrorHandler::e.DebugLow("Memory DONE: Deleting in NodeProcedure", level);
    }


    QString Build(Assembler* as) override {
        Node::Build(as);

        if (m_parameters.count()!=m_procedure->m_paramDecl.count())
            ErrorHandler::e.Error("Procedure '" + m_procedure->m_procName+"' requires "
            + QString::number(m_procedure->m_paramDecl.count()) +" parameters, not "
            + QString::number(m_parameters.count()) + ".", m_op.m_lineNumber);

        for (int i=0; i<m_parameters.count();i++) {
            // Assign all variables
            NodeVarDecl* vd = (NodeVarDecl*)m_procedure->m_paramDecl[i];
            NodeAssign* na = new NodeAssign(vd->m_varNode, m_parameters[i]->m_op, m_parameters[i]);
            na->Build(as);
        }

        as->Asm("jsr " + m_procedure->m_procName);
        return "";
    }

    PVar Execute(SymbolTable *symTab, uint lvl) override;
    void ExecuteSym(SymbolTable* symTab) override {
        m_procedure ->ExecuteSym(symTab);
    }

};


#endif // NODEPROCEDURE_H
