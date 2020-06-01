/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

#include "dialogimport.h"
#include "ui_dialogimport.h"
#include <QString>
#include <QFileDialog>

DialogImport::DialogImport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogImport)
{
    ui->setupUi(this);
    QFontDatabase d;
    ui->cmbFonts->addItems(d.families());
    ui->cmbFonts->setCurrentText("Courier 10 Pitch");
    ui->cmbDither->setCurrentIndex(1);

}

DialogImport::~DialogImport()
{
    if (m_image)
        delete m_image;
    delete ui;
}

void DialogImport::Initialize(LImage::Type imageType, LColorList::Type colorType, LImage* img)
{
    m_imageType = imageType;

    m_image = LImageFactory::Create(m_imageType, colorType);
//    m_image->CopyFrom(img);
    m_image->m_colorList.m_list = img->m_colorList.m_list;
//    m_image->m_metaParams
/*    m_image->m_footer = img->m_footer;
    m_image->setMultiColor(img->isMultiColor());
    for (int i=0;i<)
    m_image->m_extraCols = img->m_extraCols;
*/
    m_image->CopyFrom(img);
//    m_image->Clear();

    LImageVIC20* vic = dynamic_cast<LImageVIC20*>(img);
    if (vic!=nullptr) {
        LImageVIC20* i = dynamic_cast<LImageVIC20*>(m_image);
        i->SetCharSize(vic->m_charWidth, vic->m_charHeight);
        i->m_width/=2;

    }
    LImageSprites2* sprite = dynamic_cast<LImageSprites2*>(img);





    if (sprite!=nullptr) {
        m_image->CopyFrom(sprite);
//        qDebug() << QString::number(sprite->m_items[sprite->m_current].m_header[0]);
//        qDebug() << "BLOCKS: " << sprite->m_sprites[sprite->m_currentChar].m_blocksWidth;
    }

    C64FullScreenChar* petscii = dynamic_cast<C64FullScreenChar*>(img);
    if (petscii!=nullptr) {
        // Start with petscii
        isPetscii=true;
        C64FullScreenChar* target = dynamic_cast<C64FullScreenChar*>(m_image);
        target->CopyFrom(img);
//        m_image = img;
    }

    if (m_image->m_colorList.m_type!=LColorList::NES) {

    m_image->m_colorList.CreateUI(ui->layoutColors,0);
    m_image->m_colorList.FillComboBox(ui->cmbForeground);
    m_image->m_colorList.FillComboBox(ui->cmbBackground);
    m_image->m_colorList.FillComboBox(ui->cmbMC1);
    m_image->m_colorList.FillComboBox(ui->cmbMC2);

//    qDebug() << "EXTRACOL 3 " <<QString::number(m_image->m_extraCols[3]);
 //   qDebug() << "EXTRACOL 0 " <<QString::number(m_image->m_extraCols[0]);

    ui->cmbForeground->setCurrentIndex(m_image->m_extraCols[3]);
    ui->cmbBackground->setCurrentIndex(m_image->m_extraCols[0]);
    ui->cmbMC1->setCurrentIndex(m_image->m_extraCols[1]);
    ui->cmbMC2->setCurrentIndex(m_image->m_extraCols[2]);

    if (isPetscii)
        ui->cmbMC1->setCurrentIndex(6);


    //QObject::connect(this, LColorList::colorValueChanged, UpdateOutput);
    connect(&m_image->m_colorList, SIGNAL(colorValueChanged()), this, SLOT(UpdateOutput()));
}
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotOk()));
}



