/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit sofk<are development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be usekc,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

#include "source/LeLib/limage/multicolorimage.h"
#include <QDebug>
#include <QFile>
#include <QApplication>
#include <QTextStream>
#include "source/LeLib/limage/standardcolorimage.h"
#include "source/LeLib/limage/charsetimage.h"
#include <typeinfo>
#include "source/LeLib/util/util.h"
#include "source/LeLib/limage/limageio.h"
#include "source/LeLib/limage/compression.h"

static bool compareHist(const QPoint &s1, const QPoint &s2)
       {
           return s1.x() > s2.x(); // This is just an example
       }

MultiColorImage::MultiColorImage(LColorList::Type t) : LImage(t)
{
    m_width = 160;
    m_height = 200;
    m_scaleX = 2;
    //m_data.resize(m_charWidth*m_charHeight);

    Clear();
    m_type = LImage::Type::MultiColorBitmap;

    m_supports.asmExport = true;
    m_supports.binaryLoad = true;
    m_supports.binarySave = true;
    m_supports.koalaImport = true;
    m_supports.koalaExport = true;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.compressedExport = true;
    m_supports.displayCharOperations = false;

    m_GUIParams[btnLoadCharset] ="";
    m_GUIParams[btn1x1] = "";
    m_GUIParams[btn2x2] = "";
    m_GUIParams[btn2x2repeat] = "";
    m_GUIParams[btnCopy] = "";
    m_GUIParams[btnPaste] = "";
    m_GUIParams[btnFlipH] = "";
    m_GUIParams[btnFlipV] = "";

    m_GUIParams[tabCharset] = "";
    m_GUIParams[tabData] = "";
    m_GUIParams[tabLevels] = "";
    m_GUIParams[tabEffects] = "Effects";

    m_GUIParams[btnEditFullCharset] = "";

    m_supports.displayMC1 = false;
    m_supports.displayMC2 = false;


    m_GUIParams[col1] ="Background";
    m_GUIParams[col2] = "Border";
    m_GUIParams[col3] = "";
    m_GUIParams[col4] = "";

    m_exportParams["StartX"] = 0;
    m_exportParams["EndX"] = m_charWidth;
    m_exportParams["StartY"] = 0;
    m_exportParams["EndY"] = m_charHeight;
    m_exportParams["Compression"] = 0;

    //m_data.resize(m_charWidth*m_charHeight);

//    qDebug() << m_charWidth*m_charHeight;



    EnsureSystemColours();
    for (int i=0;i<4;i++)
        m_extraCols[i] = 0;
}

void MultiColorImage::setPixel(int x, int y, unsigned int color)
{

    if (x>=m_width || x<0 || y>=m_height || y<0)
        return;
    PixelChar& pc = getPixelChar(x,y);

//    qDebug() << m_minCol;
  //  qDebug() << m_noColors;

//    qDebug() << m_extraCols[0] << m_background;
    pc.Reorganize(m_bitMask, m_scale,m_minCol, m_noColors, m_background);

    int ix = x % (8/m_scale);//- (dx*m_charWidth);
    int iy = y % 8;//- (dy*m_charHeight);

    //if (ix==0 || ix == 2 || ix == 4 || ix == 6)
    pc.set(m_scale*ix, iy, color, m_bitMask, m_noColors, m_minCol);

}





unsigned int MultiColorImage::getPixel(int x, int y)
{

    if (x>=m_width || x<0 || y>=m_height || y<0)
        return 0;
    PixelChar& pc = getPixelChar(x,y);

    int ix = x % (8/m_scale);//- (dx*m_charWidth);
    int iy = y % 8;//- (dy*m_charHeight);

    return pc.get(m_scale*ix, iy, m_bitMask);

}

void MultiColorImage::setForeground(unsigned int col)
{
    m_border = col;
}

void MultiColorImage::setBackground(unsigned int col)
{
    m_background = col;
    for (int i=0;i<m_charWidth*m_charHeight;i++) {
        m_data[i].c[0] = col;
    }
    if (m_charset!=nullptr)
        m_charset->setBackground(col);
}

void MultiColorImage::Reorganize()
{
#pragma omp parallel for
    for (int i=0;i<m_charWidth*m_charHeight;i++)
        m_data[i].Reorganize(m_bitMask, m_scale, m_minCol, m_noColors,m_extraCols[0]);
}

bool MultiColorImage::KeyPress(QKeyEvent *e)
{
/*    int add = 0;
    if (QApplication::keyboardModifiers() & Qt::ShiftModifier) {
        add=10;
    }
    qDebug() << QString::number(e->key()==Qt::Key_0);*/
    int add = 0;
        if (e->key()==Qt::Key_0 ) { Data::data.currentColor = 0+add;}
        if (e->key()==Qt::Key_1 ) { Data::data.currentColor = 1+add;}
        if (e->key()==Qt::Key_2 ) { Data::data.currentColor = 2+add;}
        if (e->key()==Qt::Key_3 ) { Data::data.currentColor = 3+add;}
        if (e->key()==Qt::Key_4 ) { Data::data.currentColor = 4+add;}
        if (e->key()==Qt::Key_5 ) { Data::data.currentColor = 5+add;}
        if (e->key()==Qt::Key_6 ) { Data::data.currentColor = 6+add;}
        if (e->key()==Qt::Key_7 ) { Data::data.currentColor = 7+add;}
        if (e->key()==Qt::Key_8 ) { Data::data.currentColor = 8+add;}
        if (e->key()==Qt::Key_9 ) { Data::data.currentColor = 9+add;}

/*        add = 10;
        if (e->key()==Qt::Key_Equal ) { Data::data.currentColor = 0+add;}
        if (e->key()==Qt::Key_Exclam ) { Data::data.currentColor = 1+add;}
        if (e->key()==Qt::Key_QuoteDbl ) { Data::data.currentColor = 2+add;}
        if (e->key()==Qt::Key_ ) { Data::data.currentColor = 3+add;}
        if (e->key()==Qt::Key_4 ) { Data::data.currentColor = 4+add;}
        if (e->key()==Qt::Key_5 ) { Data::data.currentColor = 5+add;}
        if (e->key()==Qt::Key_6 ) { Data::data.currentColor = 6+add;}
        if (e->key()==Qt::Key_7 ) { Data::data.currentColor = 7+add;}
        if (e->key()==Qt::Key_8 ) { Data::data.currentColor = 8+add;}
        if (e->key()==Qt::Key_9 ) { Data::data.currentColor = 9+add;}*/
        return true;
}

