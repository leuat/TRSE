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
//    m_colorList.m_supportsFooterPen = true;


    EnsureSystemColours();
    InitPens();
}

void MultiColorImage::setPixel(int x, int y, unsigned int color)
{
    if (x>=m_width || x<0 || y>=m_height || y<0)
        return;

    PixelChar& pc = getPixelChar(x,y);



    pc.Reorganize(m_bitMask, m_scale,m_minCol, m_noColors, getBackground());
//    qDebug() << m_noColors;

    int ix = x % (8/m_scale);//- (dx*m_charWidth);
    int iy = y % 8;//- (dy*m_charHeight);
/*    if (rand()%100>98) {
        qDebug() << Util::numToHex(color) << Util::numToHex(m_noColors) << Util::numToHex(m_minCol);
    }
*/
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

void MultiColorImage::setBackground(unsigned int col)
{
    m_colorList.setPen(0,col);
    //qDebug() << "SetBackrouund VIC "<<col;
    if (m_footer.get(LImageFooter::POS_DISPLAY_MULTICOLOR)==1 && m_bitMask!=1)
    for (int i=0;i<m_charWidth*m_charHeight;i++) {
        m_data[i].c[0] = col;
    }

 //   qDebug() << "SETBACK";
    if (m_bitMask==0b11 || m_footer.get(LImageFooter::POS_DISPLAY_HYBRID)==1) {
//        qDebug() << "HERE SETBACKGROUND " << m_colorList.getPen(1) << m_colorList.getPen(2) ;;;
        if (m_type != MultiColorBitmap)
        for (int i=0;i<m_charWidth*m_charHeight;i++) {
            //m_data[i].c[0] = col;

            m_data[i].c[1] = m_colorList.getPen(1);
            m_data[i].c[2] = m_colorList.getPen(2);
        }

    }

    if (m_charset!=nullptr)
        m_charset->setBackground(col);
}

void MultiColorImage::Reorganize()
{
//#pragma omp parallel for
    for (int i=0;i<m_charWidth*m_charHeight;i++)
        m_data[i].Reorganize(m_bitMask, m_scale, m_minCol, m_noColors,getBackground());
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
    char dummy = m_colorList.getPen(0);
    file.write( ( char * )( &dummy ),  1 ); //bakcground
    file.write( ( char * )( &dummy ), 1 ); // unused
    file.write( ( char * )( &m_data ),  m_charHeight*m_charWidth*12 );


}

void MultiColorImage::LoadBin(QFile& file)
{
    char dummy;
    file.read( ( char * )( &dummy ),1 );
  //  m_colorList.setPen(0,dummy); // background
    file.read( ( char * )( &dummy ), 1); // Unused
 //   qDebug() << sizeof(PixelChar);
    file.read( ( char * )( &m_data ),  m_charHeight*m_charWidth*12 );
/*    for (int i=0;i<1000;i++)
        if (m_data[i].c[0]!=0)
        qDebug() << i<<Util::numToHex(m_data[i].c[0]);*/
/*    int c1 = 255;
    int c2 = 255;
    for (int i=0;i<m_charWidth*m_charHeight;i++) {
        if (m_data[i].c[1]!=255) c1=m_data[i].c[1];
        if (m_data[i].c[2]!=255) c2=m_data[i].c[2];
    }

    m_colorList.setPen(2,c1); // background
    m_colorList.setPen(1,c2); // background
    qDebug() << "LoadBin"<< c1 << c2;
*/
//    QByteArray data = file.read(m_charHeight*m_charWidth*12);
    //  memcpy(&m_data, &data, m_charHeight*m_charWidth*12);
}

void MultiColorImage::Color2Raw(QByteArray &ba, int yl)
{
    for (int i=0;i<m_charWidth*m_charHeight;i++)
        ba.append(m_data[i].c[3]);

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
        pc.Reorganize(m_bitMask, m_scale,m_minCol, m_noColors, getBackground());
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
    fdata.append((uchar)getBackground());

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

void MultiColorImage::OrdererdDither(QImage &img, LColorList &colors, QVector3D strength, QPoint size, float gamma = 1.0)
{
    int height  =std::min(img.height(), m_height);
    int width  =std::min(img.width(), m_width);
    QMatrix4x4 bayer4x4 = QMatrix4x4(0,8,2,10,  12,4,14,6, 3,11,1,9, 15,7,13,5);
    bayer4x4 = bayer4x4*1/16.0*strength.x();

/*    QMatrix4x4 bayer4x4 = QMatrix4x4(0,2,0,0,  3,1,0,0, 0,0,0,0, 0,0,0,0);
    bayer4x4 = bayer4x4*1/4.0*strength.x();
*/

    QVector<QPoint> hist;
    hist.resize(16);

  //  QElapsedTimer timer;
    //    timer.start();

/*    width = m_width;
    height = m_height;
    qDebug() << width << height << img.width() << img.height();
*/
    for (int y=0;y<height;y++) {
        for (int x=0;x<width;x++) {

//            color.R = color.R + bayer8x8[x % 8, y % 8] * GAP / 65;

            double dx = x/(double)width*img.width();
            double dy = y/(double)height*img.height();
            int xx = (dx-img.width()/2.0)*m_importScaleX + img.width()/2.0;
            int yy = (dy-img.height()/2.0)*m_importScaleY + img.height()/2.0;

//            xx = x;
 //           yy = y;

            QColor color = QColor(img.pixel(xx,yy));
            int yp = y + x%(int)strength.y();
            int xp = x + y%(int)strength.z();
            color.setRed(std::min((float)pow(color.red(),gamma) + bayer4x4(xp % size.x(),yp % size.y()),255.0f));
            color.setGreen(std::min((float)pow(color.green(),gamma) + bayer4x4(xp % size.x(),yp % size.y()),255.0f));
            color.setBlue(std::min((float)pow(color.blue(),gamma) + bayer4x4(xp % size.x(),yp % size.y()),255.0f));

            int winner = 0;
            QColor newPixel = colors.getClosestColor(color, winner);
//            PixelChar& pc = getPixelChar(x,y);
            setPixel(x,y,winner);

        }
    }
    //qDebug() << "The slow operation took" << timer.elapsed() << "milliseconds";

}

void MultiColorImage::CopyImageData(LImage *img)
{
    MultiColorImage* mc = dynamic_cast<MultiColorImage*>(img);
    //qDebug() << "HERE " <<mc;
    if (mc!=nullptr)
        memcpy((char*)m_data,(char*)mc->m_data,sizeof(PixelChar)*m_charWidth*m_charWidth);
    else qDebug() << "MultiColorImage::CopyImageData - unknown image type!";

}

void MultiColorImage::fromQImage(QImage *img, LColorList &lst)
{
//  #pragma omp parallel for
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
    m_footer.m_data = img->m_footer.m_data;
    m_colorList.CopyFrom(&img->m_colorList);
    if (mc!=nullptr)
    {
        //        m_footer.m_data = mc->m_footer.m_data;
        m_currentChar = mc->m_currentChar;
        m_charWidth = mc->m_charWidth;
        m_charHeight = mc->m_charHeight;
        m_scale = mc->m_scale;
        m_bitMask = mc->m_bitMask;
        m_width = mc->m_width;
        m_height = mc->m_height;
        m_scaleX = mc->m_scaleX;
        m_noColors = img->m_noColors;
        m_minCol = img->m_minCol;
        m_charWidthDisplay = mc->m_charWidthDisplay;
        m_charHeightDisplay = mc->m_charHeightDisplay;

        CopyImageData(img);

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
    for (int i=1;i<m_charWidth*m_charHeight;i++) {
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

    QString filenameBase = Util::getFileWithoutEnding(f);

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
    colorData.append(getBackground());
    colorData.append(getBackground());
    data.clear();
    int charC = 3;


    if (m_bitMask== 0b1 || m_type==LImage::Type::HiresBitmap || m_footer.get(LImageFooter::POS_DISPLAY_MULTICOLOR)==0) { // Regular color
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

// export small bitmap chars as large PETSCII 'block' characters (2x2 blocks per character)
void MultiColorImage::PBMExport(QFile &file, int start, int width, int height, int exportType)
{
    QByteArray data;
    QVector<PixelChar*> pcList;

    // petscii characters that represent the blocks
//    uchar binscii[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}; // nibble pattern
    uchar petscii[16] = {32,124,126,226,108,225,127,251,123,255,97,236,98,254,252,224};

//    int st = (start % m_charWidthDisplay) + (( start / m_charWidthDisplay));

    int blockRows = height * 4; // 2x2 pixels per bloc, so 4 rows to process per char

    // loop for height
    for (int r = 0; r < blockRows; r++ ) {

        // walk the characters
        for (int c = 0; c < width; c++ ) {

            int pos = start + c + ( (r / 4) * m_charWidthDisplay );
            PixelChar& pc = m_data[pos];

            // process the 8 rows of pixels, grouped as 2x2
            //for ( int i = 0; i < 8; i = i + 2 ) {

            // which pixel row?
            int i = (r % 4) * 2;


            if ( exportType != 2 ) {

                // UNPACKED  METHOD -  4 chars per 8 pixels
                // go through the four bit blocks 2x2
                // pairs row 0 [11 22 33 44]
                // pairs row 1 [11 22 33 44]
                for ( int p = 3; p >= 0; p-- ) {

                    // generate a nibble 1
                    uchar r0 = PixelChar::reverse( pc.p[ i ] ) >> ( p * 2 ) & 0b11;
                    uchar r1 = (PixelChar::reverse( pc.p[ i + 1 ] ) >> ( p * 2 ) & 0b11) << 2;
                    uchar petBlock;
                    if ( exportType == 0 )
                        petBlock = petscii[ r0 + r1 ];  // pet chars
                    else
                        petBlock = r0 + r1;  // binary representation

                    data.append( petBlock );
                }

            } else {

                // COMPACT METHOD - 2 char per 8 pixels - to use in a program must be unpacked to two chars
                // get two pixel rows
                uchar rev0 = PixelChar::reverse( pc.p[ i ] );
                uchar rev1 = PixelChar::reverse( pc.p[ i + 1 ] );

                uchar b_76_0 = rev0 & 0b11000000;
                uchar b_76_1 = rev1 & 0b11000000;
                uchar b_54_0 = rev0 & 0b00110000;
                uchar b_54_1 = rev1 & 0b00110000;

                uchar b_32_0 = rev0 & 0b00001100;
                uchar b_32_1 = rev1 & 0b00001100;
                uchar b_10_0 = rev0 & 0b00000011;
                uchar b_10_1 = rev1 & 0b00000011;

                uchar nibbleUpperA = (b_76_1) + (b_76_0 >> 2);
                uchar nibbleLowerA = (b_54_1 >> 2) + (b_54_0 >> 4);

                uchar nibbleUpperB = (b_32_1 << 4) + (b_32_0 << 2);
                uchar nibbleLowerB = (b_10_1 << 2) + (b_10_0);

                uchar block0 = nibbleUpperA + nibbleLowerA;
                uchar block1 = nibbleUpperB + nibbleLowerB;

                // write two bytes
                data.append( block0 );
                data.append( block1 );

            }

        }

    }

    file.write(data);

        /*
    for (int i=start;i<end;i++) { // walk characters

        // Convert to POS in charset:
        int x = i%m_charWidthDisplay; // return 0 -39
        int y = height*((i/m_charWidthDisplay));
        int pos = x + ( y * m_charWidthDisplay );

        for (int j=0;j<height;j++) { // walk line height

            // characters
            if (pos>=0 && pos< m_charWidth*m_charHeight) {

                PixelChar& pc = m_data[pos];
                for (int i=0;i<8;i=i+2) {  // process 8 pixel lines in character data
                    //data.append( PixelChar::reverse(pc.p[i]));
                    unsigned char r0 = PixelChar::reverse(pc.p[i]);
                    unsigned char r1 = PixelChar::reverse(pc.p[i+1]);
                }
            }
            pos +=m_charWidthDisplay;
        }
        //        qDebug() << x << y;
    }
    */
    //file.write(data);
}

// exports binary in a format suitable for VBM. It arranges by a complete row at a time rather than columns of 8 rows.
void MultiColorImage::VBMExport(QFile &file, int start, int end, int height, int isMulticolor)
{
    QByteArray data;
    QVector<PixelChar*> pcList;

    for (int i=start;i<end;i++) { // walk characters

        // Convert to POS in charset:
        int x = i%m_charWidthDisplay;
        int y = height*((i/m_charWidthDisplay));
        int pos = x+y*m_charWidthDisplay;

        for (int j=0;j<height;j++) { // walk line height

            // characters
            if (pos>=0 && pos< m_charWidth*m_charHeight) {

                PixelChar& pc = m_data[pos];
                for (int i=0;i<8;i++) {  // process 8 pixel lines in character data
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
void MultiColorImage::VBMExportColor(QFile &file, int start, int width, int height, int lineHeight)
{

    // do colour map
    QByteArray cdata;
    QVector<PixelChar*> pcList;

    for (int j=0; j<height; j++) { // y
        for (int i=0;i<width;i++) { // x
            // Convert to POS in charset:
            int x = i; // % m_charWidthDisplay;
            int y = j; // ((i/m_charWidthDisplay));
            int pos = x+(y*m_charWidthDisplay) + start;

            //qDebug() <<i <<j <<"-" << x << y << pos;

            // check in bounds
            if (pos>=0 && pos< m_charWidth*m_charHeight) {
                //PixelChar& pc = m_data[pos];
                // colour
                int line = y  % lineHeight;

                if (line == 0) { // process colour lines (will be every even line if vic 20)
                    PixelChar& pc = m_data[pos];
                    cdata.append( pc.c[ 3 ] ); // char selection colour
                }
            }

        }
    }
    file.write(cdata);

/*
    for (int i=start;i<end;i++) { // walk characters

        // Convert to POS in charset:
        int x = i%m_charWidthDisplay;
        int y = height*((i/m_charWidthDisplay));
        int pos = x+y*m_charWidthDisplay;

        // colour
        int line = y  % 1;
        if (m_colorList.m_type == LColorList::VIC20) line = y % 2; // for colour data

        if (line == 0) { // process colour lines (will be every even line if vic 20)
            if (pos>=0 && pos< m_charWidth*m_charHeight) {
                PixelChar& pc = m_data[pos];
                cdata.append( pc.c[ 3 ] ); // char selection colour
            }
        }

        pos +=m_charWidthDisplay;

        //        qDebug() << x << y;
    }
    file.write(cdata);
    */
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
                    else\
                        data.append( PixelChar::reverse(pc.p[i]));
                }
                pos +=m_charWidthDisplay;
            }

        }
    }
    file.write(data);
}
/*bool SortFunc(const int &s1, const int &s2)
{
    return s1 < s2;
}
*/

bool colsSort(const QVector3D s1, const QVector3D s2)
{
    return s1.x() > s2.x();
}

void MultiColorImage::FromLImageQImage(LImage *other)
{
    Clear();
/*   QImage test(other->m_width, other->m_height, QImage::Format_ARGB32);
   ((LImageQImage*)(other))->ToQImage(other->m_colorList,test,1,QPointF(80,100));
   test.save("test.png");*/
    for (int y=0; y<m_charHeight;y++)
        for (int x=0; x<m_charWidth;x++) {
            int num  =x + y*m_charWidth;
            PixelChar& pc = m_data[num];
            int dx = x*8/m_scale;
            int dy = y*8;

            // First, analyze chars
            QVector<QVector3D> cols;
            QVector<int> winners;

            cols.resize(m_colorList.m_list.count());
            for (int i=0;i<cols.count();i++)
                cols[i].setY(i);

            for (int j=0;j<8;j++) {
                for (int i=0;i<8/m_scale;i++) {
                    uchar c = other->getPixel(dx+i, dy+j);
                 //   qDebug() << QString::number(c);
                    cols[c].setX(cols[c].x()+1);

                }
            }

            std::sort(cols.begin(),cols.end(),colsSort);

            // Pick out the 3 best winners + background first
            int j=0;
            if (m_bitMask==0b11) {
                winners.append(getBackground());
                for (int i=0;i<3;i++) {
                    if (cols[j].y()==getBackground()) j++;
                    winners.append(cols[j].y());
                    j++;
                }

            }
            else
                for (int i=0;i<4;i++) {
                    winners.append(cols[j].y());
                    j++;
                }
            for (int i=0;i<4;i++)
                pc.c[i] = winners[i];

            m_colorList.EnableColors(winners);
            for (int j=0;j<8;j++) {
                for (int i=0;i<8/m_scale;i++) {
                    uchar c = other->getPixel(dx+i, dy+j);
  //                  if (winners.contains(c))
                    int winner =  0;
                    m_colorList.getClosestColor(other->m_colorList.m_list[c].color, winner);
                    pc.set(i*m_scale,j,winner,m_bitMask);
//                    pc.set(i*m_scale,j,c,m_bitMask);
                }
            }


        }
    m_colorList.EnableColors(QVector<int>() <<0 <<1<<2<<3<<4<<5<<6<<7<<8<<9<<10<<11<<12<<13<<14<<15);

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

void MultiColorImage::ExportCompressed(QString f1, QString f2, QString f3)
{
    //int x0,int x1, int y0, int y1, int& noChars, int compression, int maxChars) {

    Compression c;
//    c.OptimizeAndPackCharsetData()
    QByteArray charData, screenData, colorData;
    QVector<int> i_screenData;
    int noChars;

    ForceColorFlattening();

    CompressAndSave(charData, i_screenData,
                    m_exportParams["StartX"],m_exportParams["EndX"],
                    m_exportParams["StartY"],m_exportParams["EndY"],
                    noChars,    (double)m_exportParams["Compression"]/100.0,255,1,true);
//    qDebug() << "No chars :" << noChars;
    screenData = Util::toQByteArray(i_screenData);
    FixUp(charData);
    Util::SaveByteArray(screenData, f1);
    Util::SaveByteArray(charData, f2);
    Color2Raw(colorData,1);
    Util::SaveByteArray(colorData, f3);
    QString s = "Compression level: " +QString::number((int)m_exportParams["Compression"])+"<br>";
    s+= "Packed image to : "+QString::number(noChars) + " characters";
    m_exportMessage = s;
    if (!m_silentExport) {
        QMessageBox msgBox;
        msgBox.setText(s);
        msgBox.exec();

    }



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

    InitPens();

}

void MultiColorImage::SetHybridMode(bool checked) {
    LImage::SetHybridMode(checked);
    if (m_charset!=nullptr)
        m_charset->SetHybridMode(checked);

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

void MultiColorImage::InitPens() {

    if (m_colorList.m_type==LColorList::NES) {
        if (m_charset==nullptr)
            m_colorList.InitNESPens();

        else {
            m_charset->m_colorList.InitNESPens();
            m_colorList.CopyFrom(&m_charset->m_colorList);

        }

    }

    if (m_colorList.m_type==LColorList::VIC20 && m_type == LImage::LevelEditor) {
        if (m_charset==nullptr) {
            m_colorList.SetVIC20Pens(m_bitMask == 0b11);
        }
        else {
//            QVector<int> keep = m_charset->m_colorList.getPenList();
            m_charset->m_colorList.SetVIC20Pens(m_bitMask == 0b11);
            m_colorList.CopyFrom(&m_charset->m_colorList);
  //          for (int i=0;i<keep.count();i++)
    //          m_colorList.setPen(i,keep[i]);
//            m_colorList.SetVIC20Pens(m_bitMask == 0b11);
        }
        setBackground(getBackground());
        return;
    }

    if (m_colorList.m_type==LColorList::C64 || m_colorList.m_type==LColorList::VIC20) {
        if (m_charset==nullptr)
            m_colorList.SetC64Pens(m_bitMask == 0b11,(m_type==LImage::LImage::Type::CharMapMulticolor));
        else {
            m_charset->m_colorList.SetC64Pens(m_bitMask == 0b11,(m_type==LImage::LImage::Type::CharMapMulticolor));
            m_colorList.CopyFrom(&m_charset->m_colorList);
        }

    }

/*    if (m_colorList.m_type==LColorList::VIC20) {
        if (m_charset==nullptr)
            m_colorList.SetVIC20Pens(m_bitMask == 0b11);
        else {
            m_charset->m_colorList.SetVIC20Pens(m_bitMask == 0b11);
            m_colorList.CopyFrom(&m_charset->m_colorList);
        }

    }
*/
    setBackground(getBackground());

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

void MultiColorImage::setHybrid()
{
    m_width = 320;
    m_height = 200;
    //m_scaleX = 1.2f;
    m_bitMask = 0b1;
    m_noColors = 4;
    m_scale = 1;
    m_scaleX = 1.0;
    m_minCol = 0;

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
        m_colorList.InitC64Multicolor();
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

//     SetPens();
    if (m_type!=Sprites2)
        InitPens();
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



void MultiColorImage::ForceBackgroundColor(int col, int swapCol)
{
    for (int i=0;i<1000;i++) {
        PixelChar& pc= m_data[i];
        pc.ForceBackgroundColor(col, swapCol);
    }
    setBackground(swapCol);
 //   m_background = swapCol;
   // m_extraCols[0] = swapCol;
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

/*    if (m_colorList.m_type == LColorList::VIC20 ||  m_colorList.m_type == LColorList::C64) {
        m_colorList.UpdateCommodoreRestrictions(m_extraCols);

    }*/
    m_colorList.CreateUI(m_colorList.m_layout,1);

}


void MultiColorImage::ToQImage(LColorList& lst, QImage& img, float zoom, QPointF center)
{
//    return;
    int height  =std::min(img.height(), m_height);
    int width  =std::min(img.width(), m_width);
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

void MultiColorImage::CompressAndSave(QByteArray& chardata, QVector<int>& screen, int x0,int x1, int y0, int y1,
                                      int& noChars, double compression, int maxChars, int type, bool addChars=true) {
    CharsetImage* ni = new CharsetImage(m_colorList.m_type);

    QVector<PixelChar> chars;
//    QByteArray data;
    QVector<int> data;
    int sx = x1-x0;
    int sy = y1-y0;

//    qDebug() << "*******" <<sx << sy;
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
    if (addChars) {
    chars.append(p0);
    chars.append(p1);
    chars.append(p2);
    chars.append(p3);
    }
    for (int j=0;j<sy;j++) {

        for (int i=0;i<sx;i++)
        {
            PixelChar& pc = m_data[i+x0 + (j+y0)*m_charWidth];
            int pi = 0;
            bool found = false;
            double cur = 1E5;

            if (compression!=0.0)
            for (PixelChar& p : chars) {
//                if (found)
  //                  break;
              //  double metric = pc.CompareLength3(p);
                double metric = 0;
                if (type==1) metric = (pc.CompareLengthSSIM(p));
                if (type==0) metric = pc.CompareLength3(p);
            //    if (rand()%100>98) qDebug() << metric;
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

//    qDebug() << "CHARDATA : " << chars.count();
    delete ni;

   // return out;


}

void MultiColorImage::SetColor(uchar col, uchar idx)
{
    //m_extraCols[idx] = col;
    m_colorList.SetMulticolor(idx,col);
//    qDebug()<< m_extraCols[idx];

}

void MultiColorImage::Invert() {
    for (int i=0;i<m_charWidth*m_charHeight;i++)
        m_data[i].Invert();
}
