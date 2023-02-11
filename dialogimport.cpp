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
#include "source/Compiler/syntax.h"

DialogImport::DialogImport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogImport)
{
    ui->setupUi(this);
#if QT_VERSION <= 0x060000
    QFontDatabase d;
    ui->cmbFonts->addItems(d.families());
#else
    ui->cmbFonts->addItems(QFontDatabase::families());
    ui->cmbFonts->setCurrentText("Courier 10 Pitch");

#endif
    ui->cmbDitherY->setCurrentIndex(1);
    ui->cmbDitherX->setCurrentIndex(0);
    UpdateText();
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
//    qDebug() << "Currenttype: " <<QString::number(LImage::TypeToChar(m_imageType)) <<m_image->m_width<<img->m_width;

    m_image = LImageFactory::Create(m_imageType, colorType);
    m_work.Initialize(m_image->m_width, m_image->m_height);
    m_work.m_colorList.CopyFrom(&img->m_colorList);
    //qDebug() << "COUNT : " << m_work.m_colorList.m_list.count();
//    qDebug() << m_image->m_width << m_imageType;

    if (!Syntax::s.m_currentSystem->m_hasVariableColorPalette)
        ui->chkGenPal->setVisible(false);
//    m_image->m_colorList.CopyFrom(&img->m_colorList);
//    m_image->CopyFrom(img);
//    m_image->m_metaParams
/*    m_image->m_footer = img->m_footer;
    m_image->setMultiColor(img->isMultiColor());
    for (int i=0;i<)
    m_image->m_extraCols = img->m_extraCols;
*/
    m_image->CopyFrom(img);
//    m_image->Clear();
    m_image->m_colorList.CopyFrom(&img->m_colorList);

   if (m_image->m_colorList.m_type == LColorList::C64 || m_image->m_colorList.m_type == LColorList::VIC20)
       m_image->m_colorList.m_selectClosestFromPen = false;


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

    if (m_image->m_colorList.m_type==LColorList::C64 || m_image->m_colorList.m_type==LColorList::VIC20) {

        m_image->m_colorList.CreateUI(ui->layoutColors,0);
        /*    m_image->m_colorList.FillComboBox(ui->cmbForeground);
    m_image->m_colorList.FillComboBox(ui->cmbBackground);
    m_image->m_colorList.FillComboBox(ui->cmbMC1);
    m_image->m_colorList.FillComboBox(ui->cmbMC2);
*/
        //    qDebug() << "EXTRACOL 3 " <<QString::number(m_image->m_extraCols[3]);
        //   qDebug() << "EXTRACOL 0 " <<QString::number(m_image->m_extraCols[0]);

        /*    if (dynamic_cast<MultiColorImage*>(m_image)!=nullptr) {
        ui->cmbForeground->setCurrentIndex(m_image->m_colorList.getPen(3));
        ui->cmbBackground->setCurrentIndex(m_image->m_colorList.getPen(0));
        ui->cmbMC1->setCurrentIndex(m_image->m_colorList.getPen(1));
        ui->cmbMC2->setCurrentIndex(m_image->m_colorList.getPen(2));
    }

    if (isPetscii)
        ui->cmbMC1->setCurrentIndex(6);
*/

        //QObject::connect(this, LColorList::colorValueChanged, UpdateOutput);
        connect(&m_image->m_colorList, SIGNAL(colorValueChanged()), this, SLOT(UpdateOutput()));
    }
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotOk()));
  //  qDebug() << "INIT1" <<img->m_colorList.m_list.count() << m_imageType << colorType <<m_image->m_type;
  //  qDebug() << "INIT2" <<m_image->m_colorList.m_list.count();
}



