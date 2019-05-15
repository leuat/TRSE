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

#include "formraseditor.h"
#include "ui_formraseditor.h"
#include <QProcess>
#include "source/Compiler/errorhandler.h"
#include <QElapsedTimer>
#include "source/LeLib/util/util.h"


FormRasEditor::FormRasEditor(QWidget *parent) :
    TRSEDocument(parent),
    ui(new Ui::FormRasEditor)
{
    ui->setupUi(this);
    m_fileExtension = "ras";
}

FormRasEditor::~FormRasEditor()
{
    delete ui;
}

void FormRasEditor::ExecutePrg(QString fileName, QString system)
{

    QString emu = m_iniFile->getString("emulator");
    QStringList params;

    if (m_projectIniFile->getString("system")=="VIC20") {
        emu = m_iniFile->getString("vic20_emulator");
        params<< "-autostartprgmode" << "1";
        params<< "-memory" << m_projectIniFile->getString("vic_memory_config");
    }
    if (m_projectIniFile->getString("system")=="PET") {
        emu = m_iniFile->getString("pet_emulator");
        params<< "-autostartprgmode" << "1";
//        params<< "-memory" << m_projectIniFile->getString("vic_memory_config");
    }
    if (m_projectIniFile->getString("system")=="C128") {
        emu = m_iniFile->getString("c128_emulator");

        //params << "-" + m_projectIniFile->getString("columns")+"col";
    }
    if (m_projectIniFile->getString("system")=="NES") {
        emu = m_iniFile->getString("nes_emulator");
    }
    if (m_projectIniFile->getString("system")=="BBCM") {
        emu = m_iniFile->getString("bbcm_emulator");
    }


    if (!QFile::exists(emu)) {
        Messages::messages.DisplayMessage(Messages::messages.NO_EMULATOR);
        ui->txtOutput->setText("Could not find the emulator for system '"+m_projectIniFile->getString("system")+"'\nMake sure you have set a correct path in the TRSE settings dialoge!\n\n"+
        "Example: VICE 'c64','c128','xvic' or NES 'mednafen'.");
        return;
    }
    QProcess process;
    if (m_projectIniFile->getString("system")=="VIC20" || m_projectIniFile->getString("system")=="C64" || m_projectIniFile->getString("system")=="C128")
        if (m_iniFile->getdouble("auto_inject")==1.0) {
           params << "-autostartprgmode" << "1";
        }
    params << QDir::toNativeSeparators(fileName);
    process.waitForFinished();
#ifdef _WIN32
    QProcess::execute("taskkill /im \"x64.exe\" /f");
#endif
//    qDebug() << emulator << " " << params <<  QDir::toNativeSeparators(fileName);
    process.startDetached(emu, params);
//    process.pi
    QString output(process.readAllStandardOutput());

}

QStringList FormRasEditor::getFileList()
{
    QString pawFile = m_projectIniFile->getString("d64_paw_file");
    if (pawFile=="none") return QStringList();
    CIniFile paw;
    paw.Load(m_currentDir + "/"+pawFile);
    QStringList data = paw.getStringList("data");
    QStringList ret;
    int count = data.count()/3;
    for (int i=0;i<count;i++) {
        ret<< data[3*i];
    }

    QString pawFile2 = m_projectIniFile->getString("d64_paw_file_disk2");
    if (pawFile2=="none") return ret;
    CIniFile paw2;
    paw2.Load(m_currentDir + "/"+pawFile2);
    QStringList data2 = paw2.getStringList("data");
    int count2 = data2.count()/3;
    for (int i=0;i<count2;i++) {
        ret<< data2[3*i];
    }


    return ret;

}

void FormRasEditor::InitDocument(WorkerThread *t, CIniFile *ini, CIniFile* pro)
{
    TRSEDocument::InitDocument(t,ini, pro);
    FillFromIni();

    setupEditor();
}



