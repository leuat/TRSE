#include "dialognewtrt.h"
#include "ui_dialognewtrt.h"

DialogNewTRT::DialogNewTRT(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewTRT)
{
    ui->setupUi(this);
}

DialogNewTRT::~DialogNewTRT()
{
    delete ui;
}

void DialogNewTRT::on_pushButton_2_clicked()
{
    cancel = true;
    close();
}

void DialogNewTRT::on_pushButton_clicked()
{
    close();
}

int DialogNewTRT::getRows()
{
    return ui->cmbPatternLength->currentText().toInt();
}

int DialogNewTRT::getChannels()
{
    return ui->cmbChannels->currentText().toInt();

}
