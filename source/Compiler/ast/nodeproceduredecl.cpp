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

#include "nodeproceduredecl.h"
#include "nodebuiltinmethod.h"
#include "source/Compiler/syntax.h"


NodeProcedureDecl::NodeProcedureDecl(Token t, QString m):Node() {
    m_op = t;
    m_procName = m;
    m_isInterrupt = false;
}

NodeProcedureDecl::NodeProcedureDecl(Token t, QString m, QVector<Node *> paramDecl, Node *block, bool isInterrupt) : Node() {
    m_op = t;

    m_procName = m;
    m_block = block;
    m_paramDecl = paramDecl;
    NodeBlock* b = (NodeBlock*)block;
    for (int i=0;i<m_paramDecl.count();i++)
        b->m_decl.append(m_paramDecl[i]);

    m_isInterrupt = isInterrupt;

}

void NodeProcedureDecl::Delete() {
    ErrorHandler::e.DebugLow("Memory: Deleting in NodeProcedureDecl", level);

    for (Node* n: m_paramDecl) {
        n->Delete();
        delete n;
    }
    m_paramDecl.clear();
    if (m_block) {
        m_block->Delete();
        delete m_block;
        m_block = nullptr;
    }
    ErrorHandler::e.DebugLow("Memory DONE: Deleting in NodeProcedureDecl", level);
}

void NodeProcedureDecl::SetParametersValue(QVector<PVar> &lst) {
    if (lst.count()!=m_paramDecl.count())
        ErrorHandler::e.Error("Incorrect number of parameters calling procedure '" +
                              m_procName +"'. Requires "+QString::number(m_paramDecl.count()) +
                              " parameters but is called with "+QString::number(lst.count()));

    for (int i=0;i<m_paramDecl.count();i++) {
        NodeVarDecl* vd = (NodeVarDecl*)m_paramDecl[i];
        NodeVar* v= ((NodeVar*)vd->m_varNode);
        QString name = v->value;
        PVar val = lst[i];
        ((NodeBlock*)m_block)->SetParameter(name, val);

    }
    //        ((NodeBlock*)m_block)->SetParameters(lst, names);
}

QString NodeProcedureDecl::Build(Assembler *as)
{
    Node::Build(as);
    // In case memory block is acive
//    as->EndMemoryBlock();

    int ret = MaintainBlocks(as);

    if (ret==3) m_curMemoryBlock=nullptr;
    if (as->m_currentBlock!=nullptr) {
        if (m_curMemoryBlock==nullptr) {
            bool ok;
//            qDebug() << "Creating new block procedure for " << m_procName;
            QString p = as->m_currentBlock->m_pos;
            int pos = p.remove("$").toInt(&ok, 16);
            m_curMemoryBlock = new MemoryBlock(pos,pos,MemoryBlock::ARRAY, m_blockInfo.m_blockName);
            as->blocks.append(m_curMemoryBlock);
        }
    }
    else
        m_curMemoryBlock=nullptr;



    //MaintainBlocks(as);
    if (m_block==nullptr)  // Is builtin procedure
        m_block = new NodeBuiltinMethod(m_procName, QVector<Node*>());

    bool isInitFunction=false;
    bool isBuiltinFunction=false;
    if (Syntax::s.builtInFunctions.contains(m_procName)) {
        isBuiltinFunction = true;
        isInitFunction = Syntax::s.builtInFunctions[m_procName].m_initFunction;
    }

    as->Asm("");
    as->Asm("");
    as->Comment("***********  Defining procedure : " + m_procName);
    QString type = (isBuiltinFunction) ? "Built-in function" : "User-defined procedure";
    as->Comment("   Procedure type : " + type);
    if (isBuiltinFunction) {
        type = (isInitFunction) ? "yes" : "no";
        as->Comment("   Requires initialization : " + type);
    }
    as->Asm("");


    if (!isInitFunction) {
        //as->Asm("jmp afterProc_" + m_procName);


        //as->Label(m_procName);
    }
//    if (m_isInterrupt)
  //      as->Asm("dec $d019        ; acknowledge IRQ");
    if (m_block!=nullptr) {
        NodeBlock* b = dynamic_cast<NodeBlock*>(m_block);
        if (b!=nullptr)
            b->forceLabel=m_procName;
        m_block->Build(as);
    }
    if (!isInitFunction) {
        if (!m_isInterrupt)
            as->Asm("rts");
        else as->Asm("rti");
      //as->Label("afterProc_" + m_procName);
    }

    if (m_curMemoryBlock!=nullptr) {
        m_curMemoryBlock->m_end+=10;
    }


    return 0;
}

PVar NodeProcedureDecl::Execute(SymbolTable *symTab, uint lvl) {
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);

    level = lvl+1;
    ErrorHandler::e.DebugLow("Calling ProcedureDecl Node",level);

    if (m_block!=nullptr) {
        return m_block->Execute(symTab, level);
    }
    return PVar();

}

void NodeProcedureDecl::ExecuteSym(SymbolTable *symTab) {
    m_block->ExecuteSym(symTab);
}
