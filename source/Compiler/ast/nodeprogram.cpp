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

#include "nodeprogram.h"


void NodeProgram::Delete() {
    if (m_NodeBlock) {
        m_NodeBlock->Delete();
        delete m_NodeBlock;
        m_NodeBlock = nullptr;
    }
}

PVar NodeProgram::Execute(SymbolTable *symTab, uint lvl) {
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
    level = lvl+1;
    ErrorHandler::e.DebugLow("Calling Program Node",level);

    m_NodeBlock->Execute(symTab, level);
    return PVar();

}

void NodeProgram::ExecuteSym(SymbolTable *symTab) {
    m_NodeBlock->ExecuteSym(symTab);
}

QString NodeProgram::Build(Assembler *a) {
    Node::Build(a);
    a->EndMemoryBlock();
    NodeBuiltinMethod::m_isInitialized.clear();
    a->Program(m_name);
    a->m_source << m_initJumps;
    m_NodeBlock->Build(a);
    a->EndProgram();
    return "";
}
