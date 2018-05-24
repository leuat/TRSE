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
#include "source/Compiler/assembler/mos6502.h"

#include "source/Compiler/errorhandler.h"
#include "source/Compiler/parser.h"
#include "source/Compiler/interpreter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   // m_work.m_colorList.CreateUI(ui->layoutColors,0);



    //QObject::connect(m_updateThread, SIGNAL(valueChanged()), this, SLOT (Update()));
    m_updateThread = new WorkerThread();
    connect(m_updateThread, SIGNAL(updateImageSignal()), this, SLOT(updateImage()));
    connect(m_updateThread, SIGNAL(updatePaletteSignal()), this, SLOT(updatePalette()));
    connect(m_updateThread, SIGNAL(requestSaveAs()), this, SLOT(SaveAs()));
    connect(m_updateThread, SIGNAL(requestCloseWindowSignal()), this, SLOT(closeWindowSlot()));


    connect( ui->tabMain, SIGNAL(tabCloseRequested(int)),this, SLOT(RemoveTab(int)));
    connect(qApp, SIGNAL(aboutToQuit()), m_updateThread, SLOT(OnQuit()));
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(OnQuit()));


    m_updateThread->m_orgPal = palette();
    m_updateThread->start();

  //  ui->centralWidget->setLayout(new QGridLayout());

    if (QFile::exists(m_iniFileName))
       m_iniFile.Load(m_iniFileName);

    VerifyDefaults();
    QPoint p = ui->splitter->pos();
//    p.setX();
    QVector3D sp = m_iniFile.getVec("splitpos");
    if (sp.length()!=0)
        ui->splitter->setSizes(QList<int>() << sp.x() << sp.y());


    Messages::messages.LoadFromCIni(&m_iniFile);
    UpdateRecentProjects();

//    m_iniFile.setString("project_path", getProjectPath().replace("\\","/"));
    //setupEditor();
    SetupFileList();
//    if (m_iniFile.getString("current_file")!="")
  //      LoadRasFile(m_iniFile.getString("current_file"));


    QImage img;
    img.load(":resources/images/trselogo.png");
    ui->lblLogo->setPixmap(QPixmap::fromImage(img));


    ui->splitter->setStretchFactor(0,10);
    ui->splitter->setStretchFactor(1,100);


    Messages::messages.DisplayMessage(Messages::messages.ALPHA_WARNING);

#ifndef USE_LIBTIFF
    //ui->btnTiff->setVisible(false);
#endif
//    m_updateThread->join();
//    m_updateThread->
}



MainWindow::~MainWindow()
{
    m_quit = true;
    delete ui;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    FormImageEditor* fe = dynamic_cast<FormImageEditor*>(m_currentDoc);
    if (fe==nullptr)
        return;
//    m_updateThread->pos = mapToGlobal( fe->getLabelImage()->rect().topLeft());
  //  qDebug() << m_updateThread->pos;
}


void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::RightButton)
        m_updateThread->m_currentButton = 2;
    if(e->buttons() == Qt::LeftButton) {
        m_updateThread->m_currentButton = 1;
    }


}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if (m_updateThread->m_currentButton==2)
        m_updateThread->m_currentButton = 0;
    else
        m_updateThread->m_currentButton = -1;

}

void MainWindow::wheelEvent(QWheelEvent *event)
{


}

void MainWindow::keyPressEvent(QKeyEvent *e)
{

    Data::data.forceRedraw = true;
    Data::data.Redraw();

}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{

}

void MainWindow::VerifyDefaults()
{
    if (!m_iniFile.contains("font_size"))
        m_iniFile.setFloat("font_size", 12);
    if (!m_iniFile.contains("tab_width"))
        m_iniFile.setFloat("tab_width", 4);
    if (!m_iniFile.contains("theme"))
        m_iniFile.setString("theme", "dark_standard.ini");
    if (!m_iniFile.contains("zeropages"))
       m_iniFile.setStringList("zeropages", AsmMOS6502::m_defaultZeroPointers.split(","));

    m_iniFile.filename = m_iniFileName;

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
        m_updateThread->SetCurrentImage(&fe->m_work, &fe->m_toolBox, fe->getLabelImage());

    }
    if (fileName.contains(".ras") || fileName.contains(".asm") || fileName.contains(".inc")  ) {
        editor = new FormRasEditor(this);
    }
    editor->InitDocument(m_updateThread, &m_iniFile, &m_currentProject.m_ini);
    editor->m_currentSourceFile = getProjectPath() + "/" + fileName;
    editor->m_currentFileShort = fileName;
    ui->tabMain->addTab(editor, fileName);
    editor->Load(editor->m_currentSourceFile);


    m_currentProject.m_ini.addStringList("open_files", editor->m_currentFileShort, true);
    m_currentProject.Save();



    editor->setFocus();
    editor->showMaximized();
    ui->tabMain->setCurrentWidget(editor);

    //m_iniFile.setString("current_file", fileName);
    //m_buildSuccess = false;
    ui->tabMain->setTabsClosable(true);
    m_documents.append(editor);
    m_currentDoc = editor;


    connect(m_currentDoc, SIGNAL(OpenOtherFile(QString, int )), this, SLOT(ForceOpenFile(QString , int)));


}


