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

#include "limage.h"
#include <algorithm>

#include "source/LeLib/util/util.h"
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QMatrix4x4>

LImage::LImage(LColorList::Type t)
{
    m_colorList.Initialize(t);
}

unsigned char LImage::TypeToChar(LImage::Type t)
{
    if (t==QImageBitmap)
        return 0;
    if (t==MultiColorBitmap)
        return 1;
    if (t==HiresBitmap)
        return 2;
    if (t==LevelEditor)
        return 3;
    if (t==CharMapMulticolor)
        return 4;
    if (t==Sprites)
        return 5;
    if (t==CharmapRegular)
        return 6;
    if (t==FullScreenChar)
        return 7;
    if (t==CharMapMultiColorFixed)
        return 8;
    if (t==VIC20_MultiColorbitmap)
        return 9;
    if (t==Sprites2)
        return 10;
    if (t==CGA)
        return 11;
    if (t==AMIGA320x200)
        return 12;
    if (t==AMIGA320x256)
        return 13;
    if (t==OK64_256x256)
        return 14;
    if (t==X16_640x480)
        return 15;
    if (t==NES)
        return 16;


    return 255;
}

LImage::Type LImage::CharToType(unsigned char c)
{
    if (c==0)
        return QImageBitmap;
    if (c==1)
        return MultiColorBitmap;
    if (c==2)
        return HiresBitmap;
    if (c==3)
        return LevelEditor;
    if (c==4)
        return CharMapMulticolor;
    if (c==5)
        return Sprites;
    if (c==6)
        return CharmapRegular;
    if (c==7)
        return FullScreenChar;
    if (c==8)
        return CharMapMultiColorFixed;
    if (c==9)
        return VIC20_MultiColorbitmap;
    if (c==10)
        return Sprites2;
    if (c==11)
        return CGA;
    if (c==12)
        return AMIGA320x200;
    if (c==13)
        return AMIGA320x256;
    if (c==14)
        return OK64_256x256;
    if (c==15)
        return X16_640x480;
    if (c==16)
        return NES;

    return NotSupported;

}

void LImage::FloydSteinbergDither(QImage &img, LColorList &colors, bool dither)
{
    int height  =min(img.height(), m_height);
    int width  =min(img.width(), m_width);
    for (int y=0;y<height;y++) {
        for (int x=0;x<width;x++) {
            QColor oldPixel = QColor(img.pixel(x,y));
            int winner = 0;
            QColor newPixel = colors.getClosestColor(oldPixel, winner);
            //int c = m_colorList.getIndex(newPixel);
            setPixel(x,y,winner);
            QVector3D qErr(oldPixel.red()-newPixel.red(),oldPixel.green()-newPixel.green(),oldPixel.blue()-newPixel.blue());
            if (dither) {
                if (x!=width-1)
                    img.setPixel(x+1,y,Util::toColor(Util::fromColor(img.pixel(x+1,y))+qErr*7/16.0).rgba());
                if (y!=height-1) {
                    if (x!=0)
                        img.setPixel(x-1,y+1,Util::toColor(Util::fromColor(img.pixel(x-1,y+1))+qErr*3/16.0).rgba());
                    img.setPixel(x,y+1,Util::toColor(Util::fromColor(img.pixel(x,y+1))+qErr*5/16.0).rgba());
                    if (x!=width-1)
                        img.setPixel(x+1,y+1,Util::toColor(Util::fromColor(img.pixel(x+1,y+1))+qErr*1/16.0).rgba());
                }
            }
        }
    }

}

void LImage::OrdererdDither(QImage &img, LColorList &colors, QVector3D strength)
{
    int height  =min(img.height(), m_height);
    int width  =min(img.width(), m_width);
    QMatrix4x4 bayer4x4 = QMatrix4x4(0,8,2,10,  12,4,14,6, 3,11,1,9, 15,7,13,5);
    bayer4x4 = bayer4x4*1/16.0*strength.x();
    for (int y=0;y<height;y++) {
        for (int x=0;x<width;x++) {

//            color.R = color.R + bayer8x8[x % 8, y % 8] * GAP / 65;

            int xx = (x-img.width()/2)*m_importScaleX + img.width()/2;
            int yy = (y-img.height()/2)*m_importScaleY + img.height()/2;


            QColor color = QColor(img.pixel(xx,yy));
            int yp = yy + xx%(int)strength.y();
            int xp = xx + yy%(int)strength.z();
            color.setRed(min(color.red() + bayer4x4(xp % 4,yp % 4),255.0f));
            color.setGreen(min(color.green() + bayer4x4(xp % 4,yp % 4),255.0f));
            color.setBlue(min(color.blue() + bayer4x4(xp % 4,yp % 4),255.0f));

            int winner = 0;
            QColor newPixel = colors.getClosestColor(color, winner);
            //int c = m_colorList.getIndex(newPixel);
            setPixel(x,y,winner);

        }
    }


}

void LImage::Rotate(QPoint center, float angle, float scale, LImage* img)
{
    for (int i=0;i<m_width;i++)
        for (int j=0;j<m_height;j++) {
            float xx = (i - center.x())/scale;
            float yy = (j - center.y())/scale/2;

            float xr = xx*cos(angle)-yy*sin(angle) + center.x();
            float yr = (yy*cos(angle)+xx*sin(angle))*2 + center.y();
            img->setPixel(i,j,getPixel(xr,yr));

        }
}





void LImage::drawLine(float x0, float y0, float x1, float y1, unsigned int col, int size)
{
        float x{x1 - x0}, y{y1 - y0};
        const float max{std::max(std::fabs(x), std::fabs(y))};
        x /= max; y /= max;
        for (float n{0}; n < max; ++n)
        {
            // draw pixel at ( x0, y0 )
            Box(x0,y0, col, size);
            x0 += x; y0 += y;
        }
}

void LImage::Box(int x, int y, unsigned char col, int size)
{
    for (int i=0;i<size;i++)
        for (int j=0;j<size;j++) {
            setPixel(x+i-size/2, y+j-size/2, col);
        }
}


void LImage::CopyFrom(LImage *img) {
#pragma omp parallel for

    for (int i=0;i<m_width;i++)
        for (int j=0;j<m_height;j++)
            setPixel(i,j,img->getPixel(i,j));

/*    if (img->m_colorList.m_list.count()==m_colorList.m_list.count()) {
        for (int i=0;i<m_colorList.m_list.count();i++)
            m_colorList.m_list[i].color = img->m_colorList.m_list[i].color;

        qDebug() << "LImage copyfrom  count : " <<img->m_colorList.m_list.count();
    }
    */
}

