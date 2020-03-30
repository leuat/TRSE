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

#ifndef NODEVARDECL_H
#define NODEVARDECL_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodevar.h"
#include "source/Compiler/ast/nodevartype.h"
#include "source/Compiler/misc/sidfile.h"
#include "source/LeLib/util/util.h"
#include "source/Compiler/assembler/abstractastdispatcher.h"

class NodeVarDecl : public Node {
public:
    Node* m_varNode = nullptr;
    Node* m_typeNode = nullptr;
    int m_fileSize=0;
    int m_dataSize=0;
    bool m_chipMem = false;

    SidFile sid;

    int m_pushedPointers = 0;

    NodeVarDecl(Node* varNode, Node* typeNode);

    void parseConstants(QSharedPointer<SymbolTable>  symTab) override {
        if (m_varNode!=nullptr)
            m_varNode->parseConstants(symTab);
        if (m_typeNode!=nullptr)
            m_typeNode->parseConstants(symTab);
    }



    void Delete() override;

    int getDataSize() {
        return m_dataSize;
    }

    void ExecuteSym(QSharedPointer<SymbolTable>  symTab) override;
    void Accept(AbstractASTDispatcher* dispatcher) override {
        dispatcher->dispatch(this);
    }

    void InitSid(QString projectDir, int VICAddress, QString type);

};



#endif // NODEVARDECL_H
