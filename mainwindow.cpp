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

#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QDebug>
#include <QThread>
#include <QProcess>
#include <QElapsedTimer>
#include <QTextCursor>
#include <QFontMetrics>
#include "source/LeLib/data.h"
#include "source/LeLib/util/util.h"
#include <QWheelEvent>
#include "dialognewimage.h"
#include "source/dialogabout.h"
#include "source/LeLib/limage/limageio.h"
#include <QMessageBox>
#include <QSettings>
#include "source/Compiler/assembler/mos6502/mos6502.h"
#include "source/dialogeffects.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/parser.h"
#include "source/Compiler/compilers/compiler.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_defaultPalette = palette();
    ui->setupUi(this);
   // m_work.m_colorList.CreateUI(ui->layoutColors,0);

    TRSEDocument::m_defaultPalette = m_defaultPalette;
    qRegisterMetaTypeStreamOperators<CItem>("CItem");

    //QObject::connect(m_updateThread, SIGNAL(valueChanged()), this, SLOT (Update()));
//    m_updateThread = new WorkerThread();
//    connect(m_updateThread, SIGNAL(updateImageSignal()), this, SLOT(updateImage()));


   Data::data.version += "   Build  " + QDate::currentDate().toString();


    this->setMouseTracking(true);
    m_currentDoc = nullptr;

#if defined(Q_OS_MAC)
    Util::path = QCoreApplication::applicationDirPath() + "/../../";

#endif

#ifdef __linux__
    Util::path = QCoreApplication::applicationDirPath() + "/../";

#endif
    m_currentPath = "";
    ui->lblCommodoreImage->setAlignment(Qt::AlignCenter);
    LoadIniFile();
    connect( ui->tabMain, SIGNAL(tabCloseRequested(int)),this, SLOT(RemoveTab(int)));
//    connect(qApp, SIGNAL(aboutToQuit()), m_updateThread, SLOT(OnQuit()));
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(OnQuit()));

   int id= QFontDatabase::addApplicationFont(":resources/fonts/c64.ttf");
   m_fontFamily = QFontDatabase::applicationFontFamilies(id).at(0);
    UpdateRecentProjects();

    SetupFileList();

    /*QImage img;
    img.load(":resources/images/trselogo.png");
    ui->lblLogo->setPixmap(QPixmap::fromImage(img));
    */

    ui->splitter->setStretchFactor(0,10);
    ui->splitter->setStretchFactor(1,100);


    //Messages::messages.DisplayMessage(Messages::messages.ALPHA_WARNING);
    ui->lblSave->setHidden(true);
    ui->lblBuild->setHidden(true);
    this->installEventFilter(this);

    m_tutorials.Read(":resources/text/tutorials.txt");
//    m_tutorials.PopulateTreeList(ui->treeTutorials);
    m_tutorials.PopulateSystemList(ui->lstSystems);
    ui->lstSystems->setCurrentRow(0);
    setWindowTitle("Turbo Rascal Syntax error, \";\" expected but \"BEGIN\" Version " + Data::data.version);
    ui->textBrowser->setText( ui->textBrowser->toHtml().replace("@version",Data::data.version));

    ui->treeSymbols->setHeaderHidden(true);

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(ShowContextMenu(const QPoint &)));

}




MainWindow::~MainWindow()
{
    m_quit = true;
    delete ui;
}

