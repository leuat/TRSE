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

    connect(&m_parser, SIGNAL(emitRequestSystemChange(QString)), this, SLOT( AcceptRequestSystemChange(QString)));

    m_tree = nullptr;
    m_parser.m_preprocessorDefines[m_projectIni->getString("system").toUpper()]="1";
    m_parser.m_preprocessorDefines[m_ini->getString("assembler").toUpper()]="1";
    if (m_projectIni->getString("qemu").startsWith("qemu")) {
        m_parser.m_preprocessorDefines["QEMU"] = "1";
    }
    m_parser.m_isTRU = m_isTRU;
    Parser::s_usedTRUs.clear(); // None TRU's are marked
    Parser::s_usedTRUNames.clear(); // None TRU's are marked
    Parser::m_tpus.clear();
    SymbolTable::pass = 0;
    // MAIN parser
    try {
        m_tree = m_parser.Parse( m_projectIni->getdouble("remove_unused_symbols")==1.0 &&
                                 Syntax::s.m_currentSystem->m_system!=AbstractSystem::NES
                                 ,m_parser.m_vicMemoryConfig,
                                 Util::fromStringList(m_projectIni->getStringList("global_defines")),
                                 m_projectIni->getdouble("pascal_settings_use_local_variables")==1.0);


    } catch (const FatalErrorException& e) {
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
    Syntax::s.m_currentSystem = system;

//    Syntax::s.m_currentSystem->DefaultValues();

    try {
        // Set up assembler and dispatcher for the current system
        InitAssemblerAnddispatcher(system);
        m_assembler->m_curDir = project_dir;
//        m_codeGen->m_rasSource = m_lexer->m_lines;
        if (system->m_processor==AbstractSystem::MOS6502)
            m_codeGen->m_outputLineNumbers =  m_ini->getdouble("display_addresses")==1.0;


    } catch (const FatalErrorException& e) {
        HandleError(e,"Error during pre-build");
        return false;
    }

    if (m_assembler==nullptr)
        return false;

    // Map forwarded symbols
    for (QString s: m_parser.m_symTab->m_forwardedVariables)
        m_assembler->m_symTab->m_forwardedSymbols[s] = m_parser.m_symTab->m_symbols[s];

//    m_assembler->m_symTab->m_forwardedSymbols = m_parser.m_symTab->m_forwardedSymbols;
    m_assembler->m_projectDir = project_dir;
    // Copy symbol table stuff, like records
    m_assembler->m_symTab->m_useLocals = m_parser.m_symTab->m_useLocals;
    m_assembler->m_symTab->m_records = m_parser.m_symTab->m_records;
    m_assembler->m_symTab->m_constants = m_parser.m_symTab->m_constants;
    // Write init assembler code

    for (QSharedPointer<SymbolTable>  st : m_parser.m_symTab->m_records)
        m_assembler->m_symTab->Define(QSharedPointer<Symbol>(new Symbol(st->m_name, "RECORD")));

    connect(m_codeGen.get(), SIGNAL(EmitTick(QString)), this, SLOT( AcceptDispatcherTick(QString)));
    SymbolTable::pass = 1;


//    qDebug() <<m_assembler->m_symTab->m_symbols.keys().contains("song");

    emit EmitTick("<br>Building []");
    if (m_tree!=nullptr)
        try {
        qSharedPointerDynamicCast<NodeProgram>(m_tree)->m_initJumps = m_parser.m_initJumps;
        m_codeGen->as = m_assembler.get();
        // Visit that AST node tree baby!
        Data::data.compilerState = Data::DISPATCHER;
//        qDebug() << "Compiler.cpp DISPATCHER starts";
        Node::s_as = m_assembler.get();
        //m_tree->ExecuteSym(m_assembler->m_symTab);
        //qDebug() << m_assembler->m_symTab->m_symbols.keys();
        m_tree->Accept(m_codeGen.get());

    } catch (const FatalErrorException& e) {
        HandleError(e,"Error during build (dispatcher) :");
        return false;
    }
    emit EmitTick("&100"); // Emit 100%


    for (QSharedPointer<MemoryBlock> mb:m_parser.m_userBlocks)
        m_assembler->blocks.append(mb);


    disconnect(m_codeGen.get(), SIGNAL(EmitTick(QString)), this, SLOT( AcceptDispatcherTick(QString)));
    disconnect(&m_parser, SIGNAL(emitRequestSystemChange(QString)), this, SLOT( AcceptRequestSystemChange(QString)));
//    emit EmitTick("<br>Connecting and optimising");

    Connect();
//    qDebug() << "Start address "<< Util::numToHex(system->m_startAddress);
    m_assembler->m_source<<m_assembler->m_endInsertAssembler;
    CleanupBlockLinenumbers();

    if (m_assembler->m_optimiser!=nullptr && m_ini->getdouble("post_optimize")==1.0) {
        m_assembler->m_source = m_assembler->m_optimiser->PostOptimize(m_assembler->m_source);
        m_assembler->m_totalOptimizedLines+=m_assembler->m_optimiser->m_linesOptimized;
    }

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

bool Compiler::SetupMemoryAnalyzer(QString filename, Orgasm *orgAsm)
{
//    qDebug() << "SETUPMEMORYANALYZER";
    if (orgAsm == nullptr)
        return false;
    //   Orgasm orgAsm;

    if (m_assembler==nullptr)
        return false;
    int i = 1;
    bool is6502 = Syntax::s.m_currentSystem->m_processor==AbstractSystem::MOS6502;
    bool isZ80 = Syntax::s.m_currentSystem->m_processor==AbstractSystem::Z80;

  //  if (is6502)
    for (QString s : orgAsm->m_symbolsList){
        if (s.toLower().startsWith("startblock")) {

            int start = orgAsm->m_symbols[s];
            QString search = s.toLower().replace("startblock","endblock");
            int end = start;
            for (QString s2 : orgAsm->m_symbolsList){
                if (s2.toLower() == search) {
                    end = orgAsm->m_symbols[s2];
                    //                    qDebug() << "Found:" <<search<<Util::numToHex(end);
                    break;
                }
            }
//                     qDebug() << "RANGE " << Util::numToHex(start) << Util::numToHex(end) <<s<<Util::numToHex(orgAsm->m_symbols[s]);
            if (start!=end) {
                if (m_assembler!=nullptr) {
                    QString name = "Code block "+QString::number(i++);
                    for (auto bl : m_assembler->userBlocks)
                        if (bl->m_start == start)
                            if (bl->m_name!="")
                                name = bl->m_name;
                    //                qDebug() << "Adding cod eblock with name : "<<name;
                    bool ok = true;
                    for (auto b: m_assembler->blocks) {
                        if (b->m_start==start)// && b->m_end==end)
                            ok = false;
                    }
                    if (ok) {
                        m_assembler->blocks.append(QSharedPointer<MemoryBlock>(new MemoryBlock(start, end, MemoryBlock::CODE, name)));
                    }

                }
            }
        }
    }

//    if (!isZ80)
    for (QSharedPointer<MemoryBlock> mb : m_assembler->blocks) {
        if (mb->m_type==MemoryBlock::USER)
            continue;
        if (mb->m_type==MemoryBlock::MUSIC)
            continue;
        if (mb->m_type==MemoryBlock::CODE) // Already taken care of
             continue;

        QString str = Util::numToHex(mb->m_start);
        str = str.toLower().remove("$");
        int end = mb->m_start;
        QString curEnd = ("endblock"+str);


        for (QString s : orgAsm->m_symbols.keys())  {
            QString chk = s;
            if (chk.toLower()==curEnd) {
  //              qDebug() << "Found real end:" <<chk.toLower();
                end = orgAsm->m_symbols[s];
                mb->m_end = end;
                break;
            }
        }



    }

    std::sort(m_assembler->blocks.begin(), m_assembler->blocks.end(),
              [](const auto& a, const auto& b) { return a->m_start < b->m_start; });


    // Check for overlaps:
    for (int i=0;i<m_assembler->blocks.count();i++) {
        auto x = m_assembler->blocks[i];
        for (int j=i+1;j<m_assembler->blocks.count();j++) {
            auto y = m_assembler->blocks[j];
            //            x1 <= y2 && y1 <= x2
            if (x->m_start<y->m_end && y->m_start<x->m_end) {
                //   qDebug() << "OVERLAP " << Util::numToHex(x->m_start)<<Util::numToHex(x->m_end) << " vs " << Util::numToHex(y->m_start)<<Util::numToHex(y->m_end);
                x->m_isOverlapping = true;
                y->m_isOverlapping = true;
                y->m_shift = x->m_shift+1;
                ErrorHandler::e.Warning("Overlapping memory regions: '"+x->m_name + "' and '"+y->m_name+"' at "+Util::numToHex(y->m_start)+" to " +Util::numToHex(x->m_end)+". See the memory analyzer for details.");

            }
        }
    }

    return orgAsm->m_success;


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

void Compiler::ApplyOptions(QMap<QString, QStringList> &opt) {
    for (auto& s: opt.keys()) {
        if (s.toLower()=="define") {
            QStringList lst = opt[s];
            if (lst.count()==1) {
                m_parser.m_preprocessorDefines[lst[0]]="1";
            }
            if (lst.count()==2)
                m_parser.m_preprocessorDefines[lst[0]]=lst[1];

        }
    }

}

void Compiler::AcceptDispatcherTick(QString val)
{
    emit EmitTick(val);
}

void Compiler::AcceptRequestSystemChange(QString val)
{
    emit emitRequestSystemChange(val);
}
