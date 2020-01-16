#include "limagemetablocksprites.h"

LImageMetaBlockSprites::LImageMetaBlockSprites(LColorList::Type t) : LImageMetaChunk(t) {
    m_type = LImage::Type::SpritesNES;
    m_supports.displayCmbColors = true;

    if (m_charset!=nullptr) {
        m_colorList.m_type = m_charset->m_colorList.m_type;
        m_colorList.m_list = m_charset->m_colorList.m_list;
        //m_charset->m_currentBank=0;
    }

    m_supports.displayCmbColors = false;

    m_GUIParams[btnFlipH] = "Paint mirror X";
    m_GUIParams[btnFlipV] = "Paint mirror Y";

//    m_currentBank = 0;
    PerformConstrainColours(QVector<int>() <<0<<1<<2<<3);

    m_supports.displayCharOperations = true;
}

void LImageMetaBlockSprites::SaveBin(QFile &file)
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
        file.write(m->m_attributes);
    }

    QByteArray fd =  QByteArray(m_charsetFilename.toLatin1());
    uchar len = fd.count();
    file.write( ( char * )( &len ),  1 );
    file.write(fd);
}

void LImageMetaBlockSprites::LoadBin(QFile &file)
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
        getCur()->m_attributes = file.read(w*h);
    }
    if (file.atEnd())
        return;
    uchar len;
    file.read( ( char * )( &len ),  1 );
    QByteArray data = file.read(len);
    m_charsetFilename = QString::fromLatin1(data);
    //  qDebug() << m_charsetFilename;
    if (QFile::exists(m_charsetFilename)) {
        LoadCharset(m_charsetFilename,0);
        //m_charset->m_currentBank=0;
        //m_currentBank = 0;
        m_charset->PerformConstrainColours(QVector<int>() <<0<<1<<2<<3);

    }
    m_current = 0;
    PerformConstrainColours(QVector<int>() <<0<<1<<2<<3);

}

void LImageMetaBlockSprites::setPixel(int x, int y, unsigned int color)
{
    QPoint p = getPos(x,y);
//        if (rand()%100>98)
  //        qDebug() << QString::number(color);

  //  if (m_writeType==Character)
        ((LMetaChunkItem*)m_items[m_current])->setPixel(p.x(),p.y(),m_currencChar,m_img->m_bitMask);

//    if (m_writeType==Color)
        ((LMetaChunkItem*)m_items[m_current])->setPixelAttrib(p.x(),p.y(),(color&3) |m_currentAttribute,m_img->m_bitMask);

}

unsigned int LImageMetaBlockSprites::getPixel(int x, int y)
{
    QPoint p = getPos(x,y);
    //    qDebug() << p;
    if (m_current>=m_items.count())
        return 0;


    uchar val = ((LMetaChunkItem*)m_items[m_current])->getPixel(p.x(),p.y(),m_img->m_bitMask);
    uchar attrib = ((LMetaChunkItem*)m_items[m_current])->getPixelAttrib(p.x(),p.y(),m_img->m_bitMask);
    uchar pal = (attrib&3)+4;
    //    if (rand()%100>97 && val!=0)
    //  qDebug() << "Vals : " << QString::number(val);
    //    val = 4;
    int xp = (float)x/(float)m_width*m_pixelWidth*getCur()->m_width;
    int yp = y/(float)m_height*m_pixelHeight*getCur()->m_height;;

    if  ((attrib&0b01000000) == 0b01000000) {
//        if (rand()%100>98)
  //          qDebug() << attrib;
        xp = m_pixelWidth-1-xp%m_pixelWidth; // Flip
    }
    if  ((attrib&0b10000000) == 0b10000000) {
        yp = m_pixelHeight-1-yp%m_pixelHeight; // Flip
    }
    int xx = ((val%m_charWidthDisplay)*m_pixelWidth) + xp%m_pixelWidth;
    int yy = ((val/(int)16)*m_pixelHeight)  +yp%m_pixelHeight;
    yy=yy+ 16*8*m_footer.get(LImageFooter::POS_CURRENT_BANK);


    if (m_charset==nullptr) {
        return 0;
    }
    m_charset->m_footer.set(LImageFooter::POS_DISPLAY_CHAR,0);

    ((LImageNES*)m_charset)->SetPalette(pal);

    return m_charset->getPixel(xx,yy);

}

void LImageMetaBlockSprites::ExportBin(QFile &file)
{
    QByteArray d;
    for (LImageContainerItem* li : m_items) {
        LMetaChunkItem *m = dynamic_cast<LMetaChunkItem*>(li);
        // Regular way
        for (int y=0;y<m->m_height;y++)
            for (int x=0;x<m->m_width;x++) {
                d.append(y*8);
                d.append(m->m_data[y*m->m_height+x]);
                d.append(m->m_attributes[y*m->m_height+x]);
                d.append(x*8);
                //file.write(m->m_data);
            }
        // Flip vertical
/*        for (int y=0;y<m->m_height;y++)
            for (int x=m->m_width-1;x>=0;x--) {
                d.append(y*8);
                d.append(m->m_data[y*m->m_height+x]);
                d.append(m->m_attributes[y*m->m_height+x] | 0b01000000);
                d.append(x*8);
                //file.write(m->m_data);
            }
*/

    }
    file.write(d);
}

void LImageMetaBlockSprites::FlipHorizontal()
{
    m_currentAttribute = Util::flipBit(m_currentAttribute,6);

}

void LImageMetaBlockSprites::FlipVertical()
{
    m_currentAttribute = Util::flipBit(m_currentAttribute,7);

}