void MainWindow::AfterStart(QString oldCurDir)
{
    if (m_commandParams.count()>1) {
        QString p1 = m_commandParams[1];
        if (p1.toLower().endsWith(".trse")) {
#ifdef _WIN32
        LoadProject(p1);
#else
        LoadProject(oldCurDir+QDir::separator()+ p1);
#endif
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    FormImageEditor* fe = dynamic_cast<FormImageEditor*>(m_currentDoc);
    if (fe==nullptr)
        return;


//    m_updateThread->pos = mapToGlobal( fe->getLabelImage()->rect().topLeft());
}


void MainWindow::mousePressEvent(QMouseEvent *e)
{
/*    if(e->buttons() == Qt::RightButton)
        m_updateThread->m_currentButton = 2;
    if(e->buttons() == Qt::LeftButton) {
        m_updateThread->m_currentButton = 1;
    }

    qDebug() << "WTOF" << m_updateThread->m_currentButton;*/

}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
}

void MainWindow::wheelEvent(QWheelEvent *event)
{


}


void MainWindow::keyPressEvent(QKeyEvent *e)
{

    Data::data.forceRedraw = true;
    Data::data.Redraw();


    if (m_currentDoc!=nullptr) {
        m_currentDoc->keyPressEvent(e);
    }
    if (e->key()==Qt::Key_F2) {
        m_currentDoc->Focus();
    }

}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{

}

void MainWindow::RestoreSettings()
{
    QSettings settings("LemonSpawn", "TRSE");
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    restoreState(settings.value("MainWindow/windowState").toByteArray());

    ui->psplitter->restoreState(settings.value("MainWindow/psplitter").toByteArray());
    ui->qsplitter->restoreState(settings.value("MainWindow/qsplitter").toByteArray());
    ui->splitter->restoreState(settings.value("MainWindow/splitter").toByteArray());

}


void MainWindow::VerifyDefaults()
{
    if (!m_iniFile->contains("windowpalette"))
        m_iniFile->setFloat("windowpalette", 0);

    if (!m_iniFile->contains("font_size"))
        m_iniFile->setFloat("font_size", 12);
    if (!m_iniFile->contains("font_size_symbols"))
        m_iniFile->setFloat("font_size_symbols", 10);
    if (!m_iniFile->contains("hide_exomizer_footprint"))
        m_iniFile->setFloat("hide_exomizer_footprint", 1);

    if (!m_iniFile->contains("tab_width"))
        m_iniFile->setFloat("tab_width", 4);

    if (!m_iniFile->contains("editor_font"))
        m_iniFile->setString("editor_font","Courier");

    if (!m_iniFile->contains("editor_font_symbols"))
        m_iniFile->setString("editor_font_symbols","Courier");

    if (!m_iniFile->contains("auto_inject"))
        m_iniFile->setFloat("auto_inject", 1);

    if (!m_iniFile->contains("editor_cursor_width"))
        m_iniFile->setFloat("editor_cursor_width",1);

    if (!m_iniFile->contains("theme"))
        m_iniFile->setString("theme", "dark_standard.ini");
    if (!m_iniFile->contains("theme_fjong"))
        m_iniFile->setString("theme_fjong", "dark_standard.ini");

    if (!m_iniFile->contains("post_optimize"))
        m_iniFile->setFloat("post_optimize", 1);

    if (!m_iniFile->contains("display_warnings"))
        m_iniFile->setFloat("display_warnings", 1);

    if (!m_iniFile->contains("memory_analyzer_font_size"))
        m_iniFile->setFloat("memory_analyzer_font_size", 17);

    if (!m_iniFile->contains("memory_analyzer_window_width"))
        m_iniFile->setFloat("memory_analyzer_window_width", 600);
    if (!m_iniFile->contains("memory_analyzer_window_height"))
        m_iniFile->setFloat("memory_analyzer_window_height", 600);
    if (!m_iniFile->contains("image_painter"))
        m_iniFile->setFloat("image_painter", 0);



    //    qDebug() << m_ini->getString("ok64_emulator");
   if (!m_iniFile->contains("ok64_emulator") || m_iniFile->getString("ok64_emulator")=="")
    #ifdef __linux__
        m_iniFile->setString("ok64_emulator","bin/OK64");
    #endif
    #ifdef _WIN32
        m_iniFile->setString("ok64_emulator","ok64.exe");
    #endif


    if (!m_iniFile->contains("optimizer_remove_unused_symbols"))
     m_iniFile->setFloat("optimizer_remove_unused_symbols",0);

    if (!m_iniFile->contains("assembler"))
        m_iniFile->setString("assembler","OrgAsm");

    m_iniFile->filename = m_iniFileName;

}

void MainWindow::VerifyProjectDefaults()
{
    m_currentProject.VerifyDefaults();


}

void MainWindow::UpdateSymbolTree(QString search)
{
    if (!m_currentDoc->m_currentFileShort.toLower().endsWith(".ras"))
        return;
    FormRasEditor* e = dynamic_cast<FormRasEditor*>(m_currentDoc);
    if (e==nullptr)
        return;


    m_symPointers.clear();
    m_orgSymPointers.clear();
    m_treeItems.clear();

    QTreeWidgetItem* Symbols = new QTreeWidgetItem(QStringList() <<"Symbols");
    QTreeWidgetItem* Procedures = new QTreeWidgetItem(QStringList() <<"Procedures");

    if (e->m_builderThread.m_builder==nullptr)
        return;
    if (e->m_builderThread.m_builder->compiler==nullptr)
        return;
    Parser* p = &e->m_builderThread.m_builder->compiler->m_parser;


    if (p->m_symTab->m_symbols.keys().count()==0)
        return;

    ui->treeSymbols->clear();
    ui->treeSymbols->addTopLevelItem(Symbols);
    ui->treeSymbols->addTopLevelItem(Procedures);


    m_symbolItems.clear();



    for (QString key : p->m_symTab->m_symbols.keys()) {
        QSharedPointer<Symbol> s = p->m_symTab->m_symbols[key];
        QString t = s->m_type;
        if (t.toLower()=="array") t = s->m_arrayTypeText.toLower()+"[ "+QString::number(s->m_size)+ " ]";
        //if (t.toLower()=="record") t = s->m_arrayTypeText +"[ "+QString::number(s->m_size)+ " ]";
//        qDebug() << t << s->m_arrayTypeText;
        //if (p-) t = TokenType::getType(s->m_arrayType).toLower()+"[ "+QString::number(s->m_size)+ " ]";
        cleanSymbol(Symbols, s->m_name, s->m_name + " : " + t.toLower(), s->m_lineNumber, s->m_fileName,p,Qt::yellow,search);
    }
    m_symbolItems.clear();

    for (QString key : p->m_procedures.keys()) {
        QSharedPointer<Node> s = p->m_procedures[key];
        QSharedPointer<NodeProcedureDecl> proc = qSharedPointerDynamicCast<NodeProcedureDecl>(s);
        QString params = "(";
        for (QSharedPointer<Node> n: proc->m_paramDecl) {
            QSharedPointer<NodeVarDecl> vd = qSharedPointerDynamicCast<NodeVarDecl>(n);
//            params+=qSharedPointerDynamicCast<NodeVar>(vd->m_varNode)->value+" : ";
            params+=qSharedPointerDynamicCast<NodeVarType>(vd->m_typeNode)->value.toLower() + ",";
        }
        if (proc->m_paramDecl.count()!=0)
            params.remove(params.length()-1,1);
        params+=")";
       cleanSymbol(Procedures,proc->m_procName,proc->m_procName+params, proc->m_op.m_lineNumber, proc->m_fileName,p,Qt::cyan,search);
    }
    Procedures->setExpanded(true);
    if (search!="")
        Symbols->setExpanded(true);

}

void MainWindow::cleanSymbol(QTreeWidgetItem* parent, QString on, QString n, int ln, QString fn, Parser* p, QColor bcol, QString search)
{
    QString name = n;

    bool exp = false;
    if (search!="") {
        // filter
        if (!n.toLower().contains(search.toLower()))
            return;
        exp = true;
    }


    for (QString s: p->s_usedTRUNames) {
        name = name.replace(s+"_",s+"::");
    }

    QTreeWidgetItem* sym = new QTreeWidgetItem(QStringList() <<name);

    QString type = "";
    QColor col = bcol;

    if (n.isUpper()) {
        col = Qt::darkGray;
        type = "Built-in";
    }
    else if (name.contains("::")) {
        col = QColor(50,100,255);
        type = name.split("::").first();
    }
    else if (n.toLower().startsWith("init")) {
        col = Qt::darkGray;
        type = "Init";
    }
    else if (p->m_symTab->m_globalList.contains(n)) {
        sym->setForeground(0,QBrush(Qt::gray));
        col = Qt::gray;
        type = "Built-in";
    }
//    else
  //      sym->setForeground(0,QBrush(Qt::cyan));

    sym->setForeground(0,QBrush(col));
    sym->setData(0,Qt::UserRole,n);
    sym->setFont(0,QFont(m_iniFile->getString("editor_font_symbols"),m_iniFile->getInt("font_size_symbols")));
    m_treeItems[on] = sym;

    int l = ln;
    QString f;
    p->m_lexer->FindLineNumberAndFile(l, f, ln);

    if (f!="")
        fn =f;
 //   qDebug() << ln << fn;
    m_symPointers[n] =
            QSharedPointer<SymbolPointer>(new SymbolPointer(n, ln+1, fn));

    m_orgSymPointers[on] = m_symPointers[n];
    if (type=="") {
        parent->addChild(sym);
        return;
    }
    if (!m_symbolItems.contains(type)) {
        QTreeWidgetItem* grp = new QTreeWidgetItem(QStringList() << type);
        grp->setForeground(0,QBrush(col));
        m_symbolItems[type] = grp;
        parent->insertChild(0,grp);
    }
    QTreeWidgetItem* grp = m_symbolItems[type];
    grp->addChild(sym);
    if (exp) {
        parent->setExpanded(true);
        grp->setExpanded(true);
    }


//    for (QTreeWidgetItem* it : parent->)
//    parent->ch


//    return sym;




}





void MainWindow::LoadDocument(QString fileName)
{
    if (!QFile::exists( getProjectPath() + "/" +fileName))
        return;

    for (TRSEDocument* d: m_documents) {
        if (d->m_currentFileShort==fileName) {
            ui->tabMain->setCurrentWidget(d);
            return;
        }
    }

    TRSEDocument* editor = nullptr;
    if (fileName.contains(".flf")) {
        editor = new FormImageEditor(this);
        FormImageEditor* fe = (FormImageEditor*)editor;
        fe->m_projectPath = getProjectPath();

    }
    if (fileName.contains(".ras") || fileName.contains(".asm") || fileName.contains(".inc") || fileName.contains(".tru") ) {
        editor = new FormRasEditor(this);
    }
    if (fileName.contains(".fjo")) {
        editor = new FormFjong(this);
    }
    if (fileName.contains(".paw")  ) {
        editor = new FormPaw(this);
    }
    editor->InitDocument(nullptr, m_iniFile, m_currentProject.m_ini);
    editor->m_currentDir = m_currentPath+"/";
    editor->m_currentSourceFile = getProjectPath() + "/" + fileName;
    editor->m_currentFileShort = fileName;
    ui->tabMain->addTab(editor, fileName);
    editor->Load(editor->m_currentSourceFile);


    m_currentProject.m_ini->addStringList("open_files", editor->m_currentFileShort, true);
    m_currentProject.Save();



    editor->setFocus();
    editor->showMaximized();
    ui->tabMain->setCurrentWidget(editor);

    m_currentProject.m_ini->setString("current_file", fileName);
    //m_buildSuccess = false;
    ui->tabMain->setTabsClosable(true);
    m_documents.append(editor);
    m_currentDoc = editor;
    ConnectDocument();

}

bool MainWindow::VerifyFile(QString file, QString message)
{
    if (QFile::exists(m_currentPath+"//"+ file))
        return true;

    QMessageBox msgBox;
    msgBox.setWindowTitle("Error");
    msgBox.setInformativeText("Could not find file '"+file+"'");
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    return false;

}
void MainWindow::ConnectDocument()
{
    connect(m_currentDoc, SIGNAL(updatePaletteSignal()), this, SLOT(updatePalette()));
    connect(m_currentDoc, SIGNAL(requestSaveAs()), this, SLOT(SaveAs()));
    connect(m_currentDoc, SIGNAL(requestCloseWindow()), this, SLOT(closeWindowSlot()));
    connect(m_currentDoc, SIGNAL(emitFindFile()), this, SLOT(FindFileDialog()));
    connect(m_currentDoc, SIGNAL(requestBuild()), this, SLOT(acceptBuild()));
    connect(m_currentDoc, SIGNAL(requestBuildMain()), this, SLOT(acceptBuildMain()));
    connect(m_currentDoc, SIGNAL(requestRunMain()), this, SLOT(acceptRunMain()));

    connect(m_currentDoc, SIGNAL(emitNewRas()), this, SLOT(on_actionRas_source_file_triggered()));
    connect(m_currentDoc, SIGNAL(emitNewImage()), this, SLOT(on_actionImage_triggered()));

    connect(m_currentDoc, SIGNAL(emitSuccess()), this, SLOT(UpdateSymbolTree()));



//    connect(m_currentDoc, SIGNAL(NotifyOtherSourceFiles()), this, SLOT(AcceptUpdateSourceFiles()));
    if (m_currentDoc->m_type == TRSEDocument::RAS) {
        connect((FormRasEditor*)m_currentDoc, &FormRasEditor::NotifyOtherSourceFiles, this, &MainWindow::AcceptUpdateSourceFiles);
        connect(m_currentDoc, SIGNAL(emitSearchSymbols()), this, SLOT(acceptSearchSymbols()));
        connect(m_currentDoc, SIGNAL(OpenOtherFile(QString, int )), this, SLOT(ForceOpenFile(QString , int)));
        connect(m_currentDoc, SIGNAL(emitGotoSymbol(QString)), this, SLOT(GotoSymbol(QString)));
    }

}

void MainWindow::SetupFileList()
{
    RefreshFileList();
}





void MainWindow::RefreshFileList()
{
//    qDebug() << m_currentPath << rand()%100;
  //  if (m_currentPath=="")
    //    return;
    if (fileSystemModel!=nullptr)
        delete fileSystemModel;
    fileSystemModel = new CustomFileSystemModel(this);


    QString rootPath= getProjectPath();
    if (m_currentPath=="") {
        ui->treeFiles->setModel(nullptr);
            return;

    }
    fileSystemModel->setReadOnly(true);
    fileSystemModel->setRootPath(rootPath);
    fileSystemModel->setFilter(QDir::NoDotAndDotDot |
                            QDir::AllDirs |QDir::AllEntries);
    fileSystemModel->setNameFilters(QStringList() << "*.ras" << "*.tru" <<"*.asm" << "*.txt"/* << "*.prg" */<< "*.inc" << "*.flf" <<"*.paw" << "*.fjo");
    fileSystemModel->setNameFilterDisables(false);

    ui->treeFiles->setModel(fileSystemModel);
    ui->treeFiles->setRootIndex(fileSystemModel->index(rootPath));

    ui->treeFiles->hideColumn(1);
    ui->treeFiles->hideColumn(2);
    ui->treeFiles->hideColumn(3);



}

void MainWindow::AcceptUpdateSourceFiles(QSharedPointer<SourceBuilder> sourceBuilder)
{
    FormRasEditor::m_broadcast=false;

    QStringList files;
    if (sourceBuilder==nullptr)
        return;
    for (FilePart& fp: sourceBuilder->compiler->m_parser.m_lexer->m_includeFiles)
        files<<fp.m_name;
  //  qDebug() << files;
    for (TRSEDocument* t : m_documents) {
        if (t==m_currentDoc)
            continue;
        FormRasEditor* r = dynamic_cast<FormRasEditor*>(t);
        if (r!=nullptr) {
            QString name = r->m_currentFileShort;
            if (files.contains(name)) {
                sourceBuilder->compiler->CleanupCycleLinenumbers(name, sourceBuilder->compiler->m_assembler->m_cycles, sourceBuilder->compiler->m_assembler->m_cyclesOut);
                sourceBuilder->compiler->CleanupCycleLinenumbers(name,sourceBuilder->compiler->m_assembler->m_blockCycles,sourceBuilder->compiler->m_assembler->m_blockCyclesOut);
                r->m_builderThread.m_builder = QSharedPointer<SourceBuilder>(sourceBuilder);
                r->HandleBuildComplete();

            }
        }

    }
    FormRasEditor::m_broadcast=true;
}

void MainWindow::acceptBuildMain() {
    acceptBuild();
    if (!VerifyFile(m_currentProject.m_ini->getString("main_ras_file"),"Error trying to build main project file. Please see project settings and specify correct path to main .ras file"))
        return;
    LoadDocument(m_currentProject.m_ini->getString("main_ras_file"));
    m_currentDoc->Build();
}

void MainWindow::acceptSearchSymbols()
{
    ui->leFilterSymbols->clear();
    ui->leFilterSymbols->setFocus();
}

void MainWindow::acceptRunMain() {
    if (!VerifyFile(m_currentProject.m_ini->getString("main_ras_file"),"Error trying to build main project file. Please see project settings and specify correct path to main .ras file"))
        return;
    LoadDocument(m_currentProject.m_ini->getString("main_ras_file"));
    m_currentDoc->Run();
//    UpdateSymbolTree();
}



void MainWindow::OpenProjectSettings()
{
    if (m_currentProject.m_filename=="")
        return;

    QString oldSystem = m_currentProject.m_ini->getString("system");

    DialogProjectSettings* dSettings = new DialogProjectSettings(m_currentPath, this);
    dSettings->SetInit(m_currentProject.m_ini);
    dSettings->exec();
    delete dSettings;

    // Set compiler syntax based on system
//    Syntax::s.Init(AbstractSystem::SystemFromString(m_currentProject.m_ini->getString("system")),&m_iniFile, &m_currentProject.m_ini);
    if (oldSystem != m_currentProject.m_ini->getString("system"))
        LoadProject(m_currentProject.m_filename);

}




void MainWindow::OnQuit()
{
//    qDebug() << m_currentProject.m_ini->getStringList("open_files");
    m_currentProject.Save();
//    qDebug() << m_currentProject.m_ini->getString("current_file");

//    m_iniFile->setVec("splitpos", QVector3D(ui->splitter->sizes()[0],ui->splitter->sizes()[1],0));
    m_iniFile->Save();

}

void MainWindow::ForceOpenFile(QString s, int ln)
{
    s.remove(getProjectPath());
    while (s.startsWith("/"))
        s = s.remove(0,1);

    if (s=="")
        return;
    QString txt = "";
    if (m_currentDoc!=nullptr)
         txt = m_currentDoc->m_outputText;

    LoadDocument(s);
    m_currentDoc->GotoLine(ln);
    FormRasEditor* fe = dynamic_cast<FormRasEditor*>(m_currentDoc);
    if (fe!=nullptr)
        fe->SetOutputText(txt);
}

void MainWindow::closeWindowSlot()
{
    int idx = ui->tabMain->currentIndex();
    RemoveTab(idx);
    //qDebug() << "DONE";
}

void MainWindow::UpdateRecentProjects()
{
    ui->lstRecentProjects->clear();
    QStringList l = m_iniFile->getStringList("recent_projects");
    l.removeDuplicates();
    l.removeAll("");
//    qDebug() << l;

    for (QString s: l) {
        QListWidgetItem* item= new QListWidgetItem();
       // item->data(Qt::UserRole).QVariant
        item->setData(Qt::UserRole,s);
        QString name = s.split("/").last();
        item->setText(name);

        if (name.trimmed()!="")
        ui->lstRecentProjects->addItem(item);
    }

}

void MainWindow::SaveAs()
{
    if (m_currentDoc==nullptr)
        return;
    if (m_currentPath=="")
        return;
    QString ext = m_currentDoc->m_fileExtension;

    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    QString f = ext +" Files (*."+ext+")";
    QString filename = dialog.getSaveFileName(NULL, "Create New File",getProjectPath(),f);

    if (filename=="")
        return;
    QString orgFile;
    //filename = filename.split("/").last();

    m_currentDoc->m_currentSourceFile = filename;
    filename = filename.toLower().remove(getProjectPath().toLower());
    m_currentDoc->m_currentFileShort = filename;
    m_currentDoc->SaveCurrent();

    ui->tabMain->setTabText(ui->tabMain->currentIndex(),filename);
    updatePalette();


}

bool MainWindow::RemoveTab(int idx, bool save)
{
    if (idx==0)
        return true;


    idx--;
    TRSEDocument* doc = m_documents[idx];
    if (!doc->SaveChanges())
        return false;
//    m_updateThread->Park();
//    QThread::msleep(30);

    doc->PrepareClose();

    if (doc==nullptr)
        return false;
    if (save) {
        m_currentProject.m_ini->removeFromList("open_files", doc->m_currentFileShort);
        m_currentProject.Save();
    }


//    disconnect(m_currentDoc, SIGNAL(requestCloseWindow()), this, SLOT(closeWindowSlot()));

    ui->tabMain->removeTab(idx+1);

    m_documents[idx]->Destroy();
    m_documents[idx];
    m_documents.remove(idx);


//    m_updateThread->SetCurrentImage(nullptr, nullptr, nullptr);



 //   TRSEDocument* d = (TRSEDocument*)ui->tabMain->currentWidget();
 //   FormImageEditor* fe = dynamic_cast<FormImageEditor*>(d);
    ui->tabMain->currentWidget()->setFocus();


/*    if (fe!=nullptr)
       m_updateThread->SetCurrentImage(&fe->m_work, &fe->m_toolBox,fe->getLabelImage());

    m_updateThread->Continue();
    */
    return true;
}

bool MainWindow::CloseAll()
{
    while (ui->tabMain->count()!=1) {
        if (!RemoveTab(1, false))
            return false;
    }
    return true;

}

QString MainWindow::getProjectPath()
{
    return m_currentPath + "/";//QDir::toNativeSeparators("/");

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (CloseAll()) {
        QSettings settings("LemonSpawn", "TRSE");
        settings.setValue("MainWindow/geometry", saveGeometry());
        settings.setValue("MainWindow/windowState", saveState());
        settings.setValue("MainWindow/psplitter", ui->psplitter->saveState());
        settings.setValue("MainWindow/splitter", ui->splitter->saveState());
        settings.setValue("MainWindow/qsplitter", ui->qsplitter->saveState());

        event->accept();
    }
    else
        event->ignore();
}

void MainWindow::CreateNewSourceFile(QString type)
{
    if (m_currentProject.m_filename=="") {
        Messages::messages.DisplayMessage(Messages::messages.NO_PROJECT);
        return;
    }
    QString Type = type;
    Type[0] = type[0].toUpper();
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    QString f = Type+" Files (*."+type+")";
    QString filename = dialog.getSaveFileName(nullptr, "Create New File",getProjectPath(),f);

    if (filename=="")
        return;
    if (!filename.toLower().endsWith("."+type+"")) {
        filename = filename + "."+type+"";
    }
    QString orgFile;
    //filename = filename.split("/").last();
    filename = filename.toLower().remove(getProjectPath().toLower());

    qDebug() << filename;
  //  qDebug() << getProjectPath().toLower();
    QString fn = getProjectPath() + filename;
    if (QFile::exists(fn))
        QFile::remove(fn);

    QFile file(fn);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        if (type=="ras") {
            QTextStream s(&file);
            s<< "program MyProgram;\n";
            s<< "var  \n";
            s<< "   i: byte; \n\n";
            s<< "begin\n\n";
            s<< "end.\n";
        }
        if (type=="tru") {
            QTextStream s(&file);
            s<< "Unit MyUnit;\n";
            s<< "var  \n";
            s<< "   i: byte; \n\n";
            s<< "procedure myProcedure();\n";
            s<< "begin\n";
            s<< "end;\n\n";
            s<< "end.\n";
        }
    }

    file.close();
//    LoadRasFile(filename);
    LoadDocument(filename);
    RefreshFileList();

}

