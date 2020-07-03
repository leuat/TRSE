#include "dialogsimplelineedit.h"
#include "ui_dialogsimplelineedit.h"

DialogSimpleLineEdit::DialogSimpleLineEdit(QString wt, QString info, QString val,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSimpleLineEdit)
{
    ui->setupUi(this);
    ui->lblInfo->setText(info);
    ui->leValue->setText(val);
    setWindowTitle(wt);
}

DialogSimpleLineEdit::~DialogSimpleLineEdit()
{
    delete ui;
}

QString DialogSimpleLineEdit::getValue()
{
    return ui->leValue->text();
}

void DialogSimpleLineEdit::on_pushButton_clicked()
{
    close();
}

void DialogSimpleLineEdit::on_pushButton_2_clicked()
{
    m_ok = false;
    close();
}
