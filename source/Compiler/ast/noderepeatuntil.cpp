#include "noderepeatuntil.h"

NodeRepeatUntil::NodeRepeatUntil(Token op, int forcePage, NodeBinaryClause *cond, Node *block)
{
    m_op = op;
    m_forcePage = forcePage;
    m_block = block;
    m_clause = cond;
}

void NodeRepeatUntil::Delete()
{
    Node::Delete();
    if (m_clause!=nullptr) {
        m_clause->Delete();
        delete m_clause;
        m_clause = nullptr;
    }
    if (m_block!=nullptr) {
        m_block->Delete();
        delete m_block;
        m_block = nullptr;
    }
}