void MainWindow::ShowContextMenu(const QPoint &pos)
{
    if (!ui->lstRecentProjects->rect().contains( ui->lstRecentProjects->mapFromGlobal(QCursor::pos())))
        return;

    QMenu contextMenu(tr("Context menu"), this);

    QAction action1("Remove from list", this);
    connect(&action1, SIGNAL(triggered()), this, SLOT(removeFromRecentList()));
    contextMenu.addAction(&action1);

    contextMenu.exec(mapToGlobal(pos));
}

void MainWindow::FindFileDialog()
{

    QStringList lst = QStringList() <<"*.asm" << "*.ras" << "*.tru"<< "*.fjo" << "*.flf" << "*.paw";
    QDirIterator it(getProjectPath(), lst, QDir::Files, QDirIterator::Subdirectories);
    QVector<QString> files;
    while (it.hasNext()) {
        QString s = it.next();
        s=s.remove(getProjectPath());
        files.append(s);
    }
    DialogFindFile* df = new DialogFindFile();
    df->Init(files);

    df->exec();
    if (df->m_selected!="")
        LoadDocument(df->m_selected);

    delete df;

    m_currentDoc->setFocus();
}


void MainWindow::onImageMouseMove()
{
    //m_updateThread->RunContents();
}

/*QString MainWindow::getProjectPath()
{
    return m_currentProject.m_ini->getString("project_path");
}
*/

