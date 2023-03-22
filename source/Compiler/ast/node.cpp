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
#include "source/Compiler/codegen/codegen_6502.h"


int Node::m_currentLineNumber;
MemoryBlockInfo  Node::m_staticBlockInfo;
QSharedPointer<MemoryBlock> Node::m_curMemoryBlock = nullptr;
QString Node::sForceFlag = "";
Assembler* Node::s_as;

QMap<QString, bool> Node::flags;
QSharedPointer<SymbolTable>  Node::parserSymTab;
uint Node::s_nodeCount = 0;
//QMap<QSharedPointer<Node>, QSharedPointer<Node>> Node::s_uniqueSymbols;

void Node::SwapNodes() {
    if (m_hasSwapped)
        return;
    QSharedPointer<Node> n = m_left;
    m_left = m_right;
    m_right = n;
    m_hasSwapped = true;
}

void Node::ReplaceInline(Assembler* as,QMap<QString, QSharedPointer<Node> >& inp)
{
    if (m_left != nullptr)
        m_left->ReplaceInline(as,inp);
    if (m_right != nullptr)
        m_right->ReplaceInline(as,inp);
}

TokenType::Type Node::getClassvariableType() {
    TokenType::Type t1=TokenType::NADA,t2=TokenType::NADA;
    if (m_left!=nullptr)
        t1 = m_left->getClassvariableType();
    if (m_left!=nullptr)
        t2 = m_left->getClassvariableType();

    if (t1!=TokenType::NADA) return t1;
    if (t2!=TokenType::NADA) return t2;
    return TokenType::NADA;
}

bool Node::isPure() {
    return isPureNumericOrAddress() || isPureVariable();
}

Node::Node() {
    m_blockInfo = m_staticBlockInfo;
    s_nodeCount++;
}

void Node::DispatchConstructor(Assembler *as, AbstractCodeGen* dispatcher) {
    //        m_blockInfo = m_staticBlockInfo;s
    m_currentLineNumber = m_op.m_lineNumber;
    bool ok = true;
    dispatcher->UpdateDispatchCounter();
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
 //       qDebug() << "Starting block at " << m_blockInfo.m_blockPos ;
        as->StartMemoryBlock(m_blockInfo.m_blockPos);
        return 1;
    }
    if (as->m_currentBlock!=nullptr) {
        if (m_blockInfo.m_blockPos!=as->m_currentBlock->m_pos) {
   //         qDebug() << "Switchingblocks at " << m_blockInfo.m_blockPos << as->m_currentBlock->m_pos ;
            as->StartMemoryBlock(m_blockInfo.m_blockPos);
            return 3;
        }

    }
    return 0;

}

void Node::ForceAddress() {
    m_forceAddress = true;
    if (m_left!=nullptr)
        m_left->ForceAddress();
    if (m_right!=nullptr)
        m_right->ForceAddress();
}

void Node::FindPotentialSymbolsInAsmCode(QStringList &lst)
{
    if (m_left)
        m_left->FindPotentialSymbolsInAsmCode(lst);
    if (m_right)
        m_right->FindPotentialSymbolsInAsmCode(lst);
}



void Node::RequireAddress(QSharedPointer<Node> n, QString name, int ln) {
    if (!n->isAddress()) {
       // ErrorHandler::e.Error("'"+name + "' requires parameter to be a memory address. Did you forget a '^' symbol such as ^$D800?", ln);
    }
}

void Node::RequireNumber(QSharedPointer<Node> n, QString name, int ln) {
    if (!n->isPureNumeric())
        ErrorHandler::e.Error(name + " requires parameter to be pure numeric", ln);
}

bool Node::verifyBlockBranchSize(Assembler *as, QSharedPointer<Node> testBlockA,QSharedPointer<Node> testBlockB, AbstractCodeGen* dispatcher)
{
    //QSharedPointer<Assembler> as =
    //    Asm6502 tmpAsm;
  //  tmpAsm.m_symTab = as->m_symTab;
//    CodeGen6502 dispatcher;
//    dispatcher.as = &tmpAsm;
//    s_isInOffpageTest = true;

    auto app = QSharedPointer<Appendix>(new Appendix);
    auto newtemp = QSharedPointer<Appendix>(new Appendix);
    auto keep2 = as->m_tempVarsBlock;
  //  if (as->offPageStack==0)
        as->m_tempVarsBlock = newtemp;
    auto keep = as->m_currentBlock;
    as->offPageStack++;

    as->m_currentBlock = app;
    QStringList keepTemps = as->m_tempVars;
    if (testBlockA!=nullptr)
        testBlockA->Accept(dispatcher);
    if (testBlockB!=nullptr)
        testBlockB->Accept(dispatcher);
    as->m_tempVars = keepTemps;

    int count = as->CodeSizeEstimator(app->m_source);
    as->m_currentBlock = keep;
    as->offPageStack--;

//    if (as->offPageStack==0)
        as->m_tempVarsBlock = keep2;


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

bool Node::hasFlag(Assembler *as, QString flag) {
    return false;
}

void Node::setReference(bool ref) {
    m_op.m_isReference = ref;
    if (m_left!=nullptr)
        m_left->setReference(ref);
    if (m_right!=nullptr)
        m_right->setReference(ref);

}

void Node::clearComment() {
    m_comment = "";
    if (m_right!=nullptr)
        m_right->clearComment();;
    if (m_left!=nullptr)
        m_left->clearComment();;
}

void Node::ReplaceVariable(Assembler* as, QString name, QSharedPointer<Node> node)
{
    if (m_right!=nullptr)
        if (m_right->isPureVariable() && m_right->getValue(as)==name)
            m_right = node;
    if (m_left!=nullptr)
        if (m_left->isPureVariable() && m_left->getValue(as)==name)
            m_left = node;

    if (m_right!=nullptr)
        m_right->ReplaceVariable(as,name,node);
    if (m_left!=nullptr)
        m_left->ReplaceVariable(as,name,node);
}

int Node::getArrayDataSize(Assembler* as) {
    if (getArrayType(as)==TokenType::INTEGER) return 2;
    if (getArrayType(as)==TokenType::POINTER) return Syntax::s.m_currentSystem->getPointerSize();
    if (getArrayType(as)==TokenType::LONG) return 4;
//        if (getArrayType(as)==TokenType::POINTER) return Syntax::s.m_currentSystem->getPointerSize();
    return 1;

}
