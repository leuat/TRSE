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
    QStringList m_initJumps;
    NodeBlock* m_NodeBlock;
    NodeProgram(QString n, NodeBlock* b):Node() {
        m_NodeBlock = b;
        m_name = n;
    }




    void Delete() override;

    PVar Execute(SymbolTable* symTab, uint lvl) override;
    void ExecuteSym(SymbolTable* symTab) override;

    QString Build(Assembler* a) override;

};

#endif // NODEPROGRAM_H
