#include "dialogabout.h"
#include "ui_dialogabout.h"
#include <QFile>

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);


    QFile f(":resources/text/about.txt");
  //  if (QFile::exists(":resources/text/about.txt"))
      //  exit(1);
    f.open(QIODevice::ReadOnly | QFile::Text) ;
    ui->txtAbout->setText(f.readAll());
    f.close();


}

DialogAbout::~DialogAbout()
{
    delete ui;
}

void DialogAbout::on_pushButton_clicked()
{
    close();
}
