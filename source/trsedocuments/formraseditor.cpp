﻿/*
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
#include "formraseditor.h"
#include "ui_formraseditor.h"
#include <QProcess>
#include "source/Compiler/errorhandler.h"
#include <QElapsedTimer>
#include "source/LeLib/util/util.h"
#include <QScrollBar>
#include "source/chip8emu/dialogchip8.h"



bool FormRasEditor::m_broadcast = true;
QString FormRasEditor::m_globalOutput = "";
//SourceBuilder* BuilderThread::m_builder = nullptr;


FormRasEditor::FormRasEditor(QWidget *parent) :
    TRSEDocument(parent),
    ui(new Ui::FormRasEditor)
{
    ui->setupUi(this);
    m_fileExtension = "ras";

    connect(&m_builderThread, SIGNAL(emitText()), this, SLOT(HandleUpdateBuildText()));
    connect(&m_builderThread, SIGNAL(emitSuccess()), this, SLOT(HandleBuildComplete()));
    connect(&m_builderThread, SIGNAL(emitError()), this, SLOT(HandleBuildError()));
    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::GAMEBOY)
        m_programEndingType = "gb";


    m_type = RAS;
    m_timer.start();
    m_lastBuild = m_timer.elapsed();

    ui->chkExomize->setText(Syntax::s.m_currentSystem->m_exomizerName);
    if (!Syntax::s.m_currentSystem->m_supportsExomizer) {
        ui->chkExomize->setVisible(false);
        ui->btnHelpExomize->setVisible(false);

    }

    connect(ui->txtEditor, SIGNAL(emitLookupWord()),this,SLOT(LookupSymbolUnderCursor()));


    // Enable shadow builds?
    /*    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(ShadowBuild()));
    timer->start(5000); //time specified in ms
*/
    //    ui->txtOutput->set
}

FormRasEditor::~FormRasEditor()
{
    delete ui;
}

void FormRasEditor::UpdateHelpText(QStringList& truFiles)
{
    ui->widgetHelp->BuildTRU(truFiles);
    m_truList = truFiles;
}

void FormRasEditor::FocusOnOutput()
{
    //ui->tabOutputs->setCurrentIndex(0);
}

void FormRasEditor::setOutputText(QString text) {

    QStringList lst = text.split("<br>");
    QString out, warning;
    for (auto& s : lst) {
        if (s.contains("Warning"))
            warning+=s+"<br>";
       else
            out+=s+"<br>";
    }
    ui->txtOutput->setHtml(out);
    ui->txtWarnings->setHtml(warning);
//    PropagateMainOutput();
}


void FormRasEditor::ExecutePrg(QString fileName)
{
//    QString emu = m_iniFile->getString("emulator");
    QString emu = Syntax::s.m_currentSystem->getEmulatorName();


    QStringList params;

    Syntax::s.m_currentSystem->applyEmulatorParameters(params,fileName+".sym", fileName,m_builderThread.m_builder->m_projectIniFile.get());
    if (emu=="internal") {
        // Internal emulator!
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::CHIP8) {
            dialogchip8* dc8 = new dialogchip8(params[0]);
            dc8->exec();
            delete dc8;
            return;
        }
    }

    QString name = "emulator_additional_parameters_"+ AbstractSystem::StringFromSystem(Syntax::s.m_currentSystem->m_system);
    // Additional parameters
    if (m_iniFile->contains(name)) {
        QStringList pl = m_iniFile->getString(name).trimmed().split(" ");
        //        qDebug() << "Additional params: "<<pl;
        pl.removeAll("");
        if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::MOS6502)
            params = pl<<params;
        else
            params<<pl;
    }


    // Macos considerations (.app ending)
#ifdef __APPLE__
    if (emu.toLower().endsWith(".app") || emu.toLower().endsWith(".app/")) {
        if (emu.endsWith("/"))
            emu.remove(emu.length()-1,1);
        QString ls = emu.split("/").last().remove(".app");
        emu = emu + "/Contents/MacOS/"+ls;
    }

#endif
    // Custom must be at last, since it overwrites parameters
    if (Syntax::s.m_currentSystem->isCustom()) {
        emu = m_projectIniFile->getString("custom_system_emulator");
    }


    if (!QFile::exists(emu)) {
        Messages::messages.DisplayMessage(Messages::messages.NO_EMULATOR);
        ui->txtOutput->setText("Could not find the emulator for system '" + AbstractSystem::StringFromSystem(Syntax::s.m_currentSystem->m_system) + "'\nMake sure you have set a correct path in the TRSE settings!\n\n"+
                               "Example: VICE 'c64','c128','xvic', 'dosbox' or NES 'mednafen'.");
        return;
    }
    QProcess process;

#ifdef __APPLE__
    if (emu.endsWith(".exe")) {
        // USE WINE
        params.insert(0,emu);
        emu = "/opt/homebrew/bin/wine64";
       }
#endif

    process.waitForFinished();
    QString orgDir = QDir::currentPath();

    if (Syntax::s.m_currentSystem->m_requireEmulatorWorkingDirectory) {
        process.setWorkingDirectory(QFileInfo(emu).path());
        QDir::setCurrent(QFileInfo(emu).path());
    }



#ifdef _WIN32
    QProcess::execute("taskkill /im \"x64.exe\" /f");
#endif
    //    qDebug() << emu << " " << params <<  QDir::toNativeSeparators(fileName);
#ifdef __APPLE__
    if (emu.startsWith("x"))
        QProcess::execute("killall",QStringList() << QFileInfo(emu).fileName());

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::JDH8)
        QProcess::execute("killall",QStringList() << "emu");


    if (emu.toLower().contains("openemu"))
        QProcess::execute("killall",QStringList() << QFileInfo(emu).fileName());

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::M1ARM) {

//        process.startDetached("zsh",QStringList() <<"-c" <<fileName);
        process.startDetached("open",QStringList()<<"-a"<<"Terminal"<<fileName);
        QString output(process.readAllStandardOutput());
        return;
    }


    if (emu.endsWith(".app")) {
        process.setArguments(params);
        process.setProgram(emu);
        process.startDetached();
    }
    else {
        qDebug() <<emu<<params;
        process.startDetached(emu, params);

    }


