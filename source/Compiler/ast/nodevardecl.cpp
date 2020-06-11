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


NodeVarDecl::NodeVarDecl(QSharedPointer<Node> varNode, QSharedPointer<Node> typeNode):Node() {
    m_varNode = varNode;
    m_typeNode = typeNode;
}

void NodeVarDecl::ExecuteSym(QSharedPointer<SymbolTable> symTab) {

    QSharedPointer<NodeVarType> typeNode = qSharedPointerDynamicCast<NodeVarType>(m_typeNode);
    QString typeName = typeNode->value;
    QString varName = qSharedPointerDynamicCast<NodeVar>(m_varNode)->value;

//    if (qSharedPointerDynamicCast<NodeVar>(m_varNode)->m_isGlobal)
  //      return;

    if (symTab->exists(varName))
          ErrorHandler::e.Error("Variable '" + varName +"' is already defined!",m_op.m_lineNumber);


    if (typeName=="ARRAY"){

        //if (symTab->m_records.contains(typeNode->m_arrayVarType.m_value))
        if (typeNode->m_arrayVarType.m_type==TokenType::RECORD)
            typeName = typeNode->m_arrayVarType.m_value;
    }

    QSharedPointer<Symbol> typeSymbol = symTab->Lookup(typeName, m_op.m_lineNumber);


    varName = varName.remove(symTab->getCurrentProcedure());
//    qDebug() << "Nodevardecl:ExecuteSym " << varName << " " << varSymbol->m_type;
    if (symTab->m_records.contains(typeSymbol->m_name)) {
        // Create record symbol table
//        qDebug() << "EXECUTESYM " << typeSymbol->m_name << varName;
        QSharedPointer<SymbolTable>  ns = symTab->m_records[typeSymbol->m_name];
            for (QSharedPointer<Symbol> s : ns->m_symbols) {
                QSharedPointer<Symbol> ns = QSharedPointer<Symbol>(new Symbol(varName + "_" + typeSymbol->m_name + "_"+s->m_name, s->m_type));
//
                // Make sure that the array is the correct type (int / long)
                ns->m_arrayType = s->getTokenType();
                ns->m_arrayTypeText = s->m_type;

//                qDebug() << "TS "<<s->m_type << ns->m_name << ns->m_arrayTypeText << TokenType::getType(ns->getTokenType());
/*                if (s->m_type.toUpper()=="ARRAY") {
                    ns->m_arrayType = s->m_arrayType;
                    ns->m_arrayTypeText = s->m_arrayTypeText;
                    qDebug() << ns->m_value << " VS " << s->m_arrayTypeText;;

//                    qDebug() << "Defining : "<<varName + "_" + typeSymbol->m_name + "_"+ns->m_name << ns->m_type << TokenType::getType(ns->m_arrayType);
                }*/
                ns->m_flags = typeNode->m_flags;
                ns->m_bank = typeNode->m_bank;
                symTab->Define(ns);
            }
    }

    QSharedPointer<Symbol> varSymbol = QSharedPointer<Symbol>(new VarSymbol(varName, typeSymbol->m_name));
    bool isFlaggedAsUsed = false;
    if (typeName == "INCSID")
        isFlaggedAsUsed = true;

    varSymbol->m_flags = typeNode->m_flags;
    varSymbol->m_bank = typeNode->m_bank;

    symTab->Define(varSymbol,isFlaggedAsUsed);


}

void NodeVarDecl::InitSid(QSharedPointer<SymbolTable> symtab, QString projectDir, int VICAddress, QString type) {
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(m_typeNode);


    int headerShift = 0;
    if (t->m_position!="") {
        QString val = t->m_position;

        headerShift = Util::NumberFromStringHex(val);;
    }
    if (type=="sid") {
        sid.Load(t->m_filename, projectDir);
        sid.Convert(headerShift,VICAddress,type,0x7C,Syntax::s.m_currentSystem->m_system == AbstractSystem::OK64);
    }
    if (type=="nsf") {
        sid.LoadNSF(t->m_filename, projectDir);
        sid.Convert(headerShift,VICAddress,type,0x80,false);
    }


/*            qDebug() << "SID LOAD: " << QString::number(sid.m_loadAddress,16);
                qDebug() << "SID INIT: " << QString::number(sid.m_initAddress,16);
               qDebug() << "SID PLAY: " << QString::number(sid.m_playAddress,16);
*/
    if (sid.m_loadAddress ==0 ) {
  //      qDebug() << "BUG load sid at ZERO?";
        sid.m_loadAddress = sid.m_initAddress;
    }

    symtab->DefineSid(sid.m_initAddress, sid.m_playAddress);

}
