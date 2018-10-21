#include "dialogfindfile.h"
#include "ui_dialogfindfile.h"

DialogFindFile::DialogFindFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFindFile)
{
    ui->setupUi(this);
}

DialogFindFile::~DialogFindFile()
{
    delete ui;
}
