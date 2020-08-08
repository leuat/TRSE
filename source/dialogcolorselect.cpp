#include "dialogcolorselect.h"
#include "ui_dialogcolorselect.h"

DialogColorSelect::DialogColorSelect(QWidget *parent):
    QDialog(parent),
      ui(new Ui::DialogColorSelect)

{
    ui->setupUi(this);

}

DialogColorSelect::~DialogColorSelect()
{
    delete ui;
}

void DialogColorSelect::Init(QColor color, QVector3D bpp)
{
    m_color = color;
    m_bpp = bpp;

}

void DialogColorSelect::on_pushButton_clicked()
{
    close();
}

void DialogColorSelect::on_pushButton_2_clicked()
{
    m_ok = false;
    close();
}