void DialogImport::Convert()
{
//    qDebug()<< "Type" << m_imageType;
    m_output.Release();
    bool useDither = ui->chkDither->isChecked();
    if (m_work.m_qImage==nullptr)
        return;
    if (m_image == nullptr)
        return;
    m_output.m_qImage = m_work.Resize(m_image->m_width, m_image->m_height, m_image->m_colorList, m_contrast, m_shift, m_hsv, m_saturation, m_scale, useDither);
//    qDebug() << m_image->m_width << m_output.m_qImage->width();
    //exit(1);
//    m_image->Clear();
    m_image->m_importScaleX = 1+ (ui->hsScaleX->value()/100.0 - 0.5)*4;
    m_image->m_importScaleY = 1+ (ui->hsScaleY->value()/100.0 - 0.5)*4;
//    m_image->setPixel(10,10,1);
//    qDebug() << m_image->m_importScaleX;
    SetColors();
    QVector3D strength = QVector3D(1,1,1);
    strength.setX( (ui->hsDither->value()/100.0)*300.0);
//    m_output.m_qImage->save("temp.png");

//    for (int i=0;i<4;i++)
  //      qDebug() << "COLS : S " <<QString::number(m_image->m_extraCols[i]);
    matrixSize = ui->cmbDither->currentText().toInt();
//    qDebug() << matrixSize;
    if (!useDither)
       m_image->fromQImage(m_output.m_qImage, m_image->m_colorList);
    else
//        m_image->FloydSteinbergDither(*m_output.m_qImage,m_image->m_colorList, true);

        m_image->OrdererdDither(*m_output.m_qImage,m_image->m_colorList, strength,matrixSize,1);



    if (m_output.m_qImage==nullptr)
         m_output.m_qImage = new QImage(m_image->m_width, m_image->m_height, QImage::Format_ARGB32);


    CharsetImage* chr = dynamic_cast<CharsetImage*>(m_image);
    if (chr!=nullptr && chr->m_colorList.m_type!=LColorList::NES) {
        //chr->m_currentMode=CharsetImage::Mode::FULL_IMAGE;
        chr->m_footer.set(LImageFooter::POS_DISPLAY_CHAR,0);
        chr->SetColor(0,0);
        chr->SetColor(1,1);
//        chr->set

    }
   // for (int i=0;i<200;i++)
   //     qDebug() << QColor(m_image->getPixel(rand()%320, rand()%200));
    //qDebug() << "DIALOGIMPORT:";
    //for (int i=0;i<4;i++)
    //    qDebug() << i << Util::numToHex(m_image->m_colorList.m_nesCols[i]);

    m_image->ToQImage(m_image->m_colorList,*m_output.m_qImage,1, QPoint(0.0,0.0));

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
//    QPixmap p = m_pixMapImage.scaled(QSize(grid.width(),grid.height()),  Qt::IgnoreAspectRatio, Qt::FastTransformation);
    if (m_output.m_qImage==nullptr)
        return;
    p.convertFromImage(*m_output.m_qImage);
    p = p.scaled(320,200);
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
 //   m_image->setC
    UpdateOutput();
}

void DialogImport::SetColors()
{

    if (m_image->m_colorList.m_type!=LColorList::NES) {


    int a = ui->cmbMC1->currentIndex();
    int b = ui->cmbMC2->currentIndex();
    int back = ui->cmbBackground->currentIndex();


    m_image->SetColor(back, 0);
    m_image->SetColor(a, 1);
    m_image->SetColor(b, 2);
    }

}



void DialogImport::on_btnImport_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));

    m_input.LoadQImage(fileName);
    if (ui->chkGenPal->isChecked()) {
        m_image->m_colorList.GeneratePaletteFromQImage(*m_input.m_qImage);
        m_image->m_colorList.CreateUI(ui->layoutColors,0);

    }
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



void DialogImport::on_cmbMC1_activated(int index)
{
    SetColors();
    UpdateOutput();

}

void DialogImport::on_cmbMC2_activated(int index)
{
    SetColors();
    UpdateOutput();
}

void DialogImport::on_chkDither_stateChanged(int arg1)
{
    SetColors();
    UpdateOutput();

}

void DialogImport::on_btnFromFont_clicked()
{

}

void DialogImport::on_btnImport_2_clicked()
{
    // Generate font
//    m_bf.Test();
    m_bf.Init(m_image->m_width, m_image->m_height);
    m_bf.RenderFont(ui->cmbFonts->currentText(),ui->leFontSize->text().toInt(),QFont::Normal,
                    ui->leFontCharsPerLine->text().toInt(),
                    ui->leFontCharsPerLColumn->text().toInt(),
                    ui->leStart->text().toInt()
                    );
    m_input.m_qImage = &m_bf.m_image;
    int fc = ui->leUseColors->text().toInt();
    m_image->m_colorList.ConstrainTo(fc);
    SetColors();
    Blur();

    UpdateOutput();

}

void DialogImport::on_hsDither_sliderMoved(int position)
{
    Blur();
    UpdateOutput();

}

void DialogImport::on_hsScaleX_sliderMoved(int position)
{
    UpdateOutput();

}

void DialogImport::on_hsScaleY_sliderMoved(int position)
{
    UpdateOutput();

}

void DialogImport::on_cmbDither_currentIndexChanged(int index)
{
    UpdateOutput();

}
