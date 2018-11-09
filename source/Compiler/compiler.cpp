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

#include "compiler.h"
#include <QDebug>

Compiler::Compiler(Parser* p, CIniFile* ini, CIniFile* pIni)
{
    m_parser = p;
    m_ini = ini;
    m_projectIni = pIni;
}

void Compiler::Parse()
{
    m_tree = nullptr;
    m_parser->m_preprocessorDefines[m_projectIni->getString("system").toUpper()]=1;
    m_parser->m_preprocessorDefines[m_ini->getString("assembler").toUpper()]=1;
    //qDebug() << "******" << m_ini->getString("assembler").toUpper();
    try {

        m_tree = m_parser->Parse(m_ini->getdouble("optimizer_remove_unused_symbols")==1.0,m_projectIni->getString("vic_memory_config"));
        //qDebug() << m_parser->m_preprocessorDefines["ORGASM"];
        //exit(1);
    } catch (FatalErrorException e) {
        qDebug() << "ERROR parse " << e.message;
        HandleError(e, "Error during parsing:");
    }

}

void Compiler::Visit(Node* n)
{
    n->Execute(nullptr, 0);
}

void Compiler::Interpret()
{
    if (m_tree!=nullptr)
        try {
        Visit(m_tree);
    } catch (FatalErrorException e) {

    }

}

bool Compiler::Build(Compiler::Type type, QString project_dir)
{
    if (m_tree==nullptr) {
        qDebug() << "Compiler::Build : tree not parsed!";
        return false;
    }
    if (m_assembler)
        delete m_assembler;

    if (type==MOS6502) {
        m_assembler = new AsmMOS6502();
        m_dispatcher = new ASTDispather6502();
    }



/*    if (type==PASCAL)
        m_assembler = new AsmPascal();
*/
    if (m_assembler==nullptr)
        return false;

    m_assembler->InitZeroPointers(m_projectIni->getStringList("zeropages"),m_projectIni->getStringList("temp_zeropages"));
    m_assembler->m_zeropageScreenMemory = m_projectIni->getString("zeropage_screenmemory");
    m_assembler->m_replaceValues["@DECRUNCH_ZP1"] = m_projectIni->getString("zeropage_decrunch1");
    m_assembler->m_replaceValues["@DECRUNCH_ZP2"] = m_projectIni->getString("zeropage_decrunch2");
    m_assembler->m_replaceValues["@DECRUNCH_ZP3"] = m_projectIni->getString("zeropage_decrunch3");
    m_assembler->m_replaceValues["@DECRUNCH_ZP4"] = m_projectIni->getString("zeropage_decrunch4");

    m_assembler->m_internalZP << m_projectIni->getString("zeropage_internal1");
    m_assembler->m_internalZP << m_projectIni->getString("zeropage_internal2");
    m_assembler->m_internalZP << m_projectIni->getString("zeropage_internal3");
    m_assembler->m_internalZP << m_projectIni->getString("zeropage_internal4");


    m_assembler->m_projectDir = project_dir;

    if (m_tree!=nullptr)
        try {
            dynamic_cast<NodeProgram*>(m_tree)->m_initJumps = m_parser->m_initJumps;
            m_dispatcher->as = m_assembler;
//            m_dispatcher->dispatch(m_tree);
            m_tree->Accept(m_dispatcher);

            //m_tree->Build(m_assembler);
        } catch (FatalErrorException e) {
            HandleError(e,"Error during build");
            return false;
         }
 /*       try {
           // m_tree->ExecuteSym(m_assembler->m_symTab);
        } catch (FatalErrorException e) {
            HandleError(e,"Error during symbolic check");
            return false;
    }*/

    for (MemoryBlock* mb:m_parser->m_userBlocks)
        m_assembler->blocks.append(mb);


    m_assembler->EndMemoryBlock();
    m_assembler->Label("EndSymbol");
    m_assembler->Connect();
    if (m_ini->getdouble("post_optimize")==1.0)
        m_assembler->Optimise(*m_projectIni);
    CleanupCycleLinenumbers();
    CleanupBlockLinenumbers();
    return true;

}

void Compiler::CleanupCycleLinenumbers()
{

    QMap<int, int> cycles;

    for (int i: m_assembler->m_cycles.keys()) {

        int count = m_assembler->m_cycles[i];
        int nl = i;
        for (FilePart& fp : m_parser->m_lexer->m_includeFiles) {
            // Modify bi filepart
            if (nl>fp.m_startLine && nl<fp.m_endLine) {
                cycles[fp.m_startLine]+=count;
                count=0;
            }

            if (nl>=fp.m_endLine)
                nl-=fp.m_count-1;
        }
        if (count!=0)
            cycles[nl] = count;


    }

    m_assembler->m_cycles = cycles;
}

void Compiler::CleanupBlockLinenumbers()
{
    QMap<int, int> blocks;

    for (int i: m_assembler->m_blockIndent.keys()) {

        int count = m_assembler->m_blockIndent[i];
        int nl = i;
        for (FilePart& fp : m_parser->m_lexer->m_includeFiles) {
            // Modify bi filepart
            if (nl>fp.m_startLine && nl<fp.m_endLine) {
                blocks[fp.m_startLine]+=count;
                count=0;
            }

            if (nl>=fp.m_endLine)
                nl-=fp.m_count-1;
        }
        if (count!=0)
            blocks[nl] = count;


    }

    m_assembler->m_blockIndent = blocks;

}

void Compiler::SaveBuild(QString filename)
{
    if (!m_assembler)
        return;
    m_assembler->Save(filename);
}

void Compiler::HandleError(FatalErrorException fe, QString e)
{
    QString msg = "";
    int linenr = fe.linenr;
    QString file = "";
    FindLineNumberAndFile(fe.linenr, file, linenr);
    //linenr = fe.linenr;


    QString line = " on line: " + QString::number(linenr+1);
    if (file!="")
        msg+=" in file : " + file + "\n";

    fe.file=file;

    msg +="\nFatal error " + line;
    if (linenr<m_parser->m_lexer->m_lines.count() && linenr>=0)
        msg+="\nSource: " + m_parser->m_lexer->m_lines[linenr];
    msg+="\n\nMessage: ";
    Pmm::Data::d.lineNumber = linenr+1;

    recentError = fe;
    ErrorHandler::e.CatchError(fe, e + msg);

}

void Compiler::FindLineNumberAndFile(int inLe, QString& file, int& outle)
{
    file="";
    outle = inLe;
    if (m_parser->m_lexer->m_includeFiles.count()==0) {
        return;
    }

    int cur = inLe;

    qDebug() << "input line number: " << inLe;
    qDebug() << "Start line: " << m_parser->m_lexer->m_includeFiles[0].m_startLine;
    if (cur<=m_parser->m_lexer->m_includeFiles[0].m_startLine) {
        return;
    }


    for (FilePart fp: m_parser->m_lexer->m_includeFiles) {
        if (inLe >= fp.m_startLine && inLe<fp.m_endLine) {
            file = fp.m_name;
            outle = inLe - fp.m_startLine;
            return;
        }
        qDebug() << "Include file size : " << (fp.m_endLine-fp.m_startLine);
        cur=cur - (fp.m_endLine-fp.m_startLine);
    }
    outle = cur;
}
