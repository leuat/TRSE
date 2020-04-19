#include "abstractastdispatcher.h"
#include "source/Compiler/ast/node.h"

AbstractASTDispatcher::AbstractASTDispatcher()
{

}

void AbstractASTDispatcher::dispatch(QSharedPointer<NodeBlock> node) {
    if (m_ticks++%4==0)
        emit EmitTick(".");
}

QString AbstractASTDispatcher::getValue(QSharedPointer<Node> n) {
    return n->getValue(as);
}

