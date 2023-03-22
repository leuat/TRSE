#include "limagemetachunk.h"
#include "source/LeLib/limage/limageio.h"
#include "source/Compiler/syntax.h"

LImageMetaChunk::LImageMetaChunk(LColorList::Type t) : CharsetImage(t)
{
    m_GUIParams[tabCharset] = "1";
    m_GUIParams[btnLoadCharset] ="Load Charset";
    m_width = 256;
    m_height = 256;
    m_supports.displayColors = false;
    m_GUIParams[btnFlipH] = "Paint mirror X";
    m_GUIParams[btnFlipV] = "Paint mirror Y";



    Initialize(m_width,m_height);
    if (t==LColorList::NES) {
        m_img = new LImageNES(t);
    }
    if (t==LColorList::SNES) {
        m_img = new LImageSNES(t);
    }
    m_type = LImage::Type::LMetaChunk;
    if (m_img!=nullptr) {
        m_colorList.m_type = m_img->m_colorList.m_type;
        m_colorList.CopyFrom(&m_img->m_colorList);

    }
    AddNew(2,2);

    m_supports.displayBank = true;

    if (isNes() || isSnes())
        m_charWidthDisplay = 16;

    else m_charWidthDisplay = 40;

    m_gridWidthDisplay = m_charWidthDisplay;

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

    m_updateCharsetPosition = false;
    m_supports.displayCharOperations = true;


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
        m_currentChar = img->m_currentChar;

        m_items.clear();

/*        AddNew(img->getCur()->m_width, img->getCur()->m_height);
        getCur()->m_attributes = img->getCur()->m_attributes;
        getCur()->m_data = img->getCur()->m_data;*/

        DeleteAll();
        for (auto li: img->m_items) {
            LMetaChunkItem* s= (LMetaChunkItem*)li.get();
            LMetaChunkItem* s2= new LMetaChunkItem();
            *s2 = *s;

            m_items.append(QSharedPointer<LMetaChunkItem>(s2));
        }


        m_current = img->m_current;



        //m_items[0] = img->m_items[img->m_current];

//        m_items = img->m_items;
        m_colorList.m_nesPPU = img->m_colorList.m_nesPPU;
            m_type = img->m_type;

        if (m_charset!=nullptr) {
             m_colorList.m_curPal = m_charset->m_colorList.m_curPal;
             m_charset->SetBank(m_footer.get(LImageFooter::POS_CURRENT_BANK));
//             m_currentBank = img->m_charset->m_currentBank;
        }
//        m_current = img->m_current;
//        m_current = 0;
        m_currentAttribute = img->m_currentAttribute;
        m_classvariableType = img->m_classvariableType;
        //m_currentBank = img->m_currentBank;
        m_footer = img->m_footer;
        m_charWidthDisplay = img->m_charWidthDisplay;
        m_currentChar = img->m_currentChar;
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
/*    if (m_charset->m_colorList.m_type!=m_img->m_colorList.m_type) {
        qDebug() << "Incompatible charset type!";
        m_charset = nullptr;

    }*/
//    qDebug() << "LOADING : " << file;
    if (m_charset==nullptr)
        return;
//    m_colorList.m_list = m_charset->m_colorList.m_list;
    m_colorList.m_nesPPU = m_charset->m_colorList.m_nesPPU;
    m_colorList.m_curPal = m_charset->m_colorList.m_curPal;
    m_colorList.CopyFrom(&m_charset->m_colorList);
    LImageNES* nes = dynamic_cast<LImageNES*>(m_charset);

    if (nes!=nullptr)
        nes->m_double = false;

    m_charsetFilename = file;
    if (isNes() || isSnes())
        m_charWidthDisplay = 16;
    else m_charWidthDisplay = 40;

    m_charset->m_updatePaletteInternal = false;
    m_charset->SetPalette(0);

}

void LImageMetaChunk::InitPens()
{
    m_colorList.DefaultPen(LPen::FixedSingleNumbers,4);
}

void LImageMetaChunk::SetPalette(int pal)
{
    if (m_charset==nullptr)
        return;
    m_charset->SetPalette(pal);
//    m_colorList.CopyFrom(&m_charset->m_colorList);
}

QStringList LImageMetaChunk::getPaletteNames() {
    if (m_charset==nullptr)
        return LImage::getPaletteNames();

    return m_charset->getPaletteNames();
}

void LImageMetaChunk::setPixel(int x, int y, unsigned int color)
{
    QPoint p = getPos(x,y);
    int bm = 1;
    if (m_img!=nullptr)
        bm = m_img->m_bitMask;

    ((LMetaChunkItem*)m_items[m_current].get())->setPixel(p.x(),p.y(),m_currentChar,bm);
    if (isSnes())
        ((LMetaChunkItem*)m_items[m_current].get())->setPixelAttrib(p.x(),p.y(),m_currentAttribute,bm);

}


