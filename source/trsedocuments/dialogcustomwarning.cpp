#include "dialogcustomwarning.h"
#include "ui_dialogcustomwarning.h"

DialogCustomWarning::DialogCustomWarning(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCustomWarning)
{
    ui->setupUi(this);
}

void DialogCustomWarning::Init(QString title, QString message, QString iniField, CIniFile* ini)
{
    this->setWindowTitle(title);
    ui->txt->setText(message);
    m_field = iniField;
    m_ini = ini;
}

DialogCustomWarning::~DialogCustomWarning()
{
    delete ui;
}

void DialogCustomWarning::on_pushButton_2_clicked()
{
    close();
}

void DialogCustomWarning::on_pushButton_clicked()
{
    m_ini->setFloat(m_field,1);
    close();
}
