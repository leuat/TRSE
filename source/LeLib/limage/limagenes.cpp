#include "limagenes.h"


LImageNES::LImageNES(LColorList::Type t) : CharsetImage(t)
{
    m_width = 256;
    m_height = 256;
    m_bitMask = 0b1;

    m_scaleX = 1.0f;
    m_noColors = 4;
    m_scale = 2;


    m_charWidth = 32;
    m_charHeight = 32;


    m_currentBank = 1;
    //m_data = new PixelChar[m_charWidth*m_charHeight];
    m_charWidthDisplay=16;
    m_charHeightDisplay=16;

//    m_double=false;
    Clear();
    m_type = LImage::Type::NES;
    m_supports.asmExport = true;
    m_supports.binaryLoad = true;
    m_supports.binarySave = true;
    m_supports.nesPalette = true;
    m_supports.koalaImport = false;
    m_supports.koalaExport = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.compressedExport = false;
    m_supports.displayForeground = true;
    m_supports.displayBank = true;
    m_supports.displayCmbColors = true;
    m_supports.displayColors = true;

    m_GUIParams[btnLoadCharset] ="";
    m_GUIParams[btn1x1] = "8x8";
    m_GUIParams[btn2x2] = "16x16";
    m_GUIParams[btn2x2repeat] = "16x16 tiled";
    m_GUIParams[btnCopy] = "Copy";
    m_GUIParams[btnPaste] = "Paste";
    m_GUIParams[btnFlipH] = "Flip H";
    m_GUIParams[btnFlipV] = "Flip V";

    m_GUIParams[tabCharset] = "Charset";
    m_GUIParams[tabData] = "";
    m_GUIParams[tabLevels] = "";
    m_GUIParams[tabEffects] = "Effects";

    m_GUIParams[btnEditFullCharset] = "Full charset";

    m_exportParams["StartX"] = 0;
    m_exportParams["EndX"] = m_charWidth;
    m_exportParams["StartY"] = 0;
    m_exportParams["EndY"] = m_charHeight;
    m_exportParams["Compression"] = 0;

    m_GUIParams[col1] = "Background";
    m_GUIParams[col2] = "Color 1";
    m_GUIParams[col3] = "Color 2";
    m_GUIParams[col4] = "Color 3";


    for (int i=0;i<4;i++)
        m_extraCols[i] = 0;

    //ForceColor();
}

void LImageNES::ImportBin(QFile &file)
{
    QByteArray data = file.readAll();
    int j=0;
    for (int i=0;i<m_charWidth*m_charHeight;i++) {
        for (int k=0;k<8;k++)
            m_data[i].p[k] = PixelChar::reverse(data[j+k]);
        j+=8;
    }
    ForceColor();
}

void LImageNES::ExportBin(QFile &file)
{
    QByteArray data;
    int j=0;
    for (int i=0;i<m_charWidth*m_charHeight;i++) {
        for (int k=0;k<8;k++)
            data.append( PixelChar::reverse(m_data[i].p[k]));
//        data.append( m_data[i].p[k]);
    }
    file.write(data);


    QString f = file.fileName();

    QString of = f.split(".")[0] + ".pal";
    if (QFile::exists(of))
        QFile::remove(of);
    QFile wf(of);
    wf.open(QFile::WriteOnly);
    QByteArray ba = m_colorList.m_nesPPU;

    for (int i=0;i<8;i++) {
        ba[i*4+0] = m_colorList.m_nesPPU[0];
        ba[i*4+1] = m_colorList.m_nesPPU[i*4+2];
        ba[i*4+2] = m_colorList.m_nesPPU[i*4+1];
        ba[i*4+3] = m_colorList.m_nesPPU[i*4+3];
    }

    wf.write(ba);
    wf.close();

}

void LImageNES::setMultiColor(bool doSet)
{
    ForceColor();
}

void LImageNES::ForceColor()
{
}

void LImageNES::setForeground(unsigned int col)
{
//    m_cols[1] = col;
    SetColor(col, 1);
 //   qDebug() << "HERE";
}


