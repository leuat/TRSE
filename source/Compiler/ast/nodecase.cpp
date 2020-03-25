#include "nodecase.h"

NodeCase::NodeCase(Token t)
{
    m_op = t;
}

void NodeCase::Delete()
{
    Node::Delete();
    for (Node* n : m_conditionals) {
        n->Delete();
        delete n;
    }
    m_conditionals.clear();
    for (Node* n : m_statements) {
        n->Delete();
        delete n;
    }
    m_statements.clear();

    if (m_variable!=nullptr) {
        m_variable->Delete();
        delete m_variable;
    }
    m_variable = nullptr;

    if (m_elseBlock!=nullptr) {
        m_elseBlock->Delete();
        delete m_elseBlock;
    }
    m_elseBlock= nullptr;

}
