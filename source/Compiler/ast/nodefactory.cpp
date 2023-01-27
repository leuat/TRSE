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
    auto v= QSharedPointer<NodeBinOP>(new NodeBinOP(left,t,right));
    return v;
}

QSharedPointer<NodeAssign> NodeFactory::CreateAssign(Token t,QSharedPointer<Node> left, QSharedPointer<Node> right)
{
    t.m_type = TokenType::ASSIGN;
    return QSharedPointer<NodeAssign>(new NodeAssign(left,t,right));

}

QSharedPointer<NodeAsm> NodeFactory::CreateAsm(Token t, QString text)
{
    t.m_value = text;
    return QSharedPointer<NodeAsm>(new NodeAsm(t));

}

QSharedPointer<NodeCompound> NodeFactory::CreateCompound(Token t)
{
    return QSharedPointer<NodeCompound>(new NodeCompound(t));
}

QSharedPointer<NodeBlock> NodeFactory::CreateBlock(Token t, QSharedPointer<Node> comp)
{
    return QSharedPointer<NodeBlock>(new NodeBlock(t,QVector<QSharedPointer<Node>>(),
                                                                                        comp, false));

}

QSharedPointer<NodeBinaryClause> NodeFactory::CreateBinaryClause(Token t, TokenType::Type clause, QSharedPointer<Node> left, QSharedPointer<Node> right) {
    t.m_type = clause;
    return QSharedPointer<NodeBinaryClause>(new NodeBinaryClause(t,left, right));
}

QSharedPointer<NodeConditional> NodeFactory::CreateConditional(Token t, QSharedPointer<Node> clause, QSharedPointer<Node> block, bool isLarge) {
    return QSharedPointer<NodeConditional>(
                new NodeConditional(t,isLarge,clause,block,false));
}

QSharedPointer<NodeBuiltinMethod> NodeFactory::CreateBuiltin(Token t, QString procName, QVector<QSharedPointer<Node>> paramList)
{
    auto n = QSharedPointer<NodeBuiltinMethod>(new NodeBuiltinMethod(procName,paramList,&Syntax::s.builtInFunctions[procName]));
    n->m_op.m_lineNumber = t.m_lineNumber;
    return n;

}

QSharedPointer<NodeBlock> NodeFactory::CreateBlockFromStatements(Token t, QVector<QSharedPointer<Node> > statementlist) {
    auto comp = CreateCompound(t);
    comp->children = statementlist;
    return CreateBlock(t,comp);
}

QSharedPointer<NodeConditional> NodeFactory::CreateSingleConditional(Token t,TokenType::Type clause,  bool isLarge, QSharedPointer<Node> left, QSharedPointer<Node> right, QSharedPointer<Node> block) {
    auto binaryclause = CreateBinaryClause(t,clause,left,right);
    return CreateConditional(t,binaryclause,block,isLarge);
}

QSharedPointer<NodeCast> NodeFactory::NodeFactory::CreateCast(Token t, QSharedPointer<Node> node)
{
    return QSharedPointer<NodeCast>(new NodeCast(t,node));

}

