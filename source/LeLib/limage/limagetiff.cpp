#include "limagetiff.h"
#include <QImage>
#include <omp.h>

LImageTiff::LImageTiff()
{
    m_type = LImage::Tiff;
    m_width = 0;
    m_height = 0;
}

LImageTiff::LImageTiff(LColorList::Type t) : LImageQImage(t)
{
    m_type = LImage::Tiff;
}

LImageTiff::~LImageTiff()
{
#ifdef USE_LIBTIFF

    for (LTiff* t : m_tifs)
        delete t;
    m_tifs.clear();
#endif
}

void LImageTiff::Initialize(int width, int height)
{
    Release();
    m_width = width;
    m_height = height;
    m_qImage = new QImage(width, height, QImage::Format_ARGB32);
}

bool LImageTiff::LoadTiff(QString filename)
{
#ifdef USE_LIBTIFF

    int num = omp_get_max_threads();
    for (int i=0;i<num;i++)
    {
        LTiff* m_tif = new LTiff();
        if (!m_tif->Open(filename)) {
            //LMessage::lMessage.Error("Could not open tiff file + '"+i + "' ! Please check the filename and try again.");
            qDebug() << "Could not open tiff " << filename;
            return false;
        }
        m_tif->SetupBuffers();
        m_tifs.append(m_tif);
        qDebug() << "Appending.. ";
    }
    return true;
#else
    qDebug() << "LibTIFF not compiled in this version";
#endif
    return true;
}

void LImageTiff::ToQImage(LColorList &lst, QImage *img, float zoom, QPointF center)
{
#ifdef USE_LIBTIFF
    QColor back = QColor(0,0,0);

//    m_tif.SetupBuffers();
  //  m_tif.UpdateBuffers();


    for (int i=0;i<omp_get_max_threads();i++)
        m_tifs[i]->bufferStack.SetDefault();


    float addPercent = 1.0/(m_width*m_height);
    Data::data.percent = 0;
    #pragma omp parallel for
    for (int i=0;i<m_width;i++) {
        for (int j=0;j<m_height;j++) {

            int thread = omp_get_thread_num();
            LTiff* m_tif = m_tifs[thread];

            float xx = i/(float)m_width*m_tif->m_width;
            float yy = j/(float)m_height*m_tif->m_height;

            float xp = ((xx-center.x())*zoom)+ center.x();
            float yp = ((yy-center.y())*zoom) + center.y();

            QColor col = back;

            if (xp>=0 && xp<m_tif->m_width && yp>=0 && yp<m_tif->m_height)
                col = QColor(m_tif->GetTiledRGB(xp,yp,0));
                QColor rCol = QColor(col.blue(), col.green(), col.red());
                col = rCol;
                //c = m_qImage->pixel(xp,yp);

            //            img->setPixel(i,j,QRgb(col));
            img->setPixel(i,j,col.rgb());
//            qDebug() << "Updating " << thread << " of " << m_tif.m_bufferStacks.count();
            Data::data.percent  += addPercent;
            if (j%40 == 0)
                m_tif->bufferStack.UpdateBuffer();

            if (Data::data.abort)
                break;

                //m_tif.m_bufferStacks[thread]->UpdateBuffer();

        }
        if (Data::data.abort)
            break;

    }
    Data::data.percent = 1;
#endif

    //return img;

}