void DialogImport::Convert()
{
//    qDebug()<< "Type" << m_imageType;
    m_output.Release();
    bool useDither = ui->cmbDither->currentIndex()!=0;
    if (m_work.m_qImage==nullptr)
        return;
    if (m_image == nullptr)
        return;


    m_image->m_forceD800Color = ui->leForceD800->text().toInt();

    LImageQImage* img = &m_work;
//    qDebug() << "A0" << img->m_colorList.m_list.count();
//    if (img->m_colorList.m_list.count()==0)
  //      return;

    if (ui->chkTreatCharset->isChecked()) {
        img = &m_intermediate;
        m_intermediate.Initialize(m_image->m_width, m_image->m_height);
        m_intermediate.RemapCharset(m_work.m_qImage,
                                    ui->leCharWidth->text().toInt(),
                                    ui->leCharHeight->text().toInt(),
                                    ui->leBlockWidth->text().toInt(),
                                    ui->leBlockHeight->text().toInt(),
                                    ui->leOutCharWidth->text().toInt(),
                                    ui->leOutCharHeight->text().toInt(),
                                    ui->leAllowance->text().toInt()
                                    );
    }

    double scaleX = 1+(ui->hsScaleX->value()/100.0 - 0.5)*4;
    double scaleY = 1+(ui->hsScaleY->value()/100.0 - 0.5)*4;


    m_output.m_qImage = img->Resize(m_work.m_qImage->width(),
                                    m_work.m_qImage->height(),
                                    m_image->m_colorList,
                                    m_contrast, m_shift, m_hsv, m_saturation, m_scale, useDither);


//    m_output.m_qImage->save("/Users/leuat/test.png");
//    qDebug() << m_image->m_width << m_output.m_qImage->width() << m_work.m_qImage->width();
    //exit(1);
//    m_image->Clear();
//    m_image->m_importScaleX = scaleX;//+ (ui->hsScaleX->value()/100.0 - 0.5)*4;
//    m_image->m_importScaleY = scaleY;//+ (ui->hsScaleY->value()/100.0 - 0.5)*4;
    m_image->m_importScaleX = scaleX;//+ (ui->hsScaleX->value()/100.0 - 0.5)*4;
    m_image->m_importScaleY = scaleY;//+ (ui->hsScaleY->value()/100.0 - 0.5)*4;

//    m_image->setPixel(10,10,1);
//    qDebug() << m_image->m_importScaleX;
    SetColors();
    QVector3D strength = QVector3D(1,1,1);
    strength.setX( (ui->hsDither->value()/100.0)*300.0);

//    for (int i=0;i<4;i++)
  //      qDebug() << "COLS : S " <<QString::number(m_image->m_extraCols[i]);
    matrixSizeX = ui->cmbDitherX->currentText().toInt();
    matrixSizeY = ui->cmbDitherY->currentText().toInt();
//    qDebug() << matrixSize;



    LImageQImage* inter = nullptr;
    LImage* org = nullptr;
    LColorList orgCols;
    bool useCells = dynamic_cast<MultiColorImage*>(m_image)!=nullptr && dynamic_cast<LImageContainer*>(m_image)==nullptr;
    if (dynamic_cast<CharsetImage*>(m_image)!=nullptr)
        useCells = false;
    if (dynamic_cast<LImageVIC20*>(m_image)!=nullptr)
        useCells = false;
    if (dynamic_cast<LImageSprites2*>(m_image)!=nullptr)
        useCells = false;

//    useCells = false;

    if (useCells) {
        // OK. we need to do some tricks. Convert to FAKE c64 image first:
        inter = new LImageQImage(m_image->m_colorList.m_type);
        inter->Initialize(m_image->m_width, m_image->m_height);
        org = m_image;
        m_image = inter;
        m_image->m_colorList.m_selectClosestFromPen = false;
        m_image->m_colorList.CopyFrom(&org->m_colorList);
        orgCols.CopyFrom(&org->m_colorList);
        m_image->m_colorList.EnableColors(org->m_colorList.m_enabledColors);
        m_image->m_importScaleX = 1+ (ui->hsScaleX->value()/100.0 - 0.5)*4.0;
        m_image->m_importScaleY = 1+ (ui->hsScaleY->value()/100.0 - 0.5)*4.0;
   //     qDebug() << "AA" << m_work.m_colorList.m_list.count();
        m_image->CopyFrom(img);
     //   qDebug() << m_image->m_colorList.m_list.count();

//        m_image->m_colorList.CopyFrom(&org->m_colorList);
    }

    m_image->Clear(img->getBackground());
//    m_image->setBackground(img->getBackground());
/*    MultiColorImage* mc = dynamic_cast<MultiColorImage*>(img);
    if (img!=nullptr){
        m_image->m_colorList.m_multicolors = img->m_colorList.m_multicolors;
    }*/
/*    if (!useDither)
       m_image->fromQImage(m_output.m_qImage, m_image->m_colorList);
    else
//        m_image->FloydSteinbergDither(*m_output.m_qImage,m_image->m_colorList, true);
*/
    if (!useDither)
        strength.setX(0);

//    qDebug() << "IMG WIDTH " <<m_output.m_qImage->width();
    if (org!=nullptr)
        m_image->m_colorList.CopyFrom(&org->m_colorList);
    if (ui->chkCustom->isChecked()) {
        auto lst = ui->leCustomPalette->text().split("," );
        m_image->m_colorList.m_customPalette = Util::HexQStringListToByteArray(lst);

//        org->m_colorList.m_customPalette = Util::HexQStringListToByteArray(lst);;
    }
    m_image->m_importScale = 2;
//        qDebug() << scaleX;
  //  m_output.OrdererdDither(*m_output.m_qImage,m_image->m_colorList, QVector3D(0,0,0),QPoint(matrixSizeX,matrixSizeY),1);
    //*m_output.m_qImage = QImage(m_output.m_qImage->scaled(m_image->GetWidth(),m_image->GetHeight(),Qt::IgnoreAspectRatio));
    if (ui->cmbDither->currentIndex()==1) {
        //    QPixmap p = m_pixMapImage.scaled(QSize(grid.width(),grid.height()),  Qt::IgnoreAspectRatio, Qt::FastTransformation);
  //      LImageQImage* copy = (LImageQImage*)LImageFactory::Create(LImage::QImageBitmap,LColorList::C64);
      //  m_output.CopyFrom(&m_input);
      //  copy->CopyFrom(&m_input);
     //   copy->m_importScaleX = scaleX;//+ (ui->hsScaleX->value()/100.0 - 0.5)*4;
      //  copy->m_importScaleY = scaleY;//+ (ui->hsScaleY->value()/100.0 - 0.5)*4;
       // qDebug() << "GERE" <<copy->GetWidth();
//        m_output.m_importScaleX = scaleX;//+ (ui->hsScaleX->value()/100.0 - 0.5)*4;
  //      m_output.m_importScaleY = scaleY;//+ (ui->hsScaleY->value()/100.0 - 0.5)*4;
    //    m_output.CopyFrom(&m_work);
      //  m_output.OrdererdDither(*m_input.m_qImage,m_image->m_colorList, QVector3D(1,1,1),QPoint(matrixSizeX,matrixSizeY),1);
        *m_output.m_qImage = QImage(m_output.m_qImage->scaled(m_image->GetWidth(),m_image->GetHeight(),Qt::IgnoreAspectRatio));
        m_image->FloydSteinbergDither(*m_output.m_qImage,m_image->m_colorList, true,strength.x());
//        delete copy;
    }
    else
        m_image->OrdererdDither(*m_output.m_qImage,m_image->m_colorList, strength,QPoint(matrixSizeX,matrixSizeY),1);


    if (ui->chkCustom->isChecked()) {
        m_image->m_colorList.m_customPalette = QByteArray();
    }


    if (m_output.m_qImage!=nullptr)  {
        delete m_output.m_qImage;
        m_output.m_qImage = nullptr;
    }


    m_output.m_qImage = new QImage(m_image->m_width, m_image->m_height, QImage::Format_ARGB32);



//    qDebug() << "WWW " <<m_output.m_qImage->width();

    CharsetImage* chr = dynamic_cast<CharsetImage*>(m_image);
    if (chr!=nullptr && chr->m_colorList.m_type!=LColorList::NES) {
        //chr->m_currentMode=CharsetImage::Mode::FULL_IMAGE;
        chr->m_footer.set(LImageFooter::POS_DISPLAY_CHAR,0);
//        chr->SetColor(0,0);
  //      chr->SetColor(1,1);
//        chr->set

    }
   // for (int i=0;i<200;i++)
   //     qDebug() << QColor(m_image->getPixel(rand()%320, rand()%200));
    //qDebug() << "DIALOGIMPORT:";
    //for (int i=0;i<4;i++)
    //    qDebug() << i << Util::numToHex(m_image->m_colorList.m_nesCols[i]);

    if (inter!=nullptr) {
        // Need to convert back to c64 cells
        m_image = org;
        m_image->FromLImageQImage(inter);
        m_image->m_colorList.CopyFrom(&orgCols);

        delete inter;
        inter = nullptr;

    }

    m_image->ToQImage(m_image->m_colorList,*m_output.m_qImage,1, QPoint(0.0,0.0));
}

