#include "dialoginfo.h"
#include "ui_dialoginfo.h"

DialogInfo::DialogInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInfo)
{
    ui->setupUi(this);
}

DialogInfo::~DialogInfo()
{
    delete ui;
}

void DialogInfo::SetText(QString title, QString text)
{
    setWindowTitle(title);
    ui->textBrowser->setText(text);
}

void DialogInfo::on_pushButton_clicked()
{
    close();
}