void FormRasEditor::setupEditor()
{
    m_font.setFamily("Courier");
    m_font.setFixedPitch(true);
    m_font.setPointSize(m_iniFile->getdouble("font_size"));
    //ui->txtEditor->setTextColor(QColor(220,210,190));
    SetupHighlighter();
//    highlighter->Save("dark_standard.ini");

    UpdateFromIni();
//    ui->txtEditor->setTabStopWidth(m_iniFile->getInt("tab_width") * metrics.width(' '));

}

void FormRasEditor::Compress()
{

}

void FormRasEditor::Build()
{
    SaveCurrent();

    if (m_projectIniFile->getString("main_ras_file")!="none") {
        if (m_projectIniFile->getString("main_ras_file")!=m_currentFileShort) {
            emit requestBuildMain();
            return;
        }

    }

    emit requestBuild();

    if (BuildStep())
        {
//        qDebug() << "ME";
        compiler.SaveBuild(filename + ".asm");
        QString text ="Build <b><font color=\"#90FF90\">Successful</font>!</b> ( "+  (Util::MilisecondToString(m_system->timer.elapsed())) +")<br>";
        text+="Assembler file saved to : <b>" + filename+".asm</b><br>";
        text+="Compiled <b>" + QString::number(compiler.m_parser.m_lexer->m_lines.count()) +"</b> lines of Turbo Rascal to <b>";
        text+=QString::number(compiler.m_assembler->getLineCount()) + "</b> lines of assembler instructions (and variables/labels)<br>";
        if (m_iniFile->getdouble("post_optimize")==1) {
            text+="Post-optimized away <b>" + QString::number(compiler.m_assembler->m_totalOptimizedLines) +"</b> lines of assembler instructions ";
            text=text+"(<font color=\"#70FF40\"> " + QString::number((int)(100.0*(float)compiler.m_assembler->m_totalOptimizedLines/(float)compiler.m_assembler->getLineCount()))+  " % </font> of total ) <br>";

        }
        else
            text+="<font color=\"#FFA090\">Warning:</font>Post-optimizer disabled. Enable for faster results (unless post-optimizer breaks something).<br>";

//        text+="+"<br>";

//        Assemble6502(text);
        m_system->Assemble(filename, text, m_currentDir);
        if (m_system->m_buildSuccess)
            m_system->PostProcess(filename, text, m_currentDir);


        ui->txtOutput->setText(text);
        ui->txtEditor->m_cycles =  compiler.m_assembler->m_cycles;
        ui->txtEditor->RepaintCycles();
        ui->txtEditor->InitCompleter(compiler.m_assembler->m_symTab, &compiler.m_parser);


    }
    else {
        SetOutputText(ErrorHandler::e.m_teOut);
        m_outputText = ErrorHandler::e.m_teOut;
        int ln = Pmm::Data::d.lineNumber;

        emit OpenOtherFile(compiler.recentError.file, ln);
        GotoLine(ln);
        m_system->m_buildSuccess = false;

    }
    if (m_projectIniFile->getString("system")=="NES") {
        BuildNes(m_currentSourceFile.split(".")[0]);

    }
    SetLights();
}


void FormRasEditor::SetOutputText(QString txt)
{
    ui->txtOutput->setText(ErrorHandler::e.m_teOut);

}

int FormRasEditor::FindEndSymbol(Orgasm& orgasm)
{
//    QStringList output = QString(out).split("\n");
    for (QString s : orgasm.m_symbols.keys()) {
        if (s.toLower().contains("endsymbol")) {
            return orgasm.m_symbols[s];
            //s= s.remove("EndSymbol").trimmed();
//            bool ok;
  //          qDebug() << "FOUND END " << s;
//            exit(1);
//            return s.toInt(&ok, 16);
        }
    }
    return 0;
}