void MainWindow::on_treeFiles_doubleClicked(const QModelIndex &index)
{


    QString path = FindPathInProjectFolders(index);

    // Finally load file!
    QString file = index.data().toString();
    if (file.toLower().endsWith(".tru") || file.toLower().endsWith(".ras") || file.toLower().endsWith(".asm")
            || file.toLower().endsWith(".inc") || file.toLower().endsWith(".flf")
            || file.toLower().endsWith(".paw") || file.toLower().endsWith(".fjo")) {
        LoadDocument(path + file);
    }
    if (file.toLower().endsWith(".prg")) {

/*        QString emu = m_iniFile->getString("emulator");
        if (m_currentProject.m_ini->getString("system")=="VIC20")
            emu = m_iniFile->getString("vic20_emulator");
        if (m_currentProject.m_ini->getString("system")=="C128")
            emu = m_iniFile->getString("c128_emulator");
        if (m_currentProject.m_ini->getString("system")=="NES")
            emu = m_iniFile->getString("nes_emulator");
*/
//        FormRasEditor::ExecutePrg(getProjectPath()+"/" + file, m_currentProject.m_ini->getString("system"));
    }

    Data::data.Redraw();
    Data::data.forceRedraw = true;
}


/*

*/

void MainWindow::on_tabMain_currentChanged(int index)
{
    FormImageEditor* imageedit = dynamic_cast<FormImageEditor*>(ui->tabMain->widget(index));
    FormRasEditor* rasedit = dynamic_cast<FormRasEditor*>(ui->tabMain->widget(index));


    for (TRSEDocument* doc : m_documents)
        doc->m_hasFocus = false;

    if (dynamic_cast<TRSEDocument*>(ui->tabMain->widget(index))!=nullptr) {
        m_currentDoc = dynamic_cast<TRSEDocument*>(ui->tabMain->widget(index));
        m_currentProject.m_ini->setString("current_file",m_currentDoc->m_currentFileShort);
        if (m_currentDoc!=nullptr && index!=0) {
            m_currentDoc->Reload();
            m_currentDoc->m_hasFocus = true;
        }

//        qDebug() << "dyn:" << dynamic_cast<TRSEDocument*>(ui->tabMain->widget(index));
    }

    else {
        m_currentDoc=nullptr;
    }

}

