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

#include "charsetimage.h"
#include "source/LeLib/util/util.h"
#include "source/Compiler/syntax.h"
#include <QKeyEvent>

CharsetImage::CharsetImage(LColorList::Type t) : MultiColorImage(t)
{
    m_width = 320;
    m_height = 200;
    m_scaleX = 1.0f;
    m_bitMask = 0b11;
    m_noColors = 4;
    m_scale = 2;
    m_width = 320;
    m_minCol = 0;
//    m_data = new PixelChar[m_charWidth*m_charHeight];
    m_background = 0;
    Clear();
    m_type = LImage::Type::CharMapMulticolor;
    SetColor(1,1);
    SetColor(2,2);
    SetColor(5,3);

    // Needed for NES stuff

    EnsureSystemColours();
    m_supports.koalaExport = false;
    m_supports.koalaImport = false;
    m_supports.asmExport = false;
    m_supports.binaryLoad = true;
    m_supports.binarySave = true;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.asmExport = false;

    m_supports.displayColors = true;
    m_supports.displayForeground = false;
    m_supports.displayMC2 = true;
    m_supports.displayMC1 = true;
    m_GUIParams[col2] = "";
    m_GUIParams[col3] = "Multicolor 1";
    m_GUIParams[col4] = "Multicolor 2";

    m_currentChar=0;
    //m_currentMode=Mode::FULL_IMAGE;
    m_exportParams.clear();
    m_exportParams["Start"] = 0;
    m_exportParams["End"] = 256;
    m_exportParams["IncludeColors"] = 0;
    m_exportParams["VIC20mode"] = 0;

    m_supports.displayCharOperations = true;

    m_GUIParams[btnLoadCharset] ="";
    m_GUIParams[btn1x1] = "1x1 Character set";
    m_GUIParams[btn2x2] = "2x2 Character set";
    m_GUIParams[btn2x2repeat] = "2x2 Character repeat";
    m_GUIParams[btnCopy] = "Copy";
    m_GUIParams[btnPaste] = "Paste";
    m_GUIParams[btnFlipH] = "Mirror X";
    m_GUIParams[btnFlipV] = "Mirror Y";
    m_GUIParams[btnEditFullCharset] = "Full charset";

    m_GUIParams[tabCharset] = "1";
    m_updateCharsetPosition = true;
}

int CharsetImage::FindClosestChar(PixelChar p)
{
    int topScore=1E9;
    int winner = 0;
    for (int i=0;i<m_charWidth*m_charHeight;i++) {
        int score = p.CompareLength2(m_data[i]);
        if (score<topScore) {
            topScore = score;
            winner = i;
        }

    }
    return winner;
}

QString CharsetImage::GetCurrentDataString() {
    char chr = m_currentChar;
    if (chr<32) chr+=64;
    if (m_currentChar>=0x40) chr=0;
    return "  Character : " + QString(chr) + "  "+ Util::numToHex(m_currentChar) + " (" + QString::number(m_currentChar)+ ")";
}

QString CharsetImage::getMetaInfo()
{
    QString txt="A C64 charset is very similar to a multicolor/hires image, with the exception of having no stored ";
    txt+="colour information. In addition to regular full-screen editing, you can also edit each individual characters, or a grid of 2x2 characters, in either regular or tiled mode. ";
    txt+="\nWhen exported, a C64 charset of 256 characters will take up 256*8 =2048 bytes, but the export method (either by clicking the button or using the TRSE @export command) allows for exporting fewer characters. ";
    return txt;
}


void CharsetImage::SetColor(uchar col, uchar idx)
{
    m_color.c[idx] = col;

    if (idx==0)
        m_background = col;

//    qDebug() << QString::number(idx) << QString::number(col);
    for (int i=0;i<m_charHeight*m_charWidth;i++)
        m_data[i].c[idx] = col;
    m_extraCols[idx] = col;

    m_colorList.SetMulticolor(idx,col);

    UpdateColorList();

}

