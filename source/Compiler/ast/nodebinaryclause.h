#ifndef NODEBINARYCLAUSE_H
#define NODEBINARYCLAUSE_H

#include "node.h"


class NodeBinaryClause : public Node
{
public:
    NodeBinaryClause(Token op, Node* left, Node* right) {
        m_right = right;
        m_left = left;
        m_op = op;
    }

    PVar Execute(SymbolTable *symTab, uint lvl) override{
        return PVar();
    }

    void ExecuteSym(SymbolTable* sym) override;

    void BuildToCmp(Assembler* as);

    void BuildSimple(Assembler* as, QString failedLabel);


    bool cannotBeSimplified(Assembler* as);

//    void OnlyNumVar(Assembler* as, QString a, QString b);

    void BinaryClause(Assembler* as );
    void BinaryClauseInteger(Assembler* as );
    void LogicalClause(Assembler* as );


    // Input: left, right
    // Output: a = 0, 1
    QString Build(Assembler *as) override;



};

#endif // NODEBINARYCLAUSE_H