void DialogImport::Blur()
{
    if (m_input.m_qImage == nullptr)
        return;
    m_work.Release();
    m_work.m_qImage = m_input.Blur(m_blur);

}

void DialogImport::UpdateSliders()
{
    if (ignoreText)
        return;
    ignore = true;
    ui->hsContrast->setValue(ui->leGamma->text().toFloat());
    on_hsContrast_sliderMoved(ui->leGamma->text().toInt());
    ui->hsShift->setValue(ui->leShift->text().toFloat());
    on_hsShift_sliderMoved(ui->leShift->text().toInt());
    ui->hsHsv->setValue(ui->leHsv->text().toFloat());
    on_hsHsv_sliderMoved(ui->leHsv->text().toInt());
    ui->hsSat->setValue(ui->leSat->text().toFloat());
    on_hsSat_sliderMoved(ui->leSat->text().toInt());
    ui->hsBlur->setValue(ui->leBlur->text().toFloat());
    on_hsBlur_sliderMoved(ui->leBlur->text().toInt());
    ui->hsScaleX->setValue(ui->leScaleX->text().toFloat());
    on_hsScaleX_sliderMoved(ui->leScaleX->text().toInt());
    ui->hsScaleY->setValue(ui->leScaleY->text().toFloat());
    on_hsScaleY_sliderMoved(ui->leScaleY->text().toInt());
    ui->hsDither->setValue(ui->leDither->text().toFloat());
    on_hsDither_sliderMoved(ui->leDither->text().toInt());
    ignore = false;

    UpdateOutput();


}