void CharsetImage::ImportBin(QFile &file)
{
    m_rawData = file.readAll();
//    qDebug() << "IMPORT BYTES SKIP : " <<m_skipImportBytes;
    if (m_skipImportBytes!=0)
        m_rawData.remove(0,m_skipImportBytes);
  //  m_rawData.remove(0,2);
//    m_rawData.resize(8*64);
    FromRaw(m_rawData);
/*    SetColor(0, m_background); //MULTICOLOR_CHAR_COL +0
    SetColor(1, 2);// MULTICOLOR_CHAR_COL +2
    SetColor(2, 1); // MULTICOLOR_CHAR_COL +1*/
}

void CharsetImage::ExportBin(QFile &f)
{
    ToRaw(m_rawData);
    f.write(m_rawData);

    QByteArray colorData;
    for (int i=0;i<m_charWidth*m_charHeight;i++)
        colorData.append(m_data[i].c[3]);

    QString filenameBase = f.fileName().split(".")[0];
    QString fColor = filenameBase + "_color.bin";
    QFile f2(fColor);
    f2.open(QFile::WriteOnly);
    f2.write(colorData);
    f2.close();

}


void CharsetImage::SaveBin(QFile& file)
{
    file.write( ( char * )( &m_background ),  1 );
    file.write( ( char * )( &m_border ), 1 );
    file.write( ( char * )( &m_extraCols[1] ), 1 );
    file.write( ( char * )( &m_extraCols[2] ), 1 );
    file.write( ( char * )( &m_extraCols[3] ), 1 );
    file.write( ( char * )( &m_data ),  m_charWidth*m_charHeight*12 );


}

void CharsetImage::LoadBin(QFile& file)
{
    file.read( ( char * )( &m_background ),1 );
    file.read( ( char * )( &m_border ), 1);
    file.read( ( char * )( &m_extraCols[1] ), 1 );
    file.read( ( char * )( &m_extraCols[2] ), 1 );
    file.read( ( char * )( &m_extraCols[3] ), 1 );
    file.read( ( char * )( &m_data ),  m_charWidth*m_charHeight*12 );

//    for (int i=0;i<m_charWidth*m_charHeight;i++)
  //      m_data[i].c[3]=5;

}

uchar CharsetImage::getVariableColor(PixelChar *pc)
{
    uchar col = 0;
    for (int i=0;i<4;i++) {
        col = pc->c[i];
        bool ok=true;
        for (int j=0;j<3;j++)
            if (col==m_extraCols[j])
                ok=false;
        if (ok)
            return col;
    }
    return 0;
}

void CharsetImage::LoadCharset(QString file, int skipBytes)
{
    m_charsetFilename = file;
    MultiColorImage::LoadCharset(file, skipBytes);
    if (m_charset!=nullptr)
        CopyFrom(m_charset);
}

unsigned int CharsetImage::getPixel(int x, int y)
{

    QPoint p = getXY(x,y);

    if (m_colorList.m_isMulticolor)
        return MultiColorImage::getPixel(p.x(),p.y());

    PixelChar&pc = getPixelChar(p.x(),p.y());
    if (MultiColorImage::getPixel(p.x(),p.y())!=0)
        return pc.c[3];

    return pc.c[0];


}


/*void CharsetImage::FlipVertical()
{



    uint tmp[32*32];
    float i = 160/4.0;
    float j = 200.0/8.0;
    int n = 8/m_scale;
    int ny = 8;
    if (m_currentMode==CHARSET2x2) {
        i = 160/16.0;
        j = 200.0/16.0;
        n =32/m_scale;
        ny = 16;
    }

    for (int x=0;x<n;x++)
        for (int y=0;y<ny;y++) {
            tmp[n*y + x]=getPixel(x*i,y*j+1);
        }
    for (int y=0;y<ny;y++)
        for (int x=0;x<n;x++)
        setPixel( x*i ,y*j+1, tmp[n*y + n-1-x]);



}
*/


void CharsetImage::FromRaw(QByteArray &arr)
{
//    Clear();
    for (int i=0;i<arr.count()/8;i++) {
        PixelChar& pc = m_data[i];
        int idx=i*8;
        for (int j=0;j<8;j++) {
            uchar v = PixelChar::reverse(arr[i*8+j]);
            pc.p[j] = v;
        }
    }
    // Contains colors as well
    if (arr.size()>256*8) {
        int val = 256*8;
     //   qDebug() << "Charset contains colors";
        for (int i=0;i<m_charHeight*m_charWidth;i++) {
            m_data[i].c[3] = arr[i+val]&7;
//            if (i<4) qDebug() << "Color val: " << QString::number(arr[i+val]);
       //     if (m_data[i].c[3]!=0)
         //       qDebug() << m_data[i].c[3];
        }
    }
    else SetColor(5, 3); // Set default green

}

