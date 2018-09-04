#include "nodewhileloop.h"

NodeWhileLoop::NodeWhileLoop()
{

}

void NodeWhileLoop::Delete() {
    if (m_block) {
        m_block->Delete();
        delete m_block;
        m_block = nullptr;
    }


}

void NodeWhileLoop::ExecuteSym(SymbolTable *symTab) {
    m_block->ExecuteSym(symTab);
}
