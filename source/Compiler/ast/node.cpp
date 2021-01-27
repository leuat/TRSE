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
#include "source/Compiler/assembler/mos6502/astdispatcher6502.h"

int Node::m_currentLineNumber;
MemoryBlockInfo  Node::m_staticBlockInfo;
QSharedPointer<MemoryBlock> Node::m_curMemoryBlock = nullptr;
QString Node::sForceFlag = "";

QMap<QString, bool> Node::flags;
QSharedPointer<SymbolTable>  Node::parserSymTab;

//QMap<QSharedPointer<Node>, QSharedPointer<Node>> Node::s_uniqueSymbols;

void Node::SwapNodes() {
    QSharedPointer<Node> n = m_left;
    m_left = m_right;
    m_right = n;
}

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
        c = c.replace("//","\n").replace("/*","\n").replace("\n","\n; //").replace("//*","\n");
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

//    as->Asm(";"+QString::number(m_blockInfo.m_blockID) + "  " + m_op.getType() + " " +m_op.getNumAsHexString());
    if (m_blockInfo.m_blockID == -1) {
        if (as->m_currentBlock!=nullptr && (!as->m_currentBlock->m_isMainBlock)) {
            //qDebug() << "Ending blocks " << as->m_currentBlock->m_pos ;
            // If things fuck up, turn on this again
            as->EndMemoryBlock(); // Make sure it is memoryblock!
//            m_curMemoryBlock =
//            m_curMemoryBlock = as->blocks.last();
  //           if (as->m_currentBlock!=nullptr)
    //           qDebug() << " AFTER ENDING, CURRENT BLOCK IS " << as->m_currentBlock->m_pos;
            return 2;
        }
        return 0;
    }
    if (as->m_currentBlock==nullptr) {
        //qDebug() << "Starting block at " << m_blockInfo.m_blockPos ;
        as->StartMemoryBlock(m_blockInfo.m_blockPos);
        return 1;
    }
    if (as->m_currentBlock!=nullptr) {
        if (m_blockInfo.m_blockPos!=as->m_currentBlock->m_pos) {
          //  qDebug() << "Switchingblocks at " << m_blockInfo.m_blockPos << as->m_currentBlock->m_pos ;
            as->StartMemoryBlock(m_blockInfo.m_blockPos);
            return 3;
        }

    }
    return 0;

}



void Node::RequireAddress(QSharedPointer<Node> n, QString name, int ln) {
    if (!n->isAddress()) {
        ErrorHandler::e.Error(name + " requires parameter to be memory address. Did you forget a '^' symbol such as ^$D800?", ln);
    }
}

bool Node::verifyBlockBranchSize(Assembler *as, QSharedPointer<Node> testBlockA,QSharedPointer<Node> testBlockB, AbstractASTDispatcher* dispatcher)
{
    //QSharedPointer<Assembler> as =
//    AsmMOS6502 tmpAsm;
  //  tmpAsm.m_symTab = as->m_symTab;
//    ASTDispatcher6502 dispatcher;
//    dispatcher.as = &tmpAsm;
    auto app = QSharedPointer<Appendix>(new Appendix);
    auto keep = as->m_currentBlock;
    as->m_currentBlock = app;
    QStringList keepTemps = as->m_tempVars;
    if (testBlockA!=nullptr)
        testBlockA->Accept(dispatcher);
    if (testBlockB!=nullptr)
        testBlockB->Accept(dispatcher);
    as->m_tempVars = keepTemps;

    int count = as->CodeSizeEstimator(app->m_source);
    as->m_currentBlock = keep;


    return count<120;

}

TokenType::Type Node::VerifyAndGetNumericType() {
   return m_op.m_type;
}

void Node::VerifyReferences(Assembler *as) {
    if (m_left!=nullptr)
        m_left->VerifyReferences(as);
    if (m_right!=nullptr)
        m_right->VerifyReferences(as);

}

bool Node::isSigned(Assembler *as) {
    bool isSigned = false;
    if (m_left!=nullptr)
        isSigned |= m_left->isSigned(as);
    if (m_right!=nullptr)
        isSigned |= m_right->isSigned(as);

    return isSigned;
}
