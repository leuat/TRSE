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
        if (c!=m_colorList.getPen(1) && c!=m_colorList.getPen(2)) c=c&ande;
        return c;

    }


    if (MultiColorImage::getPixel(p.x(),p.y())!=0)
        return pc.c[3];

    return pc.c[0] &ande;


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
}