void FormRasEditor::FindBlockEndSymbols(Orgasm& orgasm)
{
    m_blockEndSymbols.clear();
    for (QString s : orgasm.m_symbols.keys()) {
        if (s.toLower().contains("endblock")) {
           QString spl = s;
            spl = spl.toLower().simplified().split("block")[1];
//            bool ok;
            int i= orgasm.m_symbols[s];//.toInt(&ok, 16);
            //qDebug() << "FOUND endblock : " << s << Util::numToHex(i);
            m_blockEndSymbols.append(i);
        }
    }

}

void FormRasEditor::ConnectBlockSymbols()
{
    for (int sym : m_blockEndSymbols) {
        int winner = 0xFFFF;
        MemoryBlock* winnerBlock=nullptr;

        for (MemoryBlock* mb: compiler.m_assembler->blocks) {
//            if (mb->m_type==MemoryBlock::CODE &&  sym>mb->m_start)
                if (sym>mb->m_start)
                if (sym-mb->m_start<winner) {
                    winner = sym-mb->m_start;
                    winnerBlock  =mb;
                }
        }
        if (winnerBlock!=nullptr) {
            winnerBlock->m_end = sym;
       //     qDebug() << Util::numToHex(sym) << " " << winnerBlock->Type();
        }
    }


}

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

    header[4] = m_projectIniFile->getdouble("nes_16k_blocks"); // PRG rom kb
    // 0001 0000

    header[5] = m_projectIniFile->getdouble("nes_8k_blocks"); // CHR rom
    header[6] = 0b00000001;


    QFile f(prg+ ".prg");
    f.open(QFile::ReadOnly);
    QByteArray data = f.readAll();
    data = data.remove(0,2);
    f.close();
//    qDebug() << prg;

    data.insert(0,header);
    int dc = data.count();
//    qDebug() << dc;
//    exit(1);
    int j=pow(2,14)*m_projectIniFile->getdouble("nes_16k_blocks")-dc+16;
//    qDebug() << "Appending: " <<j;
    for (int i=0;i<j;i++)
        data.append((char)0);



    //qDebug() << data.size();

    QByteArray chr;
    QFile mmm(m_currentDir+"/"+m_projectIniFile->getString("nes_8k_file"));
    mmm.open(QFile::ReadOnly);
    chr = mmm.readAll();
    //qDebug() << chr.size();
    //chr.fill(255);
    mmm.close();

    data.append(chr);

    /*for (int i=0;i<pow(2,13);i++)
        data.append((char)rand()%255);
*/
/*    if (QFile::exists(prg+".nes"))
        QFile::remove(prg+".nes");

    QFile out(prg+ ".nes");
    out.open(QFile::WriteOnly);
//    out.write(header);
    out.write(data);
    out.close();
*/
}



void FormRasEditor::Setup()
{
    setupEditor();
}


void FormRasEditor::Run()
{
    if (m_system==nullptr)
        return;
    if (m_projectIniFile->getString("main_ras_file")!="none") {
        if (m_projectIniFile->getString("main_ras_file")!=m_currentFileShort) {
            emit requestRunMain();
            return;
        }

    }

    if (!m_system->m_buildSuccess)
        return;

    if (!m_projectIniFile->contains("output_type"))
        m_projectIniFile->setString("output_type","prg");

    QString filename = m_currentSourceFile.split(".")[0] + "."+ m_projectIniFile->getString("output_type");
    if (m_projectIniFile->getString("system")=="NES")
        filename = m_currentSourceFile.split(".")[0] + ".nes";
//    qDebug() << filename;
//    exit(1);

    ExecutePrg(filename, m_projectIniFile->getString("system"));

}

void FormRasEditor::SetLights()
{
    if (m_system==nullptr)
        return;
    if (!m_system->m_buildSuccess)
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
    colors.Load(Util::path + "themes/" + m_iniFile->getString("theme"));
    ui->txtEditor->InitColors(colors);

    QPalette p = ui->txtEditor->palette();
    p.setColor(QPalette::Base, colors.getColor("backgroundcolor"));
    p.setColor(QPalette::Text, colors.getColor("textcolor"));
    ui->txtEditor->setPalette(p);
    highlighter = new Highlighter(colors, 0, ui->txtEditor->document());

//    qDebug() << "UPDATE " << m_iniFile->getString("theme");

}


