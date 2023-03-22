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
#include "limageqimage.h"

C64Screen C64FullScreenChar::m_copy;


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

    m_supports.d800_limit = false;
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
    if (idx==0) ((C64Screen*)m_items[m_current].get())->m_data[1] = col;
 //   m_extraCols[idx] = col;
}

void C64FullScreenChar::Clear(int val=0)
{
    m_items[m_current]->Clear(val);

}

void C64FullScreenChar::ImportBin(QFile &f)
{
    C64Screen* s = (C64Screen*)m_items[m_current].get();
    s->m_rawData = f.read(1000);
    s->m_rawColors = f.read(1000);

}

void C64FullScreenChar::ExportBin(QFile &f)
{

    C64Screen* s = (C64Screen*)m_items[m_current].get();

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

void C64FullScreenChar::OrdererdDither(QImage &img, LColorList &colors, QVector3D strength, QPoint size, float gamma)
{
    if (m_charset==nullptr) {
        qDebug() << "C64FullScreenChar::orderedDither NO CHARSET loaded";
        return;
    }
    fromQImage(&img, colors);

}

void C64FullScreenChar::fromQImage(QImage *img, LColorList &lst)
{


    float sx = img->width()/m_charWidth;
    float sy = img->height()/m_charHeight;

 //   float sy = img->height()/25;
//    qDebug() <<"WHOO" << m_charWidth << m_charHeight << sx << sy << img->width() << img->height();
   // img->save("test2.png");

    lst.m_selectClosestFromPen = false;
    LImageQImage chr(LColorList::C64);
    chr.Initialize(img->width(),img->height());
 //   chr.m_colorList.CopyFrom(&m_colorList);
//    chr.m_colorList.EnableColors(m_colorList.m_enabledColors);
    chr.m_colorList.EnableColors(QVector<int>()<< 0<<1);
    chr.m_colorList.m_selectClosestFromPen = false;
    chr.FloydSteinbergDither(*img,chr.m_colorList,true,1);
    QImage out(chr.m_width, chr.m_height,QImage::Format_RGB32);
    chr.ToQImage(chr.m_colorList,out,1,QPointF(160,100));
   // out.save("test.png");
    m_charset->m_ignoreValues = m_ignoreValues;
//#pragma omp parallel for
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


                    int col = 0;
                    if (xx>0 && xx<img->width() && yy>0 && yy<img->height())
           //             col = lst.getIndex(QColor(img->pixel(xx,yy)));
                    lst.getClosestColor( lst.get(chr.getPixel(xx,yy)).color, col);
                    if (col!=getBackground() && col<winner.count()) {
                        pc.p[y] |= 1<<x;
                        winner[col]++;
                    }
             }
            m_currentChar = m_charset->FindClosestChar(pc);
            int w=0;
            int iw = 0;
            for (int i=0;i<winner.count();i++)
                if (winner[i]>w) {
                    w=winner[i];
                    iw = i;
                }
            uchar col = iw;

 /*           if (col==0)
                m_currentChar = 0x20;
            else
                m_currentChar=0xA0;
*/
            /*
            m_classvariableType=Color;
            setPixel(i*sx,j*sy,col);
            m_classvariableType=Character;
            setPixel(i*sx,j*sy,col);
            */
            m_classvariableType=Color;
            setPixel(i*8,j*8,col);
            m_classvariableType=Character;
            setPixel(i*8,j*8,col);
        }
    //   Reorganize();

}

QByteArray C64FullScreenChar::getDirArt()
{
    C64Screen* s = (C64Screen*)m_items[0].get();
    QByteArray qb;
    uchar illegal = 0;
    for (int i=0;i<m_charHeight;i++) {
        QByteArray ba;
        for (int j=0;j<16;j++) {
            uchar c = (uchar)s->m_rawData[m_charWidth*i+j];
            /*
            Screen codes $A0-$BF - no petscii equivalent
            Screen codes $E0-$FF - no petscii equivalent
            Screen code $8D, PETSCII code $0D - newline
            Screen code $CD, PETSCII code $8D - line feed
            Screen code $60, PETSCII code $A0 - used in the disk to mark end of filename
            */
            if ((c>=0xA0 && c<=0xBF) || (c>0xE0 && c<0xFF) || c==0x0d || c==0x8D)
                    c = illegal;
            ba.append(c);
        }
        qb.append(ba);
    }
    return qb;
}

void C64FullScreenChar::ReInitialize()
{
    m_charWidth = getMetaParameter("screen_width")->value;
    m_charHeight = getMetaParameter("screen_height")->value;
    m_width = m_charWidth*8;
    m_height = m_charHeight*8;
    DeleteAll();
    AddNew(m_charWidth, m_charHeight);
    emit emitImportRom();
   // qDebug() << "WOOT reinit "<<m_charWidth <<m_charHeight<< m_width << m_height;

}

