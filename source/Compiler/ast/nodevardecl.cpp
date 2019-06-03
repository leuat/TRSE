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

#include "nodevardecl.h"


NodeVarDecl::NodeVarDecl(Node *varNode, Node *typeNode):Node() {
    m_varNode = varNode;
    m_typeNode = typeNode;
}

void NodeVarDecl::Delete() {
    if (m_varNode) {
        m_varNode->Delete();
        delete m_varNode;
    }
    if (m_typeNode) {
        m_typeNode->Delete();
        delete m_typeNode;
    }

}


void NodeVarDecl::ExecuteSym(SymbolTable *symTab) {

    QString typeName = ((NodeVar*)m_typeNode)->value;
    QString varName = ((NodeVar*)m_varNode)->value;
    //if (symTab->Lookup(varName)!=nullptr)
    //       ErrorHandler::e.Error("Variable '" + varName +"' is already defined!",m_op.m_lineNumber);



    Symbol* typeSymbol = symTab->Lookup(typeName, m_op.m_lineNumber);
    //        if (typeSymbol==nullptr)
    //          ErrorHandler::e.Error("Could not find type symbol :" + typeName,m_op.m_lineNumber);

    ErrorHandler::e.DebugLow("Typename define : " + typeName + "  variable " + varName);




    Symbol* varSymbol = new VarSymbol(varName, typeSymbol->m_name);
    symTab->Define(varSymbol);
    // qDebug() << "Nodevardecl:ExecuteSym " << varName << " " << varSymbol->m_type;

}

void NodeVarDecl::InitSid(QString projectDir, int VICAddress, QString type) {
    NodeVar* v = (NodeVar*)m_varNode;
    NodeVarType* t = (NodeVarType*)m_typeNode;


    int headerShift = 0;
    if (t->m_position!="") {
        QString val = t->m_position;
        bool ok;
        headerShift = val.toInt(&ok);
        if (!ok)
            headerShift = 0;
    }
    if (type=="sid") {
        sid.Load(t->m_filename, projectDir);
        sid.Convert(headerShift,VICAddress,type,0x7C);
    }
    if (type=="nsf") {
        sid.LoadNSF(t->m_filename, projectDir);
        sid.Convert(headerShift,VICAddress,type,0x80);
    }
    /*        qDebug() << "SID LOAD: " << QString::number(sid.m_loadAddress,16);
                qDebug() << "SID INIT: " << QString::number(sid.m_initAddress,16);
               qDebug() << "SID PLAY: " << QString::number(sid.m_playAddress,16);
        */
    if (sid.m_loadAddress ==0 ) {
//        qDebug() << "BUG load sid at ZERO?";
        sid.m_loadAddress = sid.m_initAddress;
    }
    SymbolTable::DefineSid(sid.m_initAddress, sid.m_playAddress);

}
