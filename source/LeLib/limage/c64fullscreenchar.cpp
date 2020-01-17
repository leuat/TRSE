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
#include "source/LeLib/limage/limagenes.h"

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
    m_supports.displayTimestamp = true;
//    m_supports.binarySave

    m_supports.importc = true;
    m_supports.exportc = false;

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


    m_supports.asmExport=false;
    m_supports.koalaExport = false;
    m_supports.koalaImport = false;
    m_exportParams.clear();
//    m_exportParams["EndChar"] = 254;
  //  m_exportParams["SkipChar"] = 255;
   //m_exportParamsComments["EndChar"]  ="Make sure you didn't use this char in any of the screens.";
   // m_exportParamsComments["SkipChar"]  ="Make sure you didn't use this char in any of the screens.";
    m_exportParams["ExportTimeStamps"] = 1;


    m_exportParams["CompressionType"] = 2;
    m_exportParamsComments["CompressionType"]  ="CompressionType can be either\n 2 or 3. Type 2 compresses\n the animation"
                                           "by only saving \nframe changes, while type 3 saves \nthe whole image but perform a png-like\n compression.";


    m_exportParams["Fullscreen_Colors"] = 1;
    m_exportParams["Fullscreen_Chars"] = 1;

    m_metaParams.append(new MetaParameter("screen_width","Screen width",20,2,1000));
    m_metaParams.append(new MetaParameter("screen_height","Screen height",19,2,1000));

    EnsureSystemColours();
}

void C64FullScreenChar::SetColor(uchar col, uchar idx)
{
    if (m_charset!=nullptr)
//        return;
    m_charset->SetColor(col, idx);
    if (idx==0) ((C64Screen*)m_items[m_current])->m_data[1] = col;
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

    C64Screen* s = (C64Screen*)m_items[m_current];

    if (m_exportParams["Fullscreen_Chars"]==1)
       f.write(s->m_rawData);
    if (m_exportParams["Fullscreen_Colors"]==1)
    f.write(s->m_rawColors);

/*    for (int i=0;i<m_rawColors.count();i++)
        m_rawColors[i] = m_rawColors[i] + 8;

    f.write(m_rawColors);

    for (int i=0;i<m_rawColors.count();i++)
        m_rawColors[i] = m_rawColors[i] - 8;*/


}

void C64FullScreenChar::fromQImage(QImage *img, LColorList &lst)
{
    float sx = img->width()/m_charWidth;
    float sy = img->height()/m_charHeight;
 //   float sy = img->height()/25;
//    qDebug() <<m_charWidth << m_charHeight;


    for (float i=0;i<m_charWidth;i++)
        for (float j=0;j<m_charHeight;j++) {
            PixelChar pc;
            QVector<int> winner;
            winner.resize(lst.m_list.count());
            for (int y=0;y<8;y++)
                for (int x=0;x<8;x++) {
                    float xx = (i)*sx+x;
                    float yy = (j)*sy+y;
                    xx = (xx-img->width()/2)*m_importScaleX + img->width()/2;
                    yy = (yy-img->height()/2)*m_importScaleY + img->height()/2;


                    uchar col = 0;
                    if (xx>0 && xx<img->width() && yy>0 && yy<img->height())
                        col = lst.getIndex(QColor(img->pixel(xx,yy)));
                    if (col!=0) {
                        pc.p[y] |= 1<<x;
                        winner[col]++;
                    }
             }
            m_currencChar = m_charset->FindClosestChar(pc);
            int w=0;
            int iw = 0;
            for (int i=0;i<winner.count();i++)
                if (winner[i]>w) {
                    w=winner[i];
                    iw = i;
                }
            uchar col = iw;

 /*           if (col==0)
                m_currencChar = 0x20;
            else
                m_currencChar=0xA0;
*/
            m_writeType=Color;
            setPixel(i*sx,j*sy,col);
            m_writeType=Character;
            setPixel(i*sx,j*sy,col);
        }
    //   Reorganize();

}

