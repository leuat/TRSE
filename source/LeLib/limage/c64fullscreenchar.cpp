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

#include "c64fullscreenchar.h"

C64FullScreenChar::C64FullScreenChar(LColorList::Type t) : MultiColorImage(t)
{
/*    m_charset = new CharsetImage(t);
    QFile f("C:/Users/leuat/OneDrive/Documents/GitHub/pmm/pmm/charsets/test_mc.bin");
    f.open(QIODevice::ReadOnly);
    m_charset->ImportBin(f);*/

    m_width = 320;
    m_height = 200;
    m_scaleX = 1.0f;
/*    m_bitMask = 0b11;
    m_noColors = 4;
  */
    m_bitMask = 0b1;
    m_noColors = 2;


    m_scale = 1;
    m_minCol = 0;
    m_type = LImage::Type::FullScreenChar;

    AddNew(1,1);

    SetColor(1,1);
    SetColor(2,2);
    SetColor(3,3);


    m_supports.asmExport = false;
    m_supports.binaryLoad = true;
    m_supports.binarySave = true;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.asmExport = false;
    m_supports.movieExport = true;


    m_GUIParams[btnLoadCharset] ="Load Charset";
    m_GUIParams[btn1x1] = "";
    m_GUIParams[btn2x2] = "";
    m_GUIParams[btn2x2repeat] = "";
    m_GUIParams[btnCopy] = "";
    m_GUIParams[btnPaste] = "";
    m_GUIParams[btnFlipH] = "";
    m_GUIParams[btnFlipV] = "";

    m_GUIParams[tabCharset] = "Charset";
    m_GUIParams[btnEditFullCharset] = "";

    m_GUIParams[tabSprites] ="Screens";


    m_exportParams.clear();

}

void C64FullScreenChar::SetColor(uchar col, uchar idx)
{
    if (m_charset!=nullptr)
//        return;
    m_charset->SetColor(col, idx);
    m_extraCols[idx] = col;
}

void C64FullScreenChar::Clear()
{
    m_items[m_current]->Clear();

}

void C64FullScreenChar::ImportBin(QFile &f)
{
/*    m_rawData = f.read(1000);
    m_rawColors = f.read(1000);
    // Shift down
    for (int i=0;i<m_rawColors.count();i++)
        m_rawColors[i] = m_rawColors[i] - 8;
//    FromRaw(m_rawData);
*/
}

void C64FullScreenChar::ExportBin(QFile &f)
{
 //   ToRaw(m_rawData);
  //  f.write(m_rawData);
/*    for (int i=0;i<m_rawColors.count();i++)
        m_rawColors[i] = m_rawColors[i] + 8;

    f.write(m_rawColors);

    for (int i=0;i<m_rawColors.count();i++)
        m_rawColors[i] = m_rawColors[i] - 8;*/


}

bool C64FullScreenChar::KeyPress(QKeyEvent *e)
{

    if (e->key()==Qt::Key_A)
        Prev();
    if (e->key()==Qt::Key_D)
        Next();

    SetColor(m_extraCols[0],0);
    SetColor(m_extraCols[1],1);
    SetColor(m_extraCols[2],2);
    return true;
}

void C64FullScreenChar::AddNew(int w, int h) {
    C64Screen* s = new C64Screen();

    s->Init(m_charWidth, m_charHeight);
    m_items.append(s);
    m_current = m_items.count()-1;
}

void C64FullScreenChar::setPixel(int x, int y, unsigned int color)
{
    m_width=320;
    if (x>=320 || x<0 || y>=200 || y<0)
        return;
    x=x*m_charWidth/40.0;
    y=y*m_charHeight/25.0;

    if (m_writeType==Character)
        ((C64Screen*)m_items[m_current])->m_rawData[x/8+ (y/8)*m_charWidth] = m_currencChar;
    if (m_writeType==Color)
        ((C64Screen*)m_items[m_current])->m_rawColors[x/8+ (y/8)*m_charWidth] = color;
    //BuildImage();
}

unsigned int C64FullScreenChar::getPixel(int x, int y)
{
    m_width=320;
    if (m_charset==nullptr)
        return 0;
    if (x>=320 || x<0 || y>=200 || y<0)
        return 0;

    x=x*m_charWidth/40.0;
    y=y*m_charHeight/25.0;

    uchar v = ((C64Screen*)m_items[m_current])->m_rawData[(x/8) + (y/8)*m_charWidth];
    uchar col = ((C64Screen*)m_items[m_current])->m_rawColors[(x/8) + (y/8)*m_charWidth];
    int ix = (x % 8);//- (dx*40);
    int iy = (y % 8);//- (dy*25);

 //   return pc.get(m_scale*ix, iy, m_bitMask);



    int pos = v;
//    return m_charset->m_data[pos].get(v + (2*x)&7, v+ y&7,m_bitMask);
//    uchar val = m_charset->m_data[pos].get(ix, iy,m_charset->m_bitMask);
    uchar val = m_charset->m_data[pos].get(ix, iy,m_bitMask);

    if (m_bitMask==0b11) {
        if (val==m_charset->m_data[pos].c[3])
            val = col;
    }
    else {
        if (val!=m_charset->m_data[pos].c[0])
            val = col;

    }


    return val;

}

