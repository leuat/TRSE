#include "limageamstradcpc.h"

LImageAmstradCPC::LImageAmstradCPC(LColorList::Type t)  : LImageQImage(t)
{
    SetMode(0);
    m_type = LImage::Type::AmstradCPC;
    m_supports.asmExport = false;
    m_supports.binaryLoad = false   ;
    m_supports.binarySave = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_colorList.m_supportsFooterPen = true;

}

void LImageAmstradCPC::SetMode(int mode)
{
    m_footer.set(LImageFooter::POS_CURRENT_MODE,mode);
    if (mode == 0) {
        Initialize(160,200);
        m_colors = 16;
        m_bpp = 4;
    }
    if (mode == 1) {
        Initialize(320,200);
        m_colors = 4;
        m_bpp = 2;
    }
    InitPens();
}

void LImageAmstradCPC::InitPens()
{
    //m_colorList.InitAmstradCPC();
//    qDebug() << "HERE";
  //  m_colorList.InitPalettePens(m_colors);
}

void LImageAmstradCPC::setPixel(int x, int y, unsigned int color)
{
    if (m_qImage==nullptr)
        return;

//    int val = m_colorList.getPen(color);


//    if (rand()%100>98 && color!=0)
  //  qDebug() << "SET PIXEL "<< QString::number(m_colorList.getPenIndex(color)) << QString::number(color) << val;

    if (x>=0 && x<m_qImage->width() && y>=0 && y<m_qImage->height())
        m_qImage->setPixel(x,y,QRgb(m_colorList.getPenIndex(color)));
}
/*
unsigned int LImageAmstradCPC::getPixel(int x, int y)
{
    if (m_qImage==nullptr)
        return 0;
    if (x>=0 && x<m_qImage->width() && y>=0 && y<m_qImage->height()) {
        return m_qImage->pixel(x,y);
    }


    return 0;
}
*/
void LImageAmstradCPC::ToQImage(LColorList &lst, QImage &img, float zoom, QPointF center)
{
#pragma omp parallel for
    for (int i=0;i<m_width;i++)
        for (int j=0;j<m_height;j++) {

            float xp = ((i-center.x())*zoom)+ center.x();
            float yp = ((j-center.y())*zoom) + center.y();

            unsigned int pen = getPixel(xp,yp);// % 16;
            unsigned int col = m_colorList.getPen(pen);

            //            img->setPixel(i,j,QRgb(col));
            img.setPixel(i,j,lst.get(col).color.rgb());
        }
    //return img;
}

// Not implemented yet!
void LImageAmstradCPC::fromQImage(QImage *img, LColorList &lst)
{
#pragma omp parallel for
    for (int i=0;i<m_qImage->width();i++)
        for (int j=0;j<m_qImage->height();j++) {
            //unsigned char col = lst.getIndex(QColor(img->pixel(i, j)));

           // m_qImage->setPixel(i,j,col);
        }
}