void DialogImport::UpdateText()
{
    if (ignore)
        return;
    ignoreText = true;
    if (ui->leGamma->text().toInt()!=ui->hsContrast->value()) {
//        qDebug()<<"GAMMA "<<ui->hsContrast->value();
        ui->leGamma->setText(QString::number(ui->hsContrast->value()));
    }
    if (ui->leShift->text().toInt()!=ui->hsShift->value())
      ui->leShift->setText(QString::number(ui->hsShift->value()));
    if (ui->leHsv->text().toInt()!=ui->hsHsv->value())
      ui->leHsv->setText(QString::number(ui->hsHsv->value()));
    if (ui->leSat->text().toInt()!=ui->hsSat->value())
      ui->leSat->setText(QString::number(ui->hsSat->value()));
    if (ui->leBlur->text().toInt()!=ui->hsBlur->value())
      ui->leBlur->setText(QString::number(ui->hsBlur->value()));
    if (ui->leScaleX->text().toInt()!=ui->hsScaleX->value())
      ui->leScaleX->setText(QString::number(ui->hsScaleX->value()));
    if (ui->leScaleY->text().toInt()!=ui->hsScaleY->value())
      ui->leScaleY->setText(QString::number(ui->hsScaleY->value()));
    if (ui->leDither->text().toInt()!=ui->hsDither->value())
      ui->leDither->setText(QString::number(ui->hsDither->value()));
//    UpdateOutput();
    ignoreText = false;
    UpdateOutput();

}



void DialogImport::UpdateOutput()
{
    if (ignore)
        return;
    if (ignoreText)
        return;

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
//    m_image->setForeground(index);
    UpdateOutput();
}

void DialogImport::on_cmbBackground_activated(int index)
{
//    m_image->setBackground(index);
 //   m_image->setC
    UpdateOutput();
}

void DialogImport::SetColors()
{

    if (m_image->m_colorList.m_type!=LColorList::NES) {


/*    int a = ui->cmbMC1->currentIndex();
    int b = ui->cmbMC2->currentIndex();
    int back = ui->cmbBackground->currentIndex();
*/

//    m_image->SetColor(1, 0);
 //   m_image->SetColor(2, 1);
  //  m_image->SetColor(b, 2);
    }

}



void DialogImport::on_btnImport_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp *.jpeg *.gif)"));

    m_input.LoadQImage(fileName);

    if (m_input.m_width<=m_work.m_width || m_input.m_height<=m_work.m_height) {
        QMessageBox msgBox;
        QString resIn = QString::number(m_input.m_width) + "x"+QString::number(m_input.m_height);
        QString resOut = QString::number(m_work.m_width) + "x"+QString::number(m_work.m_height);
        msgBox.setText("Your input image has a lower resolution ("+resIn+") than the output image ("+resOut+"). The input image should be the same resolution (or larger) than the target resolution, so this conversion might produce incorrect results.");
        msgBox.exec();
    }


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
    UpdateText();
    m_contrast = (float)position/100.0*4;;
//    UpdateOutput();

}

void DialogImport::on_hsShift_sliderMoved(int position)
{
    UpdateText();
    m_shift = ((float)position/100.0 - 0.5) * 255;;
  //  UpdateOutput();

}


