#include "dialognewimage.h"
#include "ui_dialognewimage.h"

DialogNewImage::DialogNewImage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewImage )
{
    ui->setupUi(this);
    connect(ui->btnResult, SIGNAL(accepted()), this, SLOT(slotOk()));

    ui->grpLevelDesignerParams->setVisible(false);

}

void DialogNewImage::Initialize(QStringList cmbData)
{
    ui->comboBox->addItems(cmbData);
}

void DialogNewImage::SetResizeMeta(CharmapGlobalData gd)
{
    isResize = true;
    ui->comboBox->setCurrentIndex(7);
    m_meta = gd;

    started = false; // prevent triggering
    ui->leScreenWidth->setText(QString::number(m_meta.m_width));
    ui->leScreenHeight->setText(QString::number(m_meta.m_height));
    ui->leLevelsX->setText(QString::number(m_meta.m_sizex));
    ui->leLevelsY->setText(QString::number(m_meta.m_sizey));
    ui->leDataChunks->setText(QString::number(m_meta.m_dataChunks));
    ui->leChunkSize->setText(QString::number(m_meta.m_dataChunkSize));
    ui->leStartX->setText(QString::number(m_meta.m_startx));
    ui->leStartY->setText(QString::number(m_meta.m_starty));
    started = true;


}

void DialogNewImage::ToMeta()
{
    if (!started)
        return;
    m_meta.m_width = ui->leScreenWidth->text().toInt();
    m_meta.m_height = ui->leScreenHeight->text().toInt();
    m_meta.m_sizex = ui->leLevelsX->text().toInt();
    m_meta.m_sizey = ui->leLevelsY->text().toInt();
    m_meta.m_startx = ui->leStartX->text().toInt();
    m_meta.m_starty = ui->leStartY->text().toInt();
//    m_meta.m_extraDataSize = ui->leExtraDataSize->text().toInt();
    m_meta.m_dataChunks = ui->leDataChunks->text().toInt();
    m_meta.m_dataChunkSize = ui->leChunkSize->text().toInt();
    m_meta.m_extraDataSize = m_meta.m_dataChunkSize*m_meta.m_dataChunks + 3;
    CreateInfo();
}

void DialogNewImage::CreateInfo()
{
    m_meta.Calculate();
    QString txt="";
    txt+= "Char Data & color size: " + QString::number(m_meta.dataSize()) + " bytes\n";
    txt+= "Extra data size: " + QString::number(m_meta.m_extraDataSize) + " bytes\n";
    txt+= "Level size: " + QString::number(m_meta.levelSize()) + " bytes\n";
    txt+= "Total no levels: " + QString::number(m_meta.m_sizex*m_meta.m_sizey) + " \n";
    txt+= "Total size: " + QString::number(m_meta.totalSize()) + " bytes\n";
    ui->lblInfo->setText(txt);
}

DialogNewImage::~DialogNewImage()
{
    delete ui;
}


void DialogNewImage::slotOk()
{
    retVal = ui->comboBox->currentIndex();
}

void DialogNewImage::on_comboBox_currentIndexChanged(int index)
{
    if (index==7)
        ui->grpLevelDesignerParams->setVisible(true);

    ToMeta();
}

void DialogNewImage::on_leScreenWidth_textChanged(const QString &arg1)
{
    ToMeta();
}

void DialogNewImage::on_leScreenHeight_textChanged(const QString &arg1)
{
      ToMeta();
}

void DialogNewImage::on_leLevelsX_textChanged(const QString &arg1)
{
      ToMeta();
}

void DialogNewImage::on_leLevelsY_textChanged(const QString &arg1)
{
      ToMeta();
}

void DialogNewImage::on_leExtraDataSize_textChanged(const QString &arg1)
{
      ToMeta();
}

void DialogNewImage::on_leStartX_textChanged(const QString &arg1)
{
      ToMeta();
}

void DialogNewImage::on_leStartY_textChanged(const QString &arg1)
{
      ToMeta();
}

void DialogNewImage::on_leChunkSize_textChanged(const QString &arg1)
{
    ToMeta();
}

void DialogNewImage::on_leDataChunks_textChanged(const QString &arg1)
{
    ToMeta();
}
