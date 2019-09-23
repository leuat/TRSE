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

}
