#include "dialogprojectbuilder.h"
#include "ui_dialogprojectbuilder.h"


DialogProjectBuilder::DialogProjectBuilder(QWidget *parent):
QDialog(parent),
ui(new Ui::DialogProjectBuilder)

{
    ui->setupUi(this);

}

void DialogProjectBuilder::Initialize(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> iniP, QString curDir, QStringList list)
{
    m_pb = new ProjectBuilder(ini,iniP,curDir);
    m_pb->LoadBuildList(list);
    connect(m_pb, SIGNAL(emitTextUpdate()), this, SLOT(UpdateText()));

    m_pb->start();
}

DialogProjectBuilder::~DialogProjectBuilder()
{
    delete ui;
    if (m_pb!=nullptr)
        delete m_pb;
}

void DialogProjectBuilder::on_pushButton_clicked()
{
    close();

}

void DialogProjectBuilder::UpdateText()
{
    ui->txtOutput->setText(m_pb->m_output);
    ui->txtOutput->verticalScrollBar()->setValue(ui->txtOutput->verticalScrollBar()->maximum());
}