void C64FullScreenChar::Initialize(int width, int height) {
//    exit(1);
    m_width = width;
    m_height = height;
    //m_charWidth = width/8;
   // m_charHeight = height/8;
//    qDebug() << "Addnew Initing : " <<m_charWidth<<m_charHeight << width << height;
//    DeleteAll();
  //  AddNew(m_charWidth, m_charHeight);

   // ReInitialize();
}

void C64FullScreenChar::setBackground(unsigned int col)
{
    //qDebug() << "SetBackrouund VIC "<<col;
    m_colorList.setPen(0,col);
    for (int i=0;i<m_charWidth*m_charHeight;i++) {
        m_data[i].c[0] = col;
    }

//    C64Screen* cur = ((C64Screen*)m_items[m_current]);

//qDebug() << "Setting bg col "<< Util::numToHex(col);


    if (m_charset!=nullptr)
        m_charset->setBackground(col);
}

bool C64FullScreenChar::KeyPress(QKeyEvent *e)
{
    if (e->key()==Qt::Key_A)
        Prev();
    if (e->key()==Qt::Key_D)
        Next();


    SetColor(((C64Screen*)m_items[m_current].get())->m_data[1],0);

    if (e->key()==Qt::Key_C) {
        m_classvariableType=Character;
        if (m_currentChar==0x20) {
            m_currentChar = m_tempChar;
        }
        else {
            m_tempChar = m_currentChar;
            m_currentChar = 0x20;

        }

    }


/*    SetColor(m_extraCols[0],0);
    SetColor(m_extraCols[1],1);
    SetColor(m_extraCols[2],2);
    */
    return true;
}

void C64FullScreenChar::AddNew(int w, int h) {
    C64Screen* s = new C64Screen();

    s->Init(m_charWidth, m_charHeight);
//    qDebug() << "Addnew Initing : " <<m_charWidth<<m_charHeight;
    m_items.append(QSharedPointer<C64Screen>(s));
    m_current = m_items.count()-1;
}

void C64FullScreenChar::setPixel(int x, int y, unsigned int color)
{
//    m_width=320;
    if (x>=m_width || x<0 || y>=m_height || y<0)
        return;
    //x=x*m_charWidth/40.0;
    //y=y*m_charHeight/25.0;

//    x=x*m_charWidth/40.0;
 //   y=y*m_charHeight/25.0;


    if (isMultiColor())
        x*=2;


    if (m_classvariableType==Character || m_forcePaintColorAndChar)
        ((C64Screen*)m_items[m_current].get())->m_rawData[x/8+ (y/8)*m_charWidth] = m_currentChar;
    if (m_classvariableType==Color || m_forcePaintColorAndChar)
        ((C64Screen*)m_items[m_current].get())->m_rawColors[x/8+ (y/8)*m_charWidth] = color;



    //BuildImage();
}

unsigned int C64FullScreenChar::getPixel(int x, int y)
{
//    m_width=320;
    if (m_charset==nullptr) {
        return 0;
         }
    if (x>=m_width || x<0 || y>=m_height || y<0)
        return 0;

    int ox = x;
    int oy = y;
  // x=x*m_charWidth/40.0;
  //  y=y*m_charHeight/25.0;
    if (isMultiColor())
        x*=2;
    int pp = (x/8) + (y/8)*m_charWidth;
 //   if (rand()%100000>99998)
   //     qDebug() << m_charWidth << m_charHeight << m_width << m_height;
    C64Screen* cur = ((C64Screen*)m_items[m_current].get());
    uchar v = 0, col = 0;
    if (pp<cur->m_rawData.length())
        v = cur->m_rawData[pp];

    if (pp<cur->m_rawColors.length())
        col = cur->m_rawColors[pp];


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

//    if (val==0)
//        val = m_colorList.getPen(0);

//    if (rand()%1000>990)
  //      qDebug() << Util::numToHex(val);
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
        m_height = c->m_height;
        m_width = c->m_width;
        m_bitMask = c->m_bitMask;
        m_forcePaintColorAndChar  = mc->m_forcePaintColorAndChar;
        DeleteAll();
        for (auto li: c->m_items) {
            C64Screen* s= (C64Screen*)li.get();
            C64Screen* s2= new C64Screen();
            *s2 = *s;

            m_items.append(QSharedPointer<C64Screen>(s2));
        }


//        m_items = c->m_items;
        m_current = c->m_current;

        m_charset = c->m_charset;
        m_classvariableType = c->m_classvariableType;
        m_currentChar = c->m_currentChar;

    }
    else
    LImage::CopyFrom(mc);

}

