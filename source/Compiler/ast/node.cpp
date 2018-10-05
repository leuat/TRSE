#include "node.h"

int Node::m_currentLineNumber;
MemoryBlockInfo  Node::m_staticBlockInfo;
MemoryBlock* Node::m_curMemoryBlock = nullptr;

int Node::MaintainBlocks(Assembler* as)
{
//    qDebug() << "Build:" << m_blockID << " and " << as->m_currentBlock << " at " << m_currentLineNumber;
//    qDebug() << "bid:" << m_blockID << "  ";
    if (m_blockInfo.m_blockID == -1) {
        if (as->m_currentBlock!=nullptr) {
    //        qDebug() << "Ending block!" << m_blockID << " at " << as->m_currentBlock->m_pos;
  //          qDebug() << as->m_currentBlock->m_source;
            as->EndMemoryBlock(); // Make sure it is memoryblock!
            return 2;
        }
        return 0;
    }
    if (as->m_currentBlock==nullptr) {
//        qDebug() << "Starting block at " << m_blockPos ;
        as->StartMemoryBlock(m_blockInfo.m_blockPos);
        return 1;
    }

    if (as->m_currentBlock!=nullptr) {
        if (m_blockInfo.m_blockPos!=as->m_currentBlock->m_pos) {
            as->StartMemoryBlock(m_blockInfo.m_blockPos);
            return 3;
        }

    }
    return 0;

}

void Node::Delete() {
    if (m_left!=nullptr) {
        m_left->Delete();
        delete m_left;
        m_left = nullptr;
    }
    if (m_right!=nullptr) {
        m_right->Delete();
        delete m_right;
        m_left = nullptr;

    }
}

QString Node::Build(Assembler *as) {
    m_currentLineNumber = m_op.m_lineNumber;

    return "";
}

void Node::RequireAddress(Node *n, QString name, int ln) {
    if (!n->isAddress())
        ErrorHandler::e.Error(name + " requires parameter to be memory address. Did you forget a '^' symbol such as ^$D800?", ln);
}

bool Node::verifyBlockBranchSize(Assembler *as, Node *testBlock)
{
    AsmMOS6502 tmpAsm;
    tmpAsm.m_symTab = as->m_symTab;
    testBlock->Build(&tmpAsm);
    //qDebug() << "block count:" << tmpAsm.m_source.count();
    int blockCount = tmpAsm.m_source.count();
    return blockCount<80;

}
