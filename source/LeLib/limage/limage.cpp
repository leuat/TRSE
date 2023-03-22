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
#include "source/Compiler/syntax.h"
//#include <omp.h>
uchar LImage::m_copy[1024*1024];
bool LImage::m_hasCopy = false;
QPoint LImage::m_copySize = QPoint(512,512);
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
    if (t==LMetaChunk)
        return 17;
    if (t==LevelEditorNES)
        return 18;
    if (t==SpritesNES)
        return 19;
    if (t==GAMEBOY)
        return 20;
    if (t==LevelEditorGameboy)
        return 21;
    if (t==ATARI320x200)
        return 22;
    if (t==HybridCharset)
        return 23;
    if (t==AmstradCPC)
        return 24;
    if (t==AmstradCPCGeneric)
        return 25;
    if (t==BBC)
        return 26;
    if (t==VGA)
        return 27;
    if (t==Spectrum)
        return 28;
    if (t==SNES)
        return 29;
    if (t==LevelEditorSNES)
        return 30;
    if (t==VZ200)
        return 31;
    if (t==CustomC64)
        return 32;
    if (t==JDH8)
        return 33;
    if (t==LImageGeneric)
        return 34;
    if (t==GenericSprites)
        return 35;
    if (t==CGA160x100)
        return 36;
    if (t==AmstradSprites)
        return 37;
    if (t==SNESGeneric)
        return 38;
    if (t==TIM)
        return 39;
    if (t==TVC)
        return 40;
    if (t==COCO3)
        return 41;
    if (t==THOMSON)
        return 42;
    if (t==TIMG)
        return 43;
    if (t==LevelEditorGeneric)
        return 44;


    return 255;
}

QString LImage::TypeToString(LImage::Type t)
{
    if (t==QImageBitmap)
        return "Bitmap image";
    if (t==MultiColorBitmap)
        return "C64 Image";
    if (t==HiresBitmap)
        return "C64 Image";
    if (t==LevelEditor)
        return "Level editor";
    if (t==CharMapMulticolor)
        return "Character map";
    if (t==Sprites)
        return "Sprites";
    if (t==CharmapRegular)
        return "Character map";
    if (t==FullScreenChar)
        return "Screen Animation";
    if (t==CharMapMultiColorFixed)
        return "Character map";
    if (t==VIC20_MultiColorbitmap)
        return "VIC Image";
    if (t==Sprites2)
        return "Sprites";
    if (t==CGA)
        return "CGA Image";
    if (t==AMIGA320x200)
        return "Amiga image";
    if (t==AMIGA320x256)
        return "Amiga image";
    if (t==OK64_256x256)
        return "OK64 image";
    if (t==X16_640x480)
        return "X16 image";
    if (t==NES)
        return "NES character map";
    if (t==LMetaChunk)
        return "Meta tiles";
    if (t==LevelEditorNES)
        return "NES level editor";
    if (t==SpritesNES)
        return "NES sprites";
    if (t==GAMEBOY)
        return "GAMEBOY";
    if (t==LevelEditorGameboy)
        return "Gameboy level editor";
    if (t==ATARI320x200)
        return "Atari ST 320x200";
    if (t==HybridCharset)
        return "Hybrid multicolor/hires";
    if (t==AmstradCPC)
        return "Amstrad CPC";
    if (t==AmstradCPCGeneric)
        return "Amstrad Generic NxM";
    if (t==BBC)
        return "BBC";
    if (t==VGA)
        return "VGA";
    if (t==Spectrum)
        return "Spectrum";
    if (t==SNES)
        return "SNES";
    if (t==LevelEditorSNES)
        return "SNES Level editor";
    if (t==VZ200)
        return "VZ200 4-color 128x64";
    if (t==CustomC64)
        return "Custom C64";
    if (t==JDH8)
        return "JDH8";
    if (t==LImageGeneric)
        return "Generic indexed image";
    if (t==GenericSprites)
        return "Generic Sprites";
    if (t==CGA160x100)
        return "CGA 160x100 16 colour";
    if (t==AmstradSprites)
        return "Amstrad sprites";
    if (t==SNESGeneric)
        return "Generic SNES image";
    if (t==TIM)
        return "Generic TIM image";
    if (t==TVC)
        return "Generic TVC image";
    if (t==COCO3)
        return "Generic TRS80 CoCo3 image";
    if (t==THOMSON)
        return "Generic Thomson mo5 image";
    if (t==TIMG)
        return "Generic TIM image";
    if (t==LevelEditorGeneric)
        return "Generic Level Editor";


    return "Unknown image type";


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
    if (c==17)
        return LMetaChunk;
    if (c==18)
        return LevelEditorNES;
    if (c==19)
        return SpritesNES;
    if (c==20)
        return GAMEBOY;
    if (c==21)
        return LevelEditorGameboy;
    if (c==22)
        return ATARI320x200;
    if (c==23)
        return HybridCharset;
    if (c==24)
        return AmstradCPC;
    if (c==25)
        return AmstradCPCGeneric;
    if (c==26)
        return BBC;
    if (c==27)
        return VGA;
    if (c==28)
        return Spectrum;
    if (c==29)
        return SNES;
    if (c==30)
        return LevelEditorSNES;
    if (c==31)
        return VZ200;
    if (c==32)
        return CustomC64;
    if (c==33)
        return JDH8;
    if (c==34)
        return LImageGeneric;
    if (c==35)
        return GenericSprites;
    if (c==36)
        return CGA160x100;
    if (c==37)
        return AmstradSprites;
    if (c==38)
        return SNESGeneric;
    if (c==39)
        return TIM;
    if (c==40)
        return TVC;
    if (c==41)
        return COCO3;
    if (c==42)
        return THOMSON;
    if (c==43)
        return TIMG;
    if (c==44)
        return LevelEditorGeneric;

    return NotSupported;

}