void C64FullScreenChar::Initialize()
{
    DeleteAll();
    AddNew(m_charWidth, m_charHeight);

}

bool C64FullScreenChar::KeyPress(QKeyEvent *e)
{

    if (e->key()==Qt::Key_A)
        Prev();
    if (e->key()==Qt::Key_D)
        Next();


    SetColor(((C64Screen*)m_items[m_current])->m_data[1],0);

    if (e->key()==Qt::Key_C) {
        m_writeType=Character;
        if (m_currencChar==0x20) {
            m_currencChar = m_tempChar;
        }
        else {
            m_tempChar = m_currencChar;
            m_currencChar = 0x20;

        }

    }


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
    if (m_charset==nullptr) {
        return 0;
         }
    if (x>=320 || x<0 || y>=200 || y<0)
        return 0;

    int ox = x;
    int oy = y;
    x=x*m_charWidth/40.0;
    y=y*m_charHeight/25.0;

    uchar v = ((C64Screen*)m_items[m_current])->m_rawData[(x/8) + (y/8)*m_charWidth];
    uchar col = ((C64Screen*)m_items[m_current])->m_rawColors[(x/8) + (y/8)*m_charWidth];


    int ix = (x % 8);//- (dx*40);
    int iy = (y % 8);//- (dy*25);


    if (dynamic_cast<LImageNES*>(m_charset)!=nullptr) {
//        m_width=m_charWidth*8;
  //      m_height = m_charHeight*8;
        ix = ((x*2) % 16);//- (dx*40);
        iy = ((y*2) % 16);//- (dy*25);

        int xx = (v%m_charset->m_charWidthDisplay)*16;
        int yy = (v/m_charset->m_charWidthDisplay-1)*16 + m_footer.get(LImageFooter::POS_CURRENT_BANK)*16;
        m_charset->SetPalette(col);
        m_charset->m_footer.set(LImageFooter::POS_DISPLAY_CHAR,0);
        return m_charset->getPixel(xx+ix,yy+iy);
    }


    int pos = v;
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
    m_footer = mc->m_footer;
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

    int compressionType = m_exportParams["CompressionType"];

    if (!m_silentExport)
    if (compressionType<2 || compressionType>3) {

        QMessageBox msgBox;
        msgBox.setText("CompressionType must be either 2 or 3");
        msgBox.exec();
        return;
    }


    QVector<QByteArray> screens;
    QByteArray mty;
    mty.resize(m_charWidth*m_charHeight*2);
    mty.fill(0);
    screens.append(mty);
    QVector<bool> used;
    used.resize(256);
    used.fill(0);
    for (LImageContainerItem* li :m_items) {
        C64Screen* screen = dynamic_cast<C64Screen*>(li);
        QByteArray s;
        for (int i=0;i<m_charWidth*m_charHeight;i++) {
            s.append(screen->m_rawData[i]);
            //qDebug() << QString::number(screen->m_rawData[i]);
            used[(uchar)screen->m_rawData[i]]=1;
            s.append(screen->m_rawColors[i]);
        }
        screens.append(s);
    }
    MovieConverter mc;
    float compr;
    int cnt=0;

    QByteArray header;

    int cur=255;
    while (used[cur]!=0) {
        cur--;
    }
    uchar endChar = cur;
    cur--;
    while (used[cur]!=0) {
        cur--;
    }
    uchar skipChar = cur;


    qDebug() << "endChar " << endChar;
    qDebug() << "skipChar " << skipChar;
//    char endChar = (char)m_exportParams["EndChar"];
  //  char skipChar = (char)m_exportParams["SkipChar"];

    header.append(m_charWidth);
    header.append(m_charHeight);
    header.append(m_items.count());
    header.append(compressionType);
    header.append((char)m_exportParams["ExportTimeStamps"]);
    header.append(endChar);
    header.append(skipChar);

    file.write(header);



    for (int i=0;i<screens.count()-1;i++) {
        LImageContainerItem* li = m_items[i ];
        C64Screen* screen = dynamic_cast<C64Screen*>(li);

        QByteArray data;

        data.append(screen->m_data[1]);
//        data.append(screen->m_data[2]);

        if (compressionType==3)
        data.append(mc.CompressScreen3(screens[i], screens[i+1],
                m_charWidth, m_charHeight,compr,
                endChar,skipChar,true));

        if (compressionType==2)
        data.append(mc.CompressScreen2(screens[i], screens[i+1],
                m_charWidth, m_charHeight,compr,endChar,skipChar, true));


       // qDebug() << screen;

        if (m_exportParams["ExportTimeStamps"]!=0)
            data.append(screen->m_data[0]);
        file.write(data);
        cnt+=data.count();
    }
//    qDebug() << m_charWidth<< screens.count();
    float total = m_charHeight*m_charWidth*screens.count()*2;

    QMessageBox msgBox;
    QString s = "Original size: " + QString::number(total)+ "bytes\n";
    s += "Packed size: " + QString::number(cnt)+ "bytes\n";
    s += "Compression: " + QString::number((1-cnt/total)*100)+ " %\n";

    msgBox.setText(s);
    if (!m_silentExport)
        msgBox.exec();







}

