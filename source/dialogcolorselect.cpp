
#include "dialogcolorselect.h"
#include "ui_dialogcolorselect.h"

DialogColorSelect::DialogColorSelect(QWidget *parent):
    QDialog(parent),
      ui(new Ui::DialogColorSelect)

{
    ui->setupUi(this);
 //   connect( ui->slRed, SIGNAL((slider->valueChanged())), this, SLOT(UpdateGUI(ui->slRed)) );
//    connect( ui->slGreen, SIGNAL((slider->valueChanged())), this, SLOT(UpdateGUI(ui->slGreen)) );
//    connect( ui->slBlue, SIGNAL((slider->valueChanged())), this, SLOT(UpdateGUI(ui->slBlue)) );
  //  ui->slRed->setTracking(true);
  //  m_color.color = Qt::blue;
 //   UpdateGUI(nullptr);
}


DialogColorSelect::~DialogColorSelect()
{
    delete ui;
}

void DialogColorSelect::UpdateGUI(QWidget* except)
{
/*    ui->slRed->setTickInterval(ival);
    ui->slGreen->setTickInterval(256/pow(2.0,m_bpp.y()));
    ui->slBlue->setTickInterval(256/pow(2.0,m_bpp.z()));
    ui->slRed->setSingleStep(ival);
    ui->slGreen->setSingleStep(256/pow(2.0,m_bpp.y()));
    ui->slBlue->setSingleStep(256/pow(2.0,m_bpp.z()));

*/
    if (ui->slRed!=except) {
        ui->slRed->setValue(m_color.color.red());
      //  ui->slRed->setValue(100);
  //      qDebug() << "HERE";

    }
    if (ui->slGreen!=except)
        ui->slGreen->setValue(m_color.color.green());
    if (ui->slBlue!=except)
        ui->slBlue->setValue(m_color.color.blue());

    if (ui->leValue!=except)
        ui->leValue->setText(m_color.toRGB8());


   QPushButton* b = ui->btnColor;
    QPalette p;
    b->setFlat(true);
    QPixmap pm = Util::CreateColorIcon(m_color.color,10);
    b->setAutoFillBackground(true);
    p.setBrush(b->backgroundRole(), QBrush(pm));
    b->setPalette(p);


}

void DialogColorSelect::Init(LColor color, QVector3D bpp)
{
    m_color = color;
    m_bpp = bpp;
    m_interval = 256/(int)(pow(2.0,m_bpp.x()));

    UpdateGUI(nullptr);
}

int DialogColorSelect::forceInterval(int val)
{
    return min(((int)((val+m_interval/2)/m_interval))*m_interval,255);
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
   // qDebug() << "R MOVED prev valye "<<QString::number(ui->slRed->value());

    m_color.color.setRed(forceInterval(ui->slRed->value()));
    UpdateGUI(ui->slRed);

}

void DialogColorSelect::on_slGreen_sliderMoved(int position)
{
    m_color.color.setGreen(forceInterval(ui->slGreen->value()));
    UpdateGUI(ui->slGreen);
}



void DialogColorSelect::on_slBlue_sliderMoved(int position)
{
    m_color.color.setBlue(forceInterval(ui->slBlue->value()));
//    ui->slBlue->setValue(m_color.color.blue());
    UpdateGUI(ui->slBlue);

}


void DialogColorSelect::on_leValue_textChanged(const QString &arg1)
{

}

void DialogColorSelect::on_leValue_editingFinished()
{
    m_color.fromRGB8(ui->leValue->text());
    m_color.color.setRed(forceInterval(m_color.color.red()));
    m_color.color.setGreen(forceInterval(m_color.color.green()));
    m_color.color.setBlue(forceInterval(m_color.color.blue()));
    ui->leValue->setText(m_color.toRGB8());
    UpdateGUI(nullptr);

}