unsigned int LImageMetaChunk::getPixel(int x, int y)
{
    if (m_charset==nullptr)
        return 0;


    QPoint p = getPos(x,y);
//    qDebug() << p;
    if (m_current>=m_items.count())
        return 0;

    int bm=1;
    if (m_img!=nullptr)
        bm = m_img->m_bitMask;
    uchar val = ((LMetaChunkItem*)m_items[m_current].get())->getPixel(p.x(),p.y(),bm);
    uchar attrib = ((LMetaChunkItem*)m_items[m_current].get())->getPixelAttrib(p.x(),p.y(),bm);

    int xp = x/(float)m_width*m_pixelWidth*getCur()->m_width;
    int yp = y/(float)m_height*m_pixelHeight*getCur()->m_height;



    if  ((attrib&0b01000000) == 0b01000000) {
        xp = m_pixelWidth-1-xp%m_pixelWidth; // Flip
    }
    if  ((attrib&0b10000000) == 0b10000000) {
        yp = m_pixelHeight-1-yp%m_pixelHeight; // Flip
    }



    int xx = ((val%m_charWidthDisplay)*m_pixelWidth) + xp%m_pixelWidth;
    int yy = ((val/(int)m_charWidthDisplay)*m_pixelHeight)  +yp%m_pixelHeight;

    if (isNes())
        yy=yy+ 16*8*m_footer.get(LImageFooter::POS_CURRENT_BANK);
    if (isSnes())
        yy=yy+ 16*16*m_footer.get(LImageFooter::POS_CURRENT_BANK);

    m_charset->m_footer.set(LImageFooter::POS_DISPLAY_CHAR,0);
//    qDebug() << xx << yy

    unsigned int col = 0;
    col = m_charset->getPixel(xx,yy);
//    if (isNes())
    if (isSnes() && m_returnActualColor)
    {
        unsigned int pen = m_charset->getPixel(xx,yy);// % 16;
        col = m_charset->m_colorList.getPen(pen);

    }
    return col;

}

void LImageMetaChunk::SaveBin(QFile &file)
{
    uchar no = m_items.count();
    file.write( ( char * )( &no ),  1 );
    for (auto li : m_items) {
        LMetaChunkItem *m = dynamic_cast<LMetaChunkItem*>(li.get());
        uchar w = m->m_width;
        uchar h = m->m_height;
        file.write( ( char * )( &w ),  1 );
        file.write( ( char * )( &h ),  1 );
        file.write(m->m_data);
  //      qDebug() << "Writing: "<<isSnes();
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::SNES)
        {
//        if (isSnes()) {
//            qDebug() << "Writing attributes;";
            file.write(m->m_attributes);
        }
    }
    AppendSaveBinCharsetFilename(file);
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
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::SNES)
            getCur()->m_attributes = file.read(w*h);
    }
    LoadBinCharsetFilename(file);
}

void LImageMetaChunk::SetColor(uchar col, uchar idx)
{
    if (m_charset==nullptr)
        return;
    m_charset->SetColor(col, idx);
}

void LImageMetaChunk::SetColor(uchar col, uchar idx, LImageMetaChunk &s)
{
    m_currentChar = col;
}

LMetaChunkItem *LImageMetaChunk::getCur()
{
    if (m_current>=m_items.count())
        return nullptr;
    return (LMetaChunkItem*)m_items[m_current].get();

}

bool LImageMetaChunk::KeyPress(QKeyEvent *e)
{
    if (e->key()==Qt::Key_A)
        Prev();
    if (e->key()==Qt::Key_D)
        Next();

    if (e->key()==Qt::Key_Plus)
        m_currentChar++;
    if (e->key()==Qt::Key_Minus)
        m_currentChar--;


    return false;
}

void LImageMetaChunk::CopyChar()
{
    m_copy = *getCur();
}

void LImageMetaChunk::PasteChar()
{
    *getCur() = m_copy;
}

/*void LImageMetaChunk::ToQImage(LColorList &lst, QImage &img, double zoom, QPointF center)
{

}
*/
void LImageMetaChunk::AddNew(int w, int h)
{
    LMetaChunkItem* s = new LMetaChunkItem();
    s->Init(w,h);
    m_items.append(QSharedPointer<LMetaChunkItem>(s));
    m_current = m_items.count()-1;


}

void LImageMetaChunk::FlipHorizontal()
{
    if (isSnes())
        m_currentAttribute = Util::flipBit(m_currentAttribute,6);
}

void LImageMetaChunk::FlipVertical()
{
    if (isSnes())
        m_currentAttribute = Util::flipBit(m_currentAttribute,7);

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
    int idx = (int)(y*m_width+x);
    if (idx<m_data.size())
        m_data[idx] = color;

}

void LMetaChunkItem::setPixelAttrib(float x, float y, uchar color, uchar bitMask)
{
    if (x<0 || x>=m_width || y<0 || y>=m_height)
        return;
    int idx = (int)(y*m_width+x);
    if (idx<m_data.size())
        m_attributes[idx] = color;

}

uchar LMetaChunkItem::getPixel(float x, float y, uchar bitMask)
{
    if (x<0 || x>=m_width || y<0 || y>=m_height)
        return 0;
    int idx = (int)(y*m_width+x);
    if (idx<m_data.size())
        return m_data[idx];

    return 0;
}

uchar LMetaChunkItem::getPixelAttrib(float x, float y, uchar bitMask)
{
    if (x<0 || x>=m_width || y<0 || y>=m_height)
        return 0;
    int idx = (int)(y*m_width+x);
    if (idx<m_data.size())
        return m_attributes[idx];
   return 0;


}


unsigned int LImageMetaChunk::getCharPixel(int pos,  int pal,int x, int y)
{
    if (m_charset==nullptr)
        return 0;
    if (pos>=m_items.count())
        return 0;

    m_current = pos;
    int w=16;//(getCur()->m_width)*8;
    int h=16;//(getCur()->m_height)*8;

    x=(x%w)*w;
    y=(y%h)*h;
    m_charset->m_updatePaletteInternal = false;

    m_charset->SetPalette(pal);
    return getPixel(x,y);
}

QPixmap LImageMetaChunk::ToQPixMap(int chr)
{
    if (m_charset==nullptr)
        return QPixmap();
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
    for (auto li : m_items) {
        LMetaChunkItem *m = dynamic_cast<LMetaChunkItem*>(li.get());
        file.write(m->m_data);
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::SNES)
            file.write(m->m_attributes);
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