void LImageNES::SaveBin(QFile &file)
{
    file.write( ( char * )( &m_data ),  m_charWidth*m_charHeight*12 );
    file.write(m_colorList.m_nesPPU);

}

bool LImageNES::KeyPress(QKeyEvent *e)
{
    CharsetImage::KeyPress(e);

    if (e->key()==Qt::Key_0 ) { Data::data.currentColor = m_cols[3];}
    if (e->key()==Qt::Key_1 ) { Data::data.currentColor = m_cols[0];}
    if (e->key()==Qt::Key_2 ) { Data::data.currentColor = m_cols[1];}
    if (e->key()==Qt::Key_3 ) { Data::data.currentColor = m_cols[2];}

    return true;
}

void LImageNES::LoadBin(QFile &file)
{
    file.read( ( char * )( &m_data ),  m_charWidth*m_charHeight*12 );
    m_colorList.m_nesPPU = file.read(0x20);
    m_cols[0] = m_colorList.m_nesPPU[0];
    m_cols[1] = m_colorList.m_nesPPU[1];
    m_cols[2] = m_colorList.m_nesPPU[2];
    m_cols[3] = m_colorList.m_nesPPU[3];

    for (PixelChar& pc: m_data)
        for (int i=0;i<4;i++)
            pc.c[i] = m_cols[i];
}

QPixmap LImageNES::ToQPixMap(int chr)
{
    //QImage img = m_data[chr].toQImage(64, m_bitMask, m_colorList, m_scale);
    int sz = 32;
    QImage img = QImage(sz,sz,QImage::Format_RGB32);
//    qDebug() << (chr*8)%16;
    int xx = 0;
    int yy = m_currentBank*16;


    int c= 0;
    while (c<chr) {
        if (xx++>=15) {
            xx=0;
            yy++;
        }
        c++;
    }
    xx*=8;
    yy*=8;
    bool isDouble = m_double;
    m_double = false;
    Mode m = m_currentMode;
    m_currentMode = Mode::FULL_IMAGE;
    for (int i=0;i<sz;i++)
        for (int j=0;j<sz;j++)
            img.setPixel(i,j,m_colorList.get(getPixel(
                                                 (int)(i/(float)sz*8)+xx,
                                                 (int)(j/(float)sz*8)+yy)
                         ).color.rgb());

    m_double = isDouble;
    m_currentMode = m;
    return QPixmap::fromImage(img);

}

void LImageNES::SetPalette(int pal)
{
     m_cols[2-1] = m_colorList.m_nesPPU[pal*4 +1 +0];
     m_cols[2-0] = m_colorList.m_nesPPU[pal*4 +1 +1];
     m_cols[2-2] = m_colorList.m_nesPPU[pal*4 +1 +2];
     m_cols[3] = m_colorList.m_nesPPU[0];
}

bool LImageNES::getXY(QPoint& xy,QPoint& p1, QPoint& p2)
{
    int x = xy.x();
    int y = xy.y();
    if (m_double) {
        x=x/2;
        y=y/2;
    }

    if (m_currentMode==Mode::CHARSET1x1) {
        int sx = (m_currencChar%m_charWidth)*8;
        int sy = (m_currencChar/m_charWidth)*16;
        x = (x / (float)m_width)*16+sx;
        y = (y / (float)m_height)*16+sy;
    }

    if (m_currentMode==Mode::CHARSET2x2) {
        int sx = (m_currencChar%m_charWidth)*8;
        int sy = (m_currencChar/m_charWidth)*16;
        x = (x / (float)m_width)*32+sx;
        y = (y / (float)m_height)*32+sy;
    }
    if (m_currentMode==Mode::CHARSET2x2_REPEAT) {
        int sx = (m_currencChar%m_charWidth)*8;
        int sy = (m_currencChar/m_charWidth)*16;
        x = (int)((x / (float)m_width)*96)%16+sx;
        y = (int)((y / (float)m_height)*96)%16+sy;
    }


    int r = x/(float)8;
    x=x+r*8;
    if (m_double)
        y=y+128*m_currentBank;

//    m_pc1 = &getPixelChar((x/2),y);
  //  m_pc2 = &getPixelChar((x/2)+4,y);

    p1 = QPoint(x/2,y);
    p2 = QPoint((x/2)+4,y);


    int ix = x %8;//- (dx*m_charWidth);
    int iy = y % 8;//- (dy*m_charHeight);

    x = ix;
    y = iy;

    if (x<0 || x>=16 || y<0 || y>=8)
        return false;


    xy = QPoint(x,y);
    return true;
}

