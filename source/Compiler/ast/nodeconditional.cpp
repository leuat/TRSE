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

#include "nodeconditional.h"
#include "source/Compiler/assembler/mos6502.h"


QString NodeConditional::Build(Assembler *as) {
    Node::Build(as);

    as->PushCounter();
    QString labelStartOverAgain = as->NewLabel("while");
    QString lblstartTrueBlock = as->NewLabel("ConditionalTrueBlock");

    QString labelElse = as->NewLabel("elseblock");
    QString labelElseDone = as->NewLabel("elsedoneblock");
   // QString labelFailed = as->NewLabel("conditionalfailed");

    if (m_isWhileLoop)
        as->Label(labelStartOverAgain);

    // Test all binary clauses:
    bool isSimplified = false;
    NodeBinaryClause* bn = dynamic_cast<NodeBinaryClause*>(m_binaryClause);
    if (verifyBlockBranchSize(as, m_block)) {
        isSimplified = !bn->cannotBeSimplified(as);
    }

    // Then, check m_forcepage
    if (m_forcePage==1) // force OFFPAGE
        isSimplified = false;

    if (m_forcePage==2) {
        if (!bn->cannotBeSimplified(as)) // force ONPAGE
            isSimplified = true;
        else ErrorHandler::e.Error("keyword onpage can only be used with 1 compare clause (no and, or etc)", m_op.m_lineNumber);
    }
    if (!isSimplified) {
        m_binaryClause->Build(as);
        // Now, a should be either true or false

        as->Asm("cmp #1");
        as->Asm("beq " + lblstartTrueBlock); // All conditionals checked out!
        // Do we have an else block?
        if (m_elseBlock!=nullptr)
            as->Asm("jmp " + labelElse); // All conditionals false: skip to end (or else block)
        as->Asm("jmp " + labelElseDone);
    }
    else {
        // Simplified version <80 instructions & just one clause

        QString failedLabel = labelElseDone;
        if (m_elseBlock!=nullptr)
            failedLabel = labelElse;
        bn->BuildSimple(as, failedLabel);
    }
    // Start main block
    as->Label(lblstartTrueBlock); // This means skip inside

    m_block->Build(as);
    if (m_elseBlock!=nullptr)
        as->Asm("jmp " + labelElseDone);

    // If while loop, return to beginning of conditionals
    if (m_isWhileLoop)
        as->Asm("jmp " + labelStartOverAgain);

    // An else block?
    if (m_elseBlock!=nullptr) {
        as->Label(labelElse);
        m_elseBlock->Build(as);

    }
    as->Label(labelElseDone); // Jump here if not

    as->PopLabel("while");
    as->PopLabel("ConditionalTrueBlock");
    as->PopLabel("elseblock");
    as->PopLabel("elsedoneblock");
//    as->PopLabel("conditionalfailed");

    as->PopCounter(m_op.m_lineNumber);

    return "";
}








NodeConditional::NodeConditional(Token op, int forcePage, Node *clause, Node *block, bool isWhile, Node *elseBlock) :Node(){
    m_block = block;
    m_isWhileLoop = isWhile;
    m_elseBlock = elseBlock;
    m_binaryClause = clause;
    m_op = op;
    m_forcePage = forcePage;
}

void NodeConditional::Delete() {
    /*if (m_a) {
            m_a->Delete();
            delete m_a;
            m_a = nullptr;
        }
        if (m_b) {
            m_b->Delete();
            delete m_b;
            m_b = nullptr;
        }*/
    if (m_block) {
        m_block->Delete();
        delete m_block;
        m_block = nullptr;
    }


}

PVar NodeConditional::Execute(SymbolTable *symTab, uint lvl) {
    return PVar();

}