void CharsetImage::ToRaw(QByteArray &arr)
{
    int start = m_exportParams["Start"];
    int end = m_exportParams["End"];
    int size = start-end;
//    qDebug() << start << " " << end;

//    m_colorOrderType = m_exportParams["m_"]


    arr.resize(size*8);
    for (int i=start;i<end;i++) {
        PixelChar& pc = m_data[i];
        for (int j=0;j<8;j++) {
            if (m_colorOrderType==0)
                arr[(i-start)*8+j] = PixelChar::reverse(pc.p[j]);
            if (m_colorOrderType==1 || m_exportParams["VIC20mode"]==1)
                arr[(i-start)*8+j] = PixelChar::reverse(PixelChar::VIC20Swap(pc.p[j]));



        }
    }
    if (m_exportParams["IncludeColors"] != 0) {
        QByteArray cols;
        cols.resize(size);

/*        qDebug() << "Extracols:";
        for (int i=0;i<3;i++)
            qDebug() << QString::number(m_extraCols[i]);
*/
        m_extraCols[0]=m_background;


        for (int i=start;i<end;i++) {
            PixelChar& pc = m_data[i];
            uchar v = getVariableColor(&pc);
            if (v==255)v=0;
            cols[(i-start)] = (v & 7) +8;
        }
        arr.append(cols);
    }
}

void CharsetImage::ToQPixMaps(QVector<QPixmap> &map)
{
    map.clear();
    for (int i=0;i<m_charWidth*m_charHeight;i++) {
        map.append(ToQPixMap(i));
    }
}

QPixmap CharsetImage::ToQPixMap(int chr)
{
    QImage img = m_data[chr].toQImage(32, m_bitMask, m_colorList, m_scale);
    return QPixmap::fromImage(img);

}

void CharsetImage::setPixel(int x, int y, unsigned int color)
{
    QPoint p = getXY(x,y);

    if (!m_forcePaintColorAndChar) {
        PixelChar& pc = getPixelChar(x,y);
        pc.c[3] = color;
        return;

    }


    if (m_colorList.m_isMulticolor)
        setLimitedPixel(p.x(),p.y(),color);
    else {
/*        if (color!=m_background)
            color = 1;
        else
            color = 0;*/
//        m_bitMask = 1;
  //      m_scale = 1;
    //    PixelChar& pc = getPixelChar(x,y);
   //     pc.c[3] = color;

        setLimitedPixel(p.x(),p.y(),color);

    }

}

void CharsetImage::SetBank(int bnk) {
    m_footer.set(LImageFooter::POS_CURRENT_BANK,bnk);
    if (m_charset!=nullptr)
        m_charset->SetBank(bnk);
}

QPoint CharsetImage::getXY(int x, int y)
{
    if (m_footer.get(LImageFooter::POS_DISPLAY_CHAR)==1) {
        int cx = m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_X)*8;
        int cy = m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_Y)*8;

        int s = m_bitMask==0b11?2:1;

        cx/=s;

        int sx = (m_currentChar%m_charWidth)*8/s;
        int sy = (m_currentChar/m_charWidth)*8;

        if (!m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_REPEAT)) {
            x = (x / (float)m_width)*cx+sx;
            y = (y / (float)m_height)*cy+sy;
        }
        else
        {
            x = (int)(x / ((float)m_width)*3*cx)%(cx)+sx;
            y = (int)(y / ((float)m_height)*3*cy)%(cy)+sy;

        }
    }
    return QPoint(x,y);
}

unsigned int CharsetImage::getCharPixel(int pos,  int pal,int x, int y)
{
    return 0;
}

void CharsetImage::RenderEffect(QMap<QString, float> params)
{


}

