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

void NodeVarDecl::IncSid(Assembler *as) {

    // Init address or load address? hmmm


    QString pos = QString::number(sid.m_loadAddress,16);
    Appendix app("$"+pos);

    app.Append("org $" +pos,1);
    //        as->Appendix(v->value,0);
    app.Append("incbin \"" + as->m_projectDir + sid.m_outFile + "\"",1);

    as->m_appendix.append(app);

    int size=0;
    QFile f(as->m_projectDir + sid.m_outFile);
    if (f.open(QIODevice::ReadOnly)){
        size = f.size();  //when file does open.
        f.close();
    }
    m_fileSize = size;

    as->blocks.append(new MemoryBlock(sid.m_loadAddress,sid.m_loadAddress+size, MemoryBlock::MUSIC, sid.m_fileName));


}

void NodeVarDecl::IncBin(Assembler *as) {
    NodeVar* v = (NodeVar*)m_varNode;
    NodeVarType* t = (NodeVarType*)m_typeNode;
    QString filename = as->m_projectDir + "/" + t->m_filename;
    if (!QFile::exists(filename))
        ErrorHandler::e.Error("Could not locate binary file for inclusion :" +filename);

    int size=0;
    QFile f(filename);
    if (f.open(QIODevice::ReadOnly)){
        size = f.size();  //when file does open.
        f.close();
    }


    if (t->m_position=="") {
        as->Label(v->value);
        as->Asm("incbin \"" + filename + "\"");
    }
    else {
        //            qDebug() << "bin: "<<v->value << " at " << t->m_position;
        Appendix app(t->m_position);

        Symbol* typeSymbol = as->m_symTab->Lookup(v->value, m_op.m_lineNumber);
        typeSymbol->m_org = Util::C64StringToInt(t->m_position);
        typeSymbol->m_size = size;
        //            qDebug() << "POS: " << typeSymbol->m_org;
        app.Append("org " +t->m_position,1);
        app.Append(v->value,0);
        app.Append("incbin \"" + filename + "\"",1);
        as->m_appendix.append(app);
        bool ok;
        int start=0;
        if (t->m_position.startsWith("$")) {
            start = t->m_position.remove("$").toInt(&ok, 16);
        }
        else start = t->m_position.toInt();

        as->blocks.append(new MemoryBlock(start,start+size, MemoryBlock::DATA,t->m_filename));

    }
}

void NodeVarDecl::InitSid(QString projectDir) {
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

    sid.Load(t->m_filename, projectDir);
    sid.Convert(headerShift);
    /*        qDebug() << "SID LOAD: " << QString::number(sid.m_loadAddress,16);
        qDebug() << "SID INIT: " << QString::number(sid.m_initAddress,16);
       qDebug() << "SID PLAY: " << QString::number(sid.m_playAddress,16);
*/
    if (sid.m_loadAddress ==0 ) {
        qDebug() << "BUG load sid at ZERO?";
        sid.m_loadAddress = sid.m_initAddress;
    }
    SymbolTable::DefineSid(sid.m_initAddress, sid.m_playAddress);

}

void NodeVarDecl::DeclarePointer(Assembler *as) {

    if (!as->CheckZPAvailability())
        ErrorHandler::e.Error("Could not allocate more free pointers! Please free some up, or declare more in the settings page. ", m_op.m_lineNumber);

    NodeVarType* t = (NodeVarType*)m_typeNode;
    QString initVal = t->initVal;

    if (initVal=="") {
        initVal = as->PushZeroPointer();
        m_pushedPointers++;
    }

    NodeVar* v = (NodeVar*)m_varNode;
    as->Label(v->value + "\t= " + initVal);

}

QString NodeVarDecl::Build(Assembler *as) {
    Node::Build(as);
    int ret = MaintainBlocks(as);

    if (ret==3) m_curMemoryBlock=nullptr;
    if (as->m_currentBlock!=nullptr) {
        if (m_curMemoryBlock==nullptr) {
            bool ok;
            QString p = as->m_currentBlock->m_pos;
            int pos = p.remove("$").toInt(&ok, 16);
            m_curMemoryBlock = new MemoryBlock(pos,pos,MemoryBlock::ARRAY, m_blockInfo.m_blockName);
            as->blocks.append(m_curMemoryBlock);
        }
    }
    else
        m_curMemoryBlock=nullptr;
 /*   if (ret==2) {
        m_curMemoryBlock = nullptr;

    }*/
    ExecuteSym(as->m_symTab);

    NodeVar* v = (NodeVar*)m_varNode;
    NodeVarType* t = (NodeVarType*)m_typeNode;

    //        v->m_op.m_type =t->m_op.m_type;
    //v->m_type = t;

    if (t->m_op.m_type==TokenType::ARRAY) {
        as->DeclareArray(v->value, t->m_arrayVarType.m_value, t->m_op.m_intVal, t->m_data, t->m_position);
        //qDebug() << "IS: " << TokenType::types[as->m_symTab->Lookup(v->value)->getTokenType()];
        m_dataSize=t->m_op.m_intVal;
        as->m_symTab->Lookup(v->value, m_op.m_lineNumber)->m_type="address";
    }else
    if (t->m_op.m_type==TokenType::STRING) {
        as->DeclareString(v->value, t->m_data);
        m_dataSize = 0;
        for (QString s: t->m_data)
            m_dataSize+=s.count();
        m_dataSize++; // 0 end
    }
    else
    if (t->m_op.m_type==TokenType::CSTRING) {
        as->DeclareCString(v->value, t->m_data);
        m_dataSize = 0;
        for (QString s: t->m_data)
            m_dataSize+=s.count();
        m_dataSize++; // 0 end
    }
    else
    if (t->m_op.m_type==TokenType::INCBIN) {
        if (m_curMemoryBlock!=nullptr)
            ErrorHandler::e.Error("IncBin can not be declared within a user-defined memory block :",m_op.m_lineNumber);

        IncBin(as);
    }
    else
    if (t->m_op.m_type==TokenType::INCSID) {
        if (m_curMemoryBlock!=nullptr)
            ErrorHandler::e.Error("IncSid can not be declared within a user-defined memory block :",m_op.m_lineNumber);
        IncSid(as);
    }
    else
    if (t->m_op.m_type==TokenType::POINTER) {
        if (m_curMemoryBlock!=nullptr)
            ErrorHandler::e.Error("Pointers can not be declared within a user-defined memory block :",m_op.m_lineNumber);
        DeclarePointer(as);
    }else {
        m_dataSize=1;
        if (t->value.toLower()=="integer") m_dataSize = 2;
        as->DeclareVariable(v->value, t->value, t->initVal);
    }

    if (m_curMemoryBlock!=nullptr) {
        m_curMemoryBlock->m_end+=m_dataSize;
    }

    return "";
}

PVar NodeVarDecl::Execute(SymbolTable *symTab, uint lvl) {
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
    level = lvl+1;
    ErrorHandler::e.DebugLow("Calling VarDecl",level);
    return PVar();

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