void C64FullScreenChar::ExportMovie(QFile &file)
{

    int compressionType = m_exportParams["CompressionType"];

    if (m_exportParams["export1"]==4) {
        int org = m_current;
        for (int i=0;i<m_items.count();i++) {
            m_current = i;
            ExportBin(file);
        }
        m_current = org;
        return;
    }

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
    for (auto li :m_items) {
        auto screen = qSharedPointerDynamicCast<C64Screen>(li);
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


  //  qDebug() << "endChar " << endChar;
  //  qDebug() << "skipChar " << skipChar;
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
        LImageContainerItem* li = m_items[i ].get();
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
        cnt+=data.length();
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

    QString bg;
    QString border;
    for (int i=1;i<lst.count();i+=1) {
        QString q = lst[i].split("}")[0];
        QStringList data = q.split(",");

        AddNew(m_charWidth, m_charHeight);
        C64Screen* s = dynamic_cast<C64Screen*>(m_items.last().get());
//        qDebug() <<data[4];
        bg+=data[3].remove("colors")+ ", ";
        border+=data[4]+ ", ";
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
    qDebug() << "Border colours:";
    qDebug() << bg;
    qDebug() << "Background colours:";
    qDebug() << border;

}

void C64FullScreenChar::Transform(int x, int y)
{
    C64FullScreenChar copy(m_colorList.m_type);
    copy.CopyFrom(this);
    for (int j=0;j<m_charHeight;j++)
        for (int i=0;i<m_charWidth;i++) {

            C64Screen* source = dynamic_cast<C64Screen*>(copy.m_items[m_current].get());
            C64Screen* cur = dynamic_cast<C64Screen*>(m_items[m_current].get());
            cur->m_rawData[j*m_charWidth+i] = source->m_rawData[((j+y)%m_charHeight)*m_charWidth+(i+x)%m_charWidth];
            cur->m_rawColors[j*m_charWidth+i] = source->m_rawColors[((j+y)%m_charHeight)*m_charWidth+(i+x)%m_charWidth];

        }
}


QString C64FullScreenChar::getMetaInfo()
{
    QString txt="";
    m_charWidth = getMetaParameter("screen_width")->value;
    m_charHeight = getMetaParameter("screen_height")->value;
//    Initialize();
    txt+= "The screen animation...";


    return txt;
}

void C64FullScreenChar::setMultiColor(bool doSet)
{
    if (doSet) {
        m_bitMask = 0b11;
        m_noColors = 4;
        m_scale = 2;
        m_minCol = 0;
        m_colorList.InitC64Multicolor();
    }
    else {
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

void C64FullScreenChar::CopyChar()
{
    if (m_current<0) return;
    m_copy = *((C64Screen*)m_items[m_current].get());
}

void C64FullScreenChar::PasteChar()
{
    if (m_copy.m_data.length()==0)
        return;

    //    if (m_items[m_current].m_height == m_copy.m_height)
    *((C64Screen*)m_items[m_current].get())=m_copy;

}



void C64FullScreenChar::SaveBin(QFile& file)
{
    char dummy =  m_colorList.getPen(0);

    file.write( ( char * )( &dummy),  1 );
    file.write( ( char * )( &dummy ), 1 );
    file.write( ( char * )( &m_charWidth ),  1 );
    file.write( ( char * )( &m_charHeight), 1 );
    uchar v = m_items.count();
    file.write( ( char * )( &v), 1 );


    //    qDebug() << "w h " << QString::number(m_charWidth) << " " << QString::number(m_charHeight);
    //  qDebug() << "cnt " << QString::number(v);

    char tmp = 0;
    for (int i=0;i<11;i++)
        file.write( ( char * )( &tmp), 1 );

    for (auto li : m_items){
//        qDebug() << "Size: " << li->ToQByteArray(0).count();
        file.write(li->ToQByteArray(0));
    }
    AppendSaveBinCharsetFilename(file);


}

void C64FullScreenChar::LoadBin(QFile& file)
{
    uchar dummy = 0;
    file.read( ( char * )( &dummy ),1 );
    m_colorList.setPen(0,dummy); // background
    file.read( ( char * )( &dummy ), 1);
    file.read( ( char * )( &m_charWidth ),  1 );
    file.read( ( char * )( &m_charHeight), 1 );
    uchar cnt;
    file.read( ( char * )( &cnt), 1 );
//qDebug() << m_charWidth << m_charHeight;
    char tmp = 0;
    for (int i=0;i<11;i++)
        file.read( ( char * )( &tmp), 1 );

    m_items.clear();
    Initialize(8*m_charWidth, 8*m_charHeight);
//    m_width = ;
  //  m_height = 8*m_charHeight;
    for (int i=0;i<cnt;i++) {
        C64Screen* s = new C64Screen();

        s->Init(m_charWidth, m_charHeight);
        s->m_rawColors = file.read( m_charWidth*m_charHeight);
        s->m_rawData = file.read(m_charWidth*m_charHeight);
        s->m_data = file.read(16);
        m_items.append(QSharedPointer<C64Screen>(s));
    }
    m_current = 0;
    LoadBinCharsetFilename(file);


}

