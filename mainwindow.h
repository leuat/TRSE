#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imageworker.h"
#include <QString>
#include <QPixmap>
#include <QFileDialog>
#include "source/LeLib/data.h"
#include <QThread>
#include <thread>
#include <QFileSystemModel>
#include <QLayout>
#include <QPointF>
#include <source/toolbox.h>
#include "source/workerthread.h"
#include "imageeditor.h"
#include "dialogimport.h"
#include "source/PmmEdit/highlighter.h"
#include "source/PmmEdit/codeeditor.h"
#include "source/LeLib/util/cinifile.h"
#include "source/trsedocuments/formraseditor.h"
#include "source/trsedocuments/formimageeditor.h"
#include "source/trsedocuments/formpaw.h"
#include "source/dialogtrsesettings.h"
#include "source/messages.h"
#include "source/LeLib/limage/movieconverter.h"
#include "ui_mainwindow.h"
#include "source/dialogprojectsettings.h"


namespace Ui {
class MainWindow;

}


class TRSEProject {
public:
    CIniFile m_ini;
    QString m_filename="";
    QStringList m_acceptedFileTypes = {"asm", "flf", "ras", "prg", "paw", "inc"};
    void Load(QString projectfile) {
        m_ini = CIniFile();
        m_ini.Load(projectfile);
        m_filename = projectfile;
        QStringList l = m_filename.split("/");
        l.removeLast();
        QString path = "";
        for (QString s: l) path+=s+"/";
        m_ini.setString("project_path", path);
    }

    void VerifyDefaults() {
        if (!m_ini.contains("zeropages"))
           m_ini.setStringList("zeropages", AsmMOS6502::m_defaultZeroPointers.split(","));


        if (!m_ini.contains("zeropage_screenmemory"))
            m_ini.setString("zeropage_screenmemory","$fe");

        if (!m_ini.contains("zeropage_decrunch1"))
            m_ini.setString("zeropage_decrunch1","$47");

        if (!m_ini.contains("zeropage_decrunch2"))
            m_ini.setString("zeropage_decrunch2","$48");

        if (!m_ini.contains("zeropage_decrunch3"))
            m_ini.setString("zeropage_decrunch3","$4A");

        if (!m_ini.contains("zeropage_decrunch4"))
            m_ini.setString("zeropage_decrunch4","$4B");





        if (!m_ini.contains("zeropage_internal1"))
            m_ini.setString("zeropage_internal1","$4C");

        if (!m_ini.contains("zeropage_internal2"))
            m_ini.setString("zeropage_internal2","$4D");

        if (!m_ini.contains("zeropage_internal3"))
            m_ini.setString("zeropage_internal3","$4E");

        if (!m_ini.contains("zeropage_internal4"))
            m_ini.setString("zeropage_internal4","$4F");

    }


    void Close() {
        m_ini = CIniFile();
        m_filename = "";

    }


    void Save() {
        m_ini.Save(m_filename);
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
                img.load(":resources/images/trselogo.png");
            if (f.contains(".asm"))
                img.load(":resources/images/asm_icon.png");
            if (f.contains(".prg"))
                img.load(":resources/images/cmb_icon.png");
            if (f.contains(".flf"))
                img.load(":resources/images/image_icon.png");
            if (f.contains(".paw"))
                img.load(":resources/images/paw_icon.png");

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


    CIniFile m_iniFile;
 //   CodeEditor m_codeEditor;
    CustomFileSystemModel *fileSystemModel;
    QString m_iniFileName = "fluff64.ini";
    QPalette m_defaultPalette;
    QVector<TRSEDocument*> m_documents;


    TRSEProject m_currentProject;


    TRSEDocument* m_currentDoc = nullptr;

    void SearchInSource();
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent *e);

    void VerifyDefaults();
    void VerifyProjectDefaults();

    void SetDarkPalette();



    WorkerThread* m_updateThread;

    void LoadDocument(QString fileName);

    bool m_quit = false;



  //  void setupEditor();
    void SetupFileList();

    void RefreshFileList();
public slots:

    void OpenProjectSettings();

    void OnQuit();

    void ForceOpenFile(QString s, int ln);

    void updateImage() {
        FormImageEditor* imageEdit = dynamic_cast<FormImageEditor*>(ui->tabMain->currentWidget());
        if (imageEdit==nullptr)
            return;


        imageEdit->UpdateImage();

    }
    void closeWindowSlot();
    void updatePalette() {
        setPalette(m_updateThread->m_pal);
    }

    void UpdateRecentProjects();
    void SaveAs();

    void RemoveTab(int, bool save=true);
    void LoadProject(QString filename);

    void CloseAll();
    QString getProjectPath();

signals:
   void ValueChanged();


private slots:


    void on_treeFiles_doubleClicked(const QModelIndex &index);



    void on_tabMain_currentChanged(int index);

    void on_btnSave_3_clicked();

    void on_actionRas_source_file_triggered();

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

private:

    QString FindPathInProjectFolders(const QModelIndex &index);

    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
