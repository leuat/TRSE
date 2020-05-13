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

Compiler::Compiler(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni)
{
    m_ini = ini;
    m_projectIni = pIni;
    m_parser.m_projectIni = pIni;
    m_parser.m_settingsIni = ini;

}

Compiler::~Compiler() {
  //  qDebug() << "~COMPILER DESTROYED "<<this;
//    Destroy();
}


void Compiler::Parse(QString text, QStringList lst, QString fname)
{
    m_parser.m_currentFileShort = fname;
    m_lexer = QSharedPointer<Lexer>(new Lexer(text, lst, m_projectIni->getString("project_path")));
    Syntax::s.m_currentSystem->m_systemParams.clear();
    m_parser.m_lexer = m_lexer;
    ErrorHandler::e.m_displayWarnings = m_ini->getdouble("display_warnings")==1;


    m_tree = nullptr;
    m_parser.m_preprocessorDefines[m_projectIni->getString("system").toUpper()]=1;
    m_parser.m_preprocessorDefines[m_ini->getString("assembler").toUpper()]=1;
    m_parser.m_isTRU = m_isTRU;
    Parser::s_usedTRUs.clear(); // None TRU's are marked
    Parser::s_usedTRUNames.clear(); // None TRU's are marked
    //qDebug() << "******" << m_ini->getString("assembler").toUpper();
    try {
        m_tree = m_parser.Parse( m_projectIni->getdouble("remove_unused_symbols")==1.0 &&
                                 Syntax::s.m_currentSystem->m_system!=AbstractSystem::NES
                                 ,m_projectIni->getString("vic_memory_config"),Util::fromStringList(m_projectIni->getStringList("global_defines")),
                                 m_projectIni->getdouble("pascal_settings_use_local_variables")==1.0);


//        m_tree->parseConstants(m_parser.m_symTab);
        //qDebug() << m_parser.m_preprocessorDefines["ORGASM"];
        //exit(1);
    } catch (FatalErrorException e) {
//        qDebug() << "ERROR parse " << e.message;
        //m_tree->Delete();
//        m_tree = nullptr;
        HandleError(e, "Error during parsing");
    }

    if (m_parser.m_symTab!=nullptr)
        m_parser.m_symTab->SetCurrentProcedure("");
}


bool Compiler::Build(QSharedPointer<AbstractSystem> system, QString project_dir)
{
    if (m_tree==nullptr) {
        return false;
    }

    system->DefaultValues();
    Syntax::s.m_currentSystem->DefaultValues();

    try {
        InitAssemblerAnddispatcher(system);
        m_assembler->m_curDir = project_dir;
    } catch (FatalErrorException e) {
        HandleError(e,"Error during pre-build");
        return false;
    }

    if (m_assembler==nullptr)
        return false;

    m_assembler->m_projectDir = project_dir;
    // Copy symbol table stuff, like records
    m_assembler->m_symTab->m_useLocals = m_parser.m_symTab->m_useLocals;
    m_assembler->m_symTab->m_records = m_parser.m_symTab->m_records;
    m_assembler->m_symTab->m_constants = m_parser.m_symTab->m_constants;

    for (QSharedPointer<SymbolTable>  st : m_parser.m_symTab->m_records)
        m_assembler->m_symTab->Define(QSharedPointer<Symbol>(new Symbol(st->m_name, "RECORD")));

    connect(m_dispatcher.get(), SIGNAL(EmitTick(QString)), this, SLOT( AcceptDispatcherTick(QString)));

    emit EmitTick("<br>Building ");
    if (m_tree!=nullptr)
        try {
        qSharedPointerDynamicCast<NodeProgram>(m_tree)->m_initJumps = m_parser.m_initJumps;
        m_dispatcher->as = m_assembler.get();

        m_tree->Accept(m_dispatcher.get());

    } catch (FatalErrorException e) {
        HandleError(e,"Error during build (dispatcher) :");
        return false;
    }


    for (QSharedPointer<MemoryBlock> mb:m_parser.m_userBlocks)
        m_assembler->blocks.append(mb);


    disconnect(m_dispatcher.get(), SIGNAL(EmitTick(QString)), this, SLOT( AcceptDispatcherTick(QString)));
    emit EmitTick("<br>Connecting and optimising");

    Connect();

    CleanupBlockLinenumbers();

    WarningUnusedVariables();
    // Make sure records aren't deleted in copy
    m_assembler->m_symTab->m_records.clear();
//    m_assembler->m_symTab->Delete();
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

    msg +="<br><font color=\"#FF0000\">Fatal error " + line+"</font>&nbsp;";
//    if (linenr<m_parser.m_lexer->m_lines.count() && linenr>=0)
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

void Compiler::AcceptDispatcherTick(QString val)
{
    emit EmitTick(val);
}
