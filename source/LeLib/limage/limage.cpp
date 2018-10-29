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


    return NotSupported;

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


