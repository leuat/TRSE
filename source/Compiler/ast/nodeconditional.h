#ifndef NODECONDITIONAL_H
#define NODECONDITIONAL_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodebinaryclause.h"
#include <QVector>

class NodeConditional : public Node {
public:

//    QVector<Node*> m_a, m_b;

    Node* m_block = nullptr;
    Node* m_elseBlock = nullptr;
    Node* m_binaryClause = nullptr;
/*    QVector<Token> m_compares;
    QVector<Token> m_conditionals;*/
    bool m_isWhileLoop;


    int m_forcePage = 0;
/*    NodeConditional(QVector<Token> op, QVector<Node*> a, QVector<Node*> b, Node* block, bool isWhile, QVector<Token> conditionals, Node* elseBlock=nullptr) {
        m_a = a;
        m_b = b;
        m_block = block;
        m_compares = op;
        m_isWhileLoop = isWhile;
        m_elseBlock = elseBlock;
        m_conditionals = conditionals;
    }*/
    NodeConditional(Token op, int forcePage, Node* clause, Node* block, bool isWhile, Node* elseBlock=nullptr) {
        m_block = block;
        m_isWhileLoop = isWhile;
        m_elseBlock = elseBlock;
        m_binaryClause = clause;
        m_op = op;
        m_forcePage = forcePage;
    }

/*    void ConditionalTryFail(Assembler* , QString labelFail, int i);
    void ConditionalTrySuccess(Assembler* , QString labelFail, int i);
*/



    void Delete() override {
        /*if (m_a) {
            m_a->Delete();
            delete m_a;
            m_a = nullptr;
        }
        if (m_b) {
            m_b->Delete();
            delete m_b;
            m_b = nullptr;
        }*/
        if (m_block) {
            m_block->Delete();
            delete m_block;
            m_block = nullptr;
        }


    }

    PVar Execute(SymbolTable* symTab, uint lvl) override;

    void ExecuteSym(SymbolTable* symTab) override {
       m_block->ExecuteSym(symTab);
    }

    QString Build(Assembler *as) override;

};

#endif // NODECONDITIONAL_H
