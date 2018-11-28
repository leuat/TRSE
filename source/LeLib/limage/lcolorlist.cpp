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

#include "lcolorlist.h"
#include <QDebug>
#include <QPalette>
#include <QSignalMapper>
#include "source/LeLib/util/util.h"
LColorList::LColorList()
{
}


LColorList::~LColorList()
{
    if (m_metric)
        delete m_metric;
}

unsigned char LColorList::TypeToChar(LColorList::Type t)
{
  if (t==C64)
      return 0;
  if (t==C64_ORG)
      return 1;
  if (t==CGA1_LOW)
      return 2;
  if (t==CGA1_HIGH)
      return 3;
  if (t==CGA2_LOW)
      return 4;
  if (t==CGA2_HIGH)
      return 5;
  if (t==VIC20)
      return 6;

  return 255;
}

LColorList::Type LColorList::CharToType(unsigned char c)
{
    if (c==0)
        return C64;
    if (c==1)
        return C64_ORG;
    if (c==2)
        return CGA1_LOW;
    if (c==3)
        return CGA1_HIGH;
    if (c==4)
        return CGA2_LOW;
    if (c==5)
        return CGA2_HIGH;
    if (c==6)
        return VIC20;

    return UNSUPPORTED;

}

void LColorList::EnableColors(QVector<int> &cols)
{
    for (int i=0;i<m_list.count();i++) {
        if (cols.contains(i))
            m_list[i].inUse = true;
        else
            m_list[i].inUse = false;

    }

}

void LColorList::Initialize(Type t)
{
    m_type = t;


    if (m_type == Type::C64)
        InitC64();
    if (m_type == Type::C64_ORG)
        InitC64_org();
    if (m_type == Type::CGA1_HIGH)
        InitCGA1_HIGH();
    if (m_type == Type::CGA1_LOW)
        InitCGA1_LOW();
    if (m_type == Type::CGA2_HIGH)
        InitCGA2_HIGH();
    if (m_type == Type::CGA2_LOW)
        InitCGA2_LOW();
    if (m_type == Type::TIFF)
        InitC64();
    if (m_type == Type::VIC20)
        InitVIC20();



    m_metric = new LinearMetric();

}

void LColorList::InitC64_org()
{
    m_list.clear();
    m_list.append(LColor(QColor(0,0,0),""));
    m_list.append(LColor(QColor(255,255,255),""));
    m_list.append(LColor(QColor(136,0,0),""));
    m_list.append(LColor(QColor(170,255,238),""));
    m_list.append(LColor(QColor(204,68,204),""));
    m_list.append(LColor(QColor(0,204,85),""));
    m_list.append(LColor(QColor(0,0,170),""));
    m_list.append(LColor(QColor(238,238,119),""));
    m_list.append(LColor(QColor(221,136,85),""));
    m_list.append(LColor(QColor(102,68,0),""));
    m_list.append(LColor(QColor(255,119,119),""));
    m_list.append(LColor(QColor(51,51,51),""));
    m_list.append(LColor(QColor(119,119,119),""));
    m_list.append(LColor(QColor(170,255,102),""));
    m_list.append(LColor(QColor(0,136,255),""));
    m_list.append(LColor(QColor(187,187,198),""));

}

void LColorList::InitC64()
{
    m_list.clear();
    m_list.append(LColor(QColor(0x0, 0x0, 0x0),""));
    m_list.append(LColor(QColor(0xFF, 0xFF, 0xFF),""));
    m_list.append(LColor(QColor(0x68, 0x37, 0x2b),""));
    m_list.append(LColor(QColor(0x70, 0xa4, 0xb2),""));
    m_list.append(LColor(QColor(0x6f, 0x3d, 0x86),""));
    m_list.append(LColor(QColor(0x58, 0x8d, 0x43),""));
    m_list.append(LColor(QColor(0x35, 0x28, 0x79),""));
    m_list.append(LColor(QColor(0xb8, 0xc7, 0x6F),""));
    m_list.append(LColor(QColor(0x6f, 0x4f, 0x25),""));
    m_list.append(LColor(QColor(0x43, 0x39, 0x00),""));
    m_list.append(LColor(QColor(0x9a, 0x67, 0x59),""));
    m_list.append(LColor(QColor(0x44, 0x44, 0x44),""));
    m_list.append(LColor(QColor(0x6c, 0x6c, 0x6c),""));
    m_list.append(LColor(QColor(0x9a, 0xd2, 0x84),""));
    m_list.append(LColor(QColor(0x6c, 0x5e, 0xb5),""));
    m_list.append(LColor(QColor(0x95, 0x95, 0x95),""));

    m_background = m_list[0];

}

