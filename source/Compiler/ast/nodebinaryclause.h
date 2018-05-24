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

    void ExecuteSym(SymbolTable* sym) {
        if (m_left!=nullptr)
            m_left->ExecuteSym(sym);
        if (m_right!=nullptr)
            m_left->ExecuteSym(sym);
    }

    void BuildToCmp(Assembler* as);

    void BuildSimple(Assembler* as, QString failedLabel);


    bool cannotBeSimplified(Assembler* as) {
       // can NOT be simplified
        return ((m_op.m_type==TokenType::AND || m_op.m_type == TokenType::OR)
                || m_left->getType(as)==TokenType::INTEGER || m_right->getType(as)==TokenType::INTEGER

                );
    }

//    void OnlyNumVar(Assembler* as, QString a, QString b);

    void BinaryClause(Assembler* as );
    void BinaryClauseInteger(Assembler* as );
    void LogicalClause(Assembler* as );


    // Input: left, right
    // Output: a = 0, 1
    QString Build(Assembler *as) override;



};

#endif // NODEBINARYCLAUSE_H
