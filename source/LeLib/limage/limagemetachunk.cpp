#include "limagemetachunk.h"
#include "source/LeLib/limage/limageio.h"


LImageMetaChunk::LImageMetaChunk(LColorList::Type t) : LImageNES(t)
{
    m_GUIParams[tabCharset] = "1";
    m_GUIParams[btnLoadCharset] ="Load Charset";
    m_width = 256;
    m_height = 256;
    m_supports.displayColors = false;

    Initialize(m_width,m_height);
    if (t==LColorList::NES) {
        m_img = new LImageNES(t);

    }
    m_type = LImage::Type::LMetaChunk;
    m_colorList.m_type = m_img->m_colorList.m_type;
    m_colorList.m_list = m_img->m_colorList.m_list;
    AddNew(2,2);

    m_supports.displayBank = true;

    m_charWidthDisplay = 16;
    m_supports.asmExport = false;
    m_supports.binaryLoad = true;
    m_supports.binarySave = true;
    m_supports.nesPalette = true;
    m_supports.koalaImport = false;
    m_supports.koalaExport = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.compressedExport = false;
    m_supports.displayForeground = true;
    m_supports.displayCmbColors = false;
    m_supports.displayColors = true;

    m_GUIParams[btnLoadCharset] ="Load charset";
    m_GUIParams[btn1x1] = "";
    m_GUIParams[btn2x2] = "";
    m_GUIParams[btn2x2repeat] = "";
    m_GUIParams[btnCopy] = "";
    m_GUIParams[btnPaste] = "";
    m_GUIParams[btnFlipH] = "";
    m_GUIParams[btnFlipV] = "";

    m_GUIParams[tabCharset] = "Charset";
    m_GUIParams[tabData] = "";
    m_GUIParams[tabLevels] = "";
    m_GUIParams[tabEffects] = "Effects";

    m_GUIParams[btnEditFullCharset] = "";

    m_GUIParams[tabSprites] ="Metachunks";



}

QPoint LImageMetaChunk::getPos(int x, int y)
{
    LMetaChunkItem* cur = getCur();
    if (cur==nullptr)
        return QPoint(0,0);
    int xx = (int)((x/(float)m_width)*cur->m_width);
    int yy = (int)((y/(float)m_height)*cur->m_height);
    return QPoint(xx,yy);
}

void LImageMetaChunk::CopyFrom(LImage *mc)
{
    LImageMetaChunk* img = dynamic_cast<LImageMetaChunk*>(mc);
    if (img!=nullptr) {
        m_img = img->m_img;
        m_charset= img->m_charset;
        m_currencChar = img->m_currencChar;
/*        for (LImageContainerItem* mci : img->m_items) {
            LMetaChunkItem *m = dynamic_cast<LMetaChunkItem*>(mci);
            AddNew(m->m_width, m->m_height);
            m_current = m_items.count()-1;
            getCur()->m_data = m->m_data;
        }*/
        m_items.clear();
        AddNew(img->getCur()->m_width, img->getCur()->m_height);
        getCur()->m_attributes = img->getCur()->m_attributes;
        getCur()->m_data = img->getCur()->m_data;
        //m_items[0] = img->m_items[img->m_current];


//        m_items = img->m_items;
        m_colorList.m_nesPPU = img->m_colorList.m_nesPPU;
            m_type = img->m_type;

        if (m_charset!=nullptr) {
             m_colorList.m_curPal = m_charset->m_colorList.m_curPal;
//             m_currentBank = img->m_charset->m_currentBank;
        }
//        m_current = img->m_current;
        m_current = 0;
        m_currentBank = img->m_currentBank;
 /*       m_colorList.m_list.clear();
        for (LColor l :  img->m_colorList.m_list)
            m_colorList.m_list.append(l);
        m_colorList.m_type = img->m_colorList.m_type;*/
    }
}

void LImageMetaChunk::Copy()
{

}


