#include "dialogmessage.h"
#include "ui_dialogmessage.h"

DialogMessage::DialogMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMessage)
{
    ui->setupUi(this);
}

DialogMessage::~DialogMessage()
{
    delete ui;
}

void DialogMessage::SetMessage(QString msg, QImage img)
{
    ui->lblMessage->setText(msg);
    ui->lbImage->setPixmap(QPixmap::fromImage(img));
}

bool DialogMessage::ignoreIsChecked()
{
    return ui->chkStop->checkState()==Qt::Checked;
}

void DialogMessage::on_pushButton_clicked()
{
    close();
}
