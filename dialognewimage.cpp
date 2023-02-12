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
#include <QLineEdit>
#include "source/Compiler/syntax.h"

DialogNewImage::DialogNewImage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewImage )
{
    ui->setupUi(this);
    connect(ui->btnResult, SIGNAL(accepted()), this, SLOT(slotOk()));

//    ui->grpLevelDesignerParams->setVisible(false);
    CreateCategories();

}

void DialogNewImage::Initialize(QVector<ImageType> types)
{
    m_types = types;

 //   ui->comboBox->addItems(cmbData);

    // Fill parameters
   CreateCategories();
}

void DialogNewImage::CreateCategories()
{
    m_categories.clear();
    for (ImageType& it: m_types)
        if (!m_categories.contains(it.category) || it.category=="all")
            m_categories << it.category;


   ui->cmbSystem->addItems(m_categories);
   ui->cmbSystem->setCurrentText(Syntax::s.m_systemString.toLower());
}

void DialogNewImage::SetResizeMeta(CharmapGlobalData gd)
{
    isResize = true;
//    ui->comboBox->setCurrentIndex(7);
    m_meta = gd;

    started = false; // prevent triggering
    started = true;


}

void DialogNewImage::ToMeta()
{
    if (!started)
        return;
}


void DialogNewImage::FromMeta()
{
    if (m_metaImage==nullptr)
        return;
//    setTabOrder()
    Util::clearLayout(ui->grdParams);
    int y=0;
    //setFocusPolicy(Qt::StrongFocus);
    //setFocus();
    QWidget* prev = ui->cmbImageType;
    for (MetaParameter* mp:m_metaImage->m_metaParams) {
        ui->grdParams->addWidget(new QLabel(mp->text),y,0);
        QLineEdit* le = new QLineEdit(QString::number(mp->value));
        ui->grdParams->addWidget(le,y,1);

        QObject::connect(le, &QLineEdit::editingFinished, [=]() {
            mp->value = le->text().toFloat();
            FromMeta();
        });
        setTabOrder(prev, le);
        prev = le;

        y++;

    }
    ui->lblInfo->setText(m_metaImage->getMetaInfo());

}


/*void DialogNewImage::CharImageToData()
{
//    m_charWidth = ui->leCharWidth->text().toInt();
  //  m_charHeight = ui->leCharHeight->text().toInt();

    QString txt="";
    int chars = m_charWidth*m_charHeight;
    txt+= "Chars (8x8) used: " + QString::number(chars) +"\n";
    txt+= "Data size: " + QString::number(m_charWidth*m_charHeight) + " bytes\n";
    txt+= "Color size: " + QString::number(m_charWidth*m_charHeight) + " bytes\n";
    txt+= "Total size: " + QString::number(m_charWidth*m_charHeight*2) + " bytes\n";
    ui->lblInfo->setText(txt);

}
*/
DialogNewImage::~DialogNewImage()
{
    delete ui;
}


void DialogNewImage::slotOk()
{
    //VICImageToData();
/*
    if (ui->comboBox->currentIndex() == 9) {
         m_meta.m_width = m_charWidth;
        m_meta.m_height = m_charHeight;
    }
    if (ui->comboBox->currentIndex()==6) {
//        ui->grpImageSize->setVisible(true);
//        CharImageToData();
        m_meta.m_width = m_charWidth;
       m_meta.m_height = m_charHeight;
 //       exit(1);
    }
*/

    retVal = 0;
    for (int i=0;i<m_types.count();i++)
        if (m_types[i].name == ui->cmbImageType->currentText())
            retVal = i;

}

void DialogNewImage::on_cmbImageType_currentIndexChanged(int index)
{
//    if (m_metaImage!=nullptr)
  //      delete m_metaImage;

    if (index>=m_currentTypes.count() ||index<0)
        return;
    m_metaImage = LImageFactory::Create(m_currentTypes[index].type, m_currentTypes[index].colorType);
//    qDebug() << m_types[index];



    FromMeta();

/*    ui->grpLevelDesignerParams->setVisible(false);
    ui->grpImageSize->setVisible(false);
    if (ui->comboBox->currentText().toLower().contains("level editor"))
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


    ToMeta();*/

}

void DialogNewImage::on_cmbSystem_currentTextChanged(const QString &arg1)
{
    m_currentTypes.clear();
    QStringList data;
    for (ImageType& it: m_types)
        if (it.category==arg1) {
            m_currentTypes.append(it);
            data.append(it.name);
        }

    ui->cmbImageType->clear();
   ui->cmbImageType->addItems(data);
}