void LImageMetaChunk::LoadCharset(QString file, int skipBytes)
{
    m_charset = LImageIO::Load(file);
    if (m_charset->m_colorList.m_type!=m_img->m_colorList.m_type) {
        qDebug() << "Incompatible charset type!";
        m_charset = nullptr;

    }
    m_colorList.m_list = m_charset->m_colorList.m_list;
    m_colorList.m_nesPPU = m_charset->m_colorList.m_nesPPU;
    m_colorList.m_curPal = m_charset->m_colorList.m_curPal;
    LImageNES* nes = dynamic_cast<LImageNES*>(m_charset);
    if (nes!=nullptr) {
        nes->m_double = false;
        nes->m_currentBank=0;
    }
    m_charsetFilename = file;
//    qDebug() << "charset " << m_charset;
//    m_charset = LImageIO::
}

void LImageMetaChunk::setPixel(int x, int y, unsigned int color)
{
    QPoint p = getPos(x,y);
//    if (rand()%100>98)
  //      qDebug() << QString::number(m_currencChar);

    ((LMetaChunkItem*)m_items[m_current])->setPixel(p.x(),p.y(),m_currencChar,m_img->m_bitMask);

}


unsigned int LImageMetaChunk::getPixel(int x, int y)
{
    QPoint p = getPos(x,y);
//    qDebug() << p;
    if (m_current>=m_items.count())
        return 0;


    uchar val = ((LMetaChunkItem*)m_items[m_current])->getPixel(p.x(),p.y(),m_img->m_bitMask);

//    if (rand()%100>97 && val!=0)
  //  qDebug() << "Vals : " << QString::number(val);
//    val = 4;
    int xp = x/(float)m_width*m_pixelWidth*getCur()->m_width;
    int yp = y/(float)m_height*m_pixelHeight*getCur()->m_height;;

    int xx = ((val%m_charWidthDisplay)*m_pixelWidth) + xp%m_pixelWidth;
    int yy = ((val/(int)16)*m_pixelHeight)  +yp%m_pixelHeight;
    yy=yy+ 16*8*m_currentBank;


    if (m_charset==nullptr) {
        return 0;
    }
    else return m_charset->getPixel(xx,yy);

}

void LImageMetaChunk::SaveBin(QFile &file)
{
    uchar no = m_items.count();
    file.write( ( char * )( &no ),  1 );
    for (LImageContainerItem* li : m_items) {
        LMetaChunkItem *m = dynamic_cast<LMetaChunkItem*>(li);
        uchar w = m->m_width;
        uchar h = m->m_height;
        file.write( ( char * )( &w ),  1 );
        file.write( ( char * )( &h ),  1 );
        file.write(m->m_data);
    }

    QByteArray fd =  QByteArray(m_charsetFilename.toLatin1());
    uchar len = fd.count();
    file.write( ( char * )( &len ),  1 );
    file.write(fd);
}

void LImageMetaChunk::LoadBin(QFile &file)
{
    m_items.clear();
    uchar no;
    file.read( ( char * )( &no ),  1 );
//    qDebug() << "NO read : " << QString::number(no);
    for (int i=0;i<no;i++) {
        uchar w,h;
        file.read( ( char * )( &w ),  1 );
        file.read( ( char * )( &h ),  1 );
  //      qDebug() << "W read : " << QString::number(w);
        AddNew(w,h);
        getCur()->m_data = file.read(w*h);
    }
    if (file.atEnd())
        return;
    uchar len;
    file.read( ( char * )( &len ),  1 );
    QByteArray data = file.read(len);
    m_charsetFilename = QString::fromLatin1(data);
    if (QFile::exists(m_charsetFilename))
        LoadCharset(m_charsetFilename,0);

}

void LImageMetaChunk::SetColor(uchar col, uchar idx)
{
    if (m_charset==nullptr)
        return;
    m_charset->SetColor(col, idx);
}

void LImageMetaChunk::SetColor(uchar col, uchar idx, LImageMetaChunk &s)
{
    m_currencChar = col;
}

LMetaChunkItem *LImageMetaChunk::getCur()
{
    if (m_current>=m_items.count())
        return nullptr;
    return (LMetaChunkItem*)m_items[m_current];

}