void MultiColorImage::SaveBin(QFile& file)
{
    file.write( ( char * )( &m_background ),  1 );
    file.write( ( char * )( &m_border ), 1 );
    file.write( ( char * )( &m_data ),  m_charHeight*m_charWidth*12 );

}

void MultiColorImage::LoadBin(QFile& file)
{
    file.read( ( char * )( &m_background ),1 );
    file.read( ( char * )( &m_border ), 1);
    file.read( ( char * )( &m_data ),  m_charHeight*m_charWidth*12 );
//    QByteArray data = file.read(m_charHeight*m_charWidth*12);
  //  memcpy(&m_data, &data, m_charHeight*m_charWidth*12);
}

void MultiColorImage::ImportKoa(QFile &f)
{
    QByteArray data, screen, color, bg;
    f.read(2);
    data = f.read(0x1F40);
    screen = f.read(0x3E8);
    color = f.read(0x3E8);
    bg = f.read(1);
    int pos = 0;

    for (int j=0;j<m_charWidth*m_charHeight;j++) {
        PixelChar& pc =  m_data[j];
        for (int i=0;i<8;i++)
            pc.p[i] = PixelChar::reverse(data[pos*8+i]);
        pc.c[1] = screen[pos]&15;
        pc.c[2] = (screen[pos]>>4)&15;
        pc.c[3] = color[pos];
        pos++;
    }
    setBackground(bg[0]);
//    qDebug() << "KOA background: " <<QString::number(bg[0]);

}

void MultiColorImage::ExportKoa(QFile &f)
{


    QByteArray data, screen, color, bg;
    int pos = 0;
    screen.resize(1000);
    color.resize(1000);
    for (int j=0;j<m_charWidth*m_charHeight;j++) {
        PixelChar& pc =  m_data[j];
        pc.Reorganize(m_bitMask, m_scale,m_minCol, m_noColors, m_background);
        for (int i=0;i<8;i++)
            data.append(PixelChar::reverse(pc.p[i]));

        screen[pos] = pc.c[1] | pc.c[2]<<4;
        color[pos] = pc.c[3];
        pos++;
    }
    //setBackground(bg[0]);

    QByteArray fdata;
    fdata.append((char)0x00);
    fdata.append((char)0x60);
    fdata.append(data);
    fdata.append(screen);
    fdata.append(color);
    fdata.append((uchar)m_background);

    f.write(fdata);
}

QString MultiColorImage::getMetaInfo() {
    QString txt = "Multicolor/hires images are the backbone of any C64 project. \n";
    txt+="A c64 hires image consist of a screen of 40x25 bytes (characters 0-255 at a total of 1000 bytes), ";
    txt+="with an additional 40x25 bytes of color data for each cell. \n";
    txt+="However, a multicolor image allows for 2 extra colours per color cell, using another 1000 bytes of data (the 4-bit colour data are stored as hi/low nybbles).";
    txt+="In addition to this, the background and border colour are also used and stored together with the raw data.";
    return txt;

}