void LColorList::InitVIC20()
{
    m_list.clear();


    m_list.append(LColor(QColor(0x0, 0x0, 0x0),""));
    m_list.append(LColor(QColor(0xFF, 0xFF, 0xFF),""));
    m_list.append(LColor(QColor(0x68, 0x37, 0x2b),""));
    m_list.append(LColor(QColor(0x70, 0xa4, 0xb2),""));
    m_list.append(LColor(QColor(0x6f, 0x3d, 0x86),""));
    m_list.append(LColor(QColor(0x58, 0x8d, 0x43),""));
    m_list.append(LColor(QColor(0x35, 0x28, 0x79),""));
    m_list.append(LColor(QColor(0xb8, 0xc7, 0x6F),""));

/*    m_list.append(LColor(QColor(0x0, 0x0, 0x0),""));
    m_list.append(LColor(QColor(0xFF, 0xFF, 0xFF),""));
    m_list.append(LColor(QColor(0x68, 0x37, 0x2b),""));
    m_list.append(LColor(QColor(0x70, 0xa4, 0xb2),""));
    m_list.append(LColor(QColor(0x6f, 0x3d, 0x86),""));
    m_list.append(LColor(QColor(0x58, 0x8d, 0x43),""));
    m_list.append(LColor(QColor(0x35, 0x28, 0x79),""));
    m_list.append(LColor(QColor(0xb8, 0xc7, 0x6F),""));
*/






 /*   m_list.append(LColor(QColor(0x0, 0x0, 0x0),""));
    m_list.append(LColor(QColor(0xFF, 0xFF, 0xFF),""));
    m_list.append(LColor(QColor(103, 55, 43),""));
    m_list.append(LColor(QColor(112,165 ,177 ),""));
    m_list.append(LColor(QColor(111, 61, 133),""));
    m_list.append(LColor(QColor(88, 140, 67),""));
    m_list.append(LColor(QColor(52, 40, 121),""));
    m_list.append(LColor(QColor(183, 198,110 ),""));
    m_list.append(LColor(QColor(111, 79, 37),""));
    m_list.append(LColor(QColor(66, 57, 0),""));
    m_list.append(LColor(QColor(153, 102, 89),""));
    m_list.append(LColor(QColor(68, 68, 68),""));
    m_list.append(LColor(QColor(108, 108, 108),""));
    m_list.append(LColor(QColor(154, 209, 131),""));
    m_list.append(LColor(QColor(107, 94, 181),""));
    m_list.append(LColor(QColor(150,150 ,150 ),""));*/
    m_background = m_list[0];

}

void LColorList::InitCGA1_LOW()
{
    m_list.clear();
    m_list.append(LColor(QColor(0,0,0),"Black"));
    m_list.append(LColor(QColor(0,0xaa,0xaa),"Cyan"));
    m_list.append(LColor(QColor(0xaa,0x00,0xaa),"Magenta"));
    m_list.append(LColor(QColor(0xaa,0xaa,0xaa),"Light gray"));
}
void LColorList::InitCGA1_HIGH()
{
    m_list.clear();
    m_list.append(LColor(QColor(0,0,0),"Black"));
    m_list.append(LColor(QColor(0x55,0xff,0xff),"Cyan"));
    m_list.append(LColor(QColor(0xff,0x55,0xff),"Magenta"));
    m_list.append(LColor(QColor(0xff,0xff,0xff),"Light gray"));
}

