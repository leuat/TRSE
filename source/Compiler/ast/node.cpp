#include "node.h"

int Node::m_currentLineNumber;

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