void MainWindow::SetupFileList()
{
    RefreshFileList();
}





void MainWindow::RefreshFileList()
{
    fileSystemModel = new CustomFileSystemModel(this);
    QString rootPath= getProjectPath();
    if (rootPath=="") {
        ui->treeFiles->setModel(nullptr);
            return;

    }
    fileSystemModel->setReadOnly(true);
    fileSystemModel->setRootPath(rootPath);
    fileSystemModel->setFilter(QDir::NoDotAndDotDot |
                            QDir::AllDirs |QDir::AllEntries);
    fileSystemModel->setNameFilters(QStringList() << "*.ras" << "*.asm" << "*.txt" << "*.prg" << "*.inc" << "*.flf");
    fileSystemModel->setNameFilterDisables(false);

    ui->treeFiles->setModel(fileSystemModel);
    ui->treeFiles->setRootIndex(fileSystemModel->index(rootPath));

    ui->treeFiles->hideColumn(1);
    ui->treeFiles->hideColumn(2);
    ui->treeFiles->hideColumn(3);

}




void MainWindow::OnQuit()
{
    m_iniFile.setVec("splitpos", QVector3D(ui->splitter->sizes()[0],ui->splitter->sizes()[1],0));
    m_iniFile.Save();
}

void MainWindow::ForceOpenFile(QString s, int ln)
{
    s.remove(getProjectPath());
    if (s.startsWith("/"))
        s = s.remove(0,1);

    if (s=="")
        return;
    QString txt = m_currentDoc->m_outputText;

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
}

void MainWindow::UpdateRecentProjects()
{
    ui->lstRecentProjects->clear();
    QStringList l = m_iniFile.getStringList("recent_projects");
    qDebug() << l;

    for (QString s: l) {
        QListWidgetItem* item= new QListWidgetItem();
       // item->data(Qt::UserRole).QVariant
        item->setData(Qt::UserRole,s);
        QString name = s.split("/").last();
        item->setText(name);

        ui->lstRecentProjects->addItem(item);
    }

}

void MainWindow::SaveAs()
{
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


}

void MainWindow::RemoveTab(int idx, bool save)
{
    if (idx==0)
        return;
    idx--;
    TRSEDocument* doc = m_documents[idx];
    if (save) {
        m_currentProject.m_ini.removeFromList("open_files", doc->m_currentFileShort);
        m_currentProject.Save();
    }

    m_documents[idx]->Destroy();
    m_documents.remove(idx);
   ui->tabMain->removeTab(idx+1);


   m_updateThread->SetCurrentImage(nullptr, nullptr, nullptr);
    TRSEDocument* d = (TRSEDocument*)ui->tabMain->currentWidget();
    FormImageEditor* fe = dynamic_cast<FormImageEditor*>(d);

    if (fe!=nullptr)
       m_updateThread->SetCurrentImage(&fe->m_work, &fe->m_toolBox,fe->getLabelImage());

    delete doc;
}

void MainWindow::CloseAll()
{
    qDebug() << "Close all";
    while (ui->tabMain->count()!=1) {
        RemoveTab(1, false);
    }
}

QString MainWindow::getProjectPath()
{
    return m_currentProject.m_ini.getString("project_path");
}


