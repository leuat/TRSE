#include "dialognewproject.h"
#include "ui_dialognewproject.h"

DialogNewProject::DialogNewProject(CIniFile* ini, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewProject)
{
    ui->setupUi(this);
    m_ini = ini;

    m_templates.Read(":resources/text/project_templates.txt");
    m_templates.PopulateTemplateList(ui->lstData,"C64");

    ui->leProjectName->setText("myProject");
    if (ini->contains("default_project_dir"))
        ui->leProjectDir->setText(ini->getString("default_project_dir"));
    else
        ui->leProjectDir->setText(Util::path);

}

DialogNewProject::~DialogNewProject()
{
    delete ui;
}

void DialogNewProject::on_comboBox_currentIndexChanged(const QString &arg1)
{

    m_templates.PopulateTemplateList(ui->lstData,arg1);
}

void DialogNewProject::on_lstData_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (current==nullptr)
        return;
    int idx = current->data(Qt::UserRole).toInt();
    Tutorial t = m_templates.m_tutorials[idx];
    ui->txtInfo->setText(t.m_text);
    m_template = t.m_file;
}

void DialogNewProject::on_leProjectName_textChanged(const QString &arg1)
{
    UpdateProjectInfo();
}

void DialogNewProject::UpdateProjectInfo()
{
    m_project = ui->leProjectName->text();
    m_dir = ui->leProjectDir->text();
    if (!(m_dir.endsWith("/")))
        m_dir=m_dir+"/";

    QString p = m_dir+ m_project+"/"+m_project+".trse";
    ui->lblInfo->setText("Project will be created under : "+p);
}

void DialogNewProject::on_leProjectDir_textChanged(const QString &arg1)
{
    UpdateProjectInfo();
}

void DialogNewProject::on_pushButton_2_clicked()
{
    close();
}

void DialogNewProject::on_pushButton_clicked()
{
    ok = true;
    m_ini->setString("default_project_dir",ui->leProjectDir->text());
    close();
}