void LColorList::InitCGA2_LOW()
{
    m_list.clear();
    m_list.append(LColor(QColor(0,0,0),"Black"));
    m_list.append(LColor(QColor(0,0xaa,0x00),"Green"));
    m_list.append(LColor(QColor(0xaa,0x00,0x00),"Red"));
    m_list.append(LColor(QColor(0xaa,0x55,0x00),"Brown"));
}
void LColorList::InitCGA2_HIGH()
{
    m_list.clear();
    m_list.append(LColor(QColor(0,0,0),"Black"));
    m_list.append(LColor(QColor(0x55,0xff,0x55),"Green"));
    m_list.append(LColor(QColor(0xff,0x55,0x55),"Red"));
    m_list.append(LColor(QColor(0xff,0xff,0x55),"Brown"));
}

QColor LColorList::getClosestColor(QColor col, int& winner)
{
    float d = 1E20;
    int i = 0;
//    qDebug() << "WHOO";
    for (int i=0;i<m_list.count();i++) {
        //qDebug() << "Metric:";
        if (!m_list[i].inUse)
            continue;
        float v = m_metric->getDistance(m_list[i].color, col);
        //qDebug() << "end:";
        if (v<d) {
            d = v;
            winner = i;
        }
    }
/*    if (rand()%500==0) {
        qDebug() << "Testing for: " << col.red() << ", " << col.green() << ", " << col.blue();
        qDebug() << "Winner: " <<m_list[winner].color.red() << ", " <<m_list[winner].color.green() << ", " << m_list[winner].color.blue();
    }*/
    return m_list[winner].color;

}

void LColorList::FillComboBox(QComboBox *cmb)
{
    cmb->clear();
    for (int i=0;i<m_list.count();i++) {
        QPixmap pixmap(16,16);
        pixmap.fill(m_list[i].color);
        QIcon icon(pixmap);
        cmb->addItem(icon,"");
    }
}

int LColorList::getIndex(QColor c)
{
    for (int i=0;i<m_list.count();i++) {
        if (m_list[i].color == c)
            return i;
    }
    return 0;
}

void LColorList::CreateUI(QLayout* ly, int type)
{

    Util::clearLayout(ly, true);

    m_buttonsEdit.clear();
    m_buttonsImport.clear();
//    m_buttons.clear();
    for(int j=0; j<m_list.count(); j++)
    {
        QPushButton *b = new QPushButton();
        //b->setGeometry(0,0,40,40);
        QPalette p;
        p.setColor(QPalette::Button, m_list[j].color);
        p.setColor(QPalette::Window, m_list[j].color);
        QString txtCol = QString::number(m_list[j].color.red()) + ", ";
        txtCol += QString::number(m_list[j].color.green()) + ", ";
        txtCol += QString::number(m_list[j].color.blue());

        b->setStyleSheet("background-color: rgb("+txtCol + "); color: rgb(0, 0, 0)");
        b->setPalette(p);
        b->setMaximumWidth(40);
        b->setMinimumWidth(40);
        b->setAutoFillBackground( true );
        if (type==0) {
            QObject::connect( b, &QPushButton::clicked,  [=](){ handleButtonImport(j);} );
        }
        if (type==1) {
            QObject::connect( b, &QPushButton::clicked,  [=](){ handleButtonEdit(j); } );
        }
        //QObject::connect( b, &QPushButton::clicked,  colorValueChanged );


        ly->addWidget(b);
        if (type==0)
            m_buttonsImport.append(b);
    }
}

void LColorList::handleButtonEdit(int data)
{
    Data::data.currentColor = data;
    Data::data.currentIsColor=true;

}

void LColorList::handleButtonImport(int data)
 {
    //qDebug() << data;
    m_list[data].inUse=!m_list[data].inUse;
    if (m_list[data].inUse)
        m_buttonsImport[data]->setText(" ");
    else {
        m_buttonsImport[data]->setText("X ");
   }

    //Data::data.redrawInput = true;
    emit colorValueChanged();
 }