void MainWindow::on_treeFiles_doubleClicked(const QModelIndex &index)
{


    QString path = FindPathInProjectFolders(index);

    // Finally load file!
    QString file = index.data().toString();
    if (file.toLower().endsWith(".ras") || file.toLower().endsWith(".asm")
            || file.toLower().endsWith(".inc")) {
        LoadDocument(path + file);
    }
    if (file.toLower().endsWith(".flf")) {
        LoadDocument(path +file);
    }
    if (file.toLower().endsWith(".prg")) {
        FormRasEditor::ExecutePrg(getProjectPath()+"/" + file, m_iniFile.getString("emulator"));
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
    if (rasedit!=nullptr) {
        m_updateThread->SetCurrentImage(nullptr, nullptr, nullptr);
    }
    if (imageedit!=nullptr) {
        m_updateThread->SetCurrentImage(&imageedit->m_work, &imageedit->m_toolBox, imageedit->getLabelImage());
    }
    m_currentDoc = (TRSEDocument*)ui->tabMain->widget(index);

    m_currentDoc->Reload();

}

void MainWindow::on_btnSave_3_clicked()
{
    if (m_currentDoc==nullptr)
        return;
    m_currentDoc->SaveCurrent();
}

void MainWindow::on_actionRas_source_file_triggered()
{
    if (m_currentProject.m_filename=="") {
        Messages::messages.DisplayMessage(Messages::messages.NO_PROJECT);
        return;
    }



    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    QString f = "Ras Files (*.ras)";
    QString filename = dialog.getSaveFileName(NULL, "Create New File",getProjectPath(),f);

    if (filename=="")
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
        s<< "program spankme;\n";
        s<< "var  \n";
        s<< "   index: byte; \n";
        s<< "begin\n\n";
        s<< "end.\n";
        qDebug() << "Done writing;";
    }

    file.close();
//    LoadRasFile(filename);
    LoadDocument(filename);
    RefreshFileList();
}

void MainWindow::on_actionDelete_file_triggered()
{
    qDebug() << (ui->treeFiles->SelectedClicked);
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


void MainWindow::on_actionImage_triggered()
{

    if (m_currentProject.m_filename=="") {
        Messages::messages.DisplayMessage(Messages::messages.NO_PROJECT);
        return;
    }


    FormImageEditor* editor = new FormImageEditor(this);
    DialogNewImage* dNewFile = new DialogNewImage(this);
    dNewFile->Initialize(editor->m_work.getImageTypes());
    dNewFile->setModal(true);
    dNewFile->exec();
    if (dNewFile->retVal!=-1) {
        editor->m_work.New(dNewFile->retVal, dNewFile->m_meta);
    } else {
        delete editor;
        delete dNewFile;
        return;
    }
    delete dNewFile;

    editor->UpdatePalette();
    editor->InitDocument(m_updateThread, &m_iniFile, &m_currentProject.m_ini);
    editor->m_currentSourceFile = "";
    editor->m_currentFileShort = "";
    ui->tabMain->addTab(editor, "New Image");

    editor->setFocus();
    editor->showMaximized();
    ui->tabMain->setCurrentWidget(editor);

    //m_iniFile.setString("current_file", fileName);
    //m_buildSuccess = false;
    ui->tabMain->setTabsClosable(true);
    m_documents.append(editor);
    m_currentDoc = editor;


}

void MainWindow::on_actionSave_As_triggered()
{
    SaveAs();
}

void MainWindow::on_actionTRSE_Settings_triggered()
{
    DialogTRSESettings* dSettings = new DialogTRSESettings(this);


    dSettings->SetInit(&m_iniFile);


    dSettings->exec();

    for (TRSEDocument* doc : m_documents) {
        doc->UpdateColors();
        doc->UpdateFromIni();
    }


    delete dSettings;

}

void MainWindow::on_actionNew_project_triggered()
{
    QFileDialog dialog;
    QString filename = dialog.getSaveFileName(this, "New project",getProjectPath(),"*.trse");
    if (filename=="")
        return;

    CloseAll();
    QStringList splt = filename.split("/");
    QString path="";
    for (int i=0;i<splt.count()-1;i++)
        path+=splt[i] + "/";

    m_currentProject = TRSEProject();
    m_currentProject.m_ini.setString("project_path", path);
    m_currentProject.m_filename = filename;
    m_currentProject.Save();
    RefreshFileList();

   // m_iniFile.setString("project_path", getProjectPath());
    m_iniFile.addStringList("recent_projects", filename, true);
    m_iniFile.Save();

    UpdateRecentProjects();

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
//    m_iniFile.setString("project_path", getProjectPath());
    m_iniFile.addStringList("recent_projects", filename, true);
    RefreshFileList();
    m_iniFile.Save();

    UpdateRecentProjects();

    QStringList files = m_currentProject.m_ini.getStringList("open_files");
    for (QString f: files) {
        LoadDocument(f);
    }


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

void MainWindow::on_btnBuild_4_clicked()
{
    if (m_currentDoc!=nullptr)
        m_currentDoc->Run();

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
    RefreshFileList();
}

void MainWindow::on_actionHelp_F1_triggered()
{
    DialogHelp* dh = new DialogHelp(this, "");
    dh->show();
}

void MainWindow::on_btnMemoryAnalyze_clicked()
{
    if (m_currentDoc!=nullptr)
        m_currentDoc->MemoryAnalyze();
}
