#include "dialogimport.h"
#include "ui_dialogimport.h"
#include <QString>
#include <QFileDialog>

DialogImport::DialogImport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogImport)
{
    ui->setupUi(this);
}

DialogImport::~DialogImport()
{
    if (m_image)
        delete m_image;
    delete ui;
}

void DialogImport::Initialize(LImage::Type imageType, LColorList::Type colorType)
{
    m_imageType = imageType;

    m_image = LImageFactory::Create(m_imageType, colorType);

    m_image->m_colorList.CreateUI(ui->layoutColors,0);
    m_image->m_colorList.FillComboBox(ui->cmbForeground);
    m_image->m_colorList.FillComboBox(ui->cmbBackground);

    //QObject::connect(this, LColorList::colorValueChanged, UpdateOutput);
    connect(&m_image->m_colorList, SIGNAL(colorValueChanged()), this, SLOT(UpdateOutput()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotOk()));

}



void DialogImport::Convert()
{
//    qDebug()<< "Type" << m_imageType;
    m_output.Release();
    m_output.m_qImage = m_work.Resize(m_image->m_width, m_image->m_height, m_image->m_colorList, m_contrast, m_shift, m_hsv, m_saturation, m_scale);
    m_image->Clear();
    m_image->fromQImage(m_output.m_qImage, m_image->m_colorList);

    if (m_output.m_qImage==nullptr)
         m_output.m_qImage = new QImage(m_image->m_width, m_image->m_height, QImage::Format_ARGB32);

    m_image->ToQImage(m_image->m_colorList,m_output.m_qImage,1, QPoint(0,0));


}

void DialogImport::Blur()
{
    m_work.Release();
    m_work.m_qImage = m_input.Blur(m_blur);

}

void DialogImport::UpdateOutput()
{
    Convert();
    QPixmap p;
    p.convertFromImage(*m_output.m_qImage);
    ui->lblTwo->setPixmap(p);
}


void DialogImport::on_cmbForeground_activated(int index)
{
    m_image->setForeground(index);
    UpdateOutput();
}

void DialogImport::on_cmbBackground_activated(int index)
{
    m_image->setBackground(index);
    UpdateOutput();
}



void DialogImport::on_btnImport_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));

    m_input.LoadQImage(fileName);
    Blur();

    UpdateOutput();

}

void DialogImport::slotOk()
{
    m_ok = true;
}


void DialogImport::on_hsContrast_sliderMoved(int position)
{
    m_contrast = (float)position/100.0*4;;
    UpdateOutput();

}

void DialogImport::on_hsShift_sliderMoved(int position)
{
    m_shift = ((float)position/100.0 - 0.5) * 255;;
    UpdateOutput();

}


void DialogImport::on_hsHsv_sliderMoved(int position)
{
    m_hsv = ((float)position/100.0) * 1;;
    UpdateOutput();

}

void DialogImport::on_hsSat_sliderMoved(int position)
{
    m_saturation = ((float)position/100.0) * 1;
    UpdateOutput();

}

void DialogImport::on_hsBlur_sliderMoved(int position)
{
    m_blur = ((float)position/100.0) ;
    Blur();
    UpdateOutput();

}