#else
    //    qDebug()<<"TEST"+QDir::toNativeSeparators(fileName)+"TEST";


    process.startDetached(emu, params);

    //qDebug() << params;
#endif
    //    process.pi
    QString output(process.readAllStandardOutput()+process.readAllStandardError());
//    qDebug() <<output;
    QDir::setCurrent(orgDir);
    Syntax::s.m_currentSystem->ExtraEmulatorCommands();
    //    process.waitForFinished();
}

void FormRasEditor::InitDocument(WorkerThread *t, QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pro)
{
    TRSEDocument::InitDocument(t,ini, pro);
    FillFromIni();

    setupEditor();
    ui->txtEditor->m_displayCycles = m_iniFile->getdouble("display_cycles")==1;
    ui->txtEditor->m_displayAddresses = m_iniFile->getdouble("display_addresses")==1;
    ui->txtEditor->m_autoComplete = m_iniFile->getdouble("editor_autocomplete_nobug")==1;
    ui->txtEditor->m_autoIndent = m_iniFile->getdouble("editor_autocomplete")==1;


}



void FormRasEditor::setupEditor()
{
    QString fnt = m_iniFile->getString("editor_font");
    if (fnt=="") fnt ="Courier";
    m_font.setFamily(fnt);
    m_font.setFixedPitch(true);
    m_font.setPointSize(m_iniFile->getdouble("font_size"));
    //ui->txtEditor->setTextColor(QColor(220,210,190));
    SetupHighlighter();
    //    highlighter->Save("dark_standard.ini");

    UpdateFromIni();
    ui->txtEditor->setCursorWidth(m_iniFile->getdouble("editor_cursor_width"));
    ui->splitter->setSizes(QList<int>() << 10000 << 3500);
    //    ui->splitter->setStretchFactor(0, 10);
    //  ui->splitter->setStretchFactor(1, 0);
    //    ui->txtEditor->setTabStopWidth(m_iniFile->getInt("tab_width") * metrics.width(' '));

    //    qDebug() << "FILE " <<m_currentSourceFile;
    if (m_currentFileShort.contains("[ext]")) {
        ui->txtOutput->setHtml("<font color=\"#FF8020\"><b>WARNING: </b></font>You are currently editing an <font color=\"#FF8020\">internal and global TRSE unit</font> that is probably being used by other projects. </font>");
    }

}

void FormRasEditor::Compress()
{

}

void FormRasEditor::Build(bool isShadow)
{
    // Enforce pause between builds

    FocusOnOutput();
    if (abs(m_timer.elapsed()-m_lastBuild)<250) // 250 ms beteen each try
        return;

    m_lastBuild = m_timer.elapsed();
    if (m_builderThread.m_isRunning) {
        return;
    }
    m_builderThread.quit();



    if (!isShadow)
        SaveCurrent();

    //    VerifyCommodoreStartChange();


    if (!(QApplication::keyboardModifiers() & Qt::ShiftModifier))
        if (!isShadow)
            if (m_projectIniFile->getString("main_ras_file")!="none") {
                if (m_projectIniFile->getString("main_ras_file")!=m_currentFileShort) {
                    emit requestBuildMain(m_run);
                    return;
                }

            }

    if (m_currentSourceFile.toLower().endsWith(".inc")) {
        ui->txtOutput->setText("You cannot compile include files");
        return;
    }



    //    if (m_builderThread.m_builder!=nullptr)
    //      delete m_builderThread.m_builder;

    /*    if (m_builderThread.m_builder!=nullptr) {
//        m_builderThread.m_builder->Destroy();
        delete m_builderThread.m_builder;
    }
*/
    if (m_builderThread.m_builder!=nullptr) {
        disconnect(m_builderThread.m_builder.get(), SIGNAL(EmitBuildString()), this, SLOT(AcceptBuildString()));
        disconnect(m_builderThread.m_builder.get(), SIGNAL(emitRequestSystemChange(QString)), this, SLOT(AcceptRequestSystemChange(QString)));
    }

    m_builderThread.m_builder = QSharedPointer<SourceBuilder>(new SourceBuilder(m_iniFile, m_projectIniFile, m_currentDir, m_currentSourceFile));
    m_builderThread.m_builder->m_isShadow = isShadow;
    connect(m_builderThread.m_builder.get(), SIGNAL(EmitBuildString()), this, SLOT(AcceptBuildString()));
    connect(m_builderThread.m_builder.get(), SIGNAL(emitRequestSystemChange(QString)), this, SLOT(AcceptRequestSystemChange(QString)));


    emit requestBuild();


    m_builderThread.m_filename = filename;
    m_builderThread.m_source = ui->txtEditor->toPlainText();
    while (m_builderThread.isRunning()) {

    }
    Data::data.isBuilding = true;
    if (m_iniFile->getdouble("compile_thread")==1)
        m_builderThread.start();
    else
        m_builderThread.run();

    Data::data.isBuilding = false;

}

/*
void FormRasEditor::setOutputText(QString txt)
{
    ui->txtOutput->setHtml(ErrorHandler::e.m_teOut);
    //    ui->txtOutput->set

}
*/

