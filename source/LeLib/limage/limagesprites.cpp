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

#include "limagesprites.h"
#include <QByteArray>
#include "source/LeLib/util/util.h"

LImageSprites::LImageSprites(LColorList::Type t) : CharsetImage(t) {
    m_type = LImage::Type::Sprites;
    m_charCount = 1024;
    m_currentMode = Mode::CHARSET1x1;
    m_currencChar=0;

    m_GUIParams[btnLoadCharset] ="";
    m_GUIParams[btn1x1] = "1x1 Character set";
    m_GUIParams[btn2x2] = "3x2 Character set";
    m_GUIParams[btn2x2repeat] = "";
    m_GUIParams[btnCopy] = "Copy";
    m_GUIParams[btnPaste] = "Paste";
    m_GUIParams[btnFlipH] = "Flip Horizontal";
    m_GUIParams[btnFlipV] = "Flip Vertical";
    m_GUIParams[btnEditFullCharset] = "";

    //Data::data.currentColor=0;

}

void LImageSprites::FromRaw(QByteArray &arr)
{
    Clear();
    int count = m_rawData.count()/64;
    // qDebug() << "Count: " << count;
    int xp = 0;
    int yp = 0;

    //    return;
    for (int i=0;i<count;i++) {
        int pos = i*64;
        int cnt = 0;


        for (int y=0;y<3;y++) {
            for (int z=0;z<8;z++) {
             for (int x=0;x<3;x++) {
                PixelChar& pc = m_data[(xp+x) + (y+yp)*40];
                    int xx = x%8;
                    int yy = y%8;

                    if (cnt<64) {
                       pc.p[z] = PixelChar::reverse(m_rawData[pos + cnt]);
                       pc.p[z] = pc.flipSpriteBit(z);
                    }
                    cnt++;

                }
            }
        }
        xp+=3;
        if (xp>=38) {
            xp=0;
            yp+=3;
        }
    }
    for (int i=0;i<25*40;i++) {
        m_data[i].c[0] = m_background;
        m_data[i].c[1] = m_extraCols[1];
        m_data[i].c[2] = m_extraCols[2];
        m_data[i].c[3] = m_extraCols[3];
    }

}

void LImageSprites::ToRaw(QByteArray &arr)
{
    arr.clear();
    int count = m_rawData.count()/64;
    // qDebug() << "Count: " << count;
    int xp = 0;
    int yp = 0;
    bool done=false;
    int i=0;
    while (!done) {
        int pos = i*64;
        int cnt = 0;

        bool allBlack = true;
        for (int y=0;y<3;y++) {
            for (int z=0;z<8;z++) {
                for (int x=0;x<3;x++) {
                    PixelChar& pc = m_data[(xp+x) + (y+yp)*40];
                    int xx = x%8;
                    int yy = y%8;
                    if (pc.p[z]!=0)
                        allBlack = false;
                    if (cnt<64)
                        arr.append(PixelChar::reverse(pc.flipSpriteBit(z)));
                    cnt++;

                }
            }
        }
        done = allBlack;
        xp+=3;
        if (xp>=38) {
            xp=0;
            yp+=3;
        }
    }
    qDebug() << "Converted : " << (arr.count()/64) << " sprites";
}


QPoint LImageSprites::getXY(int x, int y)
{
    int xx,yy, shiftx, shifty,i,j;

    if (m_currentMode!=CHARSET2x2) {


        shiftx = ((m_currencChar%40)*4)%320;

        shifty = ((int)((m_currencChar-m_currencChar%40)/(int)40))*8;

        i = x/320.0*24;
        j = y/200.0*21;

        //        if (j<0 || j>=24 || i<0 || i>=16)
        //            return QPoint(0,0);
    }
    if (m_currentMode==CHARSET2x2)
    {


        shiftx = ((m_currencChar%40)*4)%320;
        shifty = ((int)((m_currencChar-m_currencChar%40)/(int)40))*8;

        i = x/320.0*24*3;
        j = y/200.0*21*2;



        //        if (j<0 || j>=24 || i<0 || i>=16)
        //            return QPoint(0,0);


    }
    if (m_bitMask==0b1)
        shiftx*=2;
    xx = i+shiftx;
    yy = j+shifty;

    if (m_currentMode==CHARSET2x2)
    if (yy>=21) yy+=3;


    return QPoint(xx,yy);

}


bool LImageSprites::KeyPress(QKeyEvent *e)
{
    QPoint dir(0,0);

    int s = 3;



    if (e->key()==Qt::Key_W && m_currencChar>=40*s)
        m_currencChar-=40*s;
    if (e->key()==Qt::Key_A && m_currencChar>=s)
        m_currencChar-=1*s;
    if (e->key()==Qt::Key_S)
        m_currencChar+=40*s;
    if (e->key()==Qt::Key_D)
        m_currencChar+=1*s;

    m_currencChar = Util::clamp(m_currencChar,0,m_charCount);

    return true;

}
//   qDebug() << "Start2";

void LImageSprites::setPixel(int x, int y, unsigned int color)
{
    QPoint p = getXY(x,y);
    setLimitedPixel(p.x(),p.y(), color);

}

unsigned int LImageSprites::getPixel(int x, int y)
{
    QPoint p = getXY(x,y);
    return MultiColorImage::getPixel(p.x(), p.y());

}

void LImageSprites::FlipVertical()
{
    if (m_currentMode==CHARSET2x2)
        return;


    uint tmp[24*24];
    float i = 160/24.0;
    float j = 200.0/21.0;

    for (int x=0;x<24;x++)
        for (int y=0;y<21;y++) {
            tmp[24*y + x]=getPixel(x*i,y*j+8);
        }
    for (int y=0;y<21;y++)
      for (int x=0;x<23;x++)
            setPixel( x*i+8 ,y*j+8, tmp[24*y + 23-x]);


}

void LImageSprites::FlipHorizontal()
{
    if (m_currentMode==CHARSET2x2)
        return;


    uint tmp[24*24];
    float i = 160/24.0;
    float j = 200.0/21.0;

    for (int x=0;x<24;x++)
        for (int y=0;y<21;y++) {
            tmp[24*y + x]=getPixel(x*i,y*j);
        }
    for (int y=0;y<21;y++)
      for (int x=0;x<24;x++)
            setPixel( x*i ,y*j+8, tmp[24*(20-y) +x]);


}

void LImageSprites::CopyPasteChar(int type)
{
    //if (m_currentMode==CHARSET1x1)
    int max=1;
    if (m_currentMode==CHARSET2x2) max = 6;
    int x=0;
    int k0 = 0;
    int k=0;

    for (int kk=0;kk<max;kk++)
    {
       int j=0;
        int add=k;
        for (int i=0;i<9;i++) {
            if (type==0)
            m_copy[x]=m_data[m_currencChar+add];
            if (type==1)
            m_data[m_currencChar+add]=m_copy[x];

            x++;
            j++;
            if (j<=2) {
                add++;
            }
            else {
                j=0;
                add=add+38;
            }
        }
        k0=k0+1;
        k=k+3;
        if (k0==3) {
            k0=0;
            k=40*3;
        }

    }

}


void LImageSprites::CopyChar()
{
    CopyPasteChar(0);
}

void LImageSprites::PasteChar()
{
    CopyPasteChar(1);

}
