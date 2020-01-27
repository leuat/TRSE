/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

#include "node.h"
#include "source/Compiler/assembler/astdispather6502.h"

int Node::m_currentLineNumber;
MemoryBlockInfo  Node::m_staticBlockInfo;
MemoryBlock* Node::m_curMemoryBlock = nullptr;

QMap<QString, bool> Node::flags;
SymbolTable* Node::parserSymTab;

void Node::DispatchConstructor(Assembler *as) {
    //        m_blockInfo = m_staticBlockInfo;s
    m_currentLineNumber = m_op.m_lineNumber;
    bool ok = true;
/*    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::AMIGA)
        ok = false;
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::X86)
        ok = false;
*/
//    qDebug() << Syntax::s.m_currentSystem->m_system << m_comment;
    if ((m_comment!="") && ok) {
        QString c = m_comment;//.replace("//","\n").replace("/*","\n").replace("\n","\n; //");
        c = c.replace("//","\n").replace("/*","\n").replace("\n","\n; //");
        if (!c.trimmed().startsWith(";"))
            c = ";" + c;

        as->Asm(c);
//        qDebug() << c;

    }

}

int Node::MaintainBlocks(Assembler* as)
{
//    if (as->m_currentBlock!=nullptr)
  //      qDebug() << as->m_currentBlock->m_pos;

    if (m_blockInfo.m_blockID == -1) {
        if (as->m_currentBlock!=nullptr) {
    //        qDebug() << "Ending blocks " << as->m_currentBlock->m_pos ;
            as->EndMemoryBlock(); // Make sure it is memoryblock!
            return 2;
        }
        return 0;
    }
    if (as->m_currentBlock==nullptr) {
      //  qDebug() << "Starting block at " << m_blockInfo.m_blockPos ;
        as->StartMemoryBlock(m_blockInfo.m_blockPos);
        return 1;
    }
    if (as->m_currentBlock!=nullptr) {
        if (m_blockInfo.m_blockPos!=as->m_currentBlock->m_pos) {
        //    qDebug() << "Switchingblocks at " << m_blockInfo.m_blockPos << as->m_currentBlock->m_pos ;
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


void Node::RequireAddress(Node *n, QString name, int ln) {
    if (!n->isAddress())
        ErrorHandler::e.Error(name + " requires parameter to be memory address. Did you forget a '^' symbol such as ^$D800?", ln);
}

bool Node::verifyBlockBranchSize(Assembler *as, Node *testBlock)
{
    AsmMOS6502 tmpAsm;
    tmpAsm.m_symTab = as->m_symTab;
    ASTDispather6502 dispatcher;
    dispatcher.as = &tmpAsm;
    testBlock->Accept(&dispatcher);
//    testBlock->Build(&tmpAsm);
//    qDebug() << "block count:" << tmpAsm.m_source.count();
    int blockCount = tmpAsm.m_source.count();
    return blockCount<80;

}