void C64FullScreenChar::ImportC(QFile &file)
{
    QString txt = file.readAll();
    txt = txt.replace("\n","").replace("\r","");
    QStringList lst = txt.toLower().split("{");

    for (int i=1;i<lst.count();i+=1) {
        QString q = lst[i].split("}")[0];
        QStringList data = q.split(",");

        AddNew(m_charWidth, m_charHeight);
        C64Screen* s = dynamic_cast<C64Screen*>(m_items.last());
        s->m_data[1] = data[4].toInt();
        int k = 5;
        for (int i=0;i<m_charWidth*m_charHeight;i++) {
            s->m_rawData[i] =data[k].toInt();
            k++;
        }
        for (int i=0;i<m_charWidth*m_charHeight;i++) {
            s->m_rawColors[i] =data[k].toInt();
            k++;
        }

    }

}

void C64FullScreenChar::Transform(int x, int y)
{
    C64FullScreenChar copy(m_colorList.m_type);
    copy.CopyFrom(this);
    for (int j=0;j<m_charHeight;j++)
        for (int i=0;i<m_charWidth;i++) {

            C64Screen* source = dynamic_cast<C64Screen*>(copy.m_items[m_current]);
            C64Screen* cur = dynamic_cast<C64Screen*>(m_items[m_current]);
            cur->m_rawData[j*m_charWidth+i] = source->m_rawData[((j+y)%m_charHeight)*m_charWidth+(i+x)%m_charWidth];
            cur->m_rawColors[j*m_charWidth+i] = source->m_rawColors[((j+y)%m_charHeight)*m_charWidth+(i+x)%m_charWidth];

        }
}

QString C64FullScreenChar::getMetaInfo()
{
    QString txt="";
    m_charWidth = getMetaParameter("screen_width")->value;
    m_charHeight = getMetaParameter("screen_height")->value;

    txt+= "The screen animation...";


    return txt;
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


    //    qDebug() << "w h " << QString::number(m_charWidth) << " " << QString::number(m_charHeight);
    //  qDebug() << "cnt " << QString::number(v);

    char tmp = 0;
    for (int i=0;i<11;i++)
        file.write( ( char * )( &tmp), 1 );

    for (LImageContainerItem* li : m_items){
//        qDebug() << "Size: " << li->ToQByteArray(0).count();
        file.write(li->ToQByteArray(0));
    }
    AppendSaveBinCharsetFilename(file);


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
    LoadBinCharsetFilename(file);


}

