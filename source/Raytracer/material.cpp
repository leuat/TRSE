#include "material.h"
#include <QDebug>
Material::Material()
{

}

void Texture::GenerateMipMaps() {
    m_images.append(m_image);
    QImage* cur = m_image;
    int ww = cur->width();
    int hh = cur->height();
    for (int i=0;i<8;i++) {
        //qDebug() << ww;
        ww = std::max(ww/2,1);
        hh = std::max(hh/2,1);
        QImage* nxt = new QImage(m_image->width(),m_image->height(),QImage::Format_ARGB32);
        nxt->fill(QColor(0,0,0,1));
        QImage small = (cur->scaled(ww,hh,Qt::KeepAspectRatio));
        /*for (int x=0;x<xw;x++)
            for (int y=0;y<yw;y++) {
                nxt->setPixel(x,y,cur->pixel(x*2,y*2));
            }*/
        *nxt = (small.scaled(m_image->width(),m_image->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
//        qDebug() <<nxt->width();

        m_images.append(nxt);
        cur = nxt;
    }
}
