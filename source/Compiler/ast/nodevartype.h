#ifndef NODEVARTYPE_H
#define NODEVARTYPE_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodevar.h"

class NodeVarType : public Node {
public:
    QString value;
    Token m_arrayVarType;
    QStringList m_data;
    QString m_filename, m_position;
    QString initVal ="0";

    NodeVarType(Token t, QString position,   Token arrayVarType,QStringList data);
    NodeVarType(Token t, QStringList data);
    NodeVarType(Token t,  QString filename, QString position );
    NodeVarType(Token t,  QString initvalue );
    PVar Execute(SymbolTable* symTab, uint lvl) override;
    void ExecuteSym(SymbolTable* symTab) override {

    }

};
#endif // NODEVARTYPE_H
