#ifndef NODEPROGRAM_H
#define NODEPROGRAM_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodebuiltinmethod.h"
#include "source/Compiler/ast/nodeblock.h"
#include "source/Compiler/ast/nodeproceduredecl.h"

class NodeProgram : public Node {
public:
    QString m_name;
    NodeBlock* m_NodeBlock;
    NodeProgram(QString n, NodeBlock* b) {
        m_NodeBlock = b;
        m_name = n;
    }




    void Delete() override {
        if (m_NodeBlock) {
            m_NodeBlock->Delete();
            delete m_NodeBlock;
            m_NodeBlock = nullptr;
        }
    }

    PVar Execute(SymbolTable* symTab, uint lvl) override;
    void ExecuteSym(SymbolTable* symTab) override {
        m_NodeBlock->ExecuteSym(symTab);
    }

    QString Build(Assembler* a) {
        Node::Build(a);

        NodeBuiltinMethod::m_isInitialized.clear();
        a->Program(m_name);
        m_NodeBlock->Build(a);
        a->EndProgram();
        return "";
    }

};

#endif // NODEPROGRAM_H
