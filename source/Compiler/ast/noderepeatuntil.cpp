#include "noderepeatuntil.h"

NodeRepeatUntil::NodeRepeatUntil(Token op, int forcePage, Node *cond, Node *block)
{
    m_op = op;
    m_forcePage = forcePage;
    m_block = block;
    m_conditional = cond;
}

void NodeRepeatUntil::Delete()
{

}