void FormRasEditor::BuildNes(QString prg)
{
    QByteArray header;
    header.resize(16);
    header.fill(0);
    header[0] = 0x4E;
    header[1] = 0x45;
    header[2] = 0x53;
    header[3] = 0x1A;
    // 0000 1000
    //    qDebug() << "NES: " <<prg;
    header[4] = m_projectIniFile->getdouble("nes_16k_blocks"); // PRG rom kb
    // 0001 0000

    header[5] = m_projectIniFile->getdouble("nes_8k_blocks"); // CHR rom
    header[6] = 0b00000001;


    QByteArray data = Util::loadBinaryFile(prg+ ".prg");
    data = data.remove(0,2);

    data.insert(0,header);
    int dc = data.length();
    //    int dCount = m_projectIniFile->getdouble("nes_16k_blocks");
    //if (dc<pow(2,14))
    int j=pow(2,14)*2-dc+16;
    //    qDebug() << "j";
    for (int i=0;i<j;i++)
        data.append((char)0);


    data.append(Util::loadBinaryFile(m_currentDir+"/"+m_projectIniFile->getString("nes_8k_file")));
    if (m_projectIniFile->getdouble("nes_8k_blocks")>=2) {
        data.append(Util::loadBinaryFile(m_currentDir+"/"+m_projectIniFile->getString("nes_8k_file_2")));
    }


    if (QFile::exists(prg+".nes"))
        QFile::remove(prg+".nes");

    QFile out(prg+ ".nes");
    out.open(QFile::WriteOnly);
    //    out.write(header);
    out.write(data);
    out.close();

}

void FormRasEditor::LookupSymbolUnderCursor()
{
    QTextCursor tc = ui->txtEditor->textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    QString word = tc.selectedText();
    tc.select(QTextCursor::LineUnderCursor);
    QString block = tc.selectedText();

    int pos = block.indexOf(word);
    QString prefix = "";
    QString postfix = "";
    tc = ui->txtEditor->textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    // Check for stuff like Unit::IamClicked, if so add "Unit_"
    if (pos>2) {
        if (block[pos-1]==':' &&block[pos-2]==':') {
            tc.movePosition(QTextCursor::WordLeft);
            tc.movePosition(QTextCursor::WordLeft);
            tc.movePosition(QTextCursor::WordLeft);
            tc.select(QTextCursor::WordUnderCursor);
            prefix = tc.selectedText() + "_";

        }
    }
    // Reset
    tc = ui->txtEditor->textCursor();
    tc.select(QTextCursor::WordUnderCursor);

    pos += word.length();
    // Check for stuff like Unit::IamClicked, if os add "_IamClicked"

    if (pos<block.length()-3) {
        if (block[pos]==':' &&block[pos+1]==':') {
            tc.movePosition(QTextCursor::WordRight);
            tc.select(QTextCursor::WordUnderCursor);
            postfix = "_"+tc.selectedText();
        }
    }

    word = prefix + word + postfix;
    emit emitGotoSymbol(word);
}

void FormRasEditor::LookupAssemblerUnderCursor()
{
    emit emitGotoAssemblerLine(m_currentFileShort, ui->txtEditor->textCursor().blockNumber());
    //    void emitGotoAssemblerLine(QString rasSrc, int lineNuber);

}

void FormRasEditor::ToggleComment()
{
    ui->txtEditor->ToggleComments();
}



void FormRasEditor::Setup()
{
    setupEditor();
}

void FormRasEditor::VerifyCommodoreStartChange()
{
    if (!(Syntax::s.m_currentSystem->m_system == AbstractSystem::C64 ||
          Syntax::s.m_currentSystem->m_system == AbstractSystem::VIC20 ||
          Syntax::s.m_currentSystem->m_system == AbstractSystem::PLUS4 ||
          Syntax::s.m_currentSystem->m_system == AbstractSystem::C128 ||
          Syntax::s.m_currentSystem->m_system == AbstractSystem::PET)) return;

    QString iniField = "ignoremessage_commodore_start_change";
    if (m_projectIniFile->getdouble("override_target_settings")==1) {
        if (!(m_projectIniFile->contains(iniField) && m_projectIniFile->getdouble(iniField)==1)) {
            //  qDebug() << "HERE" << m_projectIniFile->getdouble("override_target_settings");
            DialogCustomWarning* d = new DialogCustomWarning();
            d->Init("Project Warning","A BASIC SYS start address has been specified in the project settings that is invalid."
" Please use [default] instead.",iniField,m_projectIniFile.get());
            d->exec();
            delete d;
        }
    }
}

void FormRasEditor::ApplySymbolList(SourceBuilder* c) {
    if (c==nullptr)
        return;
    if (c->compiler==nullptr)
        return;

    highlighter->AppendSymboltable(c->compiler->m_parser.m_procedures.keys());
    highlighter->rehighlight();

}

void FormRasEditor::Focus() {
    ui->txtEditor->setFocus();
}

bool FormRasEditor::isBuilding()
{

    return m_builderThread.m_isRunning;
}

QString FormRasEditor::getBuildText()
{
    if (m_builderThread.m_builder==nullptr)
        return "";
//    qDebug() << m_builderThread.m_builder->m_buildString;
    return m_builderThread.m_builder->m_buildString;
}


