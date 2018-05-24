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
    Node* m_compoundStatement = nullptr;
    SymbolTable* m_symTab = nullptr;
    bool m_useOwnSymTab;

    NodeBlock(Token t, QVector<Node*> decl, Node* comp, bool useOwnSymTab = true) {
        m_compoundStatement = comp;
        m_decl = decl;
        m_op = t;
        m_useOwnSymTab = useOwnSymTab;
    }

    void SetParameter(QString name, PVar var) {
        Symbol* s = m_symTab->Lookup(name, m_op.m_lineNumber);
//        if (s==nullptr)
  //          ErrorHandler::e.Error("Could not find variable :" + name,m_op.m_lineNumber);

        s->m_value = new PVar(var);
    }
    void Delete() override {
        Node::Delete();
        if (m_useOwnSymTab && m_symTab!=nullptr) {
            m_symTab->Delete();
        }
        if (m_compoundStatement!=nullptr) {
            m_compoundStatement->Delete();
            m_compoundStatement = nullptr;
        }
        for (Node* n : m_decl) {
            n->Delete();
            delete n;
        }
        m_decl.clear();

    }

    void PopZeroPointers(Assembler* as) {
        for (Node* n: m_decl) {
            NodeVarDecl* nv = dynamic_cast<NodeVarDecl*>(n);
            if (nv!=nullptr) {
                for (int i=0;i<nv->m_pushedPointers;i++)
                    as->PopZeroPointer();
            }
        }

    }

   QString Build(Assembler* as) {
       //as->VarDeclHeader();
       Node::Build(as);
       as->PushBlock(m_currentLineNumber);



       QString label = as->NewLabel("block");
       as->Asm("jmp " + label);
       bool blockLabel = false;
       bool blockProcedure = false;
       if (m_decl.count()!=0) {
//           as->PushBlock(m_decl[0]->m_op.m_lineNumber-1);
       }
        for (Node* n: m_decl) {
            // Print label at end of vardecl
            if (dynamic_cast<NodeVarDecl*>(n)==nullptr) {
                if (!blockLabel)
                    as->Label(label);
                blockLabel = true;

                if (!blockProcedure) // Print label at end of vardecl
                {
                    if (n->m_op.m_lineNumber!=0) {
  //                      as->PopBlock(n->m_op.m_lineNumber);
                        blockProcedure = true;
                     //   qDebug() << "pop" << n->m_op.m_lineNumber << " " << TokenType::getType(n->getType(as));
                    }

                }

            }
                //if (dynamic_cast<NodeProcedureDecl*>(n)==nullptr)
            //qDebug() << "VarDeclBuild:" ;
            n->Build(as);
        }
        as->VarDeclEnds();
        as->PushCounter();

        if (!blockLabel)
            as->Label(label);
        if (m_compoundStatement!=nullptr)
            m_compoundStatement->Build(as);


        as->PopCounter(m_op.m_lineNumber-1);
        as->PopBlock(m_currentLineNumber);

        PopZeroPointers(as);

//        as->PopAllZeroPointers();

//        qDebug() << "ln:" << m_currentLineNumber;
        //qDebug() << "Adding at linenumber: " << m_op.m_lineNumber << "  cycles " << m_cycleCounter;
        return "";

    }

    PVar Execute(SymbolTable* symTab, uint lvl) override;

    void ExecuteSym(SymbolTable* symTab) override;


};

#endif // NODEBLOCK_H