void MainWindow::on_btnSave_3_clicked()
{
    if (m_currentDoc==nullptr)
        return;
    m_currentDoc->SaveCurrent();
}

// New source file
void MainWindow::on_actionRas_source_file_triggered()
{
    CreateNewSourceFile("ras");
}

void MainWindow::on_actionDelete_file_triggered()
{
//    qDebug() << (ui->treeFiles->SelectedClicked);
    QModelIndex qlst = ui->treeFiles->currentIndex();
    if (qlst.data().toString()=="")
        return;
    QString path = getProjectPath() + FindPathInProjectFolders(qlst);
    QString filename = qlst.data().toString();

    QMessageBox msgBox;
    msgBox.setWindowTitle("Warning");
    msgBox.setInformativeText("Are you sure you wish to delete '"+filename+"'");
    msgBox.setText("Warning!                                         ");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if (ret==QMessageBox::Ok) {
//        qDebug() << "Removing : " << path+filename;
        QFile::remove(path + filename);
        RefreshFileList();
    }
}

QString MainWindow::FindPathInProjectFolders(const QModelIndex &index)
{
    // Find file in path.. ugh
    QString path = "";
    QStringList pathSplit = getProjectPath().toLower().replace("\\", "/").split("/");
    QString test = pathSplit.last();
    if (test=="")
        test = pathSplit[pathSplit.count()-2];

    QModelIndex cur = index.parent();
    int cnt=0;
    while (cur.data().toString().toLower()!=test) {

        path=cur.data().toString() + "/" + path;
        cur = cur.parent();
        if (cnt++>20)
            return "";
    }
    return path;
}

void MainWindow::BuildAll()
{


    DialogProjectBuilder* pb = new DialogProjectBuilder();

    pb->Initialize(m_iniFile, m_currentProject.m_ini,
                   m_currentPath,
                   m_currentProject.m_ini->getStringList("build_list"));

    pb->exec();

    delete pb;

}


void MainWindow::on_actionImage_triggered()
{

    if (m_currentProject.m_filename=="") {
        Messages::messages.DisplayMessage(Messages::messages.NO_PROJECT);
        return;
    }

    ImageWorker tmp;

    DialogNewImage* dNewFile = new DialogNewImage(this);
    dNewFile->Initialize(tmp.m_types);
    dNewFile->setModal(true);
    dNewFile->exec();
    if (dNewFile->retVal==-1) {
        delete dNewFile;
        return;
    }
    FormImageEditor* editor = new FormImageEditor(this);
    editor->m_work.New(dNewFile->m_metaImage,dNewFile->retVal);
    delete dNewFile;


    editor->UpdatePalette();
    editor->InitDocument(nullptr, m_iniFile, m_currentProject.m_ini);
    editor->m_currentSourceFile = "";
    editor->m_currentFileShort = "";
    ui->tabMain->addTab(editor, "New Image");
//    editor->PrepareImageTypeGUI();

    editor->setFocus();
    editor->showMaximized();
    ui->tabMain->setCurrentWidget(editor);

    //m_iniFile->setString("current_file", fileName);
    //m_buildSuccess = false;
    ui->tabMain->setTabsClosable(true);
    m_documents.append(editor);
    m_currentDoc = editor;
    editor->Initialize();
    editor->SetMCColors();
    ConnectDocument();

}

void MainWindow::on_actionSave_As_triggered()
{
    SaveAs();
}

void MainWindow::on_actionTRSE_Settings_triggered()
{
    DialogTRSESettings* dSettings = new DialogTRSESettings(this);


    dSettings->SetInit(m_iniFile);


    dSettings->exec();



    for (TRSEDocument* doc : m_documents) {
        doc->UpdateColors();
        doc->UpdateFromIni();
    }


    delete dSettings;

    if (m_iniFile->getdouble("windowpalette")==0)
        SetDarkPalette();
    if (m_iniFile->getdouble("windowpalette")==1)
        QApplication::setPalette(m_defaultPalette);



}

