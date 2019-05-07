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

#include "toolboxitem.h"
#include <QDebug>
#include <QPixmap>
#include "source/LeLib/limage/limagefactory.h"
ToolboxItem::ToolboxItem()
{

}

void ToolboxItem::setSize(float f)
{
    m_size = f;
}


ToolboxItem::ToolboxItem(QString name, QString imagefile)
{
    m_name= name;
    m_imageFilename = imagefile;
    if (m_imageFilename!="") {
        m_image.load(m_imageFilename);
        m_icon.addPixmap( QPixmap::fromImage(m_image) );
    }


}

void ShapeBox::Perform(int x, int y, unsigned char color, LImage* img, bool isPreview, int button)
{
    float m= m_size;
    for (int i=0;i<m;i++)
        for (int j=0;j<m;j++) {
            int d = m/2;
            float xx = i-d;
            float yy = (j-d);
            float l = sqrt(xx*xx*img->m_scaleX + yy*yy);

            bool ok = l<m/2.5;
            if (m_type==1)
                ok = abs(l-m/3)<1;
            if (ok)
            {

                img->setPixel(x+xx,y+yy,color);
            }
        }
}

void ShapeBoxFilter::Perform(int x, int y, unsigned char color, LImage* img, bool isPreview, int button)
{
    float m= m_size;
    for (int i=0;i<m;i++)
        for (int j=0;j<m;j++) {
            int d = m/2;
            float xx = i-d;
            float yy = (j-d);
            float l = sqrt(xx*xx*img->m_scaleX + yy*yy);

            bool ok = l<m/2.5;
            if (m_type==1)
                ok = abs(l-m/3)<0.5;

            if (ok)
            {
                bool regular=true;
    //            if (rand()%100>98)// for (int i=0;i<4;i++)
  //                  qDebug() << img->m_extraCols[2];
                if (img->m_background!=img->m_extraCols[2]) {
//                    qDebug() << img->m_extraCols[2];
                    regular = false;
                }


                if (regular) {
                    if (img->getPixel(x+xx,y+yy)!=img->m_background)
                        img->setPixel(x+xx,y+yy,color);
                }
                else
                    if (img->getPixel(x+xx,y+yy)==img->m_extraCols[2])
                        img->setPixel(x+xx,y+yy,color);
            }
        }
}



void Circle::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{

}

void Spray::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{
    float m= m_size;

    for (int i=0;i<m;i++)
        for (int j=0;j<m;j++) {
            int d = m/2;
            float xx = i-d;
            float yy = (j-d);
            float l = sqrt(xx*xx*img->m_scaleX + yy*yy);
            if (l<m/2.5) {
                if (rand()%100<2)
                    img->setPixel(x+xx,y+yy,color);
            }
        }

}

void Dither::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{
    float m= m_size;
    int shift = m_type;

    x = ((int)x/2)*2;
    y = ((int)y/2)*2;

    for (int i=0;i<m;i++)
        for (int j=0;j<m;j++) {
            int d = m/2;
            float xx = (i-d);
            float yy = (j-d);
            float l = sqrt(xx*xx*img->m_scaleX + yy*yy);
            if (l<m/2.5) {
                int s = (int)(xx + (int)yy%2)%2;
                if (s == 0 ) {
                    img->setPixel(x+xx+shift,y+yy,color);
                }
            }
        }

}

void Filler::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{
//    if (isPreview)
  //      return;
    unsigned char testCol = img->getPixel(x,y);
    if (color == testCol)
        return;
    Fill(x,y,color, testCol, img);
}

void Filler::Fill(int i, int j, unsigned char col, unsigned char testCol, LImage *img)
{
    if (i<0 || j<0 || j>=img->m_height || i>=img->m_width)
        return;
    if (testCol == img->getPixel(i,j)) {
        img->setPixel(i,j, col);
        if (i<img->m_width-1)
            Fill(i+1,j, col, testCol, img);
        if (i>0)
            Fill(i-1,j, col, testCol, img);
        if (j<img->m_height-1)
            Fill(i,j+1, col, testCol, img);
        if (j>0)
            Fill(i,j-1, col, testCol, img);
    }

}

void Line::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{
    if (button==0) {
        m_start = QPoint(x,y);
        return;
    }
    if (m_type==1) {
        if (abs(m_start.x()-x) > abs(m_start.y()-y)) {
            y = m_start.y();
        }
        else x = m_start.x();
    }

    img->drawLine(m_start.x(), m_start.y(), x,y, color, m_size);


}

void CopyStamp::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{

    if (button==1 && m_status == Status::Idle) {
        m_status = Status::Selecting;
        m_start = QPoint(x,y);
       // qDebug() << "Start1";
        if (m_copy==nullptr)
            m_copy = LImageFactory::Create(img->m_type, img->m_colorList.m_type);
        // New from source
        if (m_copy->m_type!=img->m_type) {
            delete m_copy;
            m_copy = LImageFactory::Create(img->m_type, img->m_colorList.m_type);

        }
     //   qDebug() << "Start2";
        m_copy->CopyFrom(img);
    }

    unsigned int frameCol = 1;

    if (m_status == Status::Selecting && button==1) {
        m_end = QPoint(x,y);
        img->drawLine(m_start.x(), m_start.y(), m_start.x(), m_end.y(), frameCol, 1);
        img->drawLine(m_start.x(), m_start.y(), m_end.x(), m_start.y(), frameCol, 1);
        img->drawLine(m_start.x(), m_end.y(), m_end.x(), m_end.y(), frameCol, 1);
        img->drawLine(m_end.x(), m_start.y(), m_end.x(), m_end.y(), frameCol, 1);
    }

    if (m_status== Status::Stamp) {
        StampImage(x,y, img);
    }

    if (button==-1 && m_status==Status::Selecting)
        m_status = Status::Stamp;


}

LImage* CopyStamp::m_copy=nullptr;
QPoint CopyStamp::m_start;
QPoint CopyStamp::m_end;

CopyStamp::Status CopyStamp::m_status;// = Idle;


void CopyStamp::StampImage(int x, int y, LImage* img)
{
    int w = abs(m_end.x()-m_start.x());
    int h = abs(m_end.y()-m_start.y());
    for (int i=0;i<w;i++)
        for (int j=0;j<h;j++) {
            unsigned int col = m_copy->getPixel(m_start.x() + i-0.5, m_start.y()+j-0.5);
            if (col!=img->m_background)
                img->setPixel(i-w/2+x,j-h/2+y, col);
//                for (int xd=0;xd<m_copy->m_scale;xd++)
  //                  img->setPixel(i-w/2.0+x + xd,j-h/2.0+y, col);
        }
}



void RotateAround::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{
    if (button==1 && m_status == Status::Idle) {
        m_status = Status::Down;
        m_start = QPoint(x,y);
       // qDebug() << "Start1";
        if (m_copy==nullptr)
            m_copy = LImageFactory::Create(img->m_type, img->m_colorList.m_type);
        // New from source
        if (m_copy->m_type!=img->m_type) {
            delete m_copy;
            m_copy = LImageFactory::Create(img->m_type, img->m_colorList.m_type);

        }
     //   qDebug() << "Start2";
        m_copy->CopyFrom(img);
    }


    if (m_status == Status::Down && button==1) {
        m_end = QPoint(x,y)-m_start;
        float angle = m_end.x()/10.0f;
        m_copy->Rotate(m_start, angle, 1, img);

    }

    if (m_status == Status::Down && button==0) {
        float angle = m_end.x()/10.0f;
        m_status = Status::Idle;
        m_copy->Rotate(m_start, angle, 1, img);

    }

}