void FormRasEditor::Run()
{
    if (m_projectIniFile->getString("main_ras_file")!="none") {
        if (m_projectIniFile->getString("main_ras_file")!=m_currentFileShort) {

            emit requestRunMain();
            return;
        }

    }
    //    if (!m_run)
    //      return;
    if (m_builderThread.m_builder->m_system==nullptr)
        return;



    if (m_builderThread.m_builder->m_system->m_system == AbstractSystem::AMIGA) {
        //        Messages::messages.DisplayMessage(Messages::messages.NO_AMIGA_EMULATOR);
        //        qDebug() << "No amiga emulator installed";
        return;
    }
    if (m_builderThread.m_builder->m_system->m_system == AbstractSystem::ATARI520ST) {
        //        Messages::messages.DisplayMessage(Messages::messages.NO_AMIGA_EMULATOR);
        //        qDebug() << "No amiga emulator installed";
        return;
    }




    if (!m_builderThread.m_builder->m_system->m_buildSuccess)
        return;

    if (m_currentSourceFile.toLower().endsWith(".tru")) {
        ui->txtOutput->setHtml("<font color=\"red\">Cannot execute Turbo Rascal Unit (.tru) files. </font>");
    }

    if (!m_projectIniFile->contains("output_type"))
        m_projectIniFile->setString("output_type","prg");

    QString ft = ".ras";
    if (m_currentSourceFile.toLower().endsWith(".tru"))
        ft =".tru";

    QString base = m_currentSourceFile.split(ft)[0];
    QString orgFile = base;
    bool renameFile = false;
    if (m_builderThread.m_builder->compiler->m_parser.m_overrideOutputTarget!="") {
        renameFile = true;
//        base = m_builderThread.m_builder->compiler->m_parser.m_overrideOutputTarget;
    }
    QString filename = base;

    if (renameFile) {
        QString newFilename = filename;
        newFilename = newFilename.replace(orgFile, m_builderThread.m_builder->compiler->m_parser.m_overrideOutputTarget);
        Util::CopyFile(filename,newFilename);
        filename = newFilename;
    }


    ExecutePrg(filename);

    m_run = false;

}

void FormRasEditor::SetLights()
{
    if (m_builderThread.m_builder==nullptr) {
        ui->lblLight->setStyleSheet("QLabel { background-color : green; color : blue; }");
        return;
    }

    if (m_builderThread.m_builder->m_system==nullptr)
        return;

    if (!m_builderThread.m_builder->m_system->m_buildSuccess)
        ui->lblLight->setStyleSheet("QLabel { background-color : red; color : blue; }");
    else
        ui->lblLight->setStyleSheet("QLabel { background-color : green; color : blue; }");


}

void FormRasEditor::SetText(QString s)
{
    ui->txtEditor->setPlainText(s);
    ui->txtEditor->m_textChanged = false;
}

void FormRasEditor::SetupHighlighter()
{
    if (highlighter != nullptr)
        delete highlighter;
    CIniFile colors;
    colors.Load(Util::GetSystemPrefix() + "themes/" + m_iniFile->getString("theme"));
    ui->txtEditor->InitColors(colors);

    QPalette p = ui->txtEditor->palette();
    p.setColor(QPalette::Base, colors.getColor("backgroundcolor"));
    p.setColor(QPalette::Text, colors.getColor("textcolor"));
    ui->txtEditor->setPalette(p);
    //if (highlighter!=nullptr)
    //    delete highlighter;
    if (m_currentFileShort.toLower().endsWith(".ras") ||
            m_currentFileShort.toLower().endsWith(".tru") ||
            m_currentFileShort.toLower().endsWith(".inc")
            )
        highlighter = new Highlighter(colors, 0, ui->txtEditor->document());
    else
        highlighter = new AsmHighlighter(colors, 0, ui->txtEditor->document());

    QStringList ls = m_iniFile->getStringList("custom_keyword_colour");
    QColor col= Qt::white;
    if (ls.count()==3)
        col = QColor(ls[0].toInt(), ls[1].toInt(), ls[2].toInt());



    highlighter->m_customColour = col;
    highlighter->m_customList = m_iniFile->getStringList("custom_keyword_list");
    highlighter->m_customItalic = m_iniFile->getdouble("custom_keyword_italic")==1.0;
    highlighter->m_customBold = m_iniFile->getdouble("custom_keyword_bold")==1.0;


    highlighter->ApplyCustomKeywordList();
    //    qDebug() << "UPDATE " << m_iniFile->getString("theme");

}


void FormRasEditor::wheelEvent(QWheelEvent *event)
{

}

