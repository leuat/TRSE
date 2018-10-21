#include "dialogdonate.h"
#include "ui_dialogdonate.h"

DialogDonate::DialogDonate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDonate)
{
    ui->setupUi(this);
}

DialogDonate::~DialogDonate()
{
    delete ui;
}

void DialogDonate::on_pushButton_clicked()
{
    close();
}
