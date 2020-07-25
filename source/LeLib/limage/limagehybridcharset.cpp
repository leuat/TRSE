#include "limagehybridcharset.h"


LImageHybridCharset::LImageHybridCharset(LColorList::Type t) : CharsetImage(t)
{
    m_type = LImage::Type::HybridCharset;
    m_bitMask = 0b1;
    m_scale = 1;
    m_width = 320;

}

unsigned int LImageHybridCharset::getPixel(int x, int y)
{
    m_bitMask = 0b1;
    m_scale = 1;
    m_width = 320;

    QPoint p = getXY(x,y);

//    PixelChar&pc = getPixelChar(p.x(),p.y());
 /*       if (pc.c[3]!=0)
            if (rand()%100>98)
                qDebug() << pc.c[3];*/
    int ande = 0x15;
    PixelChar&pc = getPixelChar(p.x(),p.y());
    if (pc.c[3]>=8) {
        m_bitMask=0b11;
        m_scale = 2;
        p.setX(p.x()/2);
        pc = getPixelChar(p.x(),p.y());

        ande = 7;
        int c = MultiColorImage::getPixel(p.x(),p.y());
        if (c!=m_extraCols[1] && c!=m_extraCols[2]) c=c&ande;
        return c;

    }


    if (MultiColorImage::getPixel(p.x(),p.y())!=0)
        return pc.c[3];

    return pc.c[0] &ande;

  //  return MultiColorImage::getPixel(p.x(),p.y());

}

void LImageHybridCharset::setPixel(int x, int y, unsigned int color)
{
    m_scale = 1;
    m_bitMask=1;
    m_width = 320;


    QPoint p = getXY(x,y);
    PixelChar&pc = getPixelChar(p.x(),p.y());
    if (pc.c[3]>=8) {
        m_bitMask=0b11;
        m_scale = 2;
        p.setX(p.x()/2);

    }


    CharsetImage::setLimitedPixel(p.x(),p.y(),color);
 //   pc.Reorganize(m_bitMask, m_scale,m_minCol, m_noColors);

  //      qDebug() << QString::number(color) << QString::number(pc.c[0]) << QString::number(pc.c[1]) << QString::number(pc.c[2]) << QString::number(pc.c[3]);

/*    int ix = x % (8/scale);//- (dx*m_charHeight);
    int iy = y % 8;//- (dy*m_charWidth);

    //if (ix==0 || ix == 2 || ix == 4 || ix == 6)
    pc.set(m_scale*ix, iy, color, bitmask);
*/
}
