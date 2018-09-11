#ifndef NODESTRING_H
#define NODESTRING_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"

class NodeString : public Node {
public:
    QStringList m_val;
    bool m_isCString = false;
    NodeString(Token op, QStringList val, bool isCString):Node() {
        m_op = op;
        m_val = val;
        m_isCString = isCString;
    }
    PVar  Execute(SymbolTable* symTab, uint lvl) override;

    QString Build(Assembler *as) override;

    void ExecuteSym(SymbolTable* symTab) override {
    }
};

#endif // NODESTRING_H
