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

Compiler::Compiler(CIniFile* ini, CIniFile* pIni)
{
    m_ini = ini;
    m_projectIni = pIni;
    m_parser.m_projectIni = pIni;
}


void Compiler::Parse(QString text, QStringList lst)
{

    m_lexer = Lexer(text, lst, m_projectIni->getString("project_path"));
    Syntax::s.m_currentSystem->m_systemParams.clear();
    m_parser.m_lexer = &m_lexer;
    ErrorHandler::e.m_displayWarnings = m_ini->getdouble("display_warnings")==1;


    m_tree = nullptr;
    m_parser.m_preprocessorDefines[m_projectIni->getString("system").toUpper()]=1;
    m_parser.m_preprocessorDefines[m_ini->getString("assembler").toUpper()]=1;
    //qDebug() << "******" << m_ini->getString("assembler").toUpper();
    try {
        m_tree = m_parser.Parse( m_ini->getdouble("optimizer_remove_unused_symbols")==1.0 &&
                                 Syntax::s.m_currentSystem->m_system!=AbstractSystem::NES
                                 ,m_projectIni->getString("vic_memory_config"),Util::fromStringList(m_projectIni->getStringList("global_defines")),
                                 m_projectIni->getdouble("pascal_settings_use_local_variables")==1.0);
//        m_tree->parseConstants(m_parser.m_symTab);
        //qDebug() << m_parser.m_preprocessorDefines["ORGASM"];
        //exit(1);
    } catch (FatalErrorException e) {
//        qDebug() << "ERROR parse " << e.message;
        HandleError(e, "Error during parsing");
    }
    if (m_parser.m_symTab!=nullptr)
        m_parser.m_symTab->SetCurrentProcedure("");
}


bool Compiler::Build(AbstractSystem* system, QString project_dir)
{
    if (m_tree==nullptr) {
        //qDebug() << "Compiler::Build : tree not parsed!";
        return false;
    }
    if (m_assembler)
        delete m_assembler;

    system->DefaultValues();
    Syntax::s.m_currentSystem->DefaultValues();

    try {
        InitAssemblerAndDispatcher(system);
    } catch (FatalErrorException e) {
        HandleError(e,"Error during pre-build");
        return false;
    }

    if (m_assembler==nullptr)
        return false;

    m_assembler->m_projectDir = project_dir;
    m_assembler->m_symTab->m_useLocals = m_parser.m_symTab->m_useLocals;


    if (m_tree!=nullptr)
        try {
        dynamic_cast<NodeProgram*>(m_tree)->m_initJumps = m_parser.m_initJumps;
        m_dispatcher->as = m_assembler;
        m_tree->Accept(m_dispatcher);

    } catch (FatalErrorException e) {
        HandleError(e,"Error during build");
        return false;
    }

    for (MemoryBlock* mb:m_parser.m_userBlocks)
        m_assembler->blocks.append(mb);


    Connect();

    CleanupBlockLinenumbers();

    WarningUnusedVariables();


    return true;

}


void Compiler::CleanupBlockLinenumbers()
{
    QMap<int, int> blocks;

    for (int i: m_assembler->m_blockIndent.keys()) {

        int count = m_assembler->m_blockIndent[i];
        int nl = i;
        for (FilePart& fp : m_parser.m_lexer->m_includeFiles) {
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
    m_parser.m_lexer->FindLineNumberAndFile(fe.linenr, file, linenr);
    //linenr = fe.linenr;


    QString line = " on line: " + QString::number(linenr+1);
    if (file!="")
        msg+=" in file : " + file + "\n";

    fe.file=file;

    msg +="<br><font color=\"#FF0000\">Fatal error " + line+"</font>";
    if (linenr<m_parser.m_lexer->m_lines.count() && linenr>=0)
//        msg+="<br><i>Source code line</i>: " + m_parser.m_lexer->m_lines[linenr];
    msg+="<br>";
//    msg+="<br><i>Message</i>: ";
    Pmm::Data::d.lineNumber = linenr+1;

    recentError = fe;
    ErrorHandler::e.CatchError(fe, e + msg);

}



void Compiler::WarningUnusedVariables()
{
    QStringList unusedVariables = m_assembler->m_symTab->getUnusedVariables();
    if (unusedVariables.count()!=0) {
        QString lstStr= "";
        for (QString s: unusedVariables)
            lstStr+= s+ ",";
        lstStr.remove(lstStr.count()-1,1); // remove last ","
        ErrorHandler::e.Warning("Unused variables : " +lstStr);
    }

}