void MainWindow::on_actionNew_project_triggered()
{

    DialogNewProject *np = new DialogNewProject(m_iniFile);
    np->exec();
    if (!np->ok)
        return;


    QString src = Util::GetSystemPrefix() + "project_templates/"+np->m_template;
    QString dst = np->m_dir + "/" + np->m_project + "/";
    // Create new project:
    QDir().mkdir(dst);
//    qDebug() << "MainWindow " <<src;
  //  qDebug() << "MainWindow " <<dst;
    QString projectFile = dst + np->m_project+".trse";
    QFile::copy(src+"/project.trse",projectFile);
    Util::CopyRecursively(src + "/files/", dst);
    UpdateRecentProjects();
    LoadProject(projectFile);

/*    QFileDialog dialog;
    QString filename = dialog.getSaveFileName(this, "New project",getProjectPath(),"*.trse");
    if (filename=="")
        return;

    CloseAll();
    QStringList splt = filename.split("/");
    QString path="";
    for (int i=0;i<splt.count()-1;i++)
        path+=splt[i] + "/";

    m_currentProject = TRSEProject();
    m_currentPath = path;
    //m_currentProject.m_ini->setString("project_path", path);
    m_currentProject.m_filename = filename;
    m_currentProject.Save();
    RefreshFileList();

   // m_iniFile->setString("project_path", getProjectPath());
    m_iniFile->addStringList("recent_projects", filename, true);
    m_iniFile->Save();

    UpdateRecentProjects();
    LoadProject(filename);
*/
}

void MainWindow::on_actionClose_all_triggered()
{
    CloseAll();
}

void MainWindow::on_actionOpen_project_triggered()
{
    QFileDialog dialog;
    QString filename = dialog.getOpenFileName(this, "Open project",getProjectPath(),"*.trse");
    if (filename=="")
        return;

    LoadProject(filename);


}

void MainWindow::LoadProject(QString filename)
{
    CloseAll();
    m_currentProject.Load(filename);
    m_currentPath = QFileInfo(QFile(filename)).absolutePath();
    Data::data.currentPath = m_currentPath;
    VerifyProjectDefaults();
//    m_iniFile->setString("project_path", getProjectPath());
    m_iniFile->addStringList("recent_projects", filename, true);

    RefreshFileList();
/*
    qDebug() << m_currentProject.m_ini->contains("build_list");
    qDebug() << m_currentProject.m_ini->getStringList("build_list");

    for (int i=0;i<m_currentProject.m_ini->items.size();i++)
        qDebug() << m_currentProject.m_ini->items[i].name << m_currentProject.m_ini->items[i].lst;

*/
    m_iniFile->Save();

    // Set compiler syntax based on system
    QString system = m_currentProject.m_ini->getString("system");
    Syntax::s.m_systemString = system;
    Syntax::s.Init(AbstractSystem::SystemFromString(system),m_iniFile, m_currentProject.m_ini);
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::AMIGA)
        Messages::messages.DisplayMessage(Messages::messages.AMIGA_WARNING);


    QImage img(":resources/images/" +system+".png");
    QPainter p;
    p.begin(&img);

    QPointF pos(5,img.height()-10);
//    p.setFont(QFont("c64", 12, QFont::Bold));
    p.setFont(QFont(m_fontFamily, 8));
    p.setPen(QPen(QColor(0,0,0)));
    p.drawText(pos, system);
    p.setPen(QPen(QColor(255,200,70)));
    p.drawText(pos-QPointF(2,2), system);
    p.end();

    ui->lblCommodoreImage->setPixmap(QPixmap::fromImage(img));


    UpdateRecentProjects();


    QStringList files = m_currentProject.m_ini->getStringList("open_files");

    QString focusFile = m_currentProject.m_ini->getString("current_file");
    for (int i=0;i<files.count();i++) {
        QString f = files[files.count()-1-i];
        if (QFile::exists(getProjectPath() + "/"+ f))
            LoadDocument(f);
    }

//    qDebug() << f;
    if (QFile::exists(getProjectPath() + "/"+ focusFile))
        if (!(QDir(getProjectPath() + "/"+ focusFile).exists()))
            LoadDocument(focusFile);


}


void MainWindow::on_lstRecentProjects_itemDoubleClicked(QListWidgetItem *item)
{
    QString projectFile = item->data(Qt::UserRole).toString();
    LoadProject(projectFile);

}

void MainWindow::on_btnBuild_3_clicked()
{
    if (m_currentDoc!=nullptr)
        m_currentDoc->Build();


}


void MainWindow::on_actionBuild_current_File_only_Shift_C_b_triggered()
{
    if (m_currentDoc!=nullptr)
        m_currentDoc->Build();

}


void MainWindow::on_btnBuild_4_clicked()
{
    if (m_currentDoc!=nullptr) {
        FormRasEditor* fre = dynamic_cast<FormRasEditor*>(m_currentDoc);
        if (fre!=nullptr) {
            fre->m_run = true;
            fre->Build();
            return;
        }
        m_currentDoc->Run();
    }

}

void MainWindow::on_actionAbout_triggered()
{
    DialogAbout* da = new DialogAbout();
    da->exec();
    delete da;
}

void MainWindow::on_actionAuto_ident_triggered()
{
    if (m_currentDoc!=nullptr)
        m_currentDoc->AutoFormat();
}

void MainWindow::on_actionClose_current_project_triggered()
{
    CloseAll();
    m_currentProject.Close();
    m_currentPath = "";
    RefreshFileList();
}

void MainWindow::on_actionHelp_F1_triggered()
{
    if (m_currentPath=="") {
        QMessageBox msgBox;
        msgBox.setText("You need to load a project before using the help window, as topics are populated based on the current computer system.");
        msgBox.setWindowTitle("TRSE help window");
        msgBox.exec();
        return;
    }

    DialogHelp* dh = new DialogHelp(nullptr, "", m_defaultPalette);
    dh->show();
//    dh->exec();
}

void MainWindow::on_btnMemoryAnalyze_clicked()
{
    if (m_currentDoc!=nullptr)
        m_currentDoc->MemoryAnalyze();
}

void MainWindow::on_actionMovie_Creator_triggered()
{
    MovieConverter mc;
/*    mc.ConvertPacked("/home/leuat/Videos/Webcam/test/","in",
               "jpg",
               "/home/leuat/Dropbox/TRSE/HFDemo/misc/movie2.bin",10, 80,30,20,1.0f);
*/
    // Test, galaxy
    mc.ConvertPacked("/home/leuat/Videos/galaxy3/","in",
               "jpg",
               "/home/leuat/Dropbox/TRSE/HFDemo/misc/movie2.bin",3, 60,30,18,0.8f);

}

void MainWindow::on_actionCheck_for_new_version_triggered()
{

}