void FormRasEditor::keyPressEvent(QKeyEvent *e)
{
//    qDebug() << "before" << m_documentIsChanged;
    bool keep = ui->txtEditor->m_textChanged;
    TRSEDocument::keyPressEvent(e);
  //  qDebug() << "after" << m_documentIsChanged;
    if (e->key() == Qt::Key_Escape && ui->leSearch2->hasFocus()) {
        ui->txtEditor->setFocus();
    }

    if (e->key() == Qt::Key_Insert) {
        ui->txtEditor->setOverwriteMode(!ui->txtEditor->overwriteMode());
    }

    /*    if (e->key()==Qt::Key_W && (QApplication::keyboardModifiers() & Qt::ControlModifier))
        emit requestCloseWindow();
        m_documentIsChanged  = ui->txtEditor->m_textChanged;
    //    Data::data.requestCloseWindow = true;
*/
    //    if (ui->txtEditor->m_textChanged)

    //   if ((e->modifiers() & Qt::ControlModifier) && (e->modifiers() & Qt::ShiftModifier) && e->key()==Qt::Key_C)
    if ((e->modifiers() & Qt::ControlModifier) && e->key()==Qt::Key_E) {
        ToggleComment();
        ui->txtEditor->m_textChanged = keep;

    }


    if (e->key()==Qt::Key_J && (QApplication::keyboardModifiers() & Qt::ControlModifier)) AutoFormat();
    if (e->key()==Qt::Key_F && QApplication::keyboardModifiers() & Qt::ControlModifier) {
        ui->leSearch2->setText("");
        m_searchFromPos = ui->txtEditor->textCursor().position();
        ui->leSearch2->setFocus();
        ui->txtEditor->m_textChanged = keep;

    }
    if (e->key()==Qt::Key_G && (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        emit emitSearchSymbols();
        ui->txtEditor->m_textChanged = keep;
    }


    if (e->key()==Qt::Key_F1) {
        if ((m_help && m_help->isVisible()) || ui->chkAlwaysUseHelpWindow->isChecked()) {
            QTextCursor tc = ui->txtEditor->textCursor();
            tc.select(QTextCursor::WordUnderCursor);
            QString word = tc.selectedText();
            Help(word);
            return;
        }

        QTextCursor tc = ui->txtEditor->textCursor();
        tc.select(QTextCursor::WordUnderCursor);
        QString word = tc.selectedText();

        //        Help(word);
        /*     QTextCursor tc = ui->txtEditor->textCursor();
        tc.select(QTextCursor::WordUnderCursor);
        QString word = tc.selectedText();
*/
        ui->widgetHelp->BuildTRU(m_truList);
        //ui->tabOutputs->setCurrentIndex(1);

        ui->widgetHelp->SetFontSize(m_iniFile->getdouble("font_size"));
        //        ui->tabHelp->setFocus();
        ui->widgetHelp->Search(word);
        ui->txtEditor->m_textChanged = keep;
    }

    if (e->key()==Qt::Key_Escape)
        FocusOnOutput();

    if (e->key()==Qt::Key_F2) {
        LookupSymbolUnderCursor();
        ui->txtEditor->m_textChanged = keep;
    }

    if (e->key()==Qt::Key_F3) {
        LookupAssemblerUnderCursor();
        ui->txtEditor->m_textChanged = keep;
    }

    if (e->key() == Qt::Key_U &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        emit emitMemoryAnalyse();
        ui->txtEditor->m_textChanged = keep;
    }
    if (e->key() == Qt::Key_Y &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        emit emitSizeAnalyse();
    }
    if (e->key() == Qt::Key_F5 || (e->key() == Qt::Key_R &&  (QApplication::keyboardModifiers() & Qt::ControlModifier))) {
        m_run=true;
        Build();
        ui->txtEditor->m_textChanged = keep;
        //        Run();
    }


    m_documentIsChanged  = ui->txtEditor->m_textChanged;


}

bool FormRasEditor::isRasFile()
{
    return !m_isTRU;
}

void FormRasEditor::Destroy() {
    m_builderThread.quit();
}

void FormRasEditor::TestForCodeOverwrite(int codeEnd, QString& output)
{
    for (MemoryBlock& mb: m_builderThread.m_builder->compiler->m_assembler->m_userWrittenBlocks) {
        //        qDebug() << Util::numToHex(mb.m_start) << " vs " << Util::numToHex(codeEnd) ;
        if (mb.m_start<codeEnd && mb.m_start>=Syntax::s.m_currentSystem->m_startAddress) {
            output +="\n<font color=\"#FF8080\">WARNING:</font>Possible code block overwrite on line <b>" +QString::number(mb.m_lineNumber) + "</b>.&nbsp;";
            output += "<font color=\"#FF8080\">Proceed with caution </font>(writing to <font color=\"#FF8080\">"+Util::numToHex(mb.m_start)+"</font>, code ends at <font color=\"#FF8080\">"+Util::numToHex(codeEnd) +")</font>. <br>";
        }
    }
}

void FormRasEditor::GotoLine(int ln)
{
    //    QTextCursor cursor(ui->txtEditor->document()->findBlockByLineNumber(ln-1));
    //    int scrollPos =  ui->txtEditor->verticalScrollBar()->value();
    //    ui->txtEditor->setTextCursor(cursor);
    ui->txtEditor->moveCursor(QTextCursor::End);
    QTextCursor cursor(ui->txtEditor->document()->findBlockByLineNumber(ln-1));
    ui->txtEditor->setTextCursor(cursor);
    ui->txtEditor->verticalScrollBar()->setValue(ui->txtEditor->verticalScrollBar()->value()-4);

    // reset the cursor position (and scroll back again)
    //  setTextCursor(prevCursor);
    // scroll again to the remembered position
}



void FormRasEditor::on_leSearch2_textChanged()
{
    SearchInSource(ui->leSearch2->text().toLower());
}

void FormRasEditor::AcceptBuildString()
{
    //ui->txtOutput->setHtml(m_builderThread.m_builder->m_buildString);
    setOutputText(m_builderThread.m_builder->m_buildString);
    //    float t = 0.02;
    //  m_curCol = m_endCol;//(m_curCol)*(1-t) + m_endCol*t;
    ui->lblLight->setStyleSheet("QLabel { background-color : \""+ Util::toColor(m_curCol).name() + "\"; color : blue; }");
    emit emitOutputTextChanged();

}

void FormRasEditor::SearchInSource(QString text)
{
    m_currentFromPos = ui->txtEditor->document()->toPlainText().toLower().indexOf(text, m_searchFromPos);
    QTextCursor cursor(ui->txtEditor->document()->findBlock(m_currentFromPos));
    ui->txtEditor->moveCursor(QTextCursor::End);
    ui->txtEditor->setTextCursor(cursor);
    //    ui->txtEditor->scroll(0,200);
}

void FormRasEditor::UpdateColors()
{
    SetupHighlighter();
}

void FormRasEditor::UpdateFromIni()
{
    QFontMetrics metrics(m_font);

    m_font.setPointSize(m_iniFile->getdouble("font_size"));
    ui->txtEditor->setFont(m_font);
    ui->txtEditor->m_font = m_font;
    ui->txtEditor->m_metrics = new QFontMetrics(m_font);
    ui->txtEditor->setTabStopDistance(m_iniFile->getInt("tab_width") * metrics.averageCharWidth());

    m_font.setPointSize(m_iniFile->getdouble("font_size")*m_iniFile->getdouble("font_size_scale"));
    ui->txtEditor->m_fontScale = m_iniFile->getdouble("font_size_scale");
    ui->txtEditor->m_font = m_font;
    ui->txtEditor->m_autoComplete = m_iniFile->getdouble("editor_autocomplete")==1;

}

void FormRasEditor::AutoFormat()
{
    /*    if (!BuildStep())
        return;

    int pos = ui->txtEditor->textCursor().position();
    QStringList source = ui->txtEditor->document()->toPlainText().split("\n");
    int ln = 0;
    int curBlock = 0;
    int nextBlock = 0;
    int singleLine = 0;
    QString outSource = "";
    bool varBlock = false;
    for (QString& s:source) {
        QString nxt = "";
        if (ln+1<source.count()-1)
            nxt = source[ln+1];
        nxt=nxt.toLower();

        int add = singleLine;
        s=s.trimmed();
        nextBlock = curBlock;
        QString k = s.toLower();
        if (k.startsWith("procedure")) {
            curBlock = 0;
            nextBlock = 0;
        }

        if (varBlock)
            if (k.startsWith("procedure") || k.startsWith("begin")) {
                varBlock = false;
                curBlock = 0;
                nextBlock = 0;
            }


        if (varBlock) {
            curBlock=1;
            nextBlock = 1;
        }
        if (k=="var")
            varBlock = true;

        if (k.contains("begin"))
            nextBlock++;
        if (k.startsWith("end")) {
            curBlock--;
            nextBlock = curBlock;
        }
        if (k.contains("if ") || k.contains("while ") || k.contains("for ") || k.contains("else")) {
            if (!k.contains("begin") && !nxt.contains("begin"))
                if (!k.endsWith(";") )
                singleLine = 1;
        }
        if (!s.trimmed().startsWith("@"))
        for (int i=0;i<curBlock+add;i++)
            s.insert(0,"\t");

        if (add!=0) {
            curBlock--;
            singleLine = 0;
        }

        ln++;
        outSource+=s+"\n";
        curBlock = nextBlock;
    }
    SetText(outSource);
    QTextCursor tc = ui->txtEditor->textCursor();
    tc.setPosition(pos);
    ui->txtEditor->setTextCursor(tc);
*/
}
/*
bool FormRasEditor::BuildStep()
{
    if (!m_currentSourceFile.toLower().endsWith(".ras")) {
        ui->txtOutput->setText("Turbo Rascal SE can only compile .ras files");
        if (m_system!=nullptr)
            m_system->m_buildSuccess = false;
        return false;
    }


    QString text = ui->txtEditor->toPlainText();
    ErrorHandler::e.m_level = ErrorHandler::e.ERROR_ONLY;
    ErrorHandler::e.m_teOut = "";
    ErrorHandler::e.exitOnError = false;
    QStringList lst = text.split("\n");


    compiler = Compiler(m_iniFile, m_projectIniFile);
    compiler.m_parser.m_diskFiles = getFileList();
    compiler.m_parser.m_currentDir = m_currentDir;

    compiler.Parse(text,lst);

    QString path = m_projectIniFile->getString("project_path") + "/";
    filename = m_currentSourceFile.split(".")[0];

    if (m_system != nullptr)
        delete m_system;


    m_system = FactorySystem::Create(AbstractSystem::SystemFromString(
                                         m_projectIniFile->getString("system")),
                                        m_iniFile, m_projectIniFile);

    m_system->timer.start();
    m_system->m_buildSuccess = true;
    return compiler.Build(m_system, path);
}

*/
void FormRasEditor::FillFromIni()
{
    ui->chkPostOpt->setChecked(m_iniFile->getdouble("post_optimize")==1);
    //    ui->chkExomize->setChecked(m_iniFile->getdouble("perform_crunch")==1);
    ui->chkExomize->setChecked(m_projectIniFile->getdouble("exomizer_toggle")==1);
    ui->chkRemoveUnusedSymbols->setChecked(m_projectIniFile->getdouble("remove_unused_symbols")==1);
    ui->chkWarnings->setChecked(m_iniFile->getdouble("display_warnings")==1);
    ui->chkWarnings->setVisible(false);
    ui->chkDisplayAddresses->setVisible(Syntax::s.m_currentSystem->HasAddressCounter());
    ui->chkDisplayCycles->setVisible(Syntax::s.m_currentSystem->HasCycleCounter());
    //    qDebug() << "FillFromIni" << m_iniFile->getdouble("perform_crunch");


    ui->chkDisplayAddresses->setChecked(m_iniFile->getdouble("display_addresses")==1);
    ui->chkDisplayCycles->setChecked(m_iniFile->getdouble("display_cycles")==1);
    ui->chkAlwaysUseHelpWindow->setChecked(m_iniFile->getdouble("always_use_help_window")==1.0);

    //    qDebug() << "PARSER "<<m_iniFile->getdouble("display_cycles");
    isInitialized=true;
}

void FormRasEditor::FillToIni()
{
    if (!isInitialized)
        return;

    m_iniFile->setFloat("post_optimize",ui->chkPostOpt->isChecked()?1:0);
    m_projectIniFile->setFloat("exomizer_toggle",ui->chkExomize->isChecked()?1:0);
    m_projectIniFile->setFloat("remove_unused_symbols",ui->chkRemoveUnusedSymbols->isChecked()?1:0);
    m_iniFile->setFloat("display_warnings",1);

    m_iniFile->setFloat("display_addresses",ui->chkDisplayAddresses->isChecked()?1:0);
    if (!Syntax::s.m_currentSystem->HasAddressCounter())
        m_iniFile->setFloat("display_addresses",0);
    if (!Syntax::s.m_currentSystem->HasCycleCounter())
        m_iniFile->setFloat("display_cycles",0);
    m_iniFile->setFloat("display_cycles",ui->chkDisplayCycles->isChecked()?1:0);

    ui->txtEditor->m_displayCycles = ui->chkDisplayCycles->isChecked();
    ui->txtEditor->m_displayAddresses = ui->chkDisplayAddresses->isChecked();

    m_iniFile->setFloat("always_use_help_window",ui->chkAlwaysUseHelpWindow->isChecked()?1:0);


    ui->txtEditor->update();
    m_iniFile->Save();
}

void FormRasEditor::MemoryAnalyze(bool isHidden)
{
    if (m_builderThread.m_isRunning)
        return;


    if (!m_currentSourceFile.endsWith(".ras")) {
        ErrorHandler::e.m_warnings.clear();
        ErrorHandler::e.m_teOut = "";
        ErrorHandler::e.Warning("Memory analyzer only works for .ras source files");
        setOutputText(ErrorHandler::e.m_teOut);
        return;
    }
    int i= m_projectIniFile->getdouble("exomizer_toggle");
    m_projectIniFile->setFloat("exomizer_toggle",0);

    if (m_builderThread.m_builder==nullptr) {
        ErrorHandler::e.m_warnings.clear();
        ErrorHandler::e.m_teOut = "";
        ErrorHandler::e.Warning("Source file must be built before memory analyzer can run.");
        setOutputText(ErrorHandler::e.m_teOut);
        return;
    }
    if (!m_builderThread.m_builder->m_buildSuccess) {
        ErrorHandler::e.m_warnings.clear();
        ErrorHandler::e.m_teOut = "";
        ErrorHandler::e.Warning("Source file must be built before memory analyzer can run.");
        setOutputText(ErrorHandler::e.m_teOut);
        return;
    }

    /*    if (!m_builderThread.m_builder->Build(ui->txtEditor->toPlainText())) {
        return;
    }
*/


    m_projectIniFile->setFloat("exomizer_toggle",i);
    m_builderThread.m_builder->compiler->SaveBuild(filename + ".asm");

    filename = m_currentSourceFile;
    filename = filename.remove(".ras");
    //    qDebug() << "Filename; "<< m_currentSourceFile;
    //    m_builderThread.m_builder->
    //    bool success = m_builderThread.m_builder->compiler->SetupMemoryAnalyzer(filename);
    if (isHidden)
        return;

    m_mca.ClassifyZP(m_builderThread.m_builder->compiler->m_assembler->blocks);

    DialogMemoryAnalyze* dma = new DialogMemoryAnalyze(m_iniFile,m_projectIniFile, m_builderThread.m_builder->m_system.get());
    dma->m_success = m_builderThread.m_builder->m_buildSuccess;
    dma->Initialize(m_builderThread.m_builder->compiler->m_assembler->blocks, m_iniFile->getInt("memory_analyzer_font_size"));
    dma->resize(m_iniFile->getdouble("memory_analyzer_window_width"),m_iniFile->getdouble("memory_analyzer_window_height"));
    dma->m_noBanks = m_builderThread.m_builder->compiler->m_assembler->m_noBanks;
    dma->exec();
    delete dma;
}

void FormRasEditor::Reload()
{
    if (!m_currentSourceFile.contains(".asm"))
        return;
    int pos = ui->txtEditor->textCursor().position();

    Load(m_currentSourceFile);
    QTextCursor tc = ui->txtEditor->textCursor();
    tc.setPosition(pos);
    ui->txtEditor->setTextCursor(tc);

}

void FormRasEditor::Save(QString filename)
{
    //  if (QFile::exists(filename))
    //    QFile::remove(filename);
    QString txt = ui->txtEditor->document()->toPlainText();
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QTextStream stream(&file);
        stream << txt;
    }
    file.close();
    m_iniFile->Save();
    ui->txtEditor->m_textChanged = false;
}

