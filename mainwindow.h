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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "imageworker.h"
#include <QString>
#include <QPixmap>
#include <QFontDatabase>
#include <QMovie>
#include <QFileDialog>
#include "source/LeLib/data.h"
#include <QThread>
#include <thread>
#include <QFileSystemModel>
#include <QLayout>
#include <QPointF>
#include <QTimer>
#include <QStandardPaths>
#include <source/toolbox.h>
#include "source/workerthread.h"
#include "imageeditor.h"
#include "dialogimport.h"
#include "source/PmmEdit/highlighter.h"
#include "source/PmmEdit/codeeditor.h"
#include "source/LeLib/util/cinifile.h"
#include "source/trsedocuments/formraseditor.h"
#include "source/trsedocuments/formimageeditor.h"
#include "source/trsedocuments/formfjong.h"
#include "source/trsedocuments/formpaw.h"
#include "source/dialogtrsesettings.h"
#include "source/messages.h"
#include "source/LeLib/limage/movieconverter.h"
#include "ui_mainwindow.h"
#include "source/dialogprojectsettings.h"
#include "source/dialogdonate.h"
#include "source/dialogfindfile.h"
#include "source/dialogexport3d.h"
#include "source/dialogprojectbuilder.h"
#include "source/tutorials.h"
#include <QDesktopServices>
#include "source/dialognewproject.h"

namespace Ui {
class MainWindow;
}



class TRSEProject {
public:
    QSharedPointer<CIniFile> m_ini;
    QString m_filename="";
//    QStringList m_acceptedFileTypes = {"asm", "flf", "ras", "prg", "paw", "inc", "fjo"};
    void Load(QString projectfile) {
        m_ini = QSharedPointer<CIniFile>(new CIniFile());
        m_ini->Load(projectfile);
        m_filename = projectfile;
        QStringList l = m_filename.split("/");
        l.removeLast();
        QString path = "";
        for (QString s: l) path+=s+"/";
        m_ini->setString("project_path", path);
    }

    void VerifyDefaults();


    void Close() {
        m_filename = "";

    }


    void Save() {
        m_ini->Save(m_filename);
    }
};


class CustomFileSystemModel : public QFileSystemModel {
    Q_OBJECT

  public:
    CustomFileSystemModel(QWidget* parent) : QFileSystemModel(parent) {

    }


    QVariant data ( const QModelIndex & index, int role ) const override
    {
        if( index.column() == 0 && role == Qt::DecorationRole ) {
            //you may want to check "index.data().toString()" for the file-name/-extension
            QImage img;
            QString f = index.data().toString();
            if (f.contains(".ras"))
//                img.load(":resources/images/trselogo.png");
               img.load(":resources/images/ras.png");
            if (f.contains(".asm"))
                img.load(":resources/images/asm_icon.png");
            if (f.contains(".prg"))
                img.load(":resources/images/cmb_icon.png");
            if (f.contains(".flf"))
                img.load(":resources/images/image_icon.png");
            if (f.contains(".paw"))
                img.load(":resources/images/paw_icon.png");
            if (f.contains(".fjo"))
                img.load(":resources/images/torus.jpg");
            if (f.contains(".tru"))
                img.load(":resources/images/tru.png");

            QIcon ic(QPixmap::fromImage(img));


            return QVariant(ic);
        }
    else
        return QFileSystemModel::data(index, role);
    }


};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QMap<QString, QSharedPointer<SymbolPointer>> m_symPointers, m_orgSymPointers;
    QMap<QString, QTreeWidgetItem*> m_treeItems;

    QSharedPointer<CIniFile> m_iniFile;
    QMap<QString, QTreeWidgetItem*> m_symbolItems;
 //   CodeEditor m_codeEditor;
    CustomFileSystemModel *fileSystemModel = nullptr;
    QStringList m_commandParams;
    QString m_iniFileNameOld = "fluff64.ini";
    QString m_iniFileName = "trse.ini";
    QString m_iniFileHomeDir = ".TRSE";
    QString m_fontFamily;
    QPalette m_defaultPalette;
    QString m_currentPath;
    QVector<TRSEDocument*> m_documents;
    Tutorials m_tutorials;




    TRSEProject m_currentProject;


    TRSEDocument* m_currentDoc = nullptr;

    void LoadIniFile();

    void AfterStart(QString oldCurDir);

    void SearchInSource();
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent *e) override;

    void RestoreSettings();

    void ConnectDocument();

    void VerifyDefaults();
    void VerifyProjectDefaults();



    void SetDarkPalette();



