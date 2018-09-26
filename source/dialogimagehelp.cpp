#include "dialogimagehelp.h"
#include "ui_dialogimagehelp.h"

DialogImageHelp::DialogImageHelp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogImageHelp)
{
    ui->setupUi(this);
}

DialogImageHelp::~DialogImageHelp()
{
    delete ui;
}

void DialogImageHelp::on_pushButton_clicked()
{
    close();
}