bool FormRasEditor::Load(QString filename)
{
    QFile file(filename);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        SetText(file.readAll());
    }
    file.close();
    ui->txtEditor->m_fileType = CodeEditor::RAS;
    m_isTRU = filename.toLower().endsWith(".tru");
    if (m_isTRU)
        ui->txtEditor->m_fileType = CodeEditor::TRU;
    if (filename.toLower().endsWith(".asm"))
        ui->txtEditor->m_fileType = CodeEditor::ASM;
    if (filename.toLower().endsWith(".inc"))
        ui->txtEditor->m_fileType = CodeEditor::INC;

    ShadowBuild();
    return true;
}



void FormRasEditor::on_leSearch2_returnPressed()
{
    m_searchFromPos=m_currentFromPos+1;
    SearchInSource(ui->leSearch2->text().toLower());

}

void FormRasEditor::on_leSearch2_textChanged(const QString &arg1)
{
    SearchInSource(ui->leSearch2->text().toLower());
}

void FormRasEditor::on_btnReplace_clicked()
{
    QString orgstr = ui->leSearch2->text();
    QString replacestr = ui->leReplace->text();

    QString source = ui->txtEditor->toPlainText();
    source = source.replace(orgstr, replacestr);

    SetText(source);

}

void FormRasEditor::on_chkExomize_stateChanged(int arg1)
{
    FillToIni();
}

