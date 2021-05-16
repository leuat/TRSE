#include "abstractmethods.h"
#include "source/Compiler/ast/nodenumber.h"
#include "source/Compiler/ast/nodebinop.h"
#include "source/Compiler/ast/nodeassign.h"


AbstractMethods::AbstractMethods()
{

}

void AbstractMethods::IncDec(Assembler *as, QString cmd)
{

    /*
         * KEEP IT SIMPLE
         *
         * Replaces inc(P) with P =P + 1 syntactically (and dec)
         *
        */
    Token t = m_node->m_op;
    t.m_type = TokenType::PLUS;
    if (cmd.toLower()=="dec")
        t.m_type = TokenType::MINUS;
    Token n = m_node->m_op;
    n.m_type=TokenType::INTEGER_CONST;
    Token na = m_node->m_op;
    na.m_type=TokenType::INTEGER_CONST;
    // manually building  P0 := P0 +- 1;
    auto num = QSharedPointer<NodeNumber>(new NodeNumber(n,1));
    auto org = m_node->m_params[0];
    auto var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var!=nullptr) {
        if (var->m_subNode==nullptr)
            var->value = var->getValue(as);
    }
    auto bop = QSharedPointer<NodeBinOP>(new NodeBinOP(org,t,num));
    auto assign = QSharedPointer<NodeAssign>(new NodeAssign(org,na,bop));
    assign->Accept(m_codeGen);


    return;

}
