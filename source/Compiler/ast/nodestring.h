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
    NodeString(Token op, QStringList val, bool isCString) {
        m_op = op;
        m_val = val;
        m_isCString = isCString;
    }
    PVar  Execute(SymbolTable* symTab, uint lvl) override {
        Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
        level = lvl +1;
        return PVar();
    }

    QString Build(Assembler *as) override {
       Node::Build(as);

       as->String(m_val);
       return "";
    }

    void ExecuteSym(SymbolTable* symTab) override {
    }
};

#endif // NODESTRING_H