void MainWindow::on_actionPaw_packed_resource_file_triggered()
{
    if (m_currentProject.m_filename=="") {
        Messages::messages.DisplayMessage(Messages::messages.NO_PROJECT);
        return;
    }



    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    QString f = "Paw Files (*.paw)";
    QString filename = dialog.getSaveFileName(NULL, "Create New File",getProjectPath(),f);

    if (filename=="")
        return;
    if (!filename.toLower().endsWith(".paw"))
        return;
    QString orgFile;
    //filename = filename.split("/").last();
    filename = filename.toLower().remove(getProjectPath().toLower());

    //qDebug() << filename;
    QString fn = getProjectPath() + filename;
    if (QFile::exists(fn))
        QFile::remove(fn);
    QFile file(fn);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream s(&file);
        s<< "ras_include_file = packed_resources1.ras\n";
        s<< "packed_address = $4000  \n";

    }

    file.close();
//    LoadRasFile(filename);
    LoadDocument(filename);
    RefreshFileList();

}
void MainWindow::SetDarkPalette() {
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    QApplication::setPalette(darkPalette);
    qApp->setStyleSheet("QToolTip { color: #ffE0C0; background-color: #000000; border: 0px; }");


}

void MainWindow::on_actionProject_Settings_triggered()
{
    OpenProjectSettings();
}

void MainWindow::on_btnProjectSettings_clicked()
{
    OpenProjectSettings();
}

void MainWindow::on_actionDonate_triggered()
{
    DialogDonate* dn = new DialogDonate();
    dn->exec();
    delete dn;
}

void MainWindow::on_actionFind_file_c_s_triggered()
{
    FindFileDialog();
}

void MainWindow::on_actionEffects_triggered()
{
    DialogEffects* de = new DialogEffects("");
    de->exec();
    delete de;
}

void MainWindow::on_actionFjong_Raymarcher_document_triggered()
{
    if (m_currentProject.m_filename=="") {
        Messages::messages.DisplayMessage(Messages::messages.NO_PROJECT);
        return;
    }



    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    QString f = "Ras Files (*.fjo)";
    QString filename = dialog.getSaveFileName(nullptr, "Create New Fjong File",getProjectPath(),f);

    if (filename=="")
        return;
    if (!filename.toLower().endsWith(".fjo")) {
        filename = filename + ".fjo";
    }
    QString orgFile;
    //filename = filename.split("/").last();
    filename = filename.toLower().remove(getProjectPath().toLower());

//    qDebug() << filename;
  //  qDebug() << getProjectPath().toLower();
    QString fn = getProjectPath() + filename;
    if (QFile::exists(fn))
        QFile::remove(fn);


    QFile file(fn);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream s(&file);
        s << "";
    }

    file.close();
//    LoadRasFile(filename);
    LoadDocument(filename);
    RefreshFileList();
}

void MainWindow::on_actionWavefront_obj_to_amiga_converter_triggered()
{
    DialogExport3D* de = new DialogExport3D(m_iniFile);

    de->exec();
    delete de;
}

void MainWindow::on_btnBuildAll_clicked()
{
    if (m_currentProject.m_filename=="")
        return;
    BuildAll();
}

void TRSEProject::VerifyDefaults() {
    if (!m_ini->contains("zeropages"))
        m_ini->setStringList("zeropages", AsmMOS6502::m_defaultZeroPointers.split(","));


    if (!m_ini->contains("temp_zeropages"))
        m_ini->setStringList("temp_zeropages", AsmMOS6502::m_defaultTempZeroPointers.split(","));

    if (!m_ini->contains("zeropage_screenmemory"))
        m_ini->setString("zeropage_screenmemory","$fe");

    if (!m_ini->contains("zeropage_decrunch1"))
        m_ini->setString("zeropage_decrunch1","$47");

    if (!m_ini->contains("zeropage_decrunch2"))
        m_ini->setString("zeropage_decrunch2","$48");


    if (!m_ini->contains("zeropage_decrunch3"))
        m_ini->setString("zeropage_decrunch3","$4A");

    if (!m_ini->contains("zeropage_decrunch4"))
        m_ini->setString("zeropage_decrunch4","$4B");


    if (!m_ini->contains("pascal_settings_use_local_variables"))
        m_ini->setFloat("pascal_settings_use_local_variables", 0);

    if (!m_ini->contains("system"))
        m_ini->setString("system", "C64");

    if (!m_ini->contains("main_ras_file"))
        m_ini->setString("main_ras_file", "none");



    if (!m_ini->contains("zeropage_internal1"))
        m_ini->setString("zeropage_internal1","$4C");

    if (!m_ini->contains("zeropage_internal2"))
        m_ini->setString("zeropage_internal2","$4E");

    if (!m_ini->contains("zeropage_internal3"))
        m_ini->setString("zeropage_internal3","$50");

    if (!m_ini->contains("zeropage_internal4"))
        m_ini->setString("zeropage_internal4","$52");


    m_ini->setFloat("post_optimizer_passlda", 1);
    m_ini->setFloat("post_optimizer_passjmp", 1);
    m_ini->setFloat("post_optimizer_passldatax", 1);
    m_ini->setFloat("post_optimizer_passstalda", 1);
    m_ini->setFloat("post_optimizer_passldx", 1);
    m_ini->setFloat("post_optimizer_passcmp", 1);
    m_ini->setFloat("post_optimizer_passphapla", 1);

    if (!m_ini->contains("machine_state"))
        m_ini->setString("machine_state", "$35");

    if (m_ini->getString("system")=="C128") {

        if (!m_ini->contains("columns")) {
            m_ini->setString("columns","40");
        }

    }

    if (!m_ini->contains("exomize_toggle")) {
        m_ini->setFloat("exomize_toggle",0);
    }


    if (m_ini->getString("system")=="X86") {
        if (!m_ini->contains("dosbox_x86_system"))
            m_ini->setString("dosbox_x86_system","default");

        if (!m_ini->contains("cpu_x86_system"))
            m_ini->setString("cpu_x86_system","8086");

    }

    if (m_ini->getString("system")=="NES") {
        if (!m_ini->contains("nes_code_start"))
            m_ini->setString("nes_code_start","$C000");
    }


    if (m_ini->getString("system")=="VIC20") {
        if (!m_ini->contains("vic_memory_config"))
            m_ini->setString("vic_memory_config","none");

        if (!m_ini->contains("via_zeropages")) {
            m_ini->setStringList("via_zeropages", AsmMOS6502::m_defaultViaZeroPointers.split(","));
        }

    }
    if (!m_ini->contains("border_color"))
        m_ini->setFloat("border_color",0);
    if (!m_ini->contains("background_color"))
        m_ini->setFloat("background_color",0);


    if (!m_ini->contains("override_target_settings"))
        m_ini->setFloat("override_target_settings",0);

    if (!m_ini->contains("remove_unused_symbols"))
        m_ini->setFloat("remove_unused_symbols",1);

    if (!m_ini->contains("override_target_settings_org")) {
           if (m_ini->getString("system")=="PLUS4")
               m_ini->setString("override_target_settings_org","$1010");
           else
               m_ini->setString("override_target_settings_org","$810");
       }
    if (!m_ini->contains("override_target_settings_ignore_sys"))
        m_ini->setFloat("override_target_settings_ignore_sys",0);

    if (!m_ini->contains("override_target_settings_ignore_prg"))
        m_ini->setFloat("override_target_settings_ignore_prg",0);

    if (!m_ini->contains("output_debug_symbols"))
        m_ini->setFloat("output_debug_symbols",1);

    if (!m_ini->contains("ignore_initial_jump"))
        m_ini->setFloat("ignore_initial_jump",0);

}

