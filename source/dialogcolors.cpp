#include "dialogcolors.h"
#include "ui_dialogcolors.h"

DialogColors::DialogColors(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogColors)
{
    ui->setupUi(this);
}

DialogColors::~DialogColors()
{
    delete m_lst;
    delete ui;
}

void DialogColors::toGUI()
{
    m_lst->FillComboBox(ui->cbmColors);
    ui->cbmColors->setCurrentIndex(m_curCol);
    ui->cbmBitplanes->setCurrentIndex(m_lst->getNoBitplanes()-1);
    if (m_curCol!=0)
        setColor(m_curCol,0);

}


void DialogColors::setColor(int cc, int ty) {
    m_curCol = cc;
    QPushButton* button = ui->btnColor;
    QPalette pal = button->palette();
    pal.setColor(QPalette::Button, m_lst->get(m_curCol).color);
    button->setAutoFillBackground(true);
    button->setPalette(pal);
    button->update();

    if (ty==0) {
        ui->leRGB->setText(m_lst->get(m_curCol).toRGB8());
        ui->leRGB4->setText(m_lst->get(m_curCol).toRGB4());
    }
    if (ty==1)
    ui->leRGB->setText(m_lst->get(m_curCol).toRGB8());
    if (ty==2)
    ui->leRGB4->setText(m_lst->get(m_curCol).toRGB4());

    ui->lblRGB4->setText("$"+QString::number(m_lst->get(m_curCol).get12BitValue(),16));

}

void DialogColors::on_pushButton_clicked()
{
    m_org->m_list = m_lst->m_list;
    close();
}

void DialogColors::on_cbmColors_activated(int index)
{
    setColor(index,0);
}

void DialogColors::on_leRGB_textChanged(const QString &arg1)
{
    m_lst->get(m_curCol).fromRGB8(arg1);
    m_lst->FillComboBox(ui->cbmColors);
    ui->cbmColors->setCurrentIndex(m_curCol);
    setColor(m_curCol,3);
}

void DialogColors::on_leRGB4_textChanged(const QString &arg1)
{
    m_lst->get(m_curCol).fromRGB4(arg1);
    m_lst->FillComboBox(ui->cbmColors);
    ui->cbmColors->setCurrentIndex(m_curCol);
    setColor(m_curCol,3);

}

void DialogColors::on_btnLoad_clicked()
{

}

void DialogColors::on_btnSave_2_clicked()
{
    QString ext = "bin";
    QString ttr  = "Export 4-bit amiga palette";
    QString f = "Binary( *."+ext+" )";

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    ttr.toStdString().c_str(), m_projectPath,
                                                    f);


//    m_work.m_currentImage->m_image->ExportAsm(fileName);
//    MultiColorImage* mi = (MultiColorImage*)dynamic_cast<MultiColorImage*>(m_work.m_currentImage->m_image);

   // fileName = fileName.remove(".bin");
    m_lst->ExportAmigaPalette(fileName);

}

void DialogColors::on_pushButton_2_clicked()
{
    close();

}

void DialogColors::on_cbmBitplanes_currentIndexChanged(int index)
{
    qDebug()  << index;
    m_lst->setNoBitplanes(index+1);
    toGUI();
}