void DialogImport::on_hsHsv_sliderMoved(int position)
{
    UpdateText();
    m_hsv = ((float)position/100.0) * 1;;
   // UpdateOutput();

}

void DialogImport::on_hsSat_sliderMoved(int position)
{
    UpdateText();
    m_saturation = ((float)position/100.0) * 1;
//    UpdateOutput();

}

void DialogImport::on_hsBlur_sliderMoved(int position)
{
    UpdateText();
    m_blur = ((float)position/100.0) ;
    Blur();
  //  UpdateOutput();

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

/*void DialogImport::on_chkDither_stateChanged(int arg1)
{
    SetColors();
    UpdateOutput();

}
*/
void DialogImport::on_btnFromFont_clicked()
{

}

void DialogImport::on_btnImport_2_clicked()
{
    // Generate font
//    m_bf.Test();
    m_bf.Init(m_image->m_width*m_image->m_scale, m_image->m_height);
//    qDebug() << m_image->m_width << m_image->m_height;
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
    UpdateText();
    Blur();
//    UpdateOutput();

}

void DialogImport::on_hsScaleX_sliderMoved(int position)
{
    UpdateText();
  //  UpdateOutput();

}

void DialogImport::on_hsScaleY_sliderMoved(int position)
{
    UpdateText();
   // UpdateOutput();

}

void DialogImport::on_cmbDither_currentIndexChanged(int index)
{
    UpdateOutput();

}

void DialogImport::on_chkTreatCharset_stateChanged(int arg1)
{
    bool enabled = ui->chkTreatCharset->isChecked();

    ui->leCharWidth->setEnabled(enabled);
    ui->leCharHeight->setEnabled(enabled);
    ui->leBlockWidth->setEnabled(enabled);
    ui->leBlockHeight->setEnabled(enabled);

    UpdateOutput();

}

void DialogImport::on_leCharWidth_textChanged(const QString &arg1)
{
    UpdateOutput();
}

void DialogImport::on_leCharHeight_textChanged(const QString &arg1)
{
    UpdateOutput();
}

void DialogImport::on_leBlockWidth_textChanged(const QString &arg1)
{
    UpdateOutput();
}

void DialogImport::on_leBlockHeight_textChanged(const QString &arg1)
{
    UpdateOutput();
}

void DialogImport::on_leOutCharWidth_textChanged(const QString &arg1)
{
    UpdateOutput();
}

void DialogImport::on_leOutCharHeight_textChanged(const QString &arg1)
{
    UpdateOutput();
}

void DialogImport::on_leAllowance_textChanged(const QString &arg1)
{
    UpdateOutput();

}

void DialogImport::on_leDither_textChanged(const QString &arg1)
{
    UpdateSliders();
//    UpdateOutput();
}

void DialogImport::on_leGamma_textChanged(const QString &arg1)
{
    UpdateSliders();
//    UpdateOutput();

}

void DialogImport::on_leShift_textChanged(const QString &arg1)
{
    UpdateSliders();
 //   UpdateOutput();

}

void DialogImport::on_leHsv_textChanged(const QString &arg1)
{
    UpdateSliders();
 //   UpdateOutput();

}

void DialogImport::on_leSat_textChanged(const QString &arg1)
{
    UpdateSliders();
  //  UpdateOutput();

}

void DialogImport::on_leBlur_textChanged(const QString &arg1)
{
    UpdateSliders();
//    UpdateOutput();

}

void DialogImport::on_leScaleX_textChanged(const QString &arg1)
{
    UpdateSliders();
  //  UpdateOutput();

}

void DialogImport::on_leScaleY_textChanged(const QString &arg1)
{
    UpdateSliders();
    //UpdateOutput();

}

void DialogImport::on_chkCustom_stateChanged(int arg1)
{
    UpdateSliders();
}

void DialogImport::on_leCustomPalette_textChanged(const QString &arg1)
{
    UpdateSliders();

}


void DialogImport::on_comboBox_currentIndexChanged(int index)
{
    ui->leCustomPalette->setText(m_customPalettes[index]);

}

void DialogImport::on_leForceD800_editingFinished()
{
    UpdateSliders();
    UpdateOutput();
}


void DialogImport::on_leForceD800_textChanged(const QString &arg1)
{
    UpdateSliders();
    UpdateOutput();
}


void DialogImport::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    SetColors();
    UpdateOutput();
}