bool LImageMetaChunk::KeyPress(QKeyEvent *e)
{
    if (e->key()==Qt::Key_A)
        Prev();
    if (e->key()==Qt::Key_D)
        Next();

    if (e->key()==Qt::Key_Plus)
        m_currencChar++;
    if (e->key()==Qt::Key_Minus)
        m_currencChar--;


    return false;
}

void LImageMetaChunk::CopyChar()
{

}

void LImageMetaChunk::PasteChar()
{

}

/*void LImageMetaChunk::ToQImage(LColorList &lst, QImage &img, float zoom, QPointF center)
{

}
*/
void LImageMetaChunk::AddNew(int w, int h)
{
    LMetaChunkItem* s = new LMetaChunkItem();
    s->Init(w,h);
    m_items.append(s);
    m_current = m_items.count()-1;


}

void LImageMetaChunk::FlipHorizontal()
{

}

void LImageMetaChunk::FlipVertical()
{

}

void LImageMetaChunk::Transform(int x, int y)
{

}

QByteArray LMetaChunkItem::ToQByteArray(int mask)
{
    return QByteArray();
}

void LMetaChunkItem::setPixel(float x, float y, uchar color, uchar bitMask)
{
    if (x<0 || x>=m_width || y<0 || y>=m_height)
        return;
    m_data[(int)(y*m_width+x)] = color;
}

void LMetaChunkItem::setPixelAttrib(float x, float y, uchar color, uchar bitMask)
{
    if (x<0 || x>=m_width || y<0 || y>=m_height)
        return;
    m_attributes[(int)(y*m_width+x)] = color;
}

uchar LMetaChunkItem::getPixel(float x, float y, uchar bitMask)
{
    if (x<0 || x>=m_width || y<0 || y>=m_height)
        return 0;
    return m_data[(int)(y*m_width+x)];


}

uchar LMetaChunkItem::getPixelAttrib(float x, float y, uchar bitMask)
{
    if (x<0 || x>=m_width || y<0 || y>=m_height)
        return 0;
    return m_attributes[(int)(y*m_width+x)];


}


void LImageMetaChunk::setForeground(unsigned int col)
{
    if (m_charset==nullptr)
        return;

    m_charset->SetColor(col, 1);
}

unsigned int LImageMetaChunk::getCharPixel(int pos,  int pal,int x, int y)
{
    if (pos>=m_items.count())
        return 0;

    m_current = pos;

    int w=16;//(getCur()->m_width);
    int h=(getCur()->m_height)*8;

    x=(x%w)*w;
    y=(y%h)*h;
    m_charset->SetPalette(pal);
    return getPixel(x,y);
}

QPixmap LImageMetaChunk::ToQPixMap(int chr)
{
    int sz = 64;
    QImage img = QImage(sz,sz,QImage::Format_RGB32);
    m_current = chr;
    for (int y=0;y<sz;y++)
        for (int x=0;x<sz;x++) {
            int i = (float)x/(float)sz*(float)m_width;
            int j = (float)y/(float)sz*(float)m_height;
            int c = getPixel(i,j);
            img.setPixel(x,y,m_charset->m_colorList.get(c).color.rgba());
        }
    return QPixmap::fromImage(img);

}

void LImageMetaChunk::ExportBin(QFile &file)
{
    for (LImageContainerItem* li : m_items) {
        LMetaChunkItem *m = dynamic_cast<LMetaChunkItem*>(li);
        file.write(m->m_data);
    }
}

QString LImageMetaChunk::getMetaInfo()
{
    QString txt ="Since NES sprites and tiles are only defined as 8x8 chuncks, it is customary to assemble these 8x8 blocks into larger meta-blocks that can be used as tiles or sprites. \n\n";
    txt+="With NES meta block tile and sprites, you first load a NES CHR image as your base charset before you can define your own NxM meta blocks consisting of 8x8 pixel blocks from the CHR image. Not only does this allow for ";
    txt+="smaller-sized data files (where you previously needed N*M bytes to specify a NxM block, you only need 1 byte now), it also allows for more internal variation since ";
    txt+="the 8x8 blocks can be shuffled around at will.\n\n";
    txt+="NES Metablocks are required for creating NES levels using the TRSE NES Level editor. Sprites are exported to the native NES PPU format.";
    return txt;
}