MetaParameter *LImage::getMetaParameter(QString name)
{
    for (MetaParameter* mp: m_metaParams)
        if (mp->name==name)
            return mp;

    return nullptr;
}

void LImage::CopyImageData(LImage *img) {
    qDebug() << "LImage::CopyImageData should not be called on pure virtual class!";
}

int LImage::getCharWidthDisplay()
{
    return m_charWidthDisplay;
}

int LImage::getCharHeightDisplay()
{
    return m_charHeightDisplay;
}

QStringList LImage::getBankNames() {
    return QStringList()<<"Bank 0";
}

void LImage::CtrlLeftShift(int x, int y) {
    Data::data.currentColor = getPixel(x,y);
}


QString LImage::GetCurrentModeString() {
    /*    if (m_currentMode==CHARSET1x1) return "1x1 charset mode";
    if (m_currentMode==CHARSET2x2) return "2x2 charset mode";
    if (m_currentMode==CHARSET2x2_REPEAT) return "2x2 charset repeat mode";
*/
    return "";
}

void LImage::FloydSteinbergDither(QImage &oimg, LColorList &colors, bool dither, double strength)
{

/*    int xx = (dx-img.width()/2.0)*m_importScaleX + img.width()/2.0;
    int yy = (dy-img.height()/2.0)*m_importScaleY + img.height()/2.0;

    if (m_importScaleX==1.0 && m_importScaleY==1.0) { // prevent rounding errors on windows.. damn
       xx = dx;
       yy = dy;
    }

  */
    QImage img = oimg.scaled(oimg.width()*m_importScaleX,oimg.height()*m_importScaleY,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    int height  =std::min(img.height(), m_height);
    int width  =std::min(img.width(), m_width);
    for (int y=0;y<height;y++) {
        for (int x=0;x<width;x++) {


            QColor oldPixel = QColor(img.pixel(x,y));
            int winner = 0;
            QColor newPixel = colors.getClosestColor(oldPixel, winner);
            //int c = m_colorList.getIndex(newPixel);
            setPixel(x,y,winner);
            QVector3D qErr(oldPixel.red()-newPixel.red(),oldPixel.green()-newPixel.green(),oldPixel.blue()-newPixel.blue());
            qErr*=strength/100.0;
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

void LImage::OrdererdDither(QImage &img, LColorList &colors, QVector3D strength, QPoint size, float gamma=1.0)
{
    int height  =std::min(img.height(), m_height);
    int width  =std::min(img.width(), m_width);
    QMatrix4x4 bayer4x4 = QMatrix4x4(0,8,2,10,  12,4,14,6, 3,11,1,9, 15,7,13,5);
    bayer4x4 = bayer4x4*1/16.0*strength.x();
//    qDebug() << img.width();
    qDebug() << m_importScaleX << img.width() << width;
    for (int y=0;y<height;y++) {
//#pragma omp parallel for
        for (int x=0;x<width;x++) {

//            color.R = color.R + bayer8x8[x % 8, y % 8] * GAP / 65;
            double dx = x/(double)width*img.width();
            double dy = y/(double)height*img.height();
            int xx = (dx-img.width()/2.0)*m_importScaleX + img.width()/2.0;
            int yy = (dy-img.height()/2.0)*m_importScaleY + img.height()/2.0;

            if (m_importScaleX==1.0 && m_importScaleY==1.0) { // prevent rounding errors on windows.. damn
               xx = dx;
               yy = dy;
            }


            QColor color = QColor(img.pixel(xx,yy));
            int yp = y + x%(int)strength.y();
            int xp = x + y%(int)strength.z();
            color.setRed(std::min((float)pow(color.red(),gamma) + bayer4x4(xp % size.x(),yp % size.y()),255.0f));
            color.setGreen(std::min((float)pow(color.green(),gamma) + bayer4x4(xp % size.x(),yp % size.y()),255.0f));
            color.setBlue(std::min((float)pow(color.blue(),gamma) + bayer4x4(xp % size.x(),yp % size.y()),255.0f));

            int winner = 0;
            QColor newPixel = colors.getClosestColor(color, winner);
            //int c = m_colorList.getIndex(newPixel);

            setPixel(x,y,winner);

        }
    }


}



void LImage::InitPens() {

    m_colorList.DefaultPen(LPen::FixedSingle);
}

void LImage::CopyChar()
{

    //   m_copySize = 1;
    /*    if (m_footer.isFullscreen()) {
        m_copySize = QPoint(m_width,m_height);
    }
    else {
        m_copySize = QPoint(m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_X)*8,
                            m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_Y)*8);
    }
*/
//   m_copySize = QPoint(m_width,m_height);
//    qDebug() << m_copySize << m_width << m_height;
   m_copySize = QPoint(m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_X)*32,
                       m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_Y)*32);

//   m_copySize = QPoint(256,256);
/*
   m_isHybridTemp = false;
   bool multi = m_bitMask==0b11;
   if (m_footer.get(LImageFooter::POS_DISPLAY_HYBRID)==1)
   {
       m_footer.set(LImageFooter::POS_DISPLAY_HYBRID,0);
       setMultiColor(false);
       m_isHybridTemp = true;
   }
*/
    for (int y=0;y<m_copySize.y();y++)
        for (int x=0;x<m_copySize.x();x++) {
            m_copy[x+y*m_copySize.x()] = getPixel((float)x/(float)m_copySize.x()*(float)m_width,
                                                  (float)y/(float)m_copySize.y()*(float)m_height);
//            if (m_copy[x+y*m_copySize.x()]>8)
  //          qDebug() << QString::number(m_copy[x+y*m_copySize.x()]);
        }
    m_hasCopy = true;

  /*  if (m_isHybridTemp) {
       m_footer.set(LImageFooter::POS_DISPLAY_MULTICOLOR,0);
       setMultiColor(false);
       m_footer.set(LImageFooter::POS_DISPLAY_HYBRID,1);
   }
   m_isHybridTemp = false;
*/


}

void LImage::SetGridSize(QString size)
{
    QStringList data = size.split("x"); // should be of the format WxH
    m_footer.set(LImageFooter::POS_GRID_SCALE_X,data[0].toInt());
    m_footer.set(LImageFooter::POS_GRID_SCALE_Y,data[1].toInt());
}

QString LImage::getGridSize()
{
    return QString::number(m_footer.get(LImageFooter::POS_GRID_SCALE_X))+"x"+QString::number(m_footer.get(LImageFooter::POS_GRID_SCALE_Y));
}

void LImage::PasteChar()
{
    if (!m_hasCopy)
        return;
    for (int y=0;y<m_copySize.y();y++)
        for (int x=0;x<m_copySize.x();x++) {
            setPixel(x/(float)m_copySize.x()*m_width,
                     y/(float)m_copySize.y()*m_height,
                     m_copy[x+y*m_copySize.x()]);
        }
}

void LImage::FlipHorizontal() {
    PushHybrid();
    CopyChar();
    for (int y=0;y<m_copySize.y();y++)
        for (int x=m_copySize.x()-1;x>=0;x--) {
                     setPixel(x*m_width/m_copySize.x(),
                              y*m_height/m_copySize.y(),
                     m_copy[m_copySize.x()-1-x+y*m_copySize.x()]);
        }

    PopHybrid();
}

void LImage::FlipVertical() {
    PushHybrid();
    CopyChar();
    for (int y=0;y<m_copySize.y();y++)
        for (int x=0;x<m_copySize.x();x++) {
            setPixel((float)(x*m_width)/(float)m_copySize.x(),
                     ((float)(m_copySize.y()-1-y)*m_height/(float)m_copySize.y()),
                     m_copy[x+y*m_copySize.x()]);
        }


    PopHybrid();
}

void LImage::Transform(int tx, int ty) {
    PushHybrid();

    CopyChar();
    for (int y=0;y<m_copySize.y();y++)
        for (int x=0;x<m_copySize.x();x++) {
            setPixel((float)(x*m_width)/(float)m_copySize.x(),
                     (float)(y*m_height)/(float)m_copySize.y(),
                     m_copy[(x+tx)%m_width+((y+ty)%m_height)*m_copySize.x()]);
        }

    PopHybrid();
}

void LImage::Clear(int val) {
    for (int y=0;y<m_height;y++)
        for (int x=0;x<m_width;x++) {
            setPixel(x,y,val);
        }

}
void LImage::PushHybrid()
{

    m_isHybridTemp = false;
    if (m_footer.get(LImageFooter::POS_DISPLAY_HYBRID)==1) {
        m_footer.set(LImageFooter::POS_DISPLAY_HYBRID,0);
        setMultiColor(false);
        m_isHybridTemp = true;
    }

}

void LImage::PopHybrid()
{
    if (m_isHybridTemp) {
        setMultiColor(false);
        m_footer.set(LImageFooter::POS_DISPLAY_HYBRID,1);
    }
    m_isHybridTemp = false;

}


void LImage::ShiftXY(int dx, int dy)
{
   // qDebug() <<m_copySize;
    PushHybrid();
    CopyChar();

/*    if (m_footer.get(LImageFooter::POS_DISPLAY_HYBRID)==1) {
        dx*=2;
    }*/
    if (m_isHybridTemp) {
        dx*=2;
    }

    dx*=m_bitMask==0b11?2:1;

    if (!m_footer.isFullscreen()) {
        int sx = m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_X)*8;
        int sy = m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_Y)*8;

  //      if (dx!=0)
            dx = dx*(float)m_width/(float)sx;
            if (dx>0) dx+=1;
            dy = dy*(float)m_height/(float)sy;
            if (dy>0) dy+=1;

    }
//    qDebug() << dx << dy << m_copySize << m_width << m_height;
    for (int y=0;y<m_copySize.y();y++)
        for (int x=0;x<m_copySize.x();x++) {
            setPixel((int)((x/(float)m_copySize.x()*(float)m_width)+dx+m_width)%m_width ,
                     (int)(((y)/(float)m_copySize.y()*(float)m_height)+dy+m_height)%m_height,

                     m_copy[x+y*m_copySize.x()]);
        }
    PopHybrid();

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

void LImage::ExportSubregion(QString outfile, int x, int y, int w, int h, int type) {

    QByteArray data;
//    qDebug() << x<<type;
    if (type==0)
        for (int j=0;j<h;j++)
            for (int i=0;i<w;i++)
            {
                data.append(getPixel(x+i,y+j));
                data.append(getPixel(x+i,y+j));
            }

    if (type == 1)
    for (int i=0;i<w;i++)
        for (int j=0;j<h;j++)
        {
            data.append(getPixel(x+i,y+j));
        }

    Util::SaveByteArray(data,outfile);



}

void LImage::ExportRGB8Palette(QString filename) {

    QByteArray b;
    m_colorList.toArray(b);
    b.remove(0,1);
    Util::SaveByteArray(b,filename);

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

void LImage::drawCircle(float x0, float y0, float r, float r0, unsigned int col)
{
    int size = r;
    float as = m_height/(float)m_width;
    int sizex = r/as;
    for (int i=0;i<sizex;i++)
        for (int j=0;j<size;j++) {
            int dx = i-sizex/2;
            int dy = j-size/2;
            int d = sqrt( 4*as*dx*dx +4*dy*dy );
            if (d<r && d>r0)
                setPixel(x0+dx, y0+dy, col);
        }
}

void LImage::Box(int x, int y, unsigned char col, int size)
{

    int sx = fmax(m_footer.get(LImageFooter::POS_CURRENT_STAMP_X),1);
    int sy = fmax(m_footer.get(LImageFooter::POS_CURRENT_STAMP_X),1);
    int w = fmax(m_footer.get(LImageFooter::LImageFooter::POS_CHARSET_WIDTH),1);
    for (int i=0;i<size;i++)
        for (int j=0;j<size;j++) {
            setPixel(x+i-size/2, y+j-size/2, col+i%sx + (j%sy)*w);
        }
}

void LImage::RBox(int x0, int y0, int x1, int y1, unsigned char col, int size)
{
    if (x0>x1) std::swap(x0,x1);
    if (y0>y1) std::swap(y0,y1);

    int sx = fmax(m_footer.get(LImageFooter::POS_CURRENT_STAMP_X),1);
    int sy = fmax(m_footer.get(LImageFooter::POS_CURRENT_STAMP_X),1);
    int w = fmax(m_footer.get(LImageFooter::LImageFooter::POS_CHARSET_WIDTH),1);

    for (int i=0;i<x1-x0;i++)
        for (int j=0;j<y1-y0;j++) {
            char c = col+i%sx + (j%sy)*w;
            if (size==0)
                setPixel(x0+i, y0+j, c);
            else {
                if (i<size || i>=x1-x0-size || j<size || j>=y1-y0-size)
                    setPixel(x0+i, y0+j, c);
            }
        }
}


void LImage::CopyFrom(LImage *img) {

    if (m_width!=img->m_width || m_height!=img->m_height) {
        m_width = img->m_width;
        m_height = img->m_height;

    }
    m_colorList.CopyFrom(&img->m_colorList);
    m_footer = img->m_footer;
    m_scaleX = img->m_scaleX;
//#pragma omp parallel for

    for (int i=0;i<m_width;i++)
        for (int j=0;j<m_height;j++)
            setPixel(i,j,img->getPixel(i,j));

}

void LImage::SaveCurrentPaletteToPPU()
{
    int pal = m_footer.get(LImageFooter::POS_CURRENT_PALETTE);
//    qDebug() << "LImage::SaveCurrentPaletteToPPU  CURRENT PALETTE "<< pal;
    int noCol = pow(2,m_colorList.m_bpp.x());
    for (int i=0;i<noCol;i++) {
        m_colorList.m_nesPPU[pal*noCol +i] = (uchar)m_colorList.getPen(i);
  //      if (i==0)
    //        qDebug() << "COLOR 0 is "<<Util::numToHex(m_colorList.getPen(i));
    }

}


void LImage::EnsureSystemColours()
{
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::VIC20) {
        m_colorList.InitVIC20();
        m_colorList.m_type = LColorList::VIC20;


        m_GUIParams[col1] ="Background";
        m_GUIParams[col4] = "Border & MC";
        m_GUIParams[col2] = "Aux Colour";
        m_GUIParams[col3] = "";
        m_supports.displayForeground = false; // Set to false
//        m_GUIParams[col4] = "";

    }
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C64)
        m_colorList.InitC64();
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::NES) {
        m_colorList.InitNES();
        m_colorList.m_type = LColorList::NES;
        m_supports.nesPalette = true;
        m_supports.displayBank = true;

    }

}

int LImage::getDx() { return 1;}

int LImage::getDy() { return 1;}