void MultiColorImage::FloydSteinbergDither(QImage &img, LColorList& colors, bool dither)
{
/*    for each y from top to bottom
       for each x from left to right
          oldpixel  := pixel[x][y]
          newpixel  := find_closest_palette_color(oldpixel)
          pixel[x][y]  := newpixel
          quant_error  := oldpixel - newpixel
          pixel[x + 1][y    ] := pixel[x + 1][y    ] + quant_error * 7 / 16
          pixel[x - 1][y + 1] := pixel[x - 1][y + 1] + quant_error * 3 / 16
          pixel[x    ][y + 1] := pixel[x    ][y + 1] + quant_error * 5 / 16
          pixel[x + 1][y + 1] := pixel[x + 1][y + 1] + quant_error * 1 / 16*/


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

void MultiColorImage::OrdererdDither(QImage &img, LColorList &colors, QVector3D strength, int size, float gamma = 1.0)
{
    int height  =min(img.height(), m_height);
    int width  =min(img.width(), m_width);
    QMatrix4x4 bayer4x4 = QMatrix4x4(0,8,2,10,  12,4,14,6, 3,11,1,9, 15,7,13,5);
    bayer4x4 = bayer4x4*1/16.0*strength.x();

/*    QMatrix4x4 bayer4x4 = QMatrix4x4(0,2,0,0,  3,1,0,0, 0,0,0,0, 0,0,0,0);
    bayer4x4 = bayer4x4*1/4.0*strength.x();
*/

    QVector<QPoint> hist;
    hist.resize(16);

    for (int y=0;y<height;y++) {
        for (int x=0;x<width;x++) {

//            color.R = color.R + bayer8x8[x % 8, y % 8] * GAP / 65;

            int xx = (x-img.width()/2.0)*m_importScaleX + img.width()/2.0;
            int yy = (y-img.height()/2.0)*m_importScaleY + img.height()/2.0;

            QColor color = QColor(img.pixel(xx,yy));
            int yp = y + x%(int)strength.y();
            int xp = x + y%(int)strength.z();
            color.setRed(min((float)pow(color.red(),gamma) + bayer4x4(xp % size,yp % 2),255.0f));
            color.setGreen(min((float)pow(color.green(),gamma) + bayer4x4(xp % size,yp % size),255.0f));
            color.setBlue(min((float)pow(color.blue(),gamma) + bayer4x4(xp % size,yp % size),255.0f));

        //    if (rand()%100>98)
         //       qDebug() << "WINNER : " <<Util::numToHex(winner) << newPixel;
            //int c = m_colorList.getIndex(newPixel);

            // Then finally: get max closest colours

/*            int sz = 4;
            for (int i=0;i<16;i++) hist[i]=QPoint(0,i);
            for (int y2=0;y2<sz;y2++) {
                for (int x2=0;x2<sz;x2++) {
                    int c;
                    int xx = (x2-sz/2+x-img.width()/2)*m_importScaleX + img.width()/2;
                    int yy = (y2-sz/2+y-img.height()/2)*m_importScaleY + img.height()/2;
                    QColor color = QColor(img.pixel(xx,yy));
                    colors.getClosestColor(color, c);
                     hist[c].setX(hist[c].x()+1);
                }

            }
            qSort(hist.begin(), hist.end(), compareHist);
//            PixelChar& pc = getPixelChar(x,y);


            int winner = 0;
            QColor newPixel = colors.getClosestColor(color, winner);
            if (hist[0].y()==winner || hist[1].y()==winner || hist[2].y()==winner || hist[3].y()==winner)
                setPixel(x,y,winner);
*/
            int winner = 0;
            QColor newPixel = colors.getClosestColor(color, winner);
//            PixelChar& pc = getPixelChar(x,y);
            setPixel(x,y,winner);

        }
    }

}

void MultiColorImage::fromQImage(QImage *img, LColorList &lst)
{
#pragma omp parallel for
    for (int i=0;i<m_width;i++)
        for (int j=0;j<m_height;j++) {
            unsigned char col = lst.getIndex(QColor(img->pixel(i, j)));
            setPixel(i,j,col);
        }
 //   Reorganize();

}

void MultiColorImage::CopyFrom(LImage* img)
{
    MultiColorImage* mc = dynamic_cast<MultiColorImage*>(img);
    //if ((typeid(*img) == typeid(MultiColorImage)) || (typeid(*img) == typeid(StandardColorImage))
    //        || (typeid(*img) == typeid(CharsetImage)))
    if (mc!=nullptr)
    {
        MultiColorImage* mc = (MultiColorImage*)img;
//        m_footer.m_data = mc->m_footer.m_data;
         m_background = mc->m_background;
         m_border = mc->m_border;
         m_charWidth = mc->m_charWidth;
         m_charHeight = mc->m_charHeight;
         m_scale = mc->m_scale;
         m_bitMask = mc->m_bitMask;
         m_width = mc->m_width;
         m_height = mc->m_height;
         m_scaleX = mc->m_scaleX;

         m_charWidthDisplay = mc->m_charWidthDisplay;
        // qDebug() << "COPY FROM";
#pragma omp parallel for
         for(int i=0;i<m_charHeight*m_charWidth;i++) {
             for (int j=0;j<8;j++)
                 m_data[i].p[j] = mc->m_data[i].p[j];
             for (int j=0;j<4;j++)
                 m_data[i].c[j] = mc->m_data[i].c[j];
         }
         for (int i=0;i<4;i++) m_extraCols[i] = img->m_extraCols[i];
    }
    else
    {
        LImage::CopyFrom(img);
        return;
    }

}

void MultiColorImage::ForceColorFlattening()
{
    PixelChar& org = m_data[0];
    for (int i=1;i<1000;i++) {
        for (int j=0;j<4;j++)
            m_data[j].c[j] = org.c[j];
    }
}

/*void MultiColorImage::ExportAsm(QString filename)
{
    QString filen = filename.split(".")[0];
    ExportRasBin(filen,"");

    return;
    // Fuck this
    if (QFile::exists(filename))
         QFile::remove(filename);
    QFile file(filename);
    if (!file.open(QFile::Append))
        return;
    QTextStream s(&file);


    s << "*=$0801\n";
    s << "  BYTE    $0E, $08, $0A, $00, $9E, $20, $28,  $32, $33\n";
    s << "  BYTE    $30, $34, $29, $00, $00, $00\n";
    s << "*=$0900\n";


    s << "IMG_CHARSET = $2000\n";
    s << "IMG_SCREENDATA = $m_charWidth00\n";
    s << "IMG_COLORDATA = IMG_SCREENDATA + 1002\n\n";

    s << "display_image\n";

    s << "  lda IMG_SCREENDATA\n";
    s << "  sta $d020 ; background \n";
    s << "  lda IMG_SCREENDATA+1 \n";
    s << "  sta $d021; foreground \n";
    s << "  ldx #$00 \n";
    s << "loaddccimage\n";
    s << "  lda IMG_SCREENDATA+2,x\n";
    s << "  sta $0m_charWidth0,x \n";
    s << "  lda IMG_SCREENDATA + #$102,x \n";
    s << "  sta $0500,x \n";
    s << "  lda IMG_SCREENDATA + #$202,x \n";
    s << "  sta $0600,x \n";
    s << "  lda IMG_SCREENDATA + #$302,x \n";
    s << "  sta $0700,x \n";
    s << "\n";
    s << "; Color \n";
    s << "  lda IMG_COLORDATA,x \n";
    s << "  sta $d800,x \n";
    s << "  lda IMG_COLORDATA+ #$100,x \n";
    s << "  sta $d900,x \n";
    s << "  lda IMG_COLORDATA+ #$200,x \n";
    s << "  sta $da00,x \n";
    s << "  lda IMG_COLORDATA+ #$300,x \n";
    s << "  sta $db00,x \n\n";
    s << "  inx \n";
    s << "  bne loaddccimage \n";

    s << " ; set bitmap mode \n";
    s << "  lda #$3b \n";
    s << "  sta $d011 \n";
    s << "; Set multicolor mode \n";
    s << "  lda #$18 \n";
    s << "  sta $d016 \n";
    s << "; Set bitmap position ($m_charWidth0 bytes) \n";
    s << "  lda #$18 \n";
    s << "  sta $d018 \n";
    s << "loop \n";
    s << "  jmp loop \n";
    s << "  rts \n \n";
    s << "*=IMG_CHARSET \n";

    for (int y=0;y<m_charHeight;y++)
        for (int x=0;x<m_charWidth;x++)
        {
        s << m_data[y*m_charWidth + x].bitmapToAssembler();
    }
    s << "\n*=IMG_SCREENDATA \n";
    s << "  byte " << m_border << ", " << m_background << "\n";
    for (int y=0;y<m_charHeight;y++)
    {
        QString str = "   byte ";
        for (int x=0;x<m_charWidth;x++) {
           str = str +  m_data[x + y*m_charWidth].colorMapToAssembler(1,2);
            if (x!=39)
                str = str + ", ";
        }
        str = str + "\n";
        s << str;
    }

    s << "\n*=IMG_COLORDATA \n";
    for (int y=0;y<m_charHeight;y++)
    {
        QString str = "   byte ";
        for (int x=0;x<m_charWidth;x++) {
           str = str +  m_data[x + y*m_charWidth].colorToAssembler();
            if (x!=39)
                str = str + ", ";
        }
        str = str + "\n";
        s << str;
    }



    file.close();

}
*/



void MultiColorImage::ExportBin(QFile& ofile)
{

    QString f = ofile.fileName();

    QString filenameBase = f.split(".")[0];

    QString fData = filenameBase + "_data.bin";
    QString fColor = filenameBase + "_color.bin";

    if (QFile::exists(fData))
        QFile::remove(fData);

    if (QFile::exists(fColor))
        QFile::remove(fColor);

    QByteArray data;
    int sx = static_cast<int>(m_exportParams["StartX"]);
    int ex = static_cast<int>(m_exportParams["EndX"]);
    int sy = static_cast<int>(m_exportParams["StartY"]);
    int ey = static_cast<int>(m_exportParams["EndY"]);


    for (int j=sy;j<ey;j++)
        for (int i=sx;i<ex;i++)
            data.append(m_data[i + j*m_charWidth].data());
/*    for (int i=0;i<m_charWidth*m_charHeight;i++) {
        data.append(m_data[i].data());
    }*/
    QFile file(fData);
    file.open(QIODevice::WriteOnly);
    file.write( data );
    file.close();

    QByteArray colorData;
    colorData.append(m_background);
    colorData.append(m_border);
    data.clear();
    int charC = 3;


    if (m_bitMask== 0b1 || m_type==LImage::Type::HiresBitmap) { // Regular color
        charC = 1;
    }
    if (charC==3)
    for (int j=sy;j<ey;j++)
        for (int i=sx;i<ex;i++)
//            data.append(m_data[i + j*m_charWidth].data());
            colorData.append((uchar)m_data[j*m_charWidth + i].colorMapToNumber(1,2));

  /*  for (int i=0;i<m_charWidth*m_charHeight;i++) {
      //  qDebug () << QString::number((uchar)colorData[colorData.count()-1]);
//        colorData.append((uchar)m_data[i].colorMapToNumber(1,2));
    }*/
    for (int j=sy;j<ey;j++)
        for (int i=sx;i<ex;i++) {

//    for (int i=0;i<m_charWidth*m_charHeight;i++) {
//        uchar c = (uchar)m_data[i].c[3];
        uchar c;
        if (charC==3)
            c = (uchar)m_data[i+j*m_charWidth].c[charC];
        if (charC==1) {
//            c = (uchar)m_data[i+j*m_charWidth].c[charC];
            c = ((uchar)m_data[j*m_charWidth + i].colorMapToNumber(0,1));
        }
        if (c==255)
            c=0;
//        if (c!=0)
  //          qDebug() << c;
        data.append((char)c);
    }
    QFile file2(fColor);
    file2.open(QIODevice::WriteOnly);
    file2.write( colorData );
    file2.write( data );
    file2.close();


    // Take care of color data!

    ofile.close();
    QFile::remove(ofile.fileName());

}

void MultiColorImage::ForceExportBin(QFile &file)
{
    MultiColorImage::ExportBin(file);
}

void MultiColorImage::ImportBin(QFile &file)
{
    QByteArray data = file.readAll();
    int j=0;
    if (file.fileName().contains("_data")) {
    for (int i=0;i<1000;i++) {
        for (int k=0;k<8;k++)
            m_data[i].p[k] = PixelChar::reverse(data[j+k]);

/*        m_data[i].c[0] = 0;
        m_data[i].c[1] = 1;
        m_data[i].c[2] = 2;
        m_data[i].c[3] = 3;*/
        j+=8;
    }
    }
    if (file.fileName().contains("_color")) {
    j=2;
    for (int i=0;i<1000;i++) {

        m_data[i].c[0] = 0;//data[j+0];
        m_data[i].c[1] = data[j+0]&0x0F;
        m_data[i].c[2] = data[j+0]>>4;
        m_data[i].c[3] = data[j+1000];
        j+=1;
    }
    }

}

void MultiColorImage::VBMExport(QFile &file, int start, int end, int height, int isMulticolor)
{
    QByteArray data;
    QVector<PixelChar*> pcList;
    for (int i=start;i<end;i++) {
        // Convert to POS in charset:
        int x = i%m_charWidthDisplay;
        int y = height*((i/m_charWidthDisplay));
        int pos = x+y*m_charWidthDisplay;
        for (int j=0;j<height;j++) {
            if (pos>=0 && pos< m_charWidth*m_charHeight) {
                PixelChar& pc = m_data[pos];
                for (int i=0;i<8;i++) {
                    // VIC20 and Multicolor mode - swap bit
                    if (m_colorList.m_type == LColorList::VIC20 && isMulticolor ==1)
                        data.append( PixelChar::reverse(PixelChar::VIC20Swap(pc.p[i])));
                    else
                        data.append( PixelChar::reverse(pc.p[i]));
                }
            }
            pos +=m_charWidthDisplay;
        }
        //        qDebug() << x << y;
    }
    file.write(data);
}
void MultiColorImage::VBMExportChunk(QFile &file, int start, int width, int height, int isMulticolor)
{
    QByteArray data;
    QVector<PixelChar*> pcList;

    for (int i=0;i<width;i++) { // x
        for (int j=0; j<height; j++) { // y
            // Convert to POS in charset:
            int x = i; // % m_charWidthDisplay;
            int y = j; // ((i/m_charWidthDisplay));
            int pos = x+(y*m_charWidthDisplay) + start;

            //qDebug() <<i <<j <<"-" << x << y << pos;

            // check in bounds
            if (pos>=0 && pos< m_charWidth*m_charHeight) {
                PixelChar& pc = m_data[pos];
                for (int i=0;i<8;i++) {
                    // VIC20 and Multicolor mode - swap bit
                    if (m_colorList.m_type == LColorList::VIC20 && isMulticolor ==1)
                        data.append( PixelChar::reverse(PixelChar::VIC20Swap(pc.p[i])));
                    else
                        data.append( PixelChar::reverse(pc.p[i]));
                }
                pos +=m_charWidthDisplay;
            }

        }
    }
    file.write(data);
}

void MultiColorImage::SetCharSize(int x, int y)
{
    m_width = x*8;
    if (m_bitMask==0x11) {
        m_width = x*4;

    }
    m_height = y*8;
    m_charWidth = x;
    m_charHeight = y;
}

void MultiColorImage::ExportCompressed(QString f1, QString f2)
{
    //int x0,int x1, int y0, int y1, int& noChars, int compression, int maxChars) {

    Compression c;
//    c.OptimizeAndPackCharsetData()
    QByteArray charData, screenData;
    QVector<int> i_screenData;
    int noChars;

    ForceColorFlattening();

    CompressAndSave(charData, i_screenData,
                    m_exportParams["StartX"],m_exportParams["EndX"],
                    m_exportParams["StartY"],m_exportParams["EndY"],
                    noChars,    m_exportParams["Compression"],255 );
//    qDebug() << "No chars :" << noChars;
    screenData = Util::toQByteArray(i_screenData);
    Util::SaveByteArray(screenData, f1);
    Util::SaveByteArray(charData, f2);


    QMessageBox msgBox;
    QString s = "Compression used: " +QString::number((int)m_exportParams["Compression"])+"\n\n";
    s+= "Packed to #chars : "+QString::number(noChars);

    msgBox.setText(s);
    msgBox.exec();


}

void MultiColorImage::LoadCharset(QString file, int skipBytes)
{
    if (!QFile::exists(file)) {
//        qDebug() << "Could not find file " << file;
        return;
    }

    if (file.toLower().endsWith(".bin") || file.toLower().endsWith(".rom")  ) {
        QFile f(file);
        f.open(QIODevice::ReadOnly);
        m_charset = new CharsetImage(m_colorList.m_type);
        if (!file.toLower().endsWith(".rom"))
         m_charset->m_skipImportBytes = skipBytes;

        m_charset->ImportBin(f);
        if (file.toLower().endsWith(".rom"))
            m_charset->setMultiColor(false);
        f.close();
    }
    if (file.toLower().endsWith(".flf")) {
        LImage* img =LImageIO::Load(file);
        m_charset = dynamic_cast<CharsetImage*>(img);
    }
    m_charsetFilename = file;
    if (m_charset==nullptr)
        return;

    if (m_footer.get(LImageFooter::POS_CHARSET_WIDTH)!=0) {
        m_charset->m_charWidthDisplay = m_footer.get(LImageFooter::POS_CHARSET_WIDTH);
    }

    m_charWidthDisplay = m_charset->m_charWidthDisplay;
    m_gridWidthDisplay = m_charset->m_gridWidthDisplay;
    m_charHeightDisplay = m_charset->m_charHeightDisplay;;//m_meta.m_height;


}

int MultiColorImage::getCharAtPos(QPoint p, float zoom, QPointF center)
{

    int xp = (((p.x()-center.x())*(double)zoom)+ center.x());
    int yp = (((p.y()-center.y())*(double)zoom)+ center.y());


    int y = yp/(float)m_height*m_charHeight;
    return xp/(float)m_width*m_charWidth + y*m_charWidth;
}

void MultiColorImage::onFocus()
{
    if (m_charsetFilename!="")
        LoadCharset(m_charsetFilename,0);
}

/*void MultiColorImage::Clear()
{
    for (int i=0;i<m_charWidth*m_charHeight;i++) {
        m_data[i].Clear(m_background);
    }
}
*/
int MultiColorImage::LookUp(PixelChar pc)
{
    for (int i=0;i<m_organized.count();i++) {
        if (pc.isEqualBytes(m_organized[i]))
            return i;
    }
    // Not found, add
    m_organized.append(pc);
    return m_organized.count()-1;

}

void MultiColorImage::setMultiColor(bool doSet)
{
    if (doSet) {
        m_width = 160;
        m_height = 200;
      //  m_scaleX = 2.5f;
        m_bitMask = 0b11;
        m_noColors = 4;
        m_scale = 2;
        m_minCol = 0;
    }
    else {
        m_width = 320;
        m_height = 200;
        //m_scaleX = 1.2f;
        m_bitMask = 0b1;
        m_noColors = 2;
        m_scale = 1;
        m_minCol = 0;

    }
//    for (int i=0;i<1000;i++)
  //      m_data->c[0] = m_extraCols[0];
     if (m_charset!=nullptr)
        m_charset->setMultiColor(doSet);
}

void MultiColorImage::CalculateCharIndices()
{
    m_organized.clear();
    m_outputData.clear();
    int add=0;
    for (int x=0;x<m_charWidth*m_charHeight;x++) {
        PixelChar pc= m_data[x];
        if (pc.isEmpty()) {
            add++;
            continue;
        }
        x+=Eat(x, add);
        add=0;
    }
    m_outputData.append((char)0);
    m_outputData.append((char)0);
}

int MultiColorImage::Eat(int start, int add)
{
    int length=0;
    int cur = start;
    QVector<uchar> arr;



    while(!m_data[cur].isEmpty()) {
        arr.append(LookUp(m_data[cur]));

        qDebug() << "Colors : " << m_data[cur].c[0] << " and " <<  m_data[cur].c[1];

        arr.append(m_data[cur].c[1]);
        cur++;
        length++;
    }
    qDebug() << "Start: " << add << " , " << length;
    m_outputData.append(add);
    m_outputData.append(length);
    for (char i: arr)
        m_outputData.append(i);

    return length;
}

void MultiColorImage::SaveCharRascal(QString fileName, QString name)
{
    if (QFile::exists(fileName))
        QFile::remove(fileName);

    QFile file(fileName);
    file.open(QIODevice::Text | QIODevice::WriteOnly);
    QTextStream s(&file);

    s<< " /* Auto generated image file */\n\n";
    s<< " /* Charset */\n";
    s<< " char_"+name+"_set : array[" + QString::number(m_organized.count()*8) +"] of byte = (\n";
    bool isEnd = false;
    for (int i=0;i<m_organized.count();i++) {
        if (i==m_organized.count()-1)
            isEnd=true;
        for (int j=0;j<8;j++) {
            s<<QString::number(PixelChar::reverse(m_organized[i].p[j]));
            if (!(isEnd && j==7))
                s<<",";
        }
        if (!isEnd)
            s<<"\n";

    }
    s<<");\n";

    s<< " /* Char data */ \n";
    s<< " char_"+name+"_data : array[" + QString::number(m_outputData.count()) +"] of byte = (\n";
    isEnd = false;
    for (int i=0;i<m_outputData.count();i++) {
           if (i==m_outputData.count()-1)
               isEnd=true;

            s<< QString::number((uchar)m_outputData[i]);
            if (!isEnd)
                   s<<",";
            if (i%8==0)
               s<<"\n";
       }
       s<<");\n";


     // Then generate copying routine
       int incPos = 0;
       int shift = 8*64;
       int size = m_organized.count()*8;
       int cur = 0;
       s<< "procedure CopyChar"+name+"Data(); \n";
       s<< "begin \n";

       while (cur<size) {
           int count = size-cur;
           if (count>=255)
               count=0;
           s<< "\tmemcpy(char_"+name+"_set, #"+QString::number(incPos)+
               ", $2800+ "+QString::number(shift) +", #"+QString::number(count)+");"
            << "\n";
            incPos+=256;
            shift+=256;
            cur+=256;
       }


       s<< "end;\n";
       file.close();
}


PixelChar &MultiColorImage::getPixelChar(int x, int y)
{
    int dx = x/(8/m_scale);
    int dy = y/8;

//    qDebug() << "MCI : "<<charWidthDisplay();


    int i = Util::clamp(dx + charWidthDisplay()*dy,0,m_charWidth*m_charHeight);
     return m_data[i];

}

PixelChar::PixelChar()
{
    Clear(0);
}

unsigned char PixelChar::get(int x, int y, unsigned char bitMask)
{
    if (x<0 || x>=8 || y<0 || y>=8)
        return 0;

    unsigned char pp = (p[y]>>x) & bitMask;

    return c[pp];

}

void PixelChar::set(int x, int y, unsigned char color, unsigned char bitMask, unsigned char maxCol, unsigned char minCol)
{
    if (x<0 || x>=8 || y<0 || y>=8) {
        qDebug() << "Trying to set " << x << ", " << y << " out of bounds";
        return;
    }


     unsigned char winner = 254;
    // Does color exist in map?
    for (int i=0;i<maxCol;i++) {
        if (c[i] == color) {
            winner = i;
            break;
        }
    }

    if (winner==254) {// && color!=c[0]) {

        for (int j=minCol;j<maxCol;j++)
            if (c[j]==255) {
                winner = j;
                break;
            }

        // not available slots found
        if (winner==254)
        {
            //winner = 3;
            winner = (p[y]>>x) & bitMask;
            if (winner==0 && minCol!=0)
                winner = maxCol-1;

        }

        if (winner>=minCol)
            c[winner] = color;

    }


    unsigned int f = ~(bitMask << x);
    p[y] &= f;
    // Add

    p[y] |= winner<<x;

}

void PixelChar::set(int x, int y, unsigned char color, unsigned char bitMask)
{
    if (x<0 || x>=8 || y<0 || y>=8) {
        qDebug() << "Trying to set " << x << ", " << y << " out of bounds";
        return;
    }

    // find index
    uchar index = 10;
    if (c[0]==color) index=0;
    else
    if (c[1]==color) index=1;
    else
    if (c[2]==color) index=2;
    else
    if (index==10) {
        index=3;
        c[index] = color;

    }

    // Clear
    unsigned int f = ~(bitMask << x);
    p[y] &= f;
    // Add
    p[y] |= (index&bitMask)<<x;

}

void PixelChar::Clear(unsigned char bg)
{
    for (int i=0;i<8;i++)
        p[i] = 0;
    for (int i=1;i<4;i++)
        c[i] = 255;
    c[0] = bg;

}

QString PixelChar::bitmapToAssembler()
{
    QString s = "   byte ";
    for (int i=0;i<8;i++) {
        s = s + QString::number(reverse(p[i]));
        if (i!=7)
            s = s+", ";
    }
    s=s+"\n";
    return s;
}

QString PixelChar::colorMapToAssembler(int i, int j)
{
    if (c[i]==255) c[i] = 0;
    if (c[j]==255) c[j] = 0;
    return QString(QString::number(c[i] | c[j]<<4));
}

uchar PixelChar::colorMapToNumber(int i, int j)
{
    if (c[i]==255) c[i] = 0;
    if (c[j]==255) c[j] = 0;
    return (c[i] | c[j]<<4);
}

QByteArray PixelChar::data()
{
    QByteArray qb;
    for (int i=0;i<8;i++)
        qb.append(reverse(p[i]));

    return qb;
}

uchar PixelChar::flipSpriteBit(int cnt, int m)
{
    uchar k = p[cnt];
    if (m==0b1)
        return k;

    for (int i=0;i<8;i+=2) {
        uchar mask = 0b11 <<i;
        uchar j = (k >> i)&0b11;
        int n=j;


        if (j==1) n=3;
        if (j==3) n=1;

        k = (k & ~mask) | (n<<i);

    }
    return k;
//    p[cnt] = k;


}



QString PixelChar::colorToAssembler()
{
    if (c[3]==255) c[3] = 0;
    return QString(QString::number(c[3]));

}

QImage PixelChar::toQImage(int size, uchar bmask, LColorList& lst, int scale)
{
    QImage img= QImage(size,size,QImage::Format_RGB32);
    for (int i=0;i<size;i++)
        for (int j=0;j<size;j++) {
            int x = i/(float)(size)*8;
            int ix = (x % (8)/scale)*scale;
            int y = j/(float)(size)*8;
            uchar c = get(ix,y, bmask);

           // if (rand()%100==0 && c!=0)
           //     qDebug() << lst.m_list[c].color;
            img.setPixelColor(i,j,lst.get(c).color);
        }

    return img;
}

bool PixelChar::isEmpty()
{
    for (int i=0;i<8;i++)
        if (p[i]!=0)
            return false;

    return true;
}

bool PixelChar::isEqualBytes(PixelChar &o)
{
    for (int i=0;i<8;i++)
        if (p[i]!=o.p[i])
            return false;

    return true;
}

void PixelChar::Reorganize(unsigned char bitMask, unsigned char scale, unsigned char minCol, unsigned char maxCol, unsigned char bgCol)
{
//    if (rand()%1000==0)
  //      qDebug() << bgCol << minCol << maxCol;
    if (c[0]!=bgCol)
        for (int i=1;i<maxCol;i++)
            if (c[i]==bgCol) {
                unsigned char tmp = c[0];
                c[0]=c[i];
                c[i]=tmp;

            }
    if (minCol<1) minCol=1;
    for (int i=minCol;i<maxCol;i++) {
        unsigned int cnt = Count(i, bitMask, scale);
//        qDebug() << cnt;
        if (cnt == 0) {
            c[i] = 255;
           // qDebug() << "REMOVING COLOR";
        }
    }
}

int PixelChar::Count(unsigned int col, unsigned char bitMask, unsigned char scale)
{
    int cnt=0;
    for (int i=0;i<8/scale;i++)
        for (int j=0;j<8;j++)
            if ( ((p[j]>>scale*i) & bitMask)==col)
                cnt++;
    return cnt;
}

void PixelChar::ForceBackgroundColor(int col, int swapcol)
{
//    if (c[0] == col)
  //      return;

    int idx = -1;
    for (int i=0;i<4;i++) {
        if (c[i]==swapcol) {
            idx=i;
            break;
        }
    }
//    if (idx==-1)
  //      idx=1;
//    idx=1;
    if (idx!=-1) {
//        qDebug() << "swapping "  << rand()%100;
        unsigned char s = c[0];
//        c[0]=c[idx];
  //      c[idx] = s;
//        qDebug() << idx;
        for (int j=0;j<8;j++) {

            p[j] = SwapColor(p[j],0,idx);
/*            int ss = idx*2;
            s = p[j]&0b11;
            unsigned char s2 = (p[j]>>(ss))&0b11;
            p[j] = p[j] & 0b11111100;
            p[j] = p[j] & ~(0b11<<(ss));
            p[j] = p[j] | (s <<(ss)) | s2;*/
        }
    }
//    else c[0]=col;



}

int PixelChar::Compare(PixelChar &other) {
    int l = 0;
    for (int i=0;i<8;i++)
        l+=other.p[i] != p[i];

    /*        for (int i=0;i<4;i++)
            l+=other.c[i]!=c[i];
*/
    return l;

}

int PixelChar::CompareLength(PixelChar &other) {
    int l = 0;
    for (int i=0;i<8;i++)
        for (int j=0;j<8;j++) {
            char a = (p[i]>>(j))&0b1;
            char b = (other.p[i]>>(j))&0b1;
            if ( a != b )
                l++;

        }
    return l;

}

int PixelChar::CompareLength2(PixelChar &other) {
    int l = 0;
    for (int i=0;i<8;i++)
        for (int j=0;j<8;j++) {
            char a = (p[i]>>(j))&0b1;
            char b = (other.p[i]>>(j))&0b1;
            if ( a != b )
                l++;
            if (a==0 && b!=0) l++;
            if (b==0 && a!=0) l++;

        }
    //            if (other.p[i]!=p[i])
    //          l+=other.p[i] != p[i];

    return l;

}

int PixelChar::CompareLength3(PixelChar &other)
{
    // C = sum(Pij*Qij)^2/(sum(Pij^2)*sum(Qij^2)).
    int ap = 0;
    int bp = 0;
    int s = 0;
    for (int i=0;i<8;i++) {
        for (int j=0;j<8;j++) {
            char a = (p[i]>>(j))&0b1;
            char b = (other.p[i]>>(j))&0b1;
//            s+=pow(a-b,2);
            s+=a!=b;//abs(a-b);
        }
    }

/*    for (int i=0;i<8;i+=2) {
        for (int j=0;j<8;j+=2) {
            char a = (p[i]>>(j))&0b11 + (p[i+1]>>(j))&0b11;
            char b = (other.p[i]>>(j))&0b11 + (other.p[i+1]>>(j))&0b11;
            s+=pow(a-b,2);
        }
    }

*/
    return s;
}

uchar PixelChar::SwapColor(uchar data, uchar c1, uchar c2)
{
    for (int i=0;i<4;i++) {
        int j= i*2;
        uchar k1 = (data>>j)&0b11;
        uchar k2 = k1;
        if (k1==c1) { k2=c2;}
        //        else

        if (k1==c2) { k2=c1;}
        uchar mask = ~(0b11<<j);
        data = data & mask;
        data = data | (k2<<j);

    }
    return data;
}


void MultiColorImage::ForceBackgroundColor(int col, int swapCol)
{
    for (int i=0;i<1000;i++) {
        PixelChar& pc= m_data[i];
        pc.ForceBackgroundColor(col, swapCol);
    }
    m_background = swapCol;
    m_extraCols[0] = swapCol;
/*    for (int y=0;y<m_height;y++)
        for (int x=0;x<m_width;x++) {
            int c = getPixel(x,y);
            int nc = c;
            if (c==col) nc=swapCol;
            if (c==swapCol) nc=col;
            setPixel(x,y,nc);

        }
        */
}

void MultiColorImage::AppendSaveBinCharsetFilename(QFile &file)
{
    QString  fn = m_charsetFilename;
    QByteArray fd =  QByteArray(fn.remove(Data::data.currentPath).toLatin1());

    uchar len = fd.count();
    file.write( ( char * )( &len ),  1 );
    file.write(fd);


}

void MultiColorImage::LoadBinCharsetFilename(QFile &file)
{
    if (file.atEnd())
        return;
    uchar len;
    file.read( ( char * )( &len ),  1 );
    QByteArray data = file.read(len);

    m_charsetFilename = Data::data.currentPath+QString::fromLatin1(data);
    if (data.startsWith(":"))
        m_charsetFilename = QString::fromLatin1(data); // ROM etc

//    qDebug() << "Found charset : " << m_charsetFilename;


    if (QFile::exists(m_charsetFilename))
        LoadCharset(m_charsetFilename,0);


}

void MultiColorImage::SwapChars(int p1, int p2)
{
    PixelChar a = m_data[p1];
    m_data[p1] = m_data[p2];
    m_data[p2] = a;
}

void MultiColorImage::UpdateColorList()
{

    if (m_colorList.m_type == LColorList::VIC20 ||  m_colorList.m_type == LColorList::C64) {

        for (int i=0;i<m_colorList.m_list.count();i++) {
            if (i<8)
                m_colorList.m_list[i].displayList = true;
            else
                m_colorList.m_list[i].displayList = false;

        }
        for (int i=0;i<4;i++)
            if (m_extraCols[i]<m_colorList.m_list.count())
                m_colorList.m_list[m_extraCols[i]].displayList=true;

    }
    m_colorList.CreateUI(m_colorList.m_layout,1);

}

uchar PixelChar::Swap(int a, int b, uchar c)
{
        // damn
    uchar n1 = (c>>a) & 0b00000011;
    uchar n2 = (c>>b) & 0b00000011;

    uchar mask =  ((0b11)<<a) | ((0b11) <<b);

    c = c & ~mask;
    c = c | (n1<<b) | (n2<<a);


    return c;
}

uchar PixelChar::VIC20Swap(uchar c)
{
    uchar ret = 0;
    for (int i=0;i<4;i++) {
        uchar v = c>>(i*2) & 0b11;
/*        if (v==0b10) v=0b11;
        else
            if (v==0b11) v=0b10;
*/
        if (v==0b01) v=0b11;
        else
            if (v==0b11) v=0b01;


        ret |= (v<<(i*2));

    }
    return ret;
}


void MultiColorImage::ToQImage(LColorList& lst, QImage& img, float zoom, QPointF center)
{
//    return;
    int height  =min(img.height(), m_height);
    int width  =min(img.width(), m_width);
    //  center.setX(floor(center.x()));
    //   center.setY(floor(center.y()));


//#pragma omp parallel for
    for (int j=0;j<height;j++) {
        int* yline =(int*)img.scanLine(j);
        for (int i=0;i<width;i++) {


            //            float xp = floor(((i-center.x())*zoom)+ center.x());
            //          float yp = floor(((j-center.y())*zoom) + center.y());
            double xp = (((i-center.x())*(double)zoom)+ center.x());
            double yp = (((j-center.y())*(double)zoom)+ center.y());

            unsigned int col = 0;
            if (xp>=0 && xp<width && yp>=0 && yp<height)
                col = getPixel(xp,yp);
            else {
                if (((i+j)&1)==0)
                    //img.setPixel(i,j,QColor(40,50,70).rgb());
                    yline[i] = QColor(40,50,70).rgb();
                else
                    yline[i] = QColor(0,0,0).rgb();
                //img.setPixel(i,j,QColor(0,0,00).rgb());
                continue;

            }


            // Has transparency?

            QColor c=QColor(0,0,0);
            if (col>=1000) {
                col-=1000;
                c = QColor(255, 128, 128);
            }
            QColor scol = lst.get(col).color;
            if (c.red()>0 && renderPathGrid) {
                if ((int)(xp) %4==0 || (int)(yp+1)%4==0)
                    scol = c;
            }
            //            scol = QColor(255,0,0,255);
            QRgb rgbCol = (scol).rgb();
            //for (int k=0;k<m_scale;k++)
            //               img->setPixel(m_scale*i + k,j,rgbCol);*/
            //img.setPixel(i,j,rgbCol);
            yline[i] = rgbCol;//lst.get(col).color.rgb();
        }
    }
    //return img;
}

void MultiColorImage::RenderEffect(QMap<QString, float> params)
{
    float density = params["density"];

    QVector3D center(0.5, 0.5,0);
    for (int x=0;x<160;x++)
        for (int y=0;y<200;y++) {
            QVector3D p(x/160.0, y/200.0,0);
            p=p-center;
            //float angle = atan2(y,x);
            float angle = atan2(p.y(),p.x());


            float l = p.length();
            int as = (int)(abs(angle*params["angleStretch"]));
            int k = ((int)(l*density + as))%(int)params["noColors"];
            setPixel(x,y,k);
        }

}

void MultiColorImage::CompressAndSave(QByteArray& chardata, QVector<int>& screen, int x0,int x1, int y0, int y1, int& noChars, int compression, int maxChars) {
    CharsetImage* ni = new CharsetImage(m_colorList.m_type);

    QVector<PixelChar> chars;
//    QByteArray data;
    QVector<int> data;
    int sx = x1-x0;
    int sy = y1-y0;
    data.resize(sx*sy);
    data.fill(0x0);
    noChars = 0;
    PixelChar p0;
    PixelChar p1;
    PixelChar p2;
    PixelChar p3;
    for (int i=0;i<8;i++) {
        p0.p[i]=0xFF;
        p1.p[i]=0x00;
        p2.p[i]=0b10101010;
        p3.p[i]=0b01010101;
    }
    chars.append(p0);
    chars.append(p1);
    chars.append(p2);
    chars.append(p3);
    for (int j=0;j<sy;j++) {

        for (int i=0;i<sx;i++)
        {
            PixelChar& pc = m_data[i+x0 + (j+y0)*40];
            int pi = 0;
            bool found = false;
            int cur = 1E5;
            for (PixelChar& p : chars) {
//                if (found)
  //                  break;
                int metric = pc.CompareLength3(p);
//                int metric = pc.Compare(p);
                if (metric <=compression && metric<cur ) {
                    data[i + j*sx] = pi;
                    cur=metric;
                    found = true;
//                    break;

                }
                pi++;
//                if (pi>255) exit(1);
            }
            if (!found) {
                chars.append(pc);
                data[i + j*sx] = chars.count()-1;
                noChars++;

            }
            //        data[i]=i;
        }
    }

    //QFile f(outFile);
    //f.open(QFile::WriteOnly);
    QByteArray out;
    for (int i=0;i<maxChars;i++) {
        if (i<chars.count()) {
            for (int j=0;j<8;j++)
                out.append(PixelChar::reverse(chars[i].p[j]));
        }
        else
            for (int j=0;j<8;j++)
                out.append((char)0);

    }
//    qDebug() << data.count();
    //out.append(data);
    chardata.append(out);
    screen.append(data);
/*    f.write(out);
    f.write(data);

    f.close();
*/

    qDebug() << "CHARDATA : " << chardata.count()/8;
    delete ni;

   // return out;


}

void MultiColorImage::SetColor(uchar col, uchar idx)
{
    m_extraCols[idx] = col;
    m_colorList.SetMulticolor(idx,col);
//    qDebug()<< m_extraCols[idx];

}