void CharsetImage::CopyFrom(LImage *img)
{
   // return LImage::CopyFrom(img);

    CharsetImage* mc = dynamic_cast<CharsetImage*>(img);
    m_colorList.m_isMulticolor = img->m_colorList.m_isMulticolor;
    //if ((typeid(*img) == typeid(MultiColorImage)) || (typeid(*img) == typeid(StandardColorImage))
    //        || (typeid(*img) == typeid(CharsetImage)))
    m_footer = img->m_footer;
    if (mc!=nullptr)
    {
        m_background = mc->m_background;
        m_border = mc->m_border;
        //m_currentMode = mc->m_currentMode;
        m_currentChar = mc->m_currentChar;

        m_width = mc->m_width;
        m_height = mc->m_height;
        m_scaleX = mc->m_scaleX;
        m_bitMask = mc->m_bitMask;
        m_noColors = mc->m_noColors;
        m_scale = mc->m_scale;
        m_minCol = mc->m_minCol;
        m_charWidth = mc->m_charWidth;
        m_charHeight = mc->m_charHeight;

        for (int i=0;i<4;i++)
            m_extraCols[i]  = mc->m_extraCols[i];
        // qDebug() << "COPY FROM";
//#pragma omp parallel for
        for(int i=0;i<m_charWidth*m_charHeight;i++) {
            for (int j=0;j<8;j++)
                m_data[i].p[j] = mc->m_data[i].p[j];
            for (int j=0;j<4;j++)
                m_data[i].c[j] = mc->m_data[i].c[j];
        }
    }
    else
    {
        MultiColorImage::CopyFrom(img);
        return;
    }


}

bool CharsetImage::KeyPress(QKeyEvent *e)
{
    QPoint dir(0,0);


    if (e->key()==Qt::Key_0 ) { Data::data.currentColor = m_color.c[0];}
    if (e->key()==Qt::Key_1 ) { Data::data.currentColor = m_color.c[1];}
    if (e->key()==Qt::Key_2 ) { Data::data.currentColor = m_color.c[2];}
    if (e->key()==Qt::Key_3 ) { Data::data.currentColor = m_color.c[3];}



    int sx = m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_X);
    int sy = m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_Y);
//    if (m_currentMode==CHARSET2x2 || m_currentMode==CHARSET2x2_REPEAT)
  //      s=2;

    if (e->key()==Qt::Key_W)
        m_currentChar-=m_charWidthDisplay*sy;
    if (e->key()==Qt::Key_A)
        m_currentChar-=sx;
    if (e->key()==Qt::Key_S)
        m_currentChar+=m_charWidthDisplay*sy;
    if (e->key()==Qt::Key_D)
        m_currentChar+=sx;

    if (m_currentChar>=m_charHeight*m_charWidth)
        m_currentChar=0;

    m_currentChar = Util::clamp(m_currentChar,0,m_charWidth*m_charHeight-1);


    return true;
}

void CharsetImage::setLimitedPixel(int x, int y, unsigned int color)
{

    if (x>=m_width || x<0 || y>=m_height || y<0)
        return;
    PixelChar& pc = getPixelChar(x,y);

 //   pc.Reorganize(m_bitMask, m_scale,m_minCol, m_noColors);

  //      qDebug() << QString::number(color) << QString::number(pc.c[0]) << QString::number(pc.c[1]) << QString::number(pc.c[2]) << QString::number(pc.c[3]);

    int ix = x % (8/m_scale);//- (dx*m_charHeight);
    int iy = y % 8;//- (dy*m_charWidth);

    //if (ix==0 || ix == 2 || ix == 4 || ix == 6)
    pc.set(m_scale*ix, iy, color, m_bitMask);

//    qDebug() << color;

}

void CharsetImage::onFocus() {
    if (m_charsetFilename!="")
        LoadCharset(m_charsetFilename,0);
}

int CharsetImage::getCharWidthDisplay()
{
    if (m_footer.get(LImageFooter::POS_DISPLAY_CHAR)==1) {
        int i =1;
        if (m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_REPEAT)==1)
            i=3;
        return i*8*m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_X);
    }
    return m_charWidthDisplay;
}

int CharsetImage::getCharHeightDisplay()
{
    if (m_footer.get(LImageFooter::POS_DISPLAY_CHAR)==1) {
        int i =1;
        if (m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_REPEAT)==1)
            i=3;
        return i*8*m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_Y);
    }
    return m_charHeightDisplay;
}

