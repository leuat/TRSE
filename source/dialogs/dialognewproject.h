#ifndef DIALOGNEWPROJECT_H
#define DIALOGNEWPROJECT_H

#include <QDialog>
#include <QStringList>
#include "source/tutorials.h"
#include "source/LeLib/util/cinifile.h"
#include <QFileDialog>
namespace Ui {
class DialogNewProject;
}

class DialogNewProject : public QDialog
{
    Q_OBJECT
    Tutorials m_templates;
public:

    explicit DialogNewProject(QSharedPointer<CIniFile> ini, QWidget *parent = nullptr);
    ~DialogNewProject();
    bool ok=false;

    QString m_project, m_dir, m_template;
    QSharedPointer<CIniFile> m_ini;

private slots:

    void on_lstData_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_leProjectName_textChanged(const QString &arg1);

    void on_leProjectDir_textChanged(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_btnOk_clicked();

    void on_leProjectName_editingFinished();

    void on_btnSelectDir_clicked();

    void on_leProjectName_cursorPositionChanged(int arg1, int arg2);



    void on_cmbSystem_currentTextChanged(const QString &arg1);

private:
    Ui::DialogNewProject *ui;
    void UpdateProjectInfo();
};

#endif // DIALOGNEWPROJECT_H
