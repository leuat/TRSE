#include "nodefactory.h"

NodeFactory::NodeFactory()
{

}

QSharedPointer<NodeNumber> NodeFactory::CreateNumber(Token t,int i)
{
    t.m_type=TokenType::INTEGER_CONST;
    t.m_intVal = i;
    return QSharedPointer<NodeNumber>(new NodeNumber(t,i));
}

QSharedPointer<NodeVar> NodeFactory::CreateVariable(Token t,QString v)
{
    t.m_type=TokenType::ID;
    t.m_value = v;
    t.m_intVal = 0;

    return QSharedPointer<NodeVar>(new NodeVar(t));

}

QSharedPointer<Node> NodeFactory::CreateBinop(Token t,TokenType::Type tt, QSharedPointer<Node> left, QSharedPointer<Node> right)
{
    t.m_type = tt;
    if (t.m_type==TokenType::PLUS) {
        if (left==nullptr)
            return right;
        if (right==nullptr)
            return left;
    }
    if (left->isPureNumeric() && right->isPureNumeric()) {
        if (t.m_type==TokenType::PLUS)
            return CreateNumber(t,left->numValue()+right->numValue());
        if (t.m_type==TokenType::MINUS)
            return CreateNumber(t,left->numValue()-right->numValue());
        if (t.m_type==TokenType::MUL)
            return CreateNumber(t,left->numValue()*right->numValue());
        if (t.m_type==TokenType::DIV)
            return CreateNumber(t,left->numValue()/right->numValue());
   }

    return QSharedPointer<NodeBinOP>(new NodeBinOP(left,t,right));
}

QSharedPointer<NodeAssign> NodeFactory::CreateAssign(Token t,QSharedPointer<Node> left, QSharedPointer<Node> right)
{
    t.m_type = TokenType::ASSIGN;
    return QSharedPointer<NodeAssign>(new NodeAssign(left,t,right));

}

