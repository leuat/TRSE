#include "abstractastdispatcher.h"

AbstractASTDispatcher::AbstractASTDispatcher()
{

}

void AbstractASTDispatcher::dispatch(QSharedPointer<NodeBlock> node) {
    if (m_ticks++%4==0)
        emit EmitTick(".");
}