void FormRasEditor::on_chkRemoveUnusedSymbols_stateChanged(int arg1)
{
    FillToIni();
}

void FormRasEditor::on_chkPostOpt_stateChanged(int arg1)
{
    FillToIni();
}

void FormRasEditor::HandleBuildError()
{
    m_run = false;
    setOutputText(ErrorHandler::e.m_teOut);
    m_outputText = ErrorHandler::e.m_teOut;
    int ln = Pmm::Data::d.lineNumber;

    m_builderThread.m_builder->m_system->m_buildSuccess = false;
    SetLights();
    if (m_builderThread.m_builder->m_isShadow)
        return;

    HandleErrorDialogs(ErrorHandler::e.m_teOut);
    //    qDebug() << "FormRasEditor "<<ln;
    if (m_builderThread.m_builder!=nullptr)
        emit OpenOtherFile(m_builderThread.m_builder->compiler->recentError.file, ln);
    if (ln>1)
        GotoLine(ln);

    m_globalOutput = m_outputText;
    emit emitFailure();

}

void FormRasEditor::HandleErrorDialogs(QString& output)
{
    return;
    if (output.toLower().contains("branch out of range")) {
        Messages::messages.DisplayMessage(Messages::messages.BRANCH_ERROR);
        output += "<br>Please check your <b>onpage/offpage</b> keywords.";

    }
    else
        if (output.toLower().contains("reverse-indexed")) {
            Messages::messages.DisplayMessage(Messages::messages.MEMORY_OVERLAP_ERROR);
            output += "<br>Please reorganize your binary inclusions in ascending order of memory locations.";
        }
        else
            if (output.toLower().contains("mnemonic")) {
                output += "<br>Please make sure you have used well-defined labels and variables in your inline assembly code.";
            }

            else
                Messages::messages.DisplayMessage(Messages::messages.DASM_COMPILER_ERROR);


    if (!output.toLower().contains("complete.")) {
    }

}
void FormRasEditor::HandleUpdateBuildText()
{
    setOutputText(m_builderThread.m_builder->getOutput());

}