//    WorkerThread* m_updateThread;

    void LoadDocument(QString fileName);

    bool m_quit = false;

    bool VerifyFile(QString file, QString message);

  //  void setupEditor();
    void SetupFileList();


    void RefreshFileList();
public slots:
    void AcceptUpdateSourceFiles(QSharedPointer<SourceBuilder> sourceBuilder);

    void UpdateSymbolTree(QString search="");

    void acceptBuildMain();

    void acceptSearchSymbols();

    void acceptRunMain();

    void acceptBuild() {
        //ui->lblBuild->setHidden(false);
        //QTimer::singleShot(500, ui->lblBuild, &QLabel::hide);

    }
    void OpenProjectSettings();

    void OnQuit();

    void ForceOpenFile(QString s, int ln);

/*    void updateImage() {
        FormImageEditor* imageEdit = dynamic_cast<FormImageEditor*>(ui->tabMain->currentWidget());
        if (imageEdit==nullptr)
            return;


//        imageEdit->UpdateImage();

    }*/
    void closeWindowSlot();
    void updatePalette() {
        //setPalette(m_updateThread->m_pal);
        ui->lblSave->setHidden(false);
        QTimer::singleShot(500, ui->lblSave, &QLabel::hide);
    }

    void UpdateRecentProjects();
    void SaveAs();

    bool RemoveTab(int, bool save=true);
    void LoadProject(QString filename);

    bool CloseAll();
    QString getProjectPath();

    void closeEvent(QCloseEvent *event) override;

    void CreateNewSourceFile(QString type);

signals:
   void ValueChanged();




private slots:

   void ShowContextMenu(const QPoint &pos);

    void FindFileDialog();
    void GotoSymbol(QString s);
    void onImageMouseMove();

    void on_treeFiles_doubleClicked(const QModelIndex &index);



    void on_tabMain_currentChanged(int index);

    void on_btnSave_3_clicked();




    void on_actionDelete_file_triggered();

    void on_actionImage_triggered();

    void on_actionSave_As_triggered();

    void on_actionTRSE_Settings_triggered();

    void on_actionNew_project_triggered();

    void on_actionClose_all_triggered();

    void on_actionOpen_project_triggered();

    void on_lstRecentProjects_itemDoubleClicked(QListWidgetItem *item);

    void on_btnBuild_3_clicked();

    void on_btnBuild_4_clicked();

    void on_actionAbout_triggered();

    void on_actionAuto_ident_triggered();

    void on_actionClose_current_project_triggered();

    void on_actionHelp_F1_triggered();

    void on_btnMemoryAnalyze_clicked();

    void on_actionMovie_Creator_triggered();

    void on_actionCheck_for_new_version_triggered();

    void on_actionPaw_packed_resource_file_triggered();

    void on_actionProject_Settings_triggered();

    void on_btnProjectSettings_clicked();

    void on_actionDonate_triggered();

    void on_actionFind_file_c_s_triggered();

    void on_actionEffects_triggered();

    void on_actionFjong_Raymarcher_document_triggered();

    void on_actionWavefront_obj_to_amiga_converter_triggered();

    void on_btnBuildAll_clicked();

    void on_treeTutorials_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_treeTutorials_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_action_Project_Settings_triggered();

    void on_actionBuild_C_b_triggered();

    void on_actionBuild_All_triggered();

    void on_action_Run_triggered();

    void on_action_Memory_map_C_u_triggered();

    void on_actionSave_triggered();

    void on_actionOpen_project_location_triggered();

    void on_btnProjectDir_clicked();

    void on_btnNewProject_clicked();

    void on_treeSymbols_itemDoubleClicked(QTreeWidgetItem *item, int column);


    void on_leFilterSymbols_textChanged(const QString &arg1);

    void on_actionLook_up_symbol_F2_triggered();

    void on_treeSymbols_itemClicked(QTreeWidgetItem *item, int column);

    void on_actionBuild_current_File_only_Shift_C_b_triggered();

    void on_action_TRU_Unit_source_file_triggered();

    void on_actionRas_source_file_triggered();

    void on_lstSystems_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_lstSampleProjects_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_lstSampleProjects_itemDoubleClicked(QListWidgetItem *item);

    void removeFromRecentList();

private:
    void cleanSymbol(QTreeWidgetItem* parent, QString on, QString name, int ln, QString fn,Parser* p, QColor bcol,QString search);

    QString FindPathInProjectFolders(const QModelIndex &index);

    void BuildAll();

    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