void C64FullScreenChar::CopyFrom(LImage *mc)
{
    C64FullScreenChar* c = dynamic_cast<C64FullScreenChar*>(mc);
    if (c!=nullptr) {
        if (c->m_charset==nullptr)
            return;


        m_charWidth = c->m_charWidth;
        m_charHeight = c->m_charHeight;

        DeleteAll();
        for (LImageContainerItem* li: c->m_items) {
            C64Screen* s= (C64Screen*)li;
            C64Screen* s2= new C64Screen();
            *s2 = *s;

            m_items.append(s2);
        }


//        m_items = c->m_items;
        m_current = c->m_current;

        m_charset = c->m_charset;
        m_writeType = c->m_writeType;
        m_currencChar = c->m_currencChar;

    }
    else
    LImage::CopyFrom(mc);

}

void C64FullScreenChar::ExportMovie(QFile &file)
{
    QVector<QByteArray> screens;
    QByteArray mty;
    mty.resize(m_charWidth*m_charHeight*2);
    mty.fill(0);
    screens.append(mty);
    for (LImageContainerItem* li :m_items) {
        C64Screen* screen = dynamic_cast<C64Screen*>(li);
        QByteArray s;
        for (int i=0;i<m_charWidth*m_charHeight;i++) {
            s.append(screen->m_rawData[i]);
            s.append(screen->m_rawColors[i]);
        }
        screens.append(s);
    }
    MovieConverter mc;
    float compr;
    int cnt=0;

    QByteArray header;
    header.append(m_charWidth);
    header.append(m_charHeight);
    header.append(m_items.count());

    file.write(header);



    for (int i=0;i<screens.count()-1;i++) {
        QByteArray data = mc.CompressScreen(screens[i], screens[i+1],m_charWidth, m_charHeight,compr);
        file.write(data);
        cnt+=data.count();
    }

    float total = m_charHeight*m_charWidth*screens.count();
    qDebug() << "Compression : " << cnt/total*100 << " %";






}

void C64FullScreenChar::CopyChar()
{
    if (m_current<0) return;
    m_copy = *((C64Screen*)m_items[m_current]);
}

void C64FullScreenChar::PasteChar()
{
    if (m_copy.m_data.count()==0)
        return;

//    if (m_items[m_current].m_height == m_copy.m_height)
    *((C64Screen*)m_items[m_current])=m_copy;

}



void C64FullScreenChar::SaveBin(QFile& file)
{
    file.write( ( char * )( &m_background ),  1 );
    file.write( ( char * )( &m_border ), 1 );
    file.write( ( char * )( &m_charWidth ),  1 );
    file.write( ( char * )( &m_charHeight), 1 );
    uchar v = m_items.count();
    file.write( ( char * )( &v), 1 );


    qDebug() << "w h " << QString::number(m_charWidth) << " " << QString::number(m_charHeight);
    qDebug() << "cnt " << QString::number(v);

    char tmp = 0;
    for (int i=0;i<11;i++)
        file.write( ( char * )( &tmp), 1 );

    for (LImageContainerItem* li : m_items){
//        qDebug() << "Size: " << li->ToQByteArray(0).count();
        file.write(li->ToQByteArray(0));
    }


}

void C64FullScreenChar::LoadBin(QFile& file)
{
    file.read( ( char * )( &m_background ),1 );
    file.read( ( char * )( &m_border ), 1);
    file.read( ( char * )( &m_charWidth ),  1 );
    file.read( ( char * )( &m_charHeight), 1 );
    uchar cnt;
    file.read( ( char * )( &cnt), 1 );

    char tmp = 0;
    for (int i=0;i<11;i++)
        file.read( ( char * )( &tmp), 1 );

    m_items.clear();

    for (int i=0;i<cnt;i++) {
        C64Screen* s = new C64Screen();

        s->Init(m_charWidth, m_charHeight);
        s->m_rawColors = file.read( m_charWidth*m_charHeight);
        s->m_rawData = file.read(m_charWidth*m_charHeight);
        s->m_data = file.read(16);
        m_items.append(s);
    }
    m_current = 0;


}