void FormRasEditor::HandleBuildComplete()
{
    m_builderThread.msleep(70); // crashes if we don't sleep.. for some reason
    bool keepTextChanged = ui->txtEditor->m_textChanged;
    if (m_builderThread.m_builder->compiler->m_assembler!=nullptr) {
        ui->txtEditor->m_cycles =  m_builderThread.m_builder->compiler->m_assembler->m_cyclesOut;
        ui->txtEditor->m_addresses =  m_builderThread.m_builder->compiler->m_assembler->m_addressesOut;
        ui->txtEditor->m_blockCycles =  m_builderThread.m_builder->compiler->m_assembler->m_blockCyclesOut;
    }
    ui->txtEditor->RepaintCycles();
    if (m_builderThread.m_builder->compiler->m_assembler!=nullptr)

        ui->txtEditor->InitCompleter(m_builderThread.m_builder->compiler->m_assembler->m_symTab, &m_builderThread.m_builder->compiler->m_parser);

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::NES) {
        BuildNes(Util::getFileWithoutEnding(m_currentSourceFile));
    }
    HandleErrorDialogs(ErrorHandler::e.m_teOut);
    highlighter->m_symTab = m_builderThread.m_builder->compiler->m_parser.m_symTab;
    SetLights();
    if (m_broadcast && (m_currentFileShort.toLower().endsWith(".ras")  ||m_currentFileShort.toLower().endsWith(".tru"))) {
        emit NotifyOtherSourceFiles(m_builderThread.m_builder);
    }

    highlighter->AppendSymboltable(m_builderThread.m_builder->compiler->m_parser.m_procedures.keys());
    highlighter->rehighlight();
    //    ui->txtEditor->viewport()->update();
    ui->txtEditor->m_textChanged = keepTextChanged;

    m_globalOutput =m_builderThread.m_builder->getOutput();
    emit emitSuccess();
    //    qDebug() << "EXECUTING RUN" <<m_run <<m_currentFileShort;
    //    qDebug() << "WOOT"  <<m_builderThread.m_builder->m_system->m_buildSuccess ;
    if (m_run) {
        m_builderThread.m_builder->AddMessage("<br>Running program...");
        HandleUpdateBuildText();
        Run();
    }
    m_builderThread.m_builder->m_buildString= m_builderThread.m_builder->getOutput();
    emit emitOutputTextChanged();

    m_run = false;
}

void FormRasEditor::Help(QString word) {
    if (!(m_help && m_help->isVisible()))
        m_help = QSharedPointer<DialogHelp>(new DialogHelp(nullptr, word, m_defaultPalette,m_truList));
    else {
        m_help->Search(word);
        m_help->setFocus();
    }

    m_help->SetFontSize(m_iniFile->getdouble("font_size"));
    m_help->show();
    //    delete dh;

}

void FormRasEditor::AcceptRequestSystemChange(QString val)
{
    emit emitRequestSystemChange(val);
    Build();
}

void FormRasEditor::ShadowBuild()
{
    if (m_builderThread.m_isRunning)
        return;
    if (!m_hasFocus)
        return;

    Build(true);


}


void BuilderThread::run()
{
    m_isRunning=true;
    Data::data.isCompiling = true;
    if (m_builder->Build( m_source ))
    {
        if (!m_builder->m_isShadow) {
            if (!m_builder->m_currentSourceFile.toLower().endsWith(".asm"))
                m_builder->compiler->SaveBuild(m_filename + ".asm");
            //        else {
            //          m_builder->m_filename = m_builder->m_currentSourceFile.split(".asm")[0];
            //    }
            //        qDebug() << m_builder->m_filename;

//            m_builder->AddMessage("Assembling & compressing... ");

            //emit emitText();


            m_builder->Assemble();
            if (m_builder->m_buildSuccess) {
                //qDebug() << "FORMRASEDITOR "<<m_builder->m_currentSourceFile;
                m_builder->compiler->SetupMemoryAnalyzer(Util::getFileWithoutEnding(m_filename), m_builder->m_system->m_orgAsm.get());
                //            qDebug() << m_builder->getOutput();

            }

            emit emitText();
        }
        emit emitSuccess();

    }
    else {
        emit emitText();
        emit emitError();
    }
    m_isRunning=false;
    Data::data.isCompiling = false;

}

void FormRasEditor::on_chkWarnings_stateChanged(int arg1)
{
    FillToIni();

}

void FormRasEditor::on_btnHelpExomize_clicked()
{
    Help("Exomizer");
}

void FormRasEditor::on_btnHelpRemoveUnusedSymbols_clicked()
{
    Help("RemoveUnusedSymbols");

}

void FormRasEditor::on_btnHelpPostOptimize_clicked()
{
    Help("postoptimize");

}

void FormRasEditor::on_chkDisplayAddresses_stateChanged(int arg1)
{
    FillToIni();
}

void FormRasEditor::on_chkDisplayCycles_stateChanged(int arg1)
{
    FillToIni();
}

void FormRasEditor::on_btnViewHelp_clicked()
{
    Help(ui->widgetHelp->m_currentWord);

}
