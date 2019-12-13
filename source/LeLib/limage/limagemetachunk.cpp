#include "limagemetachunk.h"
#include "source/LeLib/limage/limageio.h"


LImageMetaChunk::LImageMetaChunk(LColorList::Type t)
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
}

QPoint LImageMetaChunk::getPos(int x, int y)
{
    LMetaChunkItem* cur = ((LMetaChunkItem*)m_items[m_current]);
    int xx = (int)((x/(float)m_width)*m_pixelWidth*cur->m_width);
    int yy = (int)((y/(float)m_height)*m_pixelHeight*cur->m_height);
    return QPoint(xx,yy);
}

void LImageMetaChunk::CopyFrom(LImage *mc)
{
    LImageMetaChunk* img = dynamic_cast<LImageMetaChunk*>(mc);
    if (img!=nullptr) {
        m_charset=img->m_charset;
        m_current = img->m_current;
        m_items = img->m_items;
        m_type = img->m_type;
    }
}

void LImageMetaChunk::Copy()
{

}

void LImageMetaChunk::setPixel(int x, int y, unsigned int color)
{
    QPoint p = getPos(x,y);
    ((LMetaChunkItem*)m_items[m_current])->setPixel(p.x(),p.y(),color,m_img->m_bitMask);

}

void LImageMetaChunk::LoadCharset(QString file, int skipBytes)
{
    m_charset = LImageIO::Load(file);
    if (m_charset->m_colorList.m_type!=m_img->m_colorList.m_type) {
        qDebug() << "Incompatible charset type!";
        m_charset = nullptr;

    }
//    qDebug() << "charset " << m_charset;
//    m_charset = LImageIO::
}

unsigned int LImageMetaChunk::getPixel(int x, int y)
{
    QPoint p = getPos(x,y);
    uchar val = ((LMetaChunkItem*)m_items[m_current])->getPixel(p.x(),p.y(),m_img->m_bitMask);

    val = 5;
    int xx = ((val%16)*16) + p.x();//x/(float)m_width*m_pixelWidth;
    int yy = ((val/(int)16)*16)  + y/(float)m_height*m_pixelHeight;


    if (m_charset==nullptr) {
        return 0;
    }
    else return m_charset->getPixel(xx,yy);



}

void LImageMetaChunk::SaveBin(QFile &file)
{

}

void LImageMetaChunk::LoadBin(QFile &file)
{
    AddNew(2,2);
}

void LImageMetaChunk::SetColor(uchar col, uchar idx)
{

}

void LImageMetaChunk::SetColor(uchar col, uchar idx, LImageMetaChunk &s)
{
    m_currencChar = col;
}

bool LImageMetaChunk::KeyPress(QKeyEvent *e)
{

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
