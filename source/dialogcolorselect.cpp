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

void DialogColorSelect::UpdateGUI(QWidget* except)
{
    ui->slRed->setTickInterval(256/pow(2.0,m_bpp.x()));
    ui->slGreen->setTickInterval(256/pow(2.0,m_bpp.y()));

    ui->slBlue->setTickInterval(256/pow(2.0,m_bpp.z()));
//    qDebug() <<"COLOR " <<m_color.color.red();
    if (ui->slRed!=except)
        ui->slRed->setValue(m_color.color.red());
    if (ui->slGreen!=except)
        ui->slGreen->setValue(m_color.color.green());
    if (ui->slBlue!=except)
        ui->slBlue->setValue(m_color.color.blue());

    if (ui->leValue!=except)
    ui->leValue->setText(m_color.toRGB8());




    QPushButton* b = ui->btnColor;
    QPalette p;
    b->setFlat(true);
    QPixmap pm = Util::CreateColorIcon(m_color.color,100);
    b->setAutoFillBackground(true);
    p.setBrush(b->backgroundRole(), QBrush(pm));
    b->setPalette(p);


}

void DialogColorSelect::Init(LColor color, QVector3D bpp)
{
    m_color = color;
    m_bpp = bpp;
    UpdateGUI(nullptr);

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

void DialogColorSelect::on_slRed_sliderMoved(int position)
{
    m_color.color.setRed(ui->slRed->value());
    UpdateGUI(ui->slRed);
}

void DialogColorSelect::on_slGreen_sliderMoved(int position)
{
    m_color.color.setGreen(ui->slGreen->value());
    UpdateGUI(ui->slGreen);
}



void DialogColorSelect::on_slBlue_sliderMoved(int position)
{
    m_color.color.setBlue(ui->slBlue->value());
    UpdateGUI(ui->slBlue);

}