void FormRasEditor::wheelEvent(QWheelEvent *event)
{

}

void FormRasEditor::keyPressEvent(QKeyEvent *e)
{
    TRSEDocument::keyPressEvent(e);
    if (e->key() == Qt::Key_Escape && ui->leSearch->hasFocus()) {
        ui->txtEditor->setFocus();
    }


    if (e->key()==Qt::Key_W && (QApplication::keyboardModifiers() & Qt::ControlModifier))
        emit requestCloseWindow();
    //    Data::data.requestCloseWindow = true;

//    if (ui->txtEditor->m_textChanged)
        m_documentIsChanged  = ui->txtEditor->m_textChanged;

    if (e->key()==Qt::Key_J && (QApplication::keyboardModifiers() & Qt::ControlModifier)) AutoFormat();
    if (e->key()==Qt::Key_F && QApplication::keyboardModifiers() & Qt::ControlModifier) {
        ui->leSearch->setText("");
        m_searchFromPos = ui->txtEditor->textCursor().position();
        ui->leSearch->setFocus();
    }

    if (e->key()==Qt::Key_F1) {
        QTextCursor tc = ui->txtEditor->textCursor();
        tc.select(QTextCursor::WordUnderCursor);
        QString word = tc.selectedText();

        DialogHelp* dh = new DialogHelp(nullptr, word, m_defaultPalette);
//        dh->setPalette(m_defaultPalette);
     //   QApplication::setPalette(m_defaultPalette);

        dh->show();

    }

    if (e->key()==Qt::Key_F2) {
        QTextCursor tc = ui->txtEditor->textCursor();
        tc.select(QTextCursor::WordUnderCursor);
        QString word = tc.selectedText();
        for (Node*n : compiler.m_parser.m_proceduresOnly) {
            NodeProcedureDecl* np = dynamic_cast<NodeProcedureDecl*>(n);
            if (np->m_procName.toLower()==word.toLower()) {
                int ln=np->m_op.m_lineNumber;
                QTextCursor cursor(ui->txtEditor->document()->findBlockByLineNumber(ln));
                ui->txtEditor->setTextCursor(cursor);

            }
        }


    }


    if (e->key() == Qt::Key_U &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        MemoryAnalyze();
    }
    if (e->key() == Qt::Key_R &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        Build();
        Run();
    }



}

void FormRasEditor::TestForCodeOverwrite(int codeEnd, QString& output)
{
    for (MemoryBlock& mb: compiler.m_assembler->m_userWrittenBlocks) {
//        qDebug() << Util::numToHex(mb.m_start) << " vs " << Util::numToHex(codeEnd) ;
        if (mb.m_start<codeEnd && mb.m_start>=Syntax::s.m_startAddress) {
            output +="\n<font color=\"#FF8080\">WARNING:</font>Possible code block overwrite on line <b>" +QString::number(mb.m_lineNumber) + "</b>.&nbsp;";
            output += "<font color=\"#FF8080\">Proceed with caution </font>(writing to <font color=\"#FF8080\">"+Util::numToHex(mb.m_start)+"</font>, code ends at <font color=\"#FF8080\">"+Util::numToHex(codeEnd) +")</font>. <br>";
        }
    }
}

void FormRasEditor::GotoLine(int ln)
{
    QTextCursor cursor(ui->txtEditor->document()->findBlockByLineNumber(ln-1));
    ui->txtEditor->setTextCursor(cursor);
}



void FormRasEditor::on_leSearch_textChanged()
{
    QString i;
    SearchInSource();
}