QString LImageNES::getMetaInfo() {
    QString txt = "The NES CHR image is your standard 8kb sprite/tileset (2x256 8x8 pixels) for any 40kb NES project. \n\n";
    txt+="Data are stored in 2 bitplanes, allowing for a total of 4 colours. The NES CHR consists of 2 \"pages\", where one contains sprite data while the other tile data (can be set programmatically on the NES).\n\n ";
    txt+="Each of these pages consists of 256 8x8 pixel characters that are usually combined into 16x16 or larger blocks. \n\n";
    txt+="From a palette of 54 unique colors, you can define 8 attribute palettes of 4 colours each (4 for sprites, 4 for tiles). This palette file is exported ";
    txt+="as a binary file, and can be used directly TRSE's built-in SetPalette NES method. ";
    return txt;
}



unsigned int LImageNES::getPixel(int x, int y)
{
    if (x>=m_width || x<0 || y>=m_height || y<0)
        return m_cols[0];

    QPoint xy = QPoint(x,y);

    QPoint p1, p2;
    if (!getXY(xy, p1, p2))
        return m_cols[0];

    PixelChar& m_pc1 = getPixelChar(p1.x(), p1.y());
    PixelChar& m_pc2 = getPixelChar(p2.x(), p2.y());

    unsigned char pp = 3-((((m_pc1.p[xy.y()])>>xy.x()) & 0b1) | (((m_pc2.p[xy.y()])>>xy.x()) & 0b1)*2);

    return m_cols[pp];


//    return pc.get(m_scale*ix, iy, m_bitMask);

}

void LImageNES::setPixel(int x, int y, unsigned int col)
{
    if (x>=m_width || x<0 || y>=m_height || y<0)
        return;

    QPoint xy = QPoint(x,y);

    QPoint p1, p2;
    if (!getXY(xy, p1, p2))
        return;

    PixelChar& m_pc1 = getPixelChar(p1.x(), p1.y());
    PixelChar& m_pc2 = getPixelChar(p2.x(), p2.y());

    int j=0;
    for (int i=0;i<4;i++)
        if (m_cols[i]==col)
            j=3-i;

     m_pc1.p[xy.y()] &= ~(1<<xy.x());
     m_pc2.p[xy.y()] &= ~(1<<xy.x());
     m_pc1.p[xy.y()] |= (j&1)<<xy.x();
     m_pc2.p[xy.y()] |= ((j&3)>>1)<<xy.x();
    //return m_cols[pp];


}

void LImageNES::SetColor(uchar col, uchar idx)
{
    int li = idx;
    if (li==0)
        li=3;
    else
        if (li==3)
            li=0;

    m_cols[li] = col;
    if (idx!=0)
        m_colorList.SetPPUColors(col, idx-1);


    else m_colorList.m_nesPPU[0]=col;

//    qDebug() << "SETTING COLOR : " << idx <<  "   WITH COLOR " <<QString::number(col);

   // qDebug() << "Setting idx " << idx << " to  "<< QString::number(col);

}

void LImageNES::CopyFrom(LImage *img)
{
    LImageNES* n = dynamic_cast<LImageNES*>(img);
    for (int i=0;i<4;i++)
        m_cols[i] = n->m_cols[i];
    m_currentBank = n->m_currentBank;
    CharsetImage::CopyFrom(img);
}