void MainWindow::on_treeTutorials_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (item->data(0,Qt::UserRole).toString()=="")
        return;
    QString dir = Util::GetSystemPrefix() + "tutorials/"+item->data(0,Qt::UserRole).toString().split(";")[0];

    QString fileName = Util::findFileInDirectory("",dir,"trse");
    LoadProject(fileName);

}

void MainWindow::on_treeTutorials_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if (current->data(0,Qt::UserRole).toString()=="")
        return;
    QString text = current->data(0,Qt::UserRole).toString().split(";")[1];
    ui->txtTutorials->setText(text+"<p><font color=\"#A0FFA0\">Double click to load the project!</font>");

}

void MainWindow::on_action_Project_Settings_triggered()
{
    OpenProjectSettings();
}

void MainWindow::on_actionBuild_C_b_triggered()
{
    if (m_currentDoc!=nullptr)
        m_currentDoc->Build();

}

void MainWindow::on_actionBuild_All_triggered()
{
    BuildAll();
}

void MainWindow::on_action_Run_triggered()
{
    if (m_currentDoc!=nullptr) {
        FormRasEditor* fre = dynamic_cast<FormRasEditor*>(m_currentDoc);
        if (fre!=nullptr) {
            fre->m_run = true;
            fre->Build();
            return;
        }
        m_currentDoc->Run();
    }

}

void MainWindow::on_action_Memory_map_C_u_triggered()
{
    if (m_currentDoc!=nullptr)
        m_currentDoc->MemoryAnalyze();

}

void MainWindow::on_actionSave_triggered()
{
    if (m_currentDoc==nullptr)
        return;
    m_currentDoc->SaveCurrent();

}

void MainWindow::on_actionOpen_project_location_triggered()
{
    if (m_currentPath=="")
        return;
    QDesktopServices::openUrl( QUrl::fromLocalFile(m_currentPath) );
}

void MainWindow::on_btnProjectDir_clicked()
{
    on_actionOpen_project_location_triggered();
}

void MainWindow::on_btnNewProject_clicked()
{
    on_actionNew_project_triggered();
}

void MainWindow::on_treeSymbols_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString key = item->data(0,Qt::UserRole).toString();
    if (!m_symPointers.contains(key))
        return;
    QSharedPointer<SymbolPointer> sp = m_symPointers[key];
    ForceOpenFile(sp->m_file,sp->m_ln);
    m_currentDoc->Focus();

//    qDebug() << sp->m_file << " and " <<sp->m_ln;
}

void MainWindow::on_treeSymbols_itemClicked(QTreeWidgetItem *item, int column)
{
    on_treeSymbols_itemDoubleClicked(item,column);
}


void MainWindow::GotoSymbol(QString s)
{
//    qDebug() << s << m_symPointers.contains(s) << m_symPointers.keys();
    if (!m_orgSymPointers.contains(s))
        return;
    QSharedPointer<SymbolPointer> sp = m_orgSymPointers[s];
    ForceOpenFile(sp->m_file,sp->m_ln);
    m_currentDoc->Focus();
//    qDebug() <<  s  << m_treeItems.keys();
    ui->treeSymbols->setCurrentItem(m_treeItems[s]);
}



void MainWindow::on_leFilterSymbols_textChanged(const QString &arg1)
{
    UpdateSymbolTree(arg1);
}

void MainWindow::on_actionLook_up_symbol_F2_triggered()
{
    m_currentDoc->LookupSymbolUnderCursor();
}



void MainWindow::on_action_TRU_Unit_source_file_triggered()
{
    CreateNewSourceFile("tru");
}

void MainWindow::LoadIniFile()
{
#ifndef Q_OS_WIN
   QString pa = QDir::homePath() +QDir::separator() + m_iniFileHomeDir;
/*   QString oldFile = Util::path + m_iniFileName;
    if (!QDir().exists(pa))
        QDir().mkdir(pa);*/
    m_iniFileNameOld = pa +QDir::separator()+ m_iniFileNameOld;
/*
    // Move old file
    if (QFile::exists(oldFile)) {
        QFile::copy(oldFile, m_iniFileName);
        QFile::remove(oldFile);
    }*/

#endif

    m_iniFile = QSharedPointer<CIniFile>(new CIniFile);

    m_iniFile->isMainSettings = true;
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!QDir().exists(path)) {
        QDir().mkdir(path);
    }
    m_iniFileName = path +QDir::separator()+ m_iniFileName;

    if (QFile::exists(m_iniFileName)) {
       m_iniFile->Load(m_iniFileName);
//       qDebug() << "Loading NEW file type "<<m_iniFileName;

    }
    else {
        if (QFile::exists(m_iniFileNameOld)) {
            m_iniFile->Load(m_iniFileNameOld);
  //          qDebug() << "Loading OLD file type";
        }
    }


    VerifyDefaults();
    if (m_iniFile->getdouble("windowpalette")==0)
        SetDarkPalette();

    QVector3D sp = m_iniFile->getVec("splitpos");
    if (sp.length()!=0)
        ui->splitter->setSizes(QList<int>() << sp.x() << sp.y());


    Messages::messages.LoadFromCIni(m_iniFile);

}


void MainWindow::on_lstSystems_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    QString key = current->data(Qt::UserRole).toString();
    m_tutorials.PopulateProjectList(key,ui->lstSampleProjects);

}

void MainWindow::on_lstSampleProjects_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (current==nullptr)
        return;
    if (current->data(Qt::UserRole).toString()=="")
        return;
    QString text = current->data(Qt::UserRole).toString().split(";")[1];
    ui->txtTutorials->setText(text+"<p><font color=\"#A0FFA0\">Double click to load the project!</font>");

}

void MainWindow::on_lstSampleProjects_itemDoubleClicked(QListWidgetItem *item)
{
    if (item==nullptr) return;
    if (item->data(Qt::UserRole).toString()=="")
        return;
    QString dir = Util::GetSystemPrefix() + "tutorials/"+item->data(Qt::UserRole).toString().split(";")[0];

    QString fileName = Util::findFileInDirectory("",dir,"trse");
    LoadProject(fileName);

}

void MainWindow::removeFromRecentList()
{
    int idx = ui->lstRecentProjects->currentRow();
    QStringList l = m_iniFile->getStringList("recent_projects");
//    qDebug() << idx << l[idx] << ui->lstRecentProjects->currentItem()->text();
    l.removeAt(idx);
    m_iniFile->setStringList("recent_projects",l);
    UpdateRecentProjects();
}
