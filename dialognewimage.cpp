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

#include "dialognewimage.h"
#include "ui_dialognewimage.h"

DialogNewImage::DialogNewImage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewImage )
{
    ui->setupUi(this);
    connect(ui->btnResult, SIGNAL(accepted()), this, SLOT(slotOk()));

    ui->grpLevelDesignerParams->setVisible(false);
    ui->grpImageSize->setVisible(false);


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
    m_meta.m_useColors = !ui->chkUseColors->isChecked();
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

void DialogNewImage::VICImageToData()
{
    m_charWidth = ui->leCharWidth->text().toInt();
    m_charHeight = ui->leCharHeight->text().toInt();

    QString txt="";
    int chars = m_charWidth*m_charHeight/2;
    txt+= "Chars (8x16) used: " + QString::number(chars) +"\n";
    if (chars>192)
            txt+= "WARNING more than 192 chars! Will be truncated. \n";

    txt+= "Data size: " + QString::number(m_charWidth*m_charHeight*8) + " bytes\n";
    txt+= "Color size: " + QString::number(m_charWidth*m_charHeight) + " bytes\n";
    txt+= "Total size: " + QString::number(m_charWidth*m_charHeight*9) + " bytes\n";
    txt+= "Pixel dimensions " + QString::number(m_charWidth*4) + "x" +QString::number(m_charHeight*8);
    ui->lblInfo->setText(txt);


}

void DialogNewImage::CharImageToData()
{
    m_charWidth = ui->leCharWidth->text().toInt();
    m_charHeight = ui->leCharHeight->text().toInt();

    QString txt="";
    int chars = m_charWidth*m_charHeight;
    txt+= "Chars (8x8) used: " + QString::number(chars) +"\n";
    txt+= "Data size: " + QString::number(m_charWidth*m_charHeight) + " bytes\n";
    txt+= "Color size: " + QString::number(m_charWidth*m_charHeight) + " bytes\n";
    txt+= "Total size: " + QString::number(m_charWidth*m_charHeight*2) + " bytes\n";
    ui->lblInfo->setText(txt);

}

DialogNewImage::~DialogNewImage()
{
    delete ui;
}


void DialogNewImage::slotOk()
{
    //VICImageToData();
    if (ui->comboBox->currentIndex() == 9) {
         m_meta.m_width = m_charWidth;
        m_meta.m_height = m_charHeight;
    }
    if (ui->comboBox->currentIndex()==6) {
        ui->grpImageSize->setVisible(true);
        CharImageToData();
        m_meta.m_width = m_charWidth;
       m_meta.m_height = m_charHeight;
 //       exit(1);
    }

    retVal = ui->comboBox->currentIndex();
}

void DialogNewImage::on_comboBox_currentIndexChanged(int index)
{
    ui->grpLevelDesignerParams->setVisible(false);
    ui->grpImageSize->setVisible(false);
/*    m_types.append(ImageType("Screen animation", LImage::Type::FullScreenChar,LColorList::Type::C64));
    m_types.append(ImageType("C64 Level Editor", LImage::Type::LevelEditor,LColorList::Type::C64));
    m_types.append(ImageType("VIC20 Multicolor bitmap", LImage::Type::VIC20_MultiColorbitmap,LColorList::Type::VIC20));
*/
    if (ui->comboBox->currentText()=="C64 Level Editor")
        ui->grpLevelDesignerParams->setVisible(true);

    if (ui->comboBox->currentText()=="VIC20 Multicolor bitmap") {
        ui->grpImageSize->setVisible(true);
        VICImageToData();
    }

    if (ui->comboBox->currentText()=="Screen animation") {
        ui->grpImageSize->setVisible(true);
        CharImageToData();
 //       exit(1);
    }


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

void DialogNewImage::on_checkBox_clicked()
{
    ToMeta();
}

void DialogNewImage::on_chkUseColors_stateChanged(int arg1)
{
    ToMeta();
}

void DialogNewImage::on_leCharWidth_textChanged(const QString &arg1)
{
    VICImageToData();
}

void DialogNewImage::on_leCharHeight_textChanged(const QString &arg1)
{
    VICImageToData();
}