void FormRasEditor::SearchInSource()
{
    m_currentFromPos = ui->txtEditor->document()->toPlainText().toLower().indexOf(ui->leSearch->text().toLower(), m_searchFromPos);
    QTextCursor cursor(ui->txtEditor->document()->findBlock(m_currentFromPos));
    ui->txtEditor->setTextCursor(cursor);
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
    ui->txtEditor->setTabStopWidth(m_iniFile->getInt("tab_width") * metrics.width(' '));


}

void FormRasEditor::AutoFormat()
{
    if (!BuildStep())
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

}

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

    return compiler.Build(m_system, path);
}


void FormRasEditor::FillFromIni()
{
    ui->chkPostOpt->setChecked(m_iniFile->getdouble("post_optimize")==1);
    ui->chkExomize->setChecked(m_iniFile->getdouble("perform_crunch")==1);
//    qDebug() << "FillFromIni" << m_iniFile->getdouble("perform_crunch");
    isInitialized=true;
}

void FormRasEditor::FillToIni()
{
    if (!isInitialized)
        return;
//    qDebug() << "FillToIni A" << m_iniFile->getdouble("perform_crunch");
    if (ui->chkPostOpt->isChecked())
        m_iniFile->setFloat("post_optimize",1);
    else
        m_iniFile->setFloat("post_optimize",0);

    if (ui->chkExomize->isChecked())
        m_iniFile->setFloat("perform_crunch",1);
    else
        m_iniFile->setFloat("perform_crunch",0);

  //  qDebug() << "FillToIni B" << m_iniFile->getdouble("perform_crunch");

    m_iniFile->Save();
}

void FormRasEditor::MemoryAnalyze()
{
    int i= m_iniFile->getdouble("perform_crunch");
    m_iniFile->setFloat("perform_crunch",0);
    if (!BuildStep())
        return;
    m_iniFile->setFloat("perform_crunch",i);
    compiler.SaveBuild(filename + ".asm");

    /*QProcess process;
    process.start(m_iniFile->getString("dasm"), QStringList()<<(filename +".asm") << ("-o"+filename+".prg") << "-v3");
    process.waitForFinished();
    //process;
    QString output = process.readAllStandardOutput();
    int codeEnd=FindEndSymbol(output);
    */

    Orgasm orgAsm;
    //orgAsm.LoadCodes();
    orgAsm.Assemble(filename+".asm", filename+".prg");
    if (!orgAsm.m_success)
        return;
    int codeEnd=FindEndSymbol(orgAsm);

    FindBlockEndSymbols(orgAsm);
    ConnectBlockSymbols();
    compiler.m_assembler->blocks.append(new MemoryBlock(Syntax::s.m_startAddress, codeEnd, MemoryBlock::CODE, "code"));

    m_mca.ClassifyZP(compiler.m_assembler->blocks);

    DialogMemoryAnalyze* dma = new DialogMemoryAnalyze();
    dma->Initialize(compiler.m_assembler->blocks, m_iniFile->getInt("memory_analyzer_font_size"));
    dma->exec();

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

void FormRasEditor::Load(QString filename)
{
    QFile file(filename);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        SetText(file.readAll());
    }
    file.close();

}



void FormRasEditor::on_leSearch_returnPressed()
{
    m_searchFromPos=m_currentFromPos+1;
    SearchInSource();

}

void FormRasEditor::on_leSearch_textChanged(const QString &arg1)
{
    SearchInSource();
}

void FormRasEditor::on_btnReplace_clicked()
{
    QString orgstr = ui->leSearch->text();
    QString replacestr = ui->leReplace->text();

    QString source = ui->txtEditor->toPlainText();
    source = source.replace(orgstr, replacestr);

    SetText(source);

}

void FormRasEditor::on_chkExomize_stateChanged(int arg1)
{
    FillToIni();
}

void FormRasEditor::on_chkPostOpt_stateChanged(int arg1)
{
    FillToIni();
}
