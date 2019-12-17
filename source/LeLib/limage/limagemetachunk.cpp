#include "limagemetachunk.h"
#include "source/LeLib/limage/limageio.h"


LImageMetaChunk::LImageMetaChunk(LColorList::Type t) : LImageNES(t)
{
    m_GUIParams[tabCharset] = "1";
    m_GUIParams[btnLoadCharset] ="Load Charset";
    m_width = 320;
    m_height = 200;

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
        getCur()->m_data = img->getCur()->m_data;
        //m_items[0] = img->m_items[img->m_current];


//        m_items = img->m_items;
        m_colorList.m_nesPPU = img->m_colorList.m_nesPPU;
            m_type = img->m_type;

        if (m_charset!=nullptr) {
             m_colorList.m_curPal = m_charset->m_colorList.m_curPal;
             m_currentBank = img->m_charset->m_currentBank;
        }
//        m_current = img->m_current;
        m_current = 0;

 /*       m_colorList.m_list.clear();
        for (LColor l :  img->m_colorList.m_list)
            m_colorList.m_list.append(l);
        m_colorList.m_type = img->m_colorList.m_type;*/
    }
}

void LImageMetaChunk::Copy()
{

}

void LImageMetaChunk::setPixel(int x, int y, unsigned int color)
{
    QPoint p = getPos(x,y);
    ((LMetaChunkItem*)m_items[m_current])->setPixel(p.x(),p.y(),m_currencChar,m_img->m_bitMask);

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


//    qDebug() << "charset " << m_charset;
//    m_charset = LImageIO::
}

unsigned int LImageMetaChunk::getPixel(int x, int y)
{
    QPoint p = getPos(x,y);
//    qDebug() << p;
    uchar val = ((LMetaChunkItem*)m_items[m_current])->getPixel(p.x(),p.y(),m_img->m_bitMask);

//    qDebug() << "Vals : " << QString::number(val);
//    val = 4;
    int xp = x/(float)m_width*m_pixelWidth*getCur()->m_width;
    int yp = y/(float)m_height*m_pixelHeight*getCur()->m_height;;

    int xx = ((val%m_pixelWidth)*m_pixelWidth) + xp%m_pixelWidth;
    int yy = ((val/(int)16)*m_pixelHeight)  + +yp%m_pixelHeight + 16*8*m_currentBank;


    if (m_charset==nullptr) {
        return 0;
    }
    else return m_charset->getPixel(xx,yy);



}

void LImageMetaChunk::SaveBin(QFile &file)
{
//    unsigned short N = m_items.count();
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

}

void LMetaChunkItem::setPixel(float x, float y, uchar color, uchar bitMask)
{
    if (x<0 || x>=m_width || y<0 || y>=m_height)
        return;
    m_data[(int)(y*m_width+x)] = color;
}

uchar LMetaChunkItem::getPixel(float x, float y, uchar bitMask)
{
    if (x<0 || x>=m_width || y<0 || y>=m_height)
        return 0;
    return m_data[(int)(y*m_width+x)];


}

void LImageMetaChunk::setForeground(unsigned int col)
{
    if (m_charset==nullptr)
        return;

    m_charset->SetColor(col, 1);
}
